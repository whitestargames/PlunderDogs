#include "Battle.h"
#include "Utilities/MapParser.h"
#include "GameEventMessenger.h"

using namespace HAPISPACE;

void Battle::setTimeOfDay(float deltaTime)
{
	m_dayTime.update(deltaTime);
	if (m_dayTime.isExpired())
	{
		int timeOfDay = (int)m_map.getTimeOfDay() + 1;
		if (timeOfDay > eTimeOfDay::eNight)
		{
			timeOfDay = 0;
		}

		m_map.setTimeOfDay((eTimeOfDay)timeOfDay);
		m_dayTime.reset();
	}
}

void Battle::setWindDirectoin(float deltaTime)
{
	m_windTime.update(deltaTime);
	if (m_windTime.isExpired())
	{
		int wind = rand() % eDirection::Max;
		m_map.setWindDirection((eDirection)wind);
		m_windTime.reset();
	}
}

Battle::Battle()
	: m_players(),
	m_currentPlayersTurn(static_cast<int>(FactionName::eYellow)),
	m_map(),
	m_currentPhase(BattlePhase::ShipPlacement),
	m_battleUI(*this),
	m_dayTime(20.0f),
	m_windTime(10)
{
	GameEventMessenger::getInstance().subscribe(std::bind(&Battle::onReset, this), "Battle", GameEvent::eResetBattle);
}

Battle::~Battle()
{
	GameEventMessenger::getInstance().unsubscribe("Battle", GameEvent::eResetBattle);
}

void Battle::startBattle(const std::string & newMapName, std::vector<std::pair<FactionName, std::vector<EntityProperties*>>>& newPlayers)
{
	assert(m_players.empty());
	m_map.loadmap(newMapName);

	for (auto& player : newPlayers)
	{
		m_players.emplace_back(player.first);
	}

	m_battleUI.startShipPlacement(newPlayers);
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
	m_battleUI.setCurrentFaction(getCurentFaction());
	m_battleUI.update();
	m_map.setDrawOffset(m_battleUI.getCameraPositionOffset());

	setTimeOfDay(deltaTime);
	setWindDirectoin(deltaTime);

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

void Battle::moveEntityToPosition(BattleEntity& entity, const Tile& destination, eDirection endDirection)
{
	assert(m_currentPhase == BattlePhase::Movement);
	entity.m_battleProperties.moveEntity(m_map, destination, endDirection);
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

void Battle::insertEntity(std::pair<int, int> startingPosition, eDirection startingDirection, const EntityProperties& entityProperties, FactionName factionName)
{
	assert(m_currentPhase == BattlePhase::ShipPlacement);

	auto& player = getPlayer(factionName);
	switch (factionName)
	{
	case FactionName::eYellow :
		player.m_entities.push_back(std::make_unique<BattleEntity>(startingPosition, entityProperties, m_map, factionName, startingDirection));
		break;

	case FactionName::eBlue:
		player.m_entities.push_back(std::make_unique<BattleEntity>(startingPosition, entityProperties, m_map, factionName, startingDirection));
		break;
	
	case FactionName::eRed :
		player.m_entities.push_back(std::make_unique<BattleEntity>(startingPosition, entityProperties, m_map, factionName, startingDirection));
		break;
	
	case FactionName::eGreen :
		player.m_entities.push_back(std::make_unique<BattleEntity>(startingPosition, entityProperties, m_map, factionName, startingDirection));
		break;
	}
}

void Battle::nextTurn()
{
	
	m_moveCounter.m_counter = 0;

	
	
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

void Battle::onReset()
{
	m_currentPhase = BattlePhase::ShipPlacement;
	m_currentPlayersTurn = static_cast<int>(FactionName::eYellow);
	m_dayTime.reset();
	m_windTime.reset();
	m_moveCounter.m_counter = 0;
	m_players.clear();
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