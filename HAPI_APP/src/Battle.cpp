#include "Battle.h"
#include "Utilities/MapParser.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"
#include "Global.h"

using namespace HAPISPACE;

Battle::Battle(std::vector<EntityProperties*>& player1, std::vector<EntityProperties*>& player2) 
	: m_player1Entities(),
	m_player2Entities(),
	m_map(MapParser::parseMap("Level1.tmx")),
	m_currentPhase(BattlePhase::ShipPlacement),
	m_currentFaction(FactionName::Yellow),
	m_battleUI(*this, player1, player2)
{}

void Battle::render() const
{
	m_map.drawMap();
	
	m_battleUI.renderUI();
	for (const auto& entity : m_player1Entities)
	{
		entity->m_battleProperties.render(entity->m_entityProperties.m_sprite, m_map);
	}

	for (const auto& entity : m_player2Entities)
	{
		entity->m_battleProperties.render(entity->m_entityProperties.m_sprite, m_map);
	}

	m_battleUI.renderGUI();
}

void Battle::update(float deltaTime)
{
	m_battleUI.update();
	m_map.setDrawOffset(m_battleUI.getCameraPositionOffset());

	if (m_currentPhase == BattlePhase::Movement)
	{
		updateMovementPhase(deltaTime);
	}
	else if (m_currentPhase == BattlePhase::Attack)
	{
		updateAttackPhase();
	}
}

void Battle::moveEntityToPosition(BattleEntity& entity, const Tile& destination)
{
	assert(m_currentPhase == BattlePhase::Movement);
	entity.m_battleProperties.moveEntity(m_map, destination);
}

void Battle::fireEntityWeaponAtPosition(BattleEntity& player, const Tile& tileOnAttackPosition, const std::vector<const Tile*>& targetArea)
{
	assert(m_currentPhase == BattlePhase::Attack);
	assert(!player.m_battleProperties.isWeaponFired());
	player.m_battleProperties.fireWeapon();

	//Disallow attacking same team
	if (tileOnAttackPosition.m_entityOnTile && tileOnAttackPosition.m_entityOnTile->m_factionName != m_currentFaction
			&& !tileOnAttackPosition.m_entityOnTile->m_battleProperties.isDead())
	{
		//Find entity 
		auto tileCoordinate = tileOnAttackPosition.m_entityOnTile->m_battleProperties.getCurrentPosition();
		auto cIter = std::find_if(targetArea.cbegin(), targetArea.cend(), [tileCoordinate](const auto& tile) { return tileCoordinate == tile->m_tileCoordinate; });
		//Enemy within range of weapon
		if (cIter != targetArea.cend())
		{
			auto& enemy = tileOnAttackPosition.m_entityOnTile;
			enemy->m_battleProperties.takeDamage(enemy->m_entityProperties, player.m_entityProperties.m_damage, enemy->m_factionName);
		}
	}
}

void Battle::insertEntity(std::pair<int, int> startingPosition, const EntityProperties& entityProperties, FactionName playerName)
{
	assert(m_currentPhase == BattlePhase::ShipPlacement);
	switch (playerName)
	{
	case FactionName::Yellow :
		m_player1Entities.push_back(std::make_unique<BattleEntity>(startingPosition, entityProperties, m_map, playerName));
		break;
	
	case FactionName::Blue:
		m_player2Entities.push_back(std::make_unique<BattleEntity>(startingPosition, entityProperties, m_map, playerName));
		break;
	
	}
}

void Battle::nextTurn()
{
	m_moveCounter.m_counter = 0;

	for (auto& entity : m_player1Entities)
	{
		entity->m_battleProperties.onNewTurn();
	}
	for (auto& entity : m_player2Entities)
	{
		entity->m_battleProperties.onNewTurn();
	}

	if (m_currentPhase == BattlePhase::ShipPlacement)
	{
		if (m_currentFaction == FactionName::Yellow)
		{
			m_currentFaction = FactionName::Blue;
			m_battleUI.newTurn(m_currentFaction);
		}
		else if (m_currentFaction == FactionName::Blue)
		{
			m_currentPhase = BattlePhase::Movement;
			m_currentFaction = FactionName::Yellow;
			m_battleUI.newPhase();
		}
		return;
	}

	//Player 1
	if (m_currentFaction == FactionName::Yellow && m_currentPhase == BattlePhase::Movement)
	{
		m_currentPhase = BattlePhase::Attack;
	}
	else if (m_currentFaction == FactionName::Yellow && m_currentPhase == BattlePhase::Attack)
	{
		m_currentFaction = FactionName::Blue;
		m_currentPhase = BattlePhase::Movement;
	}
	//Player 2
	else if (m_currentFaction == FactionName::Blue && m_currentPhase == BattlePhase::Movement)
	{
		m_currentPhase = BattlePhase::Attack;
	}
	else if (m_currentFaction == FactionName::Blue && m_currentPhase == BattlePhase::Attack)
	{
		m_currentFaction = FactionName::Yellow;
		m_currentPhase = BattlePhase::Movement;
	}
}

void Battle::updateMovementPhase(float deltaTime)
{
	if (m_currentFaction == FactionName::Yellow)
	{
		int totalAliveEntities = 0;
		for (auto& entity : m_player1Entities)
		{
			if (!entity->m_battleProperties.isDead())
			{
				++totalAliveEntities;
			}
			entity->m_battleProperties.update(deltaTime, m_map, entity->m_entityProperties, m_moveCounter);
		}

		if (m_moveCounter.m_counter >= totalAliveEntities)
		{
			nextTurn();
		}
	}
	else if(m_currentFaction == FactionName::Blue)
	{
		int totalAliveEntities = 0;
		for (auto& entity : m_player2Entities)
		{
			if (!entity->m_battleProperties.isDead())
			{
				++totalAliveEntities;
			}
			entity->m_battleProperties.update(deltaTime, m_map, entity->m_entityProperties, m_moveCounter);
		}

		if (m_moveCounter.m_counter >= totalAliveEntities)
		{
			nextTurn();
		}
	}
}

void Battle::updateAttackPhase()
{
	if (m_currentFaction == FactionName::Yellow)
	{
		if (allEntitiesAttacked(m_player1Entities))
		{
			nextTurn();
		}
	}
	else if (m_currentFaction == FactionName::Blue)
	{
		if (allEntitiesAttacked(m_player2Entities))
		{
			nextTurn();
		}
	}
}

bool Battle::allEntitiesAttacked(std::vector<std::unique_ptr<BattleEntity>>& playerEntities) const
{
	bool allEntitiesAttacked = true;
	for (const auto& entity : playerEntities)
	{
		if (!entity->m_battleProperties.isDead() && !entity->m_battleProperties.isWeaponFired())
		{
			allEntitiesAttacked = false;
		}
	}

	return allEntitiesAttacked;
}

const Map & Battle::getMap() const
{
	return m_map;
}

BattlePhase Battle::getCurrentPhase() const
{
	return m_currentPhase;
}

FactionName Battle::getCurentFaction() const
{
	return m_currentFaction;
}