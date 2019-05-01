#include "Battle.h"
#include "Utilities/MapParser.h"
#include "GameEventMessenger.h"
#include "AI.h"

using namespace HAPISPACE;
constexpr float DRAW_ENTITY_OFFSET_X{ 16 };
constexpr float DRAW_ENTITY_OFFSET_Y{ 32 };

Battle::ParticleSystem::ParticleSystem(float lifespan, std::shared_ptr<HAPISPACE::SpriteSheet> texture, float scale) :
	m_position(),
	m_lifeSpan(lifespan),
	m_particle(HAPI_Sprites.MakeSprite(texture)),
	m_frameNum(0),
	m_isEmitting(false),
	m_scale(scale)
{
	m_particle->SetFrameNumber(m_frameNum);
}

void Battle::ParticleSystem::setPosition(std::pair<int, int> position)
{
	m_position = position;
}

void Battle::ParticleSystem::run(float deltaTime, const Map& map)
{
	if (m_isEmitting)
	{
		const std::pair<int, int> tileTransform = map.getTileScreenPos(m_position);
		m_particle->GetTransformComp().SetPosition({
			tileTransform.first + DRAW_ENTITY_OFFSET_X * map.getDrawScale(),
			tileTransform.second + DRAW_ENTITY_OFFSET_Y * map.getDrawScale() });

		m_lifeSpan.update(deltaTime);

		if (m_lifeSpan.isExpired())
		{
			m_particle->SetFrameNumber(m_frameNum);

			m_lifeSpan.reset();
			++m_frameNum;
		}

		if (m_frameNum >= m_particle->GetNumFrames())
		{
			m_isEmitting = false;
			m_frameNum = 0;
		}
	}
}

void Battle::ParticleSystem::render()const
{
	if (m_isEmitting)
	{
		m_particle->GetTransformComp().SetOriginToCentreOfFrame();
		m_particle->GetTransformComp().SetScaling(m_scale);
		m_particle->Render(SCREEN_SURFACE);
	}
}

void Battle::ParticleSystem::orient(eDirection entityDir)
{
	eDirection direction;
	switch (entityDir)
	{
	case eNorth:
		direction = eSouth;
		break;
	case eNorthEast:
		direction = eSouthWest;
		break;
	case eSouthEast:
		direction = eNorthWest;
		break;
	case eSouth:
		direction = eNorth;
		break;
	case eSouthWest:
		direction = eNorthEast;
		break;
	case eNorthWest:
		direction = eSouthEast;
		break;
	}
	m_particle->GetTransformComp().SetRotation(DEGREES_TO_RADIANS(static_cast<int>(direction) * 60 % 360));
}


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

void Battle::handleAIMovementPhaseTimer(float deltaTime)
{
	m_timeUntilAIMovementPhase.update(deltaTime);
	if (m_timeUntilAIMovementPhase.isExpired())
	{
		assert(m_currentPhase == BattlePhase::Movement);
		AI::handleMovementPhase(*this, m_map, m_players[m_currentPlayerTurn]);
		m_timeUntilAIMovementPhase.reset();
		m_timeUntilAIMovementPhase.setActive(false);
	}
}

void Battle::handleAIAttackPhaseTimer(float deltaTime)
{
	m_timeUntilAIAttackPhase.update(deltaTime);
	if (m_timeUntilAIAttackPhase.isExpired())
	{
		assert(m_currentPhase == BattlePhase::Attack);
		AI::handleShootingPhase(*this, m_map, m_players[m_currentPlayerTurn]);
		m_timeUntilAIAttackPhase.reset();
		m_timeUntilAIAttackPhase.setActive(false);
	}
}

Battle::Battle()
	: m_players(),
	m_currentPlayerTurn(0),
	m_map(),
	m_currentPhase(BattlePhase::Deployment),
	m_battleUI(*this),
	m_dayTime(20.0f),
	m_windTime(10),
	m_explosionParticle(0.10, Textures::m_explosion, 2.5f),
	m_fireParticle(0.05, Textures::m_fire, 2.0f),
	m_timeUntilAIMovementPhase(2.0f, false),
	m_timeUntilAIAttackPhase(2.0f, false),
	m_AITurn(false)
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

void Battle::start(const std::string & newMapName, std::vector<Player>& newPlayers)
{
	assert(!newPlayers.empty());
	assert(m_players.empty());
	m_map.loadmap(newMapName);
	
	//TODO: Hack to correct sprite sizes
	for (auto& player : newPlayers)
	{
		for (auto& entity : player.m_entities)
		{
			entity.m_sprite->GetTransformComp().SetOriginToCentreOfFrame();
			entity.m_sprite->GetTransformComp().SetScaling({ 1, 1 });
		}
	}

	for (auto& player : newPlayers)
	{
		m_players.emplace_back(player.m_factionName, m_map.getSpawnPosition(), player.m_type);
	}
	
	m_battleUI.deployPlayers(newPlayers, m_map, *this);

	m_battleUI.loadGUI(m_map.getDimensions());
}

void Battle::render() const
{
	m_map.drawMap();
	m_battleUI.renderUI();

	for (const auto& player : m_players)
	{
		for (const auto& entity : player.m_entities)
		{
			entity->m_battleProperties.render(entity->m_entityProperties.m_sprite, m_map);
		}
	}
	
	m_explosionParticle.render();
	m_fireParticle.render();
	m_battleUI.renderGUI();
}

void Battle::update(float deltaTime)
{
	m_battleUI.setCurrentFaction(getCurentFaction());
	m_battleUI.update(deltaTime);
	m_map.setDrawOffset(m_battleUI.getCameraPositionOffset());

	setTimeOfDay(deltaTime);
	setWindDirection(deltaTime);

	m_explosionParticle.run(deltaTime, m_map);
	m_fireParticle.run(deltaTime, m_map);

	if (m_currentPhase == BattlePhase::Movement)
	{
		updateMovementPhase(deltaTime);
		handleAIMovementPhaseTimer(deltaTime);
	}
	else if (m_currentPhase == BattlePhase::Attack)
	{
		updateAttackPhase();
		handleAIAttackPhaseTimer(deltaTime);
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

bool Battle::fireEntityWeaponAtPosition(const Tile& tileOnPlayer, const Tile& tileOnAttackPosition, const std::vector<const Tile*>& targetArea)
{
	assert(m_currentPhase == BattlePhase::Attack);
	assert(tileOnPlayer.m_entityOnTile);
	//if (!tileOnPlayer.m_entityOnTile)
	//{
	//	return false;
	//}
	assert(!tileOnPlayer.m_entityOnTile->m_battleProperties.isWeaponFired());

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
			if (tileOnPlayer.m_entityOnTile->m_entityProperties.m_weaponType == eFlamethrower)
			{
				m_fireParticle.orient(tileOnPlayer.m_entityOnTile->m_battleProperties.getCurrentDirection());
				m_fireParticle.setPosition(targetArea[0]->m_tileCoordinate);
				m_fireParticle.m_isEmitting = true;
			}
			else
			{
				m_explosionParticle.setPosition(tileOnAttackPosition.m_entityOnTile->m_battleProperties.getCurrentPosition());
				m_explosionParticle.m_isEmitting = true;
			}

			tileOnPlayer.m_entityOnTile->m_battleProperties.fireWeapon();
			auto& enemy = tileOnAttackPosition.m_entityOnTile;
			enemy->m_battleProperties.takeDamage(enemy->m_entityProperties, tileOnPlayer.m_entityOnTile->m_entityProperties.m_damage, enemy->m_factionName);
			
			return true;
		}
	}

	return false;
}

void Battle::insertEntity(std::pair<int, int> startingPosition, eDirection startingDirection, const EntityProperties& entityProperties, FactionName factionName)
{
	assert(m_currentPhase == BattlePhase::Deployment);

	auto& player = getPlayer(factionName);
	player.m_entities.push_back(std::make_unique<BattleEntity>(startingPosition, entityProperties, m_map, factionName, startingDirection));
}

void Battle::nextTurn()
{
	FactionName currentPlayer;
	bool lastPlayer = false;
	switch (m_currentPhase)
	{
	case BattlePhase::Deployment :
		lastPlayer = (m_battleUI.isHumanDeploymentCompleted());
		incrementPlayerTurn();
		if (lastPlayer)
		{
			m_currentPhase = BattlePhase::Movement;

			//for (auto& entity : m_players[m_currentPlayerTurn].m_entities)
			//{
			//	entity->m_battleProperties.enableAction();
			//}
			
			m_currentPlayerTurn = 0;
			if (m_players[m_currentPlayerTurn].m_playerType == ePlayerType::eAI)
			{
				m_timeUntilAIMovementPhase.setActive(true);
				GameEventMessenger::getInstance().broadcast(GameEvent::eEnteredAITurn);
				m_AITurn = true;
			}
		}
		GameEventMessenger::getInstance().broadcast(GameEvent::eNewTurn);
		currentPlayer = m_players[m_currentPlayerTurn].m_factionName;
		for (auto& entity : m_players[m_currentPlayerTurn].m_entities)
		{
			entity->m_battleProperties.enableAction();
		}
		break;
	case BattlePhase::Movement :
		m_currentPhase = BattlePhase::Attack;
		GameEventMessenger::getInstance().broadcast(GameEvent::eNewTurn);
		GameEventMessenger::getInstance().broadcast(GameEvent::eEnteringAttackPhase);
		currentPlayer = m_players[m_currentPlayerTurn].m_factionName;

		for (auto& entity : m_players[m_currentPlayerTurn].m_entities)
		{
			entity->m_battleProperties.enableAction();
		}

		if (!m_players[m_currentPlayerTurn].m_eliminated && m_players[m_currentPlayerTurn].m_playerType == ePlayerType::eAI)
		{
			m_timeUntilAIAttackPhase.setActive(true);
			GameEventMessenger::getInstance().broadcast(GameEvent::eEnteredAITurn);
			m_AITurn = true;
		}
		break;
	case BattlePhase::Attack :
		m_currentPhase = BattlePhase::Movement;
		GameEventMessenger::getInstance().broadcast(GameEvent::eNewTurn);
		GameEventMessenger::getInstance().broadcast(GameEvent::eEnteringMovementPhase);

		for (auto& entity : m_players[m_currentPlayerTurn].m_entities)
		{
			entity->m_battleProperties.disableAction();
		}

		incrementPlayerTurn();
		currentPlayer = m_players[m_currentPlayerTurn].m_factionName;
		for (auto& entity : m_players[m_currentPlayerTurn].m_entities)
		{
			entity->m_battleProperties.enableAction();
		}

		if (!m_players[m_currentPlayerTurn].m_eliminated && m_players[m_currentPlayerTurn].m_playerType == ePlayerType::eAI)
		{
			m_timeUntilAIMovementPhase.setActive(true);
			GameEventMessenger::getInstance().broadcast(GameEvent::eEnteredAITurn);
			m_AITurn = true;
		}
		else if (m_players[m_currentPlayerTurn].m_playerType == ePlayerType::eHuman)
		{
			GameEventMessenger::getInstance().broadcast(GameEvent::eLeftAITurn);
			m_AITurn = false;
		}
		break;
	}
}

std::vector<FactionName> Battle::getAllFactions() const
{
	std::vector<FactionName> lol;
	for (auto& player : m_players)
	{
		lol.emplace_back(player.m_factionName);
	}

	assert(!lol.empty());
	return lol;
}

void Battle::updateMovementPhase(float deltaTime)
{
	for (auto& entity : m_players[m_currentPlayerTurn].m_entities)
	{
		if (entity->m_battleProperties.isDead())
			continue;
		entity->m_battleProperties.update(deltaTime, m_map, entity->m_entityProperties);
	}
	for (auto& entity : m_players[m_currentPlayerTurn].m_entities)
	{
		if (entity->m_battleProperties.isDead())
			continue;
		if (!entity->m_battleProperties.isDestinationSet())
		{
			return;
		}
		if (entity->m_battleProperties.isMovingToDestination())
		{
			return;
		}
	}

	nextTurn();
}

void Battle::updateAttackPhase()
{
	if (allEntitiesAttacked(m_players[m_currentPlayerTurn].m_entities))
	{	
		nextTurn();
	}
}

bool Battle::allEntitiesAttacked(std::vector<std::shared_ptr<BattleEntity>>& playerEntities) const
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
	m_currentPhase = BattlePhase::Deployment;
	m_currentPlayerTurn = 0;
	m_dayTime.reset();
	m_windTime.reset();
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

const BattlePlayer & Battle::getPlayer(FactionName factionName) const
{
	auto cIter = std::find_if(m_players.cbegin(), m_players.cend(), [factionName](const auto& player) { return player.m_factionName == factionName; });
	assert(cIter != m_players.cend());
	return *cIter;
}

bool Battle::isAIPlaying() const
{
	return m_AITurn;
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
	bool actionBeingPerformed = false;
	for (auto& entity : m_players[m_currentPlayerTurn].m_entities)
	{
		if (entity->m_battleProperties.isMovingToDestination())
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
		nextTurn();
	}
}

void Battle::onEndAttackPhaseEarly()
{
	nextTurn();
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