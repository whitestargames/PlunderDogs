#include "Battle.h"
#include "Utilities/MapParser.h"

using namespace HAPISPACE;

Battle::Battle(std::vector<std::pair<FactionName, std::vector<EntityProperties*>>>& players)
	: m_players(),
	m_currentPlayersTurn(0),
	m_map(MapParser::parseMap("Level1.tmx")),
	m_currentPhase(BattlePhase::ShipPlacement),
	m_battleUI(*this)
{
	for (auto& player : players)
	{
		m_players.emplace_back(player.first);
	}

	m_battleUI.startShipPlacement(players);
}

void Battle::render() const
{
	m_map.drawMap();
	
	m_battleUI.renderUI();

	for (auto& player : m_players)
	{
		for (auto& entity : player.m_entities)
		{
			entity->m_battleProperties.render(entity->m_entityProperties.m_sprite, m_map);
		}
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
	if (tileOnAttackPosition.m_entityOnTile && tileOnAttackPosition.m_entityOnTile->m_factionName != getCurentFaction()
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

void Battle::insertEntity(std::pair<int, int> startingPosition, const EntityProperties& entityProperties, FactionName factionName)
{
	assert(m_currentPhase == BattlePhase::ShipPlacement);

	auto& player = getPlayer(factionName);
	switch (factionName)
	{
	case FactionName::Yellow :
		player.m_entities.push_back(std::make_unique<BattleEntity>(startingPosition, entityProperties, m_map, factionName));
		break;

	case FactionName::Blue:
		player.m_entities.push_back(std::make_unique<BattleEntity>(startingPosition, entityProperties, m_map, factionName));
		break;
	
	case FactionName::Red :
		player.m_entities.push_back(std::make_unique<BattleEntity>(startingPosition, entityProperties, m_map, factionName));
		break;
	
	case FactionName::Green :
		player.m_entities.push_back(std::make_unique<BattleEntity>(startingPosition, entityProperties, m_map, factionName));
		break;
	}
}

void Battle::nextTurn()
{

	m_moveCounter.m_counter = 0;
	m_battleUI.GetCurrentFaction(getCurentFaction());
	
	//Notify all players new turn has started

	for (auto& player : m_players)
	{
		for (auto& entity : player.m_entities)
		{
			entity->m_battleProperties.onNewTurn();
		}
	}

	//Handle ship placement phase
	if (m_currentPhase == BattlePhase::ShipPlacement)
	{
		++m_currentPlayersTurn;
		
		if (m_currentPlayersTurn == static_cast<int>(m_players.size()))
		{
			m_currentPhase = BattlePhase::Movement;
			m_currentPlayersTurn = 0;
			return;
		}
		m_battleUI.newTurn(getCurentFaction());
	}

	if (m_currentPhase == BattlePhase::Movement)
	{
		m_currentPhase = BattlePhase::Attack;
	}
	else if (m_currentPhase == BattlePhase::Attack)
	{
		m_currentPhase = BattlePhase::Movement;
		++m_currentPlayersTurn;
	}

	if (m_currentPlayersTurn == m_players.size())
	{
		m_currentPlayersTurn = 0;
	}

}

void Battle::updateMovementPhase(float deltaTime)
{
	int totalAliveEntities = 0;
	for (auto& entity : m_players[m_currentPlayersTurn].m_entities)
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

void Battle::updateAttackPhase()
{
	if (allEntitiesAttacked(m_players[m_currentPlayersTurn].m_entities))
	{
		m_battleUI.FactionUpdateGUI();
		nextTurn();
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

BattlePlayer & Battle::getPlayer(FactionName factionName)
{
	auto cIter = std::find_if(m_players.begin(), m_players.end(), [factionName](const auto& player) { return factionName == player.m_factionName; });
	assert(cIter != m_players.end());
	return *cIter;
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
	
	return m_players[m_currentPlayersTurn].m_factionName;
}