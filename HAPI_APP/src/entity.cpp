#include "entity.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Textures.h"
#include "GameEventMessenger.h"

constexpr float MOVEMENT_ANIMATION_TIME(0.35f);
constexpr size_t MOVEMENT_PATH_SIZE{ 32 };
constexpr size_t WEAPON_HIGHLIGHT_SIZE{ 200 };
constexpr float DRAW_ENTITY_OFFSET_X{ 16 };
constexpr float DRAW_ENTITY_OFFSET_Y{ 32 };


//std::pair<int, int> m_currentPosition;
////std::deque<std::pair<eDirection, std::pair<int, int>>> m_pathToTile;
////Timer m_movementTimer;
////MovementPath m_movementPath;
////int m_movementPathSize;
////eDirection m_currentDirection;
////bool m_weaponFired;
//////bool m_movedToDestination;
////bool m_isDead;
////ActionSprite m_actionSprite;
////bool m_movingToDestination;
////bool m_destinationSet;
//
////ENTITY BATTLE PROPERTIES
//EntityBattleProperties::EntityBattleProperties(std::pair<int, int> startingPosition, FactionName factionName, eDirection startingDirection)
//	:  m_currentPosition(startingPosition),
//=======
//TODO: Will change
std::vector<EntityProperties> assignEntities(FactionName name)
{
	std::vector<EntityProperties> entities;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			EntityProperties newEntity(name, (EntityType)(i));
			entities.push_back(newEntity);
		}
	}
	assert(!entities.empty());
	return entities;
}


//ENTITY BATTLE PROPERTIES
EntityBattleProperties::EntityBattleProperties(std::pair<int, int> startingPosition, FactionName factionName, eDirection startingDirection)
	: m_currentPosition(startingPosition),
	m_pathToTile(),
	m_movementTimer(MOVEMENT_ANIMATION_TIME),
	m_movementPath(),
	m_movementPathSize(0),
	m_currentDirection(startingDirection),
	m_weaponFired(false),
	m_isDead(false),
	m_actionSprite(factionName),
	m_movingToDestination(false),
	m_destinationSet(false)
	//m_factionName(factionName),
	//m_currentPosition(startingPosition),
	//m_pathToTile(),
	//m_movementTimer(0.35f),
	//m_movementPath(),
	//m_movementPathSize(0),
	//m_currentDirection(startingDirection),
	//m_weaponFired(false),
	//m_isDead(false),
	//m_actionSprite(factionName)
{
	GameEventMessenger::getInstance().subscribe(std::bind(&EntityBattleProperties::onNewTurn, this), "EntityBattleProperties", GameEvent::eNewTurn);
}

EntityBattleProperties::~EntityBattleProperties()
{
	GameEventMessenger::getInstance().unsubscribe("EntityBattleProperties", GameEvent::eNewTurn);
}

eDirection EntityBattleProperties::getCurrentDirection() const
{
	return m_currentDirection;
}


std::pair<int, int> EntityBattleProperties::getCurrentPosition() const
{
	return m_currentPosition;
}

bool EntityBattleProperties::isWeaponFired() const
{
	return m_weaponFired;
}

bool EntityBattleProperties::isDead() const
{
	return m_isDead;
}


bool EntityBattleProperties::isMovingToDestination() const
{
	return m_movingToDestination;
}

bool EntityBattleProperties::isDestinationSet() const
{
	return m_destinationSet;
}

//MOVEMENT PATH NODE
EntityBattleProperties::MovementPath::PathNode::PathNode()
	: sprite(std::make_unique<Sprite>(Textures::m_mouseCrossHair)),
	activate(false)
{
	sprite->GetTransformComp().SetOriginToCentreOfFrame();
}

//
//MOVEMENT PATH
//
EntityBattleProperties::MovementPath::MovementPath()
	: m_movementPath()	
{
	m_movementPath.reserve(size_t(MOVEMENT_PATH_SIZE));
	for (int i = 0; i < MOVEMENT_PATH_SIZE; ++i)
	{
		m_movementPath.push_back({});
	}
}

void EntityBattleProperties::MovementPath::render(const Map& map) const
{
	for (const auto& i : m_movementPath)
	{
		if (i.activate)
		{
			const std::pair<int, int> tileTransform = map.getTileScreenPos(i.m_position);
			float scale = map.getDrawScale();

			i.sprite->GetTransformComp().SetPosition({
				static_cast<float>(tileTransform.first + DRAW_ENTITY_OFFSET_X * scale),
				static_cast<float>(tileTransform.second + DRAW_ENTITY_OFFSET_Y * scale) });
			i.sprite->GetTransformComp().SetScaling({ scale / 2, scale / 2 });

			i.sprite->Render(SCREEN_SURFACE);
		}	
	}
}

int EntityBattleProperties::MovementPath::generatePath(const Map& map, const Tile& source, const Tile& destination)
{
	auto pathToTile = PathFinding::getPathToTile(map, source.m_tileCoordinate, destination.m_tileCoordinate);
	if (pathToTile.empty())
	{
		return 0;
	}

	clearPath();

	int bonusMove = 0;
	int movementPointsUsed = 0;
	int prevDir = source.m_entityOnTile->m_battleProperties.m_currentDirection;
	//Don't interact with path from source.
	int i = 1;
	for (i ; i < pathToTile.size(); ++i)
	{
		++movementPointsUsed;

		int pathDir = pathToTile[i].first;
		int movementCost = getDirectionCost(prevDir, pathDir);
		prevDir = pathDir;

		movementPointsUsed += movementCost;

		if (prevDir == map.getWindDirection() && bonusMove == 0)
		{
			bonusMove = static_cast<int>(source.m_entityOnTile->m_entityProperties.m_movementPoints * map.getWindStrength());
			movementPointsUsed -= bonusMove;
		}

		source.m_entityOnTile->m_battleProperties.m_movementPathSize = i;
		if (movementPointsUsed <=
			source.m_entityOnTile->m_entityProperties.m_movementPoints)
		{
			auto tileScreenPosition = map.getTileScreenPos(pathToTile[i].second);
			m_movementPath[i - 1].sprite->GetTransformComp().SetPosition({
				static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * map.getDrawScale()),
				static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * map.getDrawScale()) });
			m_movementPath[i - 1].activate = true;
			m_movementPath[i - 1].m_position = pathToTile[i].second;
		}
		else
		{
			return i;
		}
	}
	return i;
}

void EntityBattleProperties::MovementPath::eraseNode(std::pair<int, int> position, const Map& map)
{
	for (auto iter = m_movementPath.begin(); iter != m_movementPath.end(); ++iter)
	{
		auto i = map.getMouseClickCoord({ iter->sprite->GetTransformComp().GetPosition().x, iter->sprite->GetTransformComp().GetPosition().y });
		if (i == position)
		{
			iter->activate = false;
		}
	}
}

void EntityBattleProperties::MovementPath::clearPath()
{
	for (auto& i : m_movementPath)
	{
		i.activate = false;
	}
}

int EntityBattleProperties::generateMovementGraph(const Map & map, const Tile & source, const Tile & destination)
{
	return m_movementPath.generatePath(map, source, destination);
}

void EntityBattleProperties::clearMovementPath()
{
	m_movementPath.clearPath();
}

void EntityBattleProperties::enableAction()
{
	if (!m_isDead)
	{
		m_actionSprite.active = true;
	}
}

void EntityBattleProperties::disableAction()
{
	m_actionSprite.active = false;
}

bool EntityBattleProperties::moveEntity(Map& map, const Tile& tile)
{
	if (!m_destinationSet)
	{
		auto pathToTile = PathFinding::getPathToTile(map, m_currentPosition, tile.m_tileCoordinate);
		if (!pathToTile.empty() && pathToTile.size() <= m_movementPathSize + 1)
		{
			m_pathToTile = pathToTile;
			map.moveEntity(m_currentPosition, pathToTile.back().second);
			m_destinationSet = true;
			m_movingToDestination = true;
			m_actionSprite.active = false;
			return true;
		}
		else
		{
			clearMovementPath();
			return false;
		}
	}

	return true;
}

bool EntityBattleProperties::moveEntity(Map& map, const Tile& tile, eDirection endDirection)
{
	if (!m_destinationSet)
	{
		auto pathToTile = PathFinding::getPathToTile(map, m_currentPosition, tile.m_tileCoordinate);
		if (!pathToTile.empty() && pathToTile.size() <= m_movementPathSize + 1)
		{
			//Set end tile to the correct facing
			pathToTile[pathToTile.size() - 1].first = endDirection;
			for (int i = 0; i < pathToTile.size(); ++i)
			{
				m_movementPath.setNodePosition(i, pathToTile[i].second);
			}

			m_pathToTile = pathToTile;
			map.moveEntity(m_currentPosition, pathToTile.back().second);
			m_destinationSet = true;
			m_movingToDestination = true;
			m_actionSprite.active = false;
			return true;
		}
		else
		{
			clearMovementPath();
			return false;
		}
	}

	return true;
}

//TODO: Will change.
//Not sure how damage/health is going to be implemented yet. 
void EntityBattleProperties::takeDamage(EntityProperties & entityProperties, int damageAmount, FactionName entityFaction)
{
	entityProperties.m_currentHealth -= damageAmount;
	int currentHealth = entityProperties.m_currentHealth;
	int maxHealth = entityProperties.m_healthMax;
	auto& entitySprite = entityProperties.m_sprite;

	int healthPercentage = ((float)currentHealth / maxHealth) * 100;
	
	if (healthPercentage < 100 && healthPercentage >= 50)
	{
		entitySprite->SetFrameNumber(eShipSpriteFrame::eLowDamage);
		entitySprite->GetTransformComp().SetOriginToCentreOfFrame();
	}
	else if (healthPercentage < 50 && healthPercentage >= 1)
	{
		entitySprite->SetFrameNumber(eShipSpriteFrame::eHighDamage);
		entitySprite->GetTransformComp().SetOriginToCentreOfFrame();
	}
	else
	{
		entitySprite->SetFrameNumber(eShipSpriteFrame::eDead);
		entitySprite->GetTransformComp().SetOriginToCentreOfFrame();
		m_isDead = true;
		m_actionSprite.active = false;
		switch (entityFaction)
		{
		case FactionName::eYellow :
			GameEventMessenger::broadcast(GameEvent::eYellowShipDestroyed);
			break;
		case FactionName::eGreen :
			GameEventMessenger::broadcast(GameEvent::eGreenShipDestroyed);
			break;
		case FactionName::eRed :
			GameEventMessenger::broadcast(GameEvent::eRedShipDestroyed);
			break;
		case FactionName::eBlue :
			GameEventMessenger::broadcast(GameEvent::eBlueShipDestroyed);
			break;
		}
	}
}

void EntityBattleProperties::fireWeapon()
{
	m_weaponFired = true;
	m_actionSprite.active = false;
}

void EntityBattleProperties::setDestination()
{
	m_destinationSet = true;
}

void EntityBattleProperties::onNewTurn()
{
	//m_movedToDestination = false;
	m_weaponFired = false;
	m_destinationSet = false;
	m_movingToDestination = false;
}

void EntityBattleProperties::handleRotation(EntityProperties& entityProperties)
{
	int rotationAngle = 60;
	int directionToTurn = m_pathToTile.front().first;
	entityProperties.m_sprite->GetTransformComp().SetRotation(
		DEGREES_TO_RADIANS(directionToTurn*rotationAngle % 360));
	m_currentDirection = (eDirection)directionToTurn;
}

unsigned int EntityBattleProperties::MovementPath::getDirectionCost(int currentDirection, int newDirection)
{
	unsigned int diff = std::abs(newDirection - currentDirection);
	if (diff == 0)
	{
		return 0;
	}

	//number of direction % difference between the new and old directions
	return (static_cast<int>(eDirection::Max) % diff) + 1;
}

//ENTITY
EntityProperties::EntityProperties(FactionName factionName, EntityType entityType) : m_upgradePoints(4), m_maxUpgradePoints(4), m_selectedSprite(HAPI_Sprites.MakeSprite(Textures::m_thing))
{
	//TODO: Currently not working as intended
	//UI seems to be resetting the frameNumber somewhere in OverWorldGUI. 
	switch (entityType)
	{
	case EntityType::eCruiser:
		m_originalMovement = 15;
		m_originalHealth = 7;
		m_originalRange = 5;
		m_originalDamage = 2;
		m_movementPoints = m_originalMovement;
		m_healthMax = m_originalHealth;
		m_currentHealth = m_healthMax;
		m_range = m_originalRange;
		m_damage = m_originalDamage;
		m_weaponType = eWeaponType::eSideCannons;
		break;
	case EntityType::eBattleShip:
		m_originalMovement = 15;
		m_originalHealth = 8;
		m_originalRange = 2;
		m_originalDamage = 2;
		m_movementPoints = m_originalMovement;
		m_healthMax = m_originalHealth;
		m_currentHealth = m_healthMax;
		m_range = m_originalRange;
		m_damage = m_originalDamage;
		m_weaponType = eWeaponType::eShotgun;
		break;
	case EntityType::eDestroyer:
		m_originalMovement = 20;
		m_originalHealth = 6;
		m_originalRange = 6;
		m_originalDamage = 3;
		m_movementPoints = m_originalMovement;
		m_healthMax = m_originalHealth;
		m_currentHealth = m_healthMax;
		m_range = m_originalRange;
		m_damage = m_originalDamage;
		m_weaponType = eWeaponType::eFlamethrower;
		break;
	case EntityType::eGunBoat:
		m_originalMovement = 10;
		m_originalHealth = 5;
		m_originalRange = 12;
		m_originalDamage = 2;
		m_movementPoints = m_originalMovement;
		m_healthMax = m_originalHealth;
		m_currentHealth = m_healthMax;
		m_range = m_originalRange;
		m_damage = m_originalDamage;
		m_weaponType = eWeaponType::eStraightShot;
		break;
	}

	switch (factionName)
	{
	case FactionName::eYellow:
		switch (entityType)
		{
		case EntityType::eCruiser:
			m_sprite = std::shared_ptr<HAPISPACE::Sprite>(HAPI_Sprites.MakeSprite(Textures::m_yellowShipSideCannons));
			break;
		case EntityType::eBattleShip:

			m_sprite = std::shared_ptr<HAPISPACE::Sprite>(HAPI_Sprites.MakeSprite(Textures::m_yellowShipBomb));
			break;
		case EntityType::eDestroyer:

			m_sprite = std::shared_ptr<HAPISPACE::Sprite>(HAPI_Sprites.MakeSprite(Textures::m_yellowShipMelee));
			break;
		case EntityType::eGunBoat:

			m_sprite = std::shared_ptr<HAPISPACE::Sprite>(HAPI_Sprites.MakeSprite(Textures::m_yellowShipSnipe));
			break;

		}

		break;

	case FactionName::eBlue:
		switch (entityType)
		{
		case EntityType::eCruiser:

			m_sprite = std::shared_ptr<HAPISPACE::Sprite>(HAPI_Sprites.MakeSprite(Textures::m_blueShipSideCannons));
			break;
		case EntityType::eBattleShip:

			m_sprite = std::shared_ptr<HAPISPACE::Sprite>(HAPI_Sprites.MakeSprite(Textures::m_blueShipBomb));
			break;
		case EntityType::eDestroyer:

			m_sprite = std::shared_ptr<HAPISPACE::Sprite>(HAPI_Sprites.MakeSprite(Textures::m_blueShipMelee));
			break;
		case EntityType::eGunBoat:

			m_sprite = std::shared_ptr<HAPISPACE::Sprite>(HAPI_Sprites.MakeSprite(Textures::m_blueShipSnipe));
			break;

		}

		break;
	case FactionName::eRed:
		switch (entityType)
		{
		case EntityType::eCruiser:

			m_sprite = std::shared_ptr<HAPISPACE::Sprite>(HAPI_Sprites.MakeSprite(Textures::m_redShipSideCannons));
			break;
		case EntityType::eBattleShip:

			m_sprite = std::shared_ptr<HAPISPACE::Sprite>(HAPI_Sprites.MakeSprite(Textures::m_redShipBomb));
			break;
		case EntityType::eDestroyer:

			m_sprite = std::shared_ptr<HAPISPACE::Sprite>(HAPI_Sprites.MakeSprite(Textures::m_redShipMelee));
			break;
		case EntityType::eGunBoat:

			m_sprite = std::shared_ptr<HAPISPACE::Sprite>(HAPI_Sprites.MakeSprite(Textures::m_redShipSnipe));
			break;
		default:
			break;
		}

		break;
	case FactionName::eGreen:
		switch (entityType)
		{
		case EntityType::eCruiser:

			m_sprite = std::shared_ptr<HAPISPACE::Sprite>(HAPI_Sprites.MakeSprite(Textures::m_greenShipSideCannons));
			break;
		case EntityType::eBattleShip:

			m_sprite = std::shared_ptr<HAPISPACE::Sprite>(HAPI_Sprites.MakeSprite(Textures::m_greenShipBomb));
			break;
		case EntityType::eDestroyer:

			m_sprite = std::shared_ptr<HAPISPACE::Sprite>(HAPI_Sprites.MakeSprite(Textures::m_greenShipMelee));
			break;
		case EntityType::eGunBoat:

			m_sprite = std::shared_ptr<HAPISPACE::Sprite>(HAPI_Sprites.MakeSprite(Textures::m_greenShipSnipe));
			break;

		}
		break;
	}
	m_sprite->SetFrameNumber(eShipSpriteFrame::eMaxHealth);
	m_sprite->GetTransformComp().SetOriginToCentreOfFrame();
}

BattleEntity::BattleEntity(std::pair<int, int> startingPosition, const EntityProperties& entityProperties, Map& map, FactionName playerName, eDirection startingDirection)
	: m_entityProperties(entityProperties),
	m_battleProperties(startingPosition, playerName, startingDirection),
	m_factionName(playerName)
{
	m_entityProperties.m_sprite->GetTransformComp().SetRotation(DEGREES_TO_RADIANS(startingDirection * 60 % 360));
	map.insertEntity(*this);
}

void EntityBattleProperties::update(float deltaTime, const Map & map, EntityProperties& entityProperties)
{	
	//bool m_movingToDestination = false;
	//bool m_reachedDestination = false;
	//if (!m_movingToDestination && m_reachedDestination)
	//{
	//	return true;
	//}
	//

	if (!m_pathToTile.empty())
	{
		m_movementTimer.update(deltaTime);
		if (m_movementTimer.isExpired())
		{
			m_movementTimer.reset();
			m_currentPosition = m_pathToTile.front().second;
			m_movementPath.eraseNode(m_currentPosition, map);

			handleRotation(entityProperties);
			m_pathToTile.pop_front();

			if (m_pathToTile.empty())
			{
				m_movingToDestination = false;
				clearMovementPath();
			}
		}
	}
}

void EntityBattleProperties::render(std::shared_ptr<HAPISPACE::Sprite>& sprite, const Map & map)
{
	//Set sprite position to current position
	const std::pair<int, int> tileTransform = map.getTileScreenPos(m_currentPosition);
	float scale = map.getDrawScale();

	sprite->GetTransformComp().SetPosition({
		static_cast<float>(tileTransform.first + DRAW_ENTITY_OFFSET_X * scale),
		static_cast<float>(tileTransform.second + DRAW_ENTITY_OFFSET_Y * scale) });
	sprite->GetTransformComp().SetScaling({ scale / 2, scale / 2 });

	sprite->Render(SCREEN_SURFACE);
	m_movementPath.render(map);
	m_actionSprite.render(map, m_currentPosition);
}

//BATTLE PLAYER
BattlePlayer::BattlePlayer(FactionName name, std::pair<int, int> spawnPosition, ePlayerType playerType)
	: m_entities(),
	m_factionName(name),
	m_playerType(playerType),
	m_spawnPosition(spawnPosition),
	m_eliminated(false)
{}

EntityBattleProperties::ActionSprite::ActionSprite(FactionName factionName)
	: sprite(),
	active(false)
{
	switch (factionName)
	{
	case FactionName::eYellow :
		sprite = HAPI_Sprites.MakeSprite(Textures::m_yellowSpawnHex);
		break;
	case FactionName::eBlue :
		sprite = HAPI_Sprites.MakeSprite(Textures::m_blueSpawnHex);
		break;
	case FactionName::eGreen :
		sprite = HAPI_Sprites.MakeSprite(Textures::m_greenSpawnHex);
		break;
	case FactionName::eRed :
		sprite = HAPI_Sprites.MakeSprite(Textures::m_redSpawnHex);
		break;
	}

	sprite->GetTransformComp().SetOriginToCentreOfFrame();
	sprite->GetTransformComp().SetScaling({ 2.f, 2.f });
}

void EntityBattleProperties::ActionSprite::render(const Map& map, std::pair<int, int> currentEntityPosition) const
{
	if (active)
	{
		auto screenPosition = map.getTileScreenPos(currentEntityPosition);
		sprite->GetTransformComp().SetPosition({
		(float)screenPosition.first + DRAW_ENTITY_OFFSET_X * map.getDrawScale() ,
		(float)screenPosition.second + DRAW_ENTITY_OFFSET_Y * map.getDrawScale() });
		sprite->Render(SCREEN_SURFACE);
	}
}


Player::Player(FactionName name, ePlayerType playerType)
	: m_entities(assignEntities(name)),
	m_selectedEntities(),
	m_factionName(name),
	m_type(playerType)
{}
//
//Player::Player(FactionName name, ePlayerType playerType)
//	: m_factionName(name),
//	m_type(playerType)
//{
//}
