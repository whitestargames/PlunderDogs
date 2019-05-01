#pragma once

#include "Global.h"
#include "Map.h"
#include "BattleUI.h"
#include "entity.h"

enum class BattlePhase
{
	ShipPlacement = 0,
	Movement,
	Attack
};

class Battle
{
	class BattleManager
	{
	public:
		BattleManager();
		~BattleManager();
	
		void onYellowShipDestroyed(std::vector<BattlePlayer>& players);
		void onBlueShipDestroyed(std::vector<BattlePlayer>& players);
		void onGreenShipDestroyed(std::vector<BattlePlayer>& players);
		void onRedShipDestroyed(std::vector<BattlePlayer>& players);

	private:
		int m_yellowShipsDestroyed;
		int m_blueShipsDestroyed;
		int m_greenShipsDestroyed;
		int m_redShipsDestroyed;
		void onReset();

		void checkGameStatus(const std::vector<BattlePlayer>& players);
	};

public:
	Battle();
	~Battle();
	const Map& getMap() const;
	BattlePhase getCurrentPhase() const;
	FactionName getCurrentFaction() const;

	void startBattle(const std::string& newMapName, std::vector<std::pair<FactionName, std::vector<EntityProperties*>>>& newPlayers);
	void render() const;
	void update(float deltaTime);
	void moveEntityToPosition(BattleEntity& entity, const Tile& destination);
	void moveEntityToPosition(BattleEntity& entity, const Tile& destination, eDirection endDirection);

	bool fireEntityWeaponAtPosition(BattleEntity& player, const Tile& tileOnAttackPosition, const std::vector<const Tile*>& targetArea);
	void insertEntity(std::pair<int, int> startingPosition, eDirection startingDirection, const EntityProperties& entityProperties, FactionName factionName);
	void nextTurn();

	std::vector<FactionName> getAllFactions() const;
	const BattlePlayer& getPlayer(FactionName faction) const;
	//std::vector<std::shared_ptr<BattleEntity>>& getFactionShips(FactionName faction);
	//const std::vector<std::shared_ptr<BattleEntity>>& getFactionShips(FactionName faction) const;
private:
	std::vector<BattlePlayer> m_players;
	int m_currentPlayerTurn;
	Map m_map;
	BattlePhase m_currentPhase;
	BattleUI m_battleUI;
	Timer m_dayTime;
	Timer m_windTime;
	BattleManager m_battleManager;

	void updateMovementPhase(float deltaTime);
	void updateAttackPhase();
	bool allEntitiesAttacked(std::vector<std::shared_ptr<BattleEntity>>& playerEntities) const;
	BattlePlayer& getPlayer(FactionName factionName);

	void incrementPlayerTurn();
	void setTimeOfDay(float deltaTime);
	void setWindDirection(float deltaTime);

	void onResetBattle();
	void onYellowShipDestroyed();
	void onBlueShipDestroyed();
	void onGreenShipDestroyed();
	void onRedShipDestroyed();
	void onEndMovementPhaseEarly();
	void onEndAttackPhaseEarly();
};