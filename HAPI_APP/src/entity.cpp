#include "entity.h"
#include "Map.h"
#include "HAPIWrapper.h"
#include "Pathfinding.h"
#include "Utilities/Utilities.h"

constexpr size_t MOVEMENT_PATH_SIZE{ 32 };

//ENTITY DETAILS
EntityBattleProperties::EntityBattleProperties(std::pair<int, int> startingPosition)
	: m_currentPosition(startingPosition),
	m_oldPosition(startingPosition),
	m_pathToTile(),
	m_movementTimer(0.35f),
	m_movedToDestination(false),
	m_movementPath()
{}

//MOVEMENT PATH NODE
EntityBattleProperties::MovementPath::MovementPathNode::MovementPathNode()
	: sprite(),
	render(false)
{}

//
//MOVEMENT PATH
//
EntityBattleProperties::MovementPath::MovementPath()
	: m_movementPath(),
	m_mouseCursor(HAPI_Wrapper::loadSprite("mouseCrossHair.xml"))
{
	m_movementPath.reserve(size_t(MOVEMENT_PATH_SIZE));
	for (int i = 0; i < MOVEMENT_PATH_SIZE; ++i)
	{
		m_movementPath.push_back({});
		m_movementPath.back().sprite = HAPI_Sprites.MakeSprite(m_mouseCursor->GetSpritesheet());
	}
}

void EntityBattleProperties::MovementPath::render() const
{
	for (const auto& i : m_movementPath)
	{
		if (i.render)
		{
			i.sprite->Render(SCREEN_SURFACE);
		}
	}
}

void EntityBattleProperties::MovementPath::generatePath(Map& map, const Tile& source, const Tile& destination)
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
			m_movementPath[i - 1].render = true;
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
			m_movementPath[i - 1].render = true;
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
			iter->render = false;
		}
	}
}

void EntityBattleProperties::MovementPath::clearPath()
{
	for (auto& i : m_movementPath)
	{
		i.render = false;
	}
}

void EntityBattleProperties::generateMovementGraph(Map & map, const Tile & source, const Tile & destination)
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
			m_movedToDestination = true;
			m_pathToTile = pathToTile;
			m_oldPosition = m_currentPosition;
			map.moveEntity(m_oldPosition, pathToTile.back());
		}
		else
		{
			clearMovementPath();
		}
	}
}

//ENTITY
EntityProperties::EntityProperties(const std::string & spriteName)
	: m_sprite(HAPI_Sprites.LoadSprite(Utilities::getDataDirectory() + spriteName)),
	m_movementPoints(15)
{}

//void Entity::render(Map & map, const EntityBattleProperties & entityDetails)
//{
//	//Move entity sprite
//	const std::pair<int, int> tileTransform = map.getTileScreenPos(entityDetails.m_currentPosition);
//	m_sprite->GetTransformComp().SetPosition({
//		static_cast<float>(tileTransform.first + DRAW_OFFSET_X * map.getDrawScale()),
//		static_cast<float>(tileTransform.second + DRAW_OFFSET_Y * map.getDrawScale()) });
//	//Render entity
//	m_sprite->Render(SCREEN_SURFACE);
//
//	entityDetails.m_movementPath.render();
//}

BattleEntity::BattleEntity(std::pair<int, int> startingPosition, const std::string & spriteName)
	: m_entity(spriteName),
	m_battleProperties(startingPosition)
{}

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
		}
	}
}

void EntityBattleProperties::render(std::unique_ptr<HAPISPACE::Sprite>& sprite, const Map & map)
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