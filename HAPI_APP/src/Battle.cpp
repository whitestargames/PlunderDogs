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

void Battle::setWindDirection(float deltaTime)
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
	m_currentPlayerTurn(0),
	m_map(),
	m_currentPhase(BattlePhase::ShipPlacement),
	m_battleUI(*this),
	m_dayTime(20.0f),
	m_windTime(10)
{
	GameEventMessenger::getInstance().subscribe(std::bind(&Battle::onResetBattle, this), "Battle", GameEvent::eResetBattle);
	GameEventMessenger::getInstance().subscribe(std::bind(&Battle::onYellowShipDestroyed, this), "Battle", GameEvent::eYellowShipDestroyed);
	GameEventMessenger::getInstance().subscribe(std::bind(&Battle::onRedShipDestroyed, this), "Battle", GameEvent::eRedShipDestroyed);
	GameEventMessenger::getInstance().subscribe(std::bind(&Battle::onBlueShipDestroyed, this), "Battle", GameEvent::eBlueShipDestroyed);
	GameEventMessenger::getInstance().subscribe(std::bind(&Battle::onGreenShipDestroyed, this), "Battle", GameEvent::eGreenShipDestroyed);
	GameEventMessenger::getInstance().subscribe(std::bind(&Battle::onEndMovementPhaseEarly, this), "Battle", GameEvent::eEndMovementPhaseEarly);
	GameEventMessenger::getInstance().subscribe(std::bind(&Battle::onEndAttackPhaseEarly, this), "Battle", GameEvent::eEndAttackPhaseEarly);
}

Battle::~Battle()
{
	GameEventMessenger::getInstance().unsubscribe("Battle", GameEvent::eResetBattle);
	GameEventMessenger::getInstance().unsubscribe("Battle", GameEvent::eYellowShipDestroyed);
	GameEventMessenger::getInstance().unsubscribe("Battle", GameEvent::eRedShipDestroyed);
	GameEventMessenger::getInstance().unsubscribe("Battle", GameEvent::eBlueShipDestroyed);
	GameEventMessenger::getInstance().unsubscribe("Battle", GameEvent::eGreenShipDestroyed);
	GameEventMessenger::getInstance().unsubscribe("Battle", GameEvent::eEndMovementPhaseEarly);
	GameEventMessenger::getInstance().unsubscribe("Battle", GameEvent::eEndAttackPhaseEarly);
}

void Battle::startBattle(const std::string & newMapName, std::vector<std::pair<FactionName, std::vector<EntityProperties*>>>& newPlayers)
{
	assert(m_players.empty());
	m_map.loadmap(newMapName);

	for (auto& player : newPlayers)
	{
		m_players.emplace_back(player.first);
	}

	m_battleUI.startShipPlacement(newPlayers, m_map);
	m_battleUI.loadGUI(m_map.getDimensions());
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
	
	m_battleUI.renderParticles();
	m_battleUI.renderGUI();
	//m_battleUI.renderUI();
}

void Battle::update(float deltaTime)
{
	m_battleUI.setCurrentFaction(getCurentFaction());
	m_battleUI.update(deltaTime);
	m_map.setDrawOffset(m_battleUI.getCameraPositionOffset());

	setTimeOfDay(deltaTime);
	setWindDirection(deltaTime);

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
	player.m_entities.push_back(std::make_unique<BattleEntity>(startingPosition, entityProperties, m_map, factionName, startingDirection));
}

void Battle::nextTurn()
{
	m_moveCounter.m_counter = 0;

	bool lastPlayer = false;
	switch (m_currentPhase)
	{
	case BattlePhase::ShipPlacement :
		lastPlayer = (m_currentPlayerTurn == static_cast<int>(m_players.size()) - 1);
		incrementPlayerTurn();
		if (lastPlayer)
		{
			m_currentPhase = BattlePhase::Movement;
			m_currentPlayerTurn = 0;
		}
		GameEventMessenger::getInstance().broadcast(GameEvent::eNewTurn);
		break;
	case BattlePhase::Movement :
		m_currentPhase = BattlePhase::Attack;
		GameEventMessenger::getInstance().broadcast(GameEvent::eNewTurn);
		GameEventMessenger::getInstance().broadcast(GameEvent::eEnteringAttackPhase);
		break;
	case BattlePhase::Attack :
		m_currentPhase = BattlePhase::Movement;
		GameEventMessenger::getInstance().broadcast(GameEvent::eNewTurn);
		GameEventMessenger::getInstance().broadcast(GameEvent::eEnteringMovementPhase);
		incrementPlayerTurn();
		break;
	}
}

void Battle::updateMovementPhase(float deltaTime)
{
	int totalAliveEntities = 0;
	
	for (auto& entity : m_players[m_currentPlayerTurn].m_entities)
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
	if (allEntitiesAttacked(m_players[m_currentPlayerTurn].m_entities))
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

void Battle::onResetBattle()
{
	m_currentPhase = BattlePhase::ShipPlacement;
	m_currentPlayerTurn = 0;
	m_dayTime.reset();
	m_windTime.reset();
	m_moveCounter.m_counter = 0;
	m_players.clear();
}

void Battle::incrementPlayerTurn()
{
	++m_currentPlayerTurn;

	if (m_currentPlayerTurn == static_cast<int>(m_players.size()))
	{
		m_currentPlayerTurn = 0;
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

FactionName Battle::getCurentFaction() const
{
	return m_players[m_currentPlayerTurn].m_factionName;
}

void Battle::onYellowShipDestroyed()
{
	m_battleManager.onYellowShipDestroyed(m_players);
}

void Battle::onBlueShipDestroyed()
{
	m_battleManager.onBlueShipDestroyed(m_players);
}

void Battle::onGreenShipDestroyed()
{
	m_battleManager.onGreenShipDestroyed(m_players);
}

void Battle::onRedShipDestroyed()
{
	m_battleManager.onRedShipDestroyed(m_players);
}

void Battle::onEndMovementPhaseEarly()
{
	//FactionName currentPlayerTurn = static_cast<FactionName>(m_currentPlayerTurn);
	//m_players[m_currentPlayerTurn].
	//auto player = std::find_if(m_players.cbegin(), m_players.cend(), [currentPlayerTurn](const auto& player) { return player.m_factionName == currentPlayerTurn; });
	//assert(player != m_players.cend());
	bool actionBeingPerformed = false;
	for (auto& entity : m_players[m_currentPlayerTurn].m_entities)
	{
		if (entity->m_battleProperties.isMovedToDestination() && entity->m_battleProperties.isMoving())
		{
			actionBeingPerformed = true;
		}
	}

	if (actionBeingPerformed)
	{
		GameEventMessenger::getInstance().broadcast(GameEvent::eUnableToSkipPhase);
	}
	else
	{
		m_moveCounter.m_counter = 0;
		m_currentPhase = BattlePhase::Attack;
		GameEventMessenger::getInstance().broadcast(GameEvent::eNewTurn);
		GameEventMessenger::getInstance().broadcast(GameEvent::eEnteringAttackPhase);
	}
}

void Battle::onEndAttackPhaseEarly()
{
	m_currentPhase = BattlePhase::Movement;
	GameEventMessenger::getInstance().broadcast(GameEvent::eNewTurn);
	GameEventMessenger::getInstance().broadcast(GameEvent::eEnteringMovementPhase);
	incrementPlayerTurn();
}

Battle::BattleManager::BattleManager()
	: m_yellowShipsDestroyed(0),
	m_blueShipsDestroyed(0),
	m_greenShipsDestroyed(0),
	m_redShipsDestroyed(0)
{
	GameEventMessenger::getInstance().subscribe(std::bind(&BattleManager::onReset, this), "BattleManager", GameEvent::eResetBattle);
}

Battle::BattleManager::~BattleManager()
{
	GameEventMessenger::getInstance().unsubscribe("BattleManager", GameEvent::eResetBattle);
}

void Battle::BattleManager::onYellowShipDestroyed(std::vector<BattlePlayer>& players)
{
	++m_yellowShipsDestroyed;
	auto player = std::find_if(players.begin(), players.end(), [](const auto& player) { return player.m_factionName == FactionName::eYellow; });
	assert(player != players.end());
	if (m_yellowShipsDestroyed == static_cast<int>(player->m_entities.size()))
	{
		player->m_eliminated = true;
		checkGameStatus(players);
	}
}

void Battle::BattleManager::onBlueShipDestroyed(std::vector<BattlePlayer>& players)
{
	++m_blueShipsDestroyed;
	auto player = std::find_if(players.begin(), players.end(), [](const auto& player) { return player.m_factionName == FactionName::eBlue; });
	assert(player != players.end());
	if (m_blueShipsDestroyed == static_cast<int>(player->m_entities.size()))
	{
		player->m_eliminated = true;
		checkGameStatus(players);
	}
}

void Battle::BattleManager::onGreenShipDestroyed(std::vector<BattlePlayer>& players)
{
	++m_greenShipsDestroyed;
	auto player = std::find_if(players.begin(), players.end(), [](const auto& player) { return player.m_factionName == FactionName::eGreen; });
	assert(player != players.end());
	if (m_greenShipsDestroyed == static_cast<int>(player->m_entities.size()))
	{
		player->m_eliminated = true;
		checkGameStatus(players);
	}
}

void Battle::BattleManager::onRedShipDestroyed(std::vector<BattlePlayer>& players)
{
	++m_redShipsDestroyed;
	auto player = std::find_if(players.begin(), players.end(), [](const auto& player) { return player.m_factionName == FactionName::eRed; });
	assert(player != players.end());
	if (m_redShipsDestroyed == static_cast<int>(player->m_entities.size()))
	{
		player->m_eliminated = true;
		checkGameStatus(players);
	}
}

void Battle::BattleManager::onReset()
{
	m_yellowShipsDestroyed = 0;
	m_redShipsDestroyed = 0;
	m_blueShipsDestroyed = 0;
	m_greenShipsDestroyed = 0;
}

void Battle::BattleManager::checkGameStatus(const std::vector<BattlePlayer>& players)
{
	//Check to see if all players have been eliminated
	int playersEliminated = 0;
	for (const auto& player : players)
	{
		if (player.m_eliminated)
		{
			++playersEliminated;
		}
	}

	//Last player standing - Player wins
	if (playersEliminated == static_cast<int>(players.size()) - 1)
	{
		auto player = std::find_if(players.cbegin(), players.cend(), [](const auto& player) { return player.m_eliminated == false; });
		assert(player != players.cend());
		FactionName winningFaction = player->m_factionName;
		switch (winningFaction)
		{
		case FactionName::eYellow:
			GameEventMessenger::broadcast(GameEvent::eYellowWin);
			break;
		case FactionName::eBlue:
			GameEventMessenger::broadcast(GameEvent::eBlueWin);
			break;
		case FactionName::eGreen:
			GameEventMessenger::broadcast(GameEvent::eGreenWin);
			break;
		case FactionName::eRed:
			GameEventMessenger::broadcast(GameEvent::eRedWin);
			break;
		}
	}
}