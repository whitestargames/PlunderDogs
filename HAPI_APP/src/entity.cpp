#include "entity.h"
#include "Map.h"
#include "HAPIWrapper.h"
#include "Pathfinding.h"


//ENTITY DETAILS
EntityDetails::EntityDetails()
	: m_currentPosition(),
	m_oldPosition(),
	m_pathToTile(),
	m_movementTimer(0.35f),
	m_moving(false)
{}

//
//MOVEMENT PATH
//
Entity::MovementPath::MovementPath()
	: m_movementPath(),
	m_mouseCursor(HAPI_Wrapper::loadSprite("mouseCrossHair.xml"))
{
	m_movementPath.reserve(size_t(32));
	for (int i = 0; i < 32; ++i)
	{
		std::pair<std::unique_ptr<Sprite>, bool> sprite;
		sprite.first = HAPI_Sprites.MakeSprite(m_mouseCursor->GetSpritesheet());
		sprite.second = false;
		m_movementPath.push_back(std::move(sprite));
	}
}

void Entity::MovementPath::render() const
{
	for (const auto& i : m_movementPath)
	{
		if (i.second)
		{
			i.first->Render(SCREEN_SURFACE);
		}
	}
}

void Entity::MovementPath::generatePath(Map& map, const Tile& source, const Tile& destination)
{
	auto pathToTile = PathFinding::getPathToTile(map, source.m_tileCoordinate, destination.m_tileCoordinate);
	if (pathToTile.empty())
	{
		return;
	}

	clearPath();

	if (pathToTile.size() > source.m_entityOnTile->first->m_movementPoints + 1)
	{
		//Don't interact with path from source.
		for (int i = 1; i < source.m_entityOnTile->first->m_movementPoints + 1; ++i)
		{
			auto tileScreenPosition = map.getTileScreenPos(pathToTile[i]);
			m_movementPath[i - 1].first->GetTransformComp().SetPosition({
				static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * map.getDrawScale()),
				static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * map.getDrawScale()) });
			m_movementPath[i - 1].second = true;
		}
	}
	else
	{
		//Don't interact with path from source.
		for (int i = 1; i < pathToTile.size(); ++i)
		{
			auto tileScreenPosition = map.getTileScreenPos(pathToTile[i]);
			m_movementPath[i - 1].first->GetTransformComp().SetPosition({
				static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * map.getDrawScale()),
				static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * map.getDrawScale()) });
			m_movementPath[i - 1].second = true;
		}
	}
}

void Entity::MovementPath::eraseNode(std::pair<int, int> position, Map& map)
{
	for (auto iter = m_movementPath.begin(); iter != m_movementPath.end(); ++iter)
	{
		auto i = map.getMouseClickCoord({ iter->first->GetTransformComp().GetPosition().x, iter->first->GetTransformComp().GetPosition().y });
		if (i == position)
		{
			iter->second = false;
		}
	}
}

void Entity::MovementPath::clearPath()
{
	for (auto& i : m_movementPath)
	{
		i.second = false;
	}
}

//ENTITY
Entity::Entity(const std::string & spriteName)
	: m_sprite(HAPI_Sprites.LoadSprite(spriteName)),
	m_movementPoints(5)
{}

void Entity::update(float deltaTime, EntityDetails & entityDetails, Map& map)
{
	if (entityDetails.m_moving && !entityDetails.m_pathToTile.empty())
	{
		entityDetails.m_movementTimer.update(deltaTime);
		if (entityDetails.m_movementTimer.isExpired())
		{
			entityDetails.m_movementTimer.reset();

			entityDetails.m_oldPosition = entityDetails.m_currentPosition;
			entityDetails.m_currentPosition = entityDetails.m_pathToTile.front();
			m_movementPath.eraseNode(entityDetails.m_currentPosition, map);
			entityDetails.m_pathToTile.pop_front();
		
			map.moveEntity(entityDetails.m_oldPosition, entityDetails.m_currentPosition);
		}

		if (entityDetails.m_pathToTile.empty())
		{
			entityDetails.m_moving = false;
		}
	}
}

void Entity::render(Map & map, const EntityDetails & entityDetails)
{
	//Move entity sprite
	const std::pair<int, int> tileTransform = map.getTileScreenPos(entityDetails.m_currentPosition);
	m_sprite->GetTransformComp().SetPosition({
		static_cast<float>(tileTransform.first + DRAW_OFFSET_X * map.getDrawScale()),
		static_cast<float>(tileTransform.second + DRAW_OFFSET_Y * map.getDrawScale()) });
	//Render entity
	m_sprite->Render(SCREEN_SURFACE);

	m_movementPath.render();
}