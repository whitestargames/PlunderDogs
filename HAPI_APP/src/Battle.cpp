#include "Battle.h"
#include "Utilities/MapParser.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"

using namespace HAPISPACE;

Battle::Battle() :
	m_entities(),
	m_map(MapParser::parseMap("Level1.tmx")),
	m_battleUI(*this),
	m_currentPhase(BattlePhase::Movement)
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
	if (m_currentPhase == BattlePhase::Movement)
	{
		updateMovementPhase(deltaTime);
	}
}

void Battle::moveEntityToPosition(BattleEntity& entity, const Tile& destination)
{
	entity.m_battleProperties.moveEntity(m_map, destination, entity.m_entity.m_movementPoints);
}

void Battle::activateEntityWeapon(BattleEntity & entity)
{
	//TODO: implement attack enemy stuff
}

void Battle::insertEntity(std::pair<int, int> startingPosition)
{
	auto entity = std::make_unique<BattleEntity>(startingPosition);

	entity->setPosition(m_map);
	m_entities.push_back(std::move(entity));
	m_map.insertEntity(*m_entities.back());
}

void Battle::changePhase(BattlePhase newPhase)
{
	m_currentPhase = newPhase;
}

void Battle::updateMovementPhase(float deltaTime)
{
	bool allEntitiesReachedDestination = true;
	for (auto& entity : m_entities)
	{
		entity->m_battleProperties.update(deltaTime, m_map);

		allEntitiesReachedDestination = entity->m_battleProperties.m_movedToDestination;
	}

	//When all entities have reachd their destination.
	//For now go back to the movement phase.
	if (allEntitiesReachedDestination)
	{
		changePhase(BattlePhase::Movement);

		for (auto& entity : m_entities)
		{
			entity->m_battleProperties.m_movedToDestination = false;
		}
	}
}

const Map & Battle::getMap() const
{
	return m_map;
}

BattlePhase Battle::getCurrentPhase() const
{
	return m_currentPhase;
}