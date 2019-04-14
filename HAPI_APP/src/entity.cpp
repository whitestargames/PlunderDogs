#include "entity.h"
#include "Map.h"
#include "HAPIWrapper.h"
#include "Pathfinding.h"
#include "Utilities/Utilities.h"
#include "Textures.h"

constexpr size_t MOVEMENT_PATH_SIZE{ 32 };
constexpr size_t WEAPON_HIGHLIGHT_SIZE{ 40 };
constexpr float DRAW_ENTITY_OFFSET_X{ 16 };
constexpr float DRAW_ENTITY_OFFSET_Y{ 32 };

//ENTITY DETAILS
EntityBattleProperties::EntityBattleProperties(std::pair<int, int> startingPosition)
	: m_currentPosition(startingPosition),
	m_pathToTile(),
	m_movementTimer(0.35f),
	m_movedToDestination(false),
	m_movementPath(),
	m_movementPathSize(0),
	m_direction(eDirection::eNorth)
{}

//MOVEMENT PATH NODE
EntityBattleProperties::MovementPath::MovementPathNode::MovementPathNode()
	: sprite(std::make_unique<Sprite>(Textures::m_mouseCrossHair)),
	activate(false)
{}

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

void EntityBattleProperties::MovementPath::render() const
{
	for (const auto& i : m_movementPath)
	{
		if (i.activate)
		{
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
	int prevDir = source.m_entityOnTile->m_battleProperties.m_direction;
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
			std::cout << prevDir << "\n";
		}

		source.m_entityOnTile->m_battleProperties.m_movementPathSize = i;
		if (movementPointsUsed <=
			source.m_entityOnTile->m_entityProperties.m_movementPoints )
		{
			auto tileScreenPosition = map.getTileScreenPos(pathToTile[i].second);
			m_movementPath[i - 1].sprite->GetTransformComp().SetPosition({
				static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * map.getDrawScale()),
				static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * map.getDrawScale()) });
			m_movementPath[i - 1].activate = true;

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

void EntityBattleProperties::generateWeaponArea(const Map & map, const Tile & source)
{
	m_weapon.generateTargetArea(map, source);
}

void EntityBattleProperties::clearMovementPath()
{
	m_movementPath.clearPath();
}

void EntityBattleProperties::moveEntity(Map& map, const Tile& tile, int movementPointsAvailable)
{
	if (!m_movedToDestination)
	{
		auto pathToTile = PathFinding::getPathToTile(map, m_currentPosition, tile.m_tileCoordinate);
		if (!pathToTile.empty() && pathToTile.size() <= movementPointsAvailable+1)
		{
			m_pathToTile = pathToTile;
			map.moveEntity(m_currentPosition, pathToTile.back().second);
			m_movedToDestination = false;
		}
		else
		{
			clearMovementPath();
			

		}
	}
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
EntityProperties::EntityProperties() 
	: m_sprite(HAPI_Sprites.MakeSprite(Textures::m_ship)),
	m_movementPoints(5),
	m_healthMax(20),
	m_currentHealth(20),
	m_range(4),
	m_damage(5)
{
	m_sprite->GetTransformComp().SetOrigin({ 13, 25 });
}

BattleEntity::BattleEntity(std::pair<int, int> startingPosition)
	: m_entityProperties(),
	m_battleProperties(startingPosition)
{}

void BattleEntity::setPosition(const Map & map)
{
	//Assign entity sprite position - TODO change this at later date
	std::pair<int, int> tileScreenPoint = map.getTileScreenPos(m_battleProperties.m_currentPosition);
	m_entityProperties.m_sprite->GetTransformComp().SetPosition({
		(float)(tileScreenPoint.first + DRAW_OFFSET_X * map.getDrawScale()),
		(float)(tileScreenPoint.second + DRAW_OFFSET_Y * map.getDrawScale()) });
}

void EntityBattleProperties::update(float deltaTime, const Map & map, EntityProperties& entityProperties)
{
	if (!m_pathToTile.empty())
	{
		m_movementTimer.update(deltaTime);
		if (m_movementTimer.isExpired())
		{
			m_movementTimer.reset();

			int directionToTurn = 0;
			int rotationAngle = 60;
			m_currentPosition = m_pathToTile.front().second;
			m_movementPath.eraseNode(m_currentPosition, map);
			directionToTurn = m_pathToTile.front().first;
			entityProperties.m_sprite->GetTransformComp().SetRotation(
				DEGREES_TO_RADIANS(directionToTurn*rotationAngle % 360));
			m_direction = (eDirection)directionToTurn;

			m_pathToTile.pop_front();

			if (m_pathToTile.empty())
			{
				m_movedToDestination = true;
			}
		}
	}
}

void EntityBattleProperties::render(std::shared_ptr<HAPISPACE::Sprite>& sprite, const Map & map)
{
	//Move entity sprite
	const std::pair<int, int> tileTransform = map.getTileScreenPos(m_currentPosition);
	sprite->GetTransformComp().SetPosition({
		static_cast<float>(tileTransform.first + DRAW_ENTITY_OFFSET_X * map.getDrawScale()),
		static_cast<float>(tileTransform.second + DRAW_ENTITY_OFFSET_Y * map.getDrawScale()) });

	//Render entity
	sprite->Render(SCREEN_SURFACE);
	m_movementPath.render();
	m_weapon.render();
}

EntityBattleProperties::Weapon::WeaponHighlightNode::WeaponHighlightNode() //creating highlight node 
	: sprite(std::make_unique<Sprite>(Textures::m_mouseCrossHair)),
	activate(false){}

EntityBattleProperties::Weapon::Weapon() //populating vector with inactive nodes
	: m_spriteTargetStorage()
{
	m_spriteTargetStorage.reserve(size_t(WEAPON_HIGHLIGHT_SIZE));
	for (int i = 0; i < WEAPON_HIGHLIGHT_SIZE; ++i)
	{
		m_spriteTargetStorage.push_back({});
	}
}

void EntityBattleProperties::Weapon::render() const
{
	for (const auto& i : m_spriteTargetStorage)
	{
		if (i.activate)
		{
			i.sprite->Render(SCREEN_SURFACE);
		}
	}
}

void EntityBattleProperties::Weapon::generateTargetArea(const Map &map, const Tile& source)
{   //variable below stores the tile cones coming from the ship 
	auto m_tempTargetArea = map.getTileCone(source.m_tileCoordinate,source.m_entityOnTile->m_entityProperties.m_range,source.m_entityOnTile->m_battleProperties.m_direction);
	if (m_tempTargetArea.empty())
	{
		return;
	}
	clearGunArea();
	for (int i = 0; i < m_tempTargetArea.size(); i++)
	{
		std::pair<int,int>tilePos = map.getTileScreenPos(m_tempTargetArea[i]->m_tileCoordinate);
		m_spriteTargetStorage[i].sprite->GetTransformComp().SetPosition(
		{
		 tilePos.first + DRAW_ENTITY_OFFSET_X * map.getDrawScale(),
		 tilePos.second + DRAW_ENTITY_OFFSET_X * map.getDrawScale()
		});
		m_spriteTargetStorage[i].activate = true;
	}
}

void EntityBattleProperties::Weapon::clearGunArea()
{
	for (auto& i : m_spriteTargetStorage)
	{
		i.activate = false;
	}
}