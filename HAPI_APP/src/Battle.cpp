#include "Battle.h"
#include "Utilities/MapParser.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"

using namespace HAPISPACE;


Battle::Battle(std::deque<EntityProperties*>& selectedEntities) :
	m_entities(),
	m_map(MapParser::parseMap("Level1.tmx")),
	m_battleUI(*this, selectedEntities),
	m_currentPhase(BattlePhase::ShipPlacement)
{
	/*insertEntity({ 5, 15 });
	insertEntity({ 4, 4 });
	insertEntity({ 8, 8 });*/
}

void Battle::render() const
{
	m_map.drawMap();
	
	for (const auto& entity : m_entities)
	{
		entity->m_battleProperties.render(entity->m_entityProperties.m_sprite, m_map);
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
	entity.m_battleProperties.moveEntity(m_map, destination, entity.m_battleProperties.m_movementPathSize);
	
}

void Battle::activateEntityWeapon(BattleEntity & entity)
{
	//TODO: implement attack enemy stuff

}

void Battle::insertEntity(std::pair<int, int> startingPosition, const EntityProperties& entityProperties)
{
	m_entities.push_back(std::make_unique<BattleEntity>(startingPosition, entityProperties, m_map));
}

void Battle::nextPhase()
{
	switch (m_currentPhase)
	{
	case BattlePhase::ShipPlacement :
	{
		m_currentPhase = BattlePhase::Movement;
		break;
	}
	case BattlePhase::Movement :
	{
		m_currentPhase = BattlePhase::Attack;
		break;
	}
	case BattlePhase::Attack :
	{
		m_currentPhase = BattlePhase::Movement;
		break;
	}
	}
}

void Battle::updateMovementPhase(float deltaTime)
{
	bool allEntitiesReachedDestination = true;
	for (auto& entity : m_entities)
	{
		entity->m_battleProperties.update(deltaTime, m_map, entity->m_entityProperties);
		if (!entity->m_battleProperties.m_movedToDestination)
		{
			allEntitiesReachedDestination = false;
		}
	}

	if (allEntitiesReachedDestination)
	{
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

void Battle::start()
{
	assert(m_currentPhase == BattlePhase::ShipPlacement);
}