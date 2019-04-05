#include "Battle.h"
#include "Utilities/MapParser.h"
#include "Utilities/Utilities.h"
#include "entity.h"
#include "Pathfinding.h"
#include "OverWorld.h"
#include "HAPIWrapper.h"
#include "Timer.h"

using namespace HAPISPACE;

Battle::Battle() :
	m_entity(),
	m_map(MapParser::parseMap("Level1.tmx")),
	m_battleUI(*this)
{	
	//Initialize Entity
	m_entity.first = std::make_unique<Entity>(Utilities::getDataDirectory() + "thingy.xml");
	m_entity.second.m_currentPosition = std::make_pair<int, int>(5, 5);
	m_entity.second.m_oldPosition = std::make_pair<int, int>(5, 5);
	//Assign entity sprite position
	std::pair<int, int> tileScreenPoint = m_map.getTileScreenPos(m_entity.second.m_currentPosition);
	m_entity.first->m_sprite->GetTransformComp().SetPosition({
		(float)(tileScreenPoint.first + DRAW_OFFSET_X * m_map.getDrawScale()),
		(float)(tileScreenPoint.second + DRAW_OFFSET_Y * m_map.getDrawScale()) });
	//Insert entity into map
	m_map.insertEntity(m_entity);
}

void Battle::render()
{
	m_map.drawMap();
	m_entity.first->render(m_map, m_entity.second);
}

void Battle::update(float deltaTime)
{
	m_entity.first->update(deltaTime, m_entity.second, m_map);
}

void Battle::moveEntityTo(std::pair<std::unique_ptr<Entity>, BattleProperties>& entity, Tile & destination)
{
	if (!entity.second.m_moving)
	{
		auto pathToTile = PathFinding::getPathToTile(m_map, entity.second.m_currentPosition, destination.m_tileCoordinate);
		if (!pathToTile.empty() && pathToTile.size() <= entity.first->m_movementPoints + 1)
		{
			entity.second.m_moving = true;
			entity.second.m_pathToTile = pathToTile;
		}
	}
}

Map & Battle::getMap()
{
	return m_map;
}