#include "entity.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Textures.h"

constexpr size_t MOVEMENT_PATH_SIZE{ 32 };
constexpr size_t WEAPON_HIGHLIGHT_SIZE{ 200 };
constexpr float DRAW_ENTITY_OFFSET_X{ 16 };
constexpr float DRAW_ENTITY_OFFSET_Y{ 32 };

//ENTITY BATTLE PROPERTIES
EntityBattleProperties::EntityBattleProperties(std::pair<int, int> startingPosition)
	: m_currentPosition(startingPosition),
	m_pathToTile(),
	m_movementTimer(0.35f),
	m_movedToDestination(false),
	m_movementPath(),
	m_movementPathSize(0),
	m_currentDirection(eDirection::eNorth),
	m_weaponFired(false),
	m_isDead(false)
{}

eDirection EntityBattleProperties::getCurrentDirection() const
{
	return m_currentDirection;
}

bool EntityBattleProperties::isMovedToDestination() const
{
	return m_movedToDestination;
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

			i.sprite->GetTransformComp().SetPosition({
			static_cast<float>(tileTransform.first + DRAW_ENTITY_OFFSET_X * map.getDrawScale()),
			static_cast<float>(tileTransform.second + DRAW_ENTITY_OFFSET_Y * map.getDrawScale()) });

			i.sprite->Render(SCREEN_SURFACE);
		}	
	}
}

void EntityBattleProperties::MovementPath::generatePath(const Map& map, const Tile& source, const Tile& destination)
{
	auto pathToTile = PathFinding::getPathToTile(map, source.m_tileCoordinate, destination.m_tileCoordinate);
	if (pathToTile.empty())
	{
		return;
	}

	clearPath();

	int bonusMove = 0;
	int movementPointsUsed = 0;
	int prevDir = source.m_entityOnTile->m_battleProperties.m_currentDirection;
	//Don't interact with path from source.
	for (int i = 1; i < pathToTile.size(); ++i)
	{
		++movementPointsUsed;

		int pathDir = pathToTile[i].first;
		int movementCost = getDirectionCost(prevDir, pathDir);
		prevDir = pathDir;

		movementPointsUsed += movementCost;

		if (prevDir == map.getWindDirection() && bonusMove == 0)
		{
			bonusMove = (int)source.m_entityOnTile->m_entityProperties.m_movementPoints * map.getWindStrength();
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
			return;
		}
	}
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

void EntityBattleProperties::generateMovementGraph(const Map & map, const Tile & source, const Tile & destination)
{
	m_movementPath.generatePath(map, source, destination);
}

void EntityBattleProperties::clearMovementPath()
{
	m_movementPath.clearPath();
}

void EntityBattleProperties::moveEntity(Map& map, const Tile& tile)
{
	if (!m_movedToDestination)
	{
		auto pathToTile = PathFinding::getPathToTile(map, m_currentPosition, tile.m_tileCoordinate);
		if (!pathToTile.empty() && pathToTile.size() <= m_movementPathSize + 1)
		{
			m_pathToTile = pathToTile;
			map.moveEntity(m_currentPosition, pathToTile.back().second);
			m_movedToDestination = true;
		}
		else
		{
			clearMovementPath();
		}
	}
}

//TODO: Will change.
//Not sure how damage/health is going to be implemented yet. 
void EntityBattleProperties::takeDamage(EntityProperties & entityProperties, int damageAmount, FactionName entityFaction)
{
	entityProperties.m_currentHealth -= damageAmount;
	int currentHealth = entityProperties.m_currentHealth;
	auto& entitySprite = entityProperties.m_sprite;
	switch (entityFaction)
	{
	case FactionName::Blue :
		if (currentHealth == 2)
		{
			entitySprite->SetFrameNumber(eShipSpriteFrame::eMediumHealthBlue);
			entitySprite->GetTransformComp().SetOriginToCentreOfFrame(); 
		}
		else if (currentHealth == 1)
		{
			entitySprite->SetFrameNumber(eShipSpriteFrame::eLowHealthBlue);
			entitySprite->GetTransformComp().SetOriginToCentreOfFrame();
		}
		else if (currentHealth <= 0)
		{
			entitySprite->SetFrameNumber(eShipSpriteFrame::eDeadBlue);
			entitySprite->GetTransformComp().SetOriginToCentreOfFrame();
			m_isDead = true;
		}
		
		break;
	case FactionName::Yellow :
		if (currentHealth == 2)
		{
			entitySprite->SetFrameNumber(eShipSpriteFrame::eMediumHealthYellow);
			entitySprite->GetTransformComp().SetOriginToCentreOfFrame();
		}
		else if (currentHealth == 1)
		{
			entitySprite->SetFrameNumber(eShipSpriteFrame::eLowHealthYellow);
			entitySprite->GetTransformComp().SetOriginToCentreOfFrame();
		}
		else if (currentHealth <= 0)
		{
			entitySprite->SetFrameNumber(eShipSpriteFrame::eDeadYellow);
			entitySprite->GetTransformComp().SetOriginToCentreOfFrame();
			m_isDead = true;
		}
		break;
	case FactionName::Red :
		if (currentHealth == 2)
		{
			entitySprite->SetFrameNumber(eShipSpriteFrame::eMediumHealthRed);
			entitySprite->GetTransformComp().SetOriginToCentreOfFrame();
		}
		else if (currentHealth == 1)
		{
			entitySprite->SetFrameNumber(eShipSpriteFrame::eLowHealthRed);
			entitySprite->GetTransformComp().SetOriginToCentreOfFrame();
		}
		else if (currentHealth <= 0)
		{
			entitySprite->SetFrameNumber(eShipSpriteFrame::eDeadRed);
			entitySprite->GetTransformComp().SetOriginToCentreOfFrame();
			m_isDead = true;
		}
		break;
	case FactionName::Green :
		if (currentHealth == 2)
		{
			entitySprite->SetFrameNumber(eShipSpriteFrame::eMediumHealthGreen);
			entitySprite->GetTransformComp().SetOriginToCentreOfFrame();
		}
		else if (currentHealth == 1)
		{
			entitySprite->SetFrameNumber(eShipSpriteFrame::eLowHealthGreen);
			entitySprite->GetTransformComp().SetOriginToCentreOfFrame();
		}
		else if (currentHealth <= 0)
		{
			entitySprite->SetFrameNumber(eShipSpriteFrame::eDeadGreen);
			entitySprite->GetTransformComp().SetOriginToCentreOfFrame();
			m_isDead = true;
		}
		break;
	}
}

void EntityBattleProperties::fireWeapon()
{
	m_weaponFired = true;
}

void EntityBattleProperties::onNewTurn()
{
	m_movedToDestination = false;
	m_weaponFired = false;
}

void EntityBattleProperties::handleRotation(EntityProperties& entityProperties, const Map& map)
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
EntityProperties::EntityProperties(FactionName factionName)
	: m_sprite(HAPI_Sprites.MakeSprite(Textures::m_ship)),
	m_movementPoints(0),
	m_healthMax(0),
	m_currentHealth(0),
	m_range(0),
	m_damage(0),
	m_weaponType(eWeaponType::eSideCannons)
{
	//TODO: Currently not working as intended
	//UI seems to be resetting the frameNumber somewhere in OverWorldGUI. 
	switch (factionName)
	{
	case FactionName::Yellow :
		m_sprite->SetFrameNumber(eShipSpriteFrame::eMaxHealthYellow);
		m_sprite->GetTransformComp().SetOriginToCentreOfFrame();
		break;
	case FactionName::Blue :
		m_sprite->SetFrameNumber(eShipSpriteFrame::eMaxHealthBlue);
		m_sprite->GetTransformComp().SetOriginToCentreOfFrame();
		break;
	case FactionName::Red :
		m_sprite->SetFrameNumber(eShipSpriteFrame::eMaxHealthRed);
		m_sprite->GetTransformComp().SetOriginToCentreOfFrame();
		break;
	case FactionName::Green :
		m_sprite->SetFrameNumber(eShipSpriteFrame::eMaxHealthGreen);
		m_sprite->GetTransformComp().SetOriginToCentreOfFrame();
		break;
	}
}

BattleEntity::BattleEntity(std::pair<int, int> startingPosition, const EntityProperties& entityProperties, Map& map, FactionName playerName)
	: m_entityProperties(entityProperties),
	m_battleProperties(startingPosition),
	m_factionName(playerName)
{
	map.insertEntity(*this);
}

void EntityBattleProperties::update(float deltaTime, const Map & map, EntityProperties& entityProperties, MoveCounter& moveCounter)
{	
	if (!m_pathToTile.empty())
	{
		m_movementTimer.update(deltaTime);
		if (m_movementTimer.isExpired())
		{
			m_movementTimer.reset();
			m_currentPosition = m_pathToTile.front().second;
			m_movementPath.eraseNode(m_currentPosition, map);

			handleRotation(entityProperties, map);
			m_pathToTile.pop_front();

			if (m_pathToTile.empty())
			{
				//m_movedToDestination = true;
				++moveCounter.m_counter;
			}
		}
	}
}

void EntityBattleProperties::render(std::shared_ptr<HAPISPACE::Sprite>& sprite, const Map & map)
{
	//Set sprite position to current position
	const std::pair<int, int> tileTransform = map.getTileScreenPos(m_currentPosition);
	sprite->GetTransformComp().SetPosition({
		static_cast<float>(tileTransform.first + DRAW_ENTITY_OFFSET_X * map.getDrawScale()),
		static_cast<float>(tileTransform.second + DRAW_ENTITY_OFFSET_Y * map.getDrawScale()) });

	sprite->Render(SCREEN_SURFACE);

	m_movementPath.render(map);
}


//BATTLE PLAYER
BattlePlayer::BattlePlayer(FactionName name)
	: m_entities(),
	m_factionName(name)
{}
