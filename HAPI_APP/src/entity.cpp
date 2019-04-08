#include "entity.h"
#include "Map.h"
#include "HAPIWrapper.h"
#include "Pathfinding.h"
#include "Utilities/Utilities.h"
#include "Textures.h"

constexpr size_t MOVEMENT_PATH_SIZE{ 32 };
constexpr float DRAW_ENTITY_OFFSET_X{ 16 };
constexpr float DRAW_ENTITY_OFFSET_Y{ 32 };

//ENTITY DETAILS
EntityBattleProperties::EntityBattleProperties(std::pair<int, int> startingPosition)
	: m_currentPosition(startingPosition),
	m_pathToTile(),
	m_movementTimer(0.35f),
	m_movedToDestination(false),
	m_movementPath()
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
	: m_movementPath(),
	m_movementPointsUsed(0),
	m_currentMovementRotation(0)
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

	
	if (m_movementPointsUsed > source.m_entityOnTile->m_entityProperties.m_movementPoints + 1)
	{
		m_movementPointsUsed = 0;
		//Don't interact with path from source.
		for (int i = 1; i < source.m_entityOnTile->m_entityProperties.m_movementPoints + 1; ++i)
		{
			auto tileScreenPosition = map.getTileScreenPos(pathToTile[i].second);
			m_movementPath[i - 1].sprite->GetTransformComp().SetPosition({
				static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * map.getDrawScale()),
				static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * map.getDrawScale()) });
			m_movementPath[i - 1].activate = true;

			++m_movementPointsUsed;
			int entityDir = source.m_entityOnTile->m_entityProperties.m_direction;
			int pathDir = pathToTile[i].first;
			m_currentMovementRotation = pathToTile[1].first;
			int movementCost = getDirectionCost(entityDir, pathDir);

			source.m_entityOnTile->m_entityProperties.m_direction = (eDirection)pathDir;
			m_movementPointsUsed += movementCost;

			int bonusMove = 0;

			if (source.m_entityOnTile->m_entityProperties.m_direction == map.getWindDirection() && bonusMove == 0)
			{
				bonusMove = (int)source.m_entityOnTile->m_entityProperties.m_movementPoints * map.getWindStrength();
				m_movementPointsUsed -= bonusMove;
			}
		}


	}
	else
	{
		
		//Don't interact with path from source.
		for (int i = 1; i < pathToTile.size(); ++i)
		{
			auto tileScreenPosition = map.getTileScreenPos(pathToTile[i].second);
			m_movementPath[i - 1].sprite->GetTransformComp().SetPosition({
				static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * map.getDrawScale()),
				static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * map.getDrawScale()) });
			m_movementPath[i - 1].activate = true;

			++m_movementPointsUsed;
			int entityDir = source.m_entityOnTile->m_entityProperties.m_direction;
			int pathDir = pathToTile[i].first;
			m_currentMovementRotation = pathToTile[1].first;
			int movementCost = getDirectionCost(entityDir, pathDir);

			source.m_entityOnTile->m_entityProperties.m_direction = (eDirection)pathDir;
			m_movementPointsUsed += movementCost;

			int bonusMove = 0;

			if (source.m_entityOnTile->m_entityProperties.m_direction == map.getWindDirection() && bonusMove == 0)
			{
				bonusMove = (int)source.m_entityOnTile->m_entityProperties.m_movementPoints * map.getWindStrength();
				m_movementPointsUsed -= bonusMove;
			}
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

void EntityBattleProperties::moveEntity(Map& map, const Tile& tile, int movementPoints)
{
	if (!m_movedToDestination)
	{
		auto pathToTile = PathFinding::getPathToTile(map, m_currentPosition, tile.m_tileCoordinate);
		if (!pathToTile.empty() && pathToTile.size() <= movementPoints + 1)
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
	return (static_cast<int>(eDirection::eNorthWest) % diff) + 1;
}

//ENTITY
EntityProperties::EntityProperties() 
	: m_sprite(HAPI_Sprites.MakeSprite(Textures::m_ship)),
	m_movementPoints(5),
	m_healthMax(20),
	m_currentHealth(20),
	m_range(4),
	m_damage(5),
	m_direction(eDirection::eNorth)
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

void EntityBattleProperties::update(float deltaTime, const Map & map)
{
	if (!m_pathToTile.empty())
	{
		m_movementTimer.update(deltaTime);
		if (m_movementTimer.isExpired())
		{
			m_movementTimer.reset();

			m_currentPosition = m_pathToTile.front().second;
			m_movementPath.eraseNode(m_currentPosition, map);

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
}
