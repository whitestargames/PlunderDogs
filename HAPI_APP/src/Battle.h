#pragma once

#include "Global.h"
#include "Map.h"
#include "BattleUI.h"
#include <array>

enum class BattlePhase
{
	ShipPlacement = 0,
	Movement,
	Attack
};

class Battle
{
	class GameTimeHandler
	{
	public:
		GameTimeHandler(eGameTime startingTime, float expirationTime);
		
		eGameTime getTimeOfDay() const;
		void update(float deltaTime);

	private:
		eGameTime m_currentGameTime;
		Timer m_timer;
	};

public:
	void setTimeOfDay(float deltaTime);
	void setWindDirectoin(float deltaTime);
	Battle(std::vector<std::pair<FactionName, std::vector<EntityProperties*>>>& players);
	const Map& getMap() const;
	BattlePhase getCurrentPhase() const;
	FactionName getCurentFaction() const;

	void render() const;
	void update(float deltaTime);
	void moveEntityToPosition(BattleEntity& entity, const Tile& destination);

	void fireEntityWeaponAtPosition(BattleEntity& player, const Tile& tileOnAttackPosition, const std::vector<const Tile*>& targetArea);
	void insertEntity(std::pair<int, int> startingPosition, eDirection startingDirection, const EntityProperties& entityProperties, FactionName factionName);
	void nextTurn();

private:
	std::vector<BattlePlayer> m_players;
	int m_currentPlayersTurn;
	Map m_map;
	BattlePhase m_currentPhase;
	BattleUI m_battleUI;
	MoveCounter m_moveCounter;
	Timer m_dayTime;
	Timer m_windTime;
	GameTimeHandler m_gameTimeHandler;

	void updateMovementPhase(float deltaTime);
	void updateAttackPhase();
	bool allEntitiesAttacked(std::vector<std::unique_ptr<BattleEntity>>& playerEntities) const;
	BattlePlayer& getPlayer(FactionName factionName);
};