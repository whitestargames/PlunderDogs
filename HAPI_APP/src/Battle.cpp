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
	//ool allEntitiesMoved = true;
	for (auto& entity : m_entities)
	{
		//if (!entity->m_battleProperties.m_movedToDestination)
		//{
		//	allEntitiesMoved = false;
		//}

		entity->m_battleProperties.update(deltaTime, m_map);
	}

	//if (allEntitiesMoved)
	//{
	//	//m_currentPhase = Attack;
	//}
}

void Battle::moveEntityToPosition(BattleEntity& entity, const Tile& destination)
{
	entity.m_battleProperties.moveEntity(m_map, destination, entity.m_entity.m_movementPoints);
}

void Battle::insertEntity(std::pair<int, int> startingPosition)
{
	auto entity = std::make_unique<BattleEntity>(startingPosition);

	entity->setPosition(m_map);
	m_entities.push_back(std::move(entity));
	m_map.insertEntity(*m_entities.back());
}

const Map & Battle::getMap() const
{
	return m_map;
}