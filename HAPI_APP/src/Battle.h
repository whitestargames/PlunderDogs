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
public:
	void setTimeOfDay(float deltaTime);
	void setWindDirectoin(float deltaTime);
	Battle();
	~Battle();
	const Map& getMap() const;
	BattlePhase getCurrentPhase() const;
	FactionName getCurentFaction() const;

	void startBattle(const std::string& newMapName, std::vector<std::pair<FactionName, std::vector<EntityProperties*>>>& newPlayers);
	void render() const;
	void update(float deltaTime);
	void moveEntityToPosition(BattleEntity& entity, const Tile& destination);
	void moveEntityToPosition(BattleEntity& entity, const Tile& destination, eDirection endDirection);


	void fireEntityWeaponAtPosition(BattleEntity& player, const Tile& tileOnAttackPosition, const std::vector<const Tile*>& targetArea);
	void insertEntity(std::pair<int, int> startingPosition, eDirection startingDirection, const EntityProperties& entityProperties, FactionName factionName);
	void nextTurn();

	std::vector<std::unique_ptr<BattleEntity>>* getFactionShips(FactionName faction);
	const std::vector<std::unique_ptr<BattleEntity>>* getFactionShips(FactionName faction) const;
private:
	std::vector<BattlePlayer> m_players;
	int m_currentPlayersTurn;
	Map m_map;
	BattlePhase m_currentPhase;
	BattleUI m_battleUI;
	MoveCounter m_moveCounter;
	Timer m_dayTime;
	Timer m_windTime;

	void updateMovementPhase(float deltaTime);
	void updateAttackPhase();
	bool allEntitiesAttacked(std::vector<std::unique_ptr<BattleEntity>>& playerEntities) const;
	BattlePlayer& getPlayer(FactionName factionName);

	void onReset();
};