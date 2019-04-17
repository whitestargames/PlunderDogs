#include "Battle.h"
#include "Utilities/MapParser.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"

using namespace HAPISPACE;


Battle::Battle(std::vector<EntityProperties*>& selectedEntities) :
	m_entities(),
	m_map(MapParser::parseMap("TropicalIslands.tmx")),
	m_battleUI(*this, selectedEntities),
	m_currentPhase(BattlePhase::ShipPlacement)
{
	insertEntity({ 5, 15 }, *selectedEntities[0]);
	insertEntity({ 4, 4 }, *selectedEntities[1]);
	insertEntity({ 8, 8 }, *selectedEntities[2]);
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
	m_battleUI.update();
	m_map.setDrawOffset(m_battleUI.getCameraPositionOffset());

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
	auto entity = std::make_unique<BattleEntity>(startingPosition, entityProperties, m_map);
	entity->m_entityProperties.m_sprite->GetTransformComp().SetOrigin({ 13, 25 });
	entity->m_entityProperties.m_sprite->GetTransformComp().SetScaling({ 1,1 });

	m_entities.push_back(std::move(entity));
}

void Battle::changePhase(BattlePhase newPhase)
{
	m_currentPhase = newPhase;
}

void Battle::updateMovementPhase(float deltaTime)
{
	int entityReachedDestination = 0;
	for (auto& entity : m_entities)
	{
		entity->m_battleProperties.update(deltaTime, m_map, entity->m_entityProperties);
		if (entity->m_battleProperties.m_movedToDestination)
		{
			++entityReachedDestination;
		}
	}

	if (entityReachedDestination == static_cast<int>(m_entities.size()))
	{
		for (auto& entity : m_entities)
		{
			entity->m_battleProperties.m_movedToDestination = false;
		}

		changePhase(BattlePhase::Movement);
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