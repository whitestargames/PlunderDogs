#include "Battle.h"
#include "Utilities/MapParser.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"

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
		entity->m_battleProperties.render(entity->m_entity.m_sprite, m_map);
	}

	m_battleUI.render();
}

void Battle::update(float deltaTime)
{
	for (auto& entity : m_entities)
	{
		entity->m_battleProperties.update(deltaTime, m_map);
	}
}

void Battle::moveEntityToPosition(BattleEntity& entity, const Tile& destination)
{
	entity.m_battleProperties.moveEntity(m_map, destination, entity.m_entity.m_movementPoints);
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