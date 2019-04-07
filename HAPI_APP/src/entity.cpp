#include "entity.h"
#include "Map.h"
#include "HAPIWrapper.h"
#include "Pathfinding.h"
#include "Utilities/Utilities.h"
#include "Textures.h"

constexpr size_t MOVEMENT_PATH_SIZE{ 32 };

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

	if (pathToTile.size() > source.m_entityOnTile->m_entity.m_movementPoints + 1)
	{
		//Don't interact with path from source.
		for (int i = 1; i < source.m_entityOnTile->m_entity.m_movementPoints + 1; ++i)
		{
			auto tileScreenPosition = map.getTileScreenPos(pathToTile[i]);
			m_movementPath[i - 1].sprite->GetTransformComp().SetPosition({
				static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * map.getDrawScale()),
				static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * map.getDrawScale()) });
			m_movementPath[i - 1].activate = true;
		}
	}
	else
	{
		//Don't interact with path from source.
		for (int i = 1; i < pathToTile.size(); ++i)
		{
			auto tileScreenPosition = map.getTileScreenPos(pathToTile[i]);
			m_movementPath[i - 1].sprite->GetTransformComp().SetPosition({
				static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * map.getDrawScale()),
				static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * map.getDrawScale()) });
			m_movementPath[i - 1].activate = true;
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
			map.moveEntity(m_currentPosition, pathToTile.back());
			m_movedToDestination = false;
		}
		else
		{
			clearMovementPath();
		}
	}
}

//ENTITY
EntityProperties::EntityProperties()
	: m_sprite(std::make_unique<Sprite>(Textures::m_thing)),
	m_movementPoints(15)
{}

BattleEntity::BattleEntity(std::pair<int, int> startingPosition)
	: m_entity(),
	m_battleProperties(startingPosition)
{}

void BattleEntity::setPosition(const Map & map)
{
	//Assign entity sprite position - TODO change this at later date
	std::pair<int, int> tileScreenPoint = map.getTileScreenPos(m_battleProperties.m_currentPosition);
	m_entity.m_sprite->GetTransformComp().SetPosition({
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

			m_currentPosition = m_pathToTile.front();
			m_movementPath.eraseNode(m_currentPosition, map);

			m_pathToTile.pop_front();
			if (m_pathToTile.empty())
			{
				m_movedToDestination = true;
			}
		}
	}
}

void EntityBattleProperties::render(std::unique_ptr<HAPISPACE::Sprite>& sprite, const Map & map) const
{
	//Move entity sprite
	const std::pair<int, int> tileTransform = map.getTileScreenPos(m_currentPosition);
	sprite->GetTransformComp().SetPosition({
		static_cast<float>(tileTransform.first + DRAW_OFFSET_X * map.getDrawScale()),
		static_cast<float>(tileTransform.second + DRAW_OFFSET_Y * map.getDrawScale()) });

	//Render entity
	sprite->Render(SCREEN_SURFACE);
	m_movementPath.render();
}
