#include "Battle.h"
#include "Utilities/MapParser.h"
#include "Utilities/Utilities.h"
#include "entity.h"
#include "Pathfinding.h"
#include "OverWorld.h"
#include "HAPIWrapper.h"
#include "Timer.h"
#include <utility>

using namespace HAPISPACE;

Battle::Battle() :
	m_entities(),
	m_map(MapParser::parseMap("Level1.tmx")),
	m_battleUI(*this)
{
	insertEntity({ 5, 5 });
	insertEntity({ 4, 4 });
	insertEntity({ 8, 8 });
}

void Battle::render()
{
	m_map.drawMap();
	
	for (const auto& entity : m_entities)
	{
		entity->m_entity.render(m_map, entity->m_battleProperties);
	}

	m_battleUI.render();
}

void Battle::update(float deltaTime)
{
	for (auto& entity : m_entities)
	{
		entity->m_entity.update(deltaTime, entity->m_battleProperties, m_map);
	}
}

void Battle::moveEntityTo(BattleEntity* entity, Tile & destination)
{
	if (!entity->m_battleProperties.m_moving)
	{
		auto pathToTile = PathFinding::getPathToTile(m_map, entity->m_battleProperties.m_currentPosition, destination.m_tileCoordinate);
		//Able to move entity to new position
		if (!pathToTile.empty() && pathToTile.size() <= entity->m_entity.m_movementPoints + 1)
		{
			entity->m_battleProperties.m_moving = true;
			entity->m_battleProperties.m_pathToTile = pathToTile;
			destination.m_destinationOfEntity = true;
		}
		else
		{
			entity->m_battleProperties.clearMovementPath();
		}
	}
}

void Battle::insertEntity(std::pair<int, int> startingPosition)
{
	auto entity = std::make_unique<BattleEntity>(startingPosition, "thingy.xml");

	//Assign entity sprite position - TODO change this at later date
	std::pair<int, int> tileScreenPoint = m_map.getTileScreenPos(entity->m_battleProperties.m_currentPosition);
	entity->m_entity.m_sprite->GetTransformComp().SetPosition({
		(float)(tileScreenPoint.first + DRAW_OFFSET_X * m_map.getDrawScale()),
		(float)(tileScreenPoint.second + DRAW_OFFSET_Y * m_map.getDrawScale()) });
	
	m_entities.push_back(std::move(entity));
	m_map.insertEntity(*m_entities.back());
}

Map & Battle::getMap()
{
	return m_map;
}