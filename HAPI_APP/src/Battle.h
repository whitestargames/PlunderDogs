#pragma once

#include "Global.h"
#include "Map.h"
#include "BattleUI.h"

enum class BattlePhase
{
	ShipPlacement = 0,
	Movement,
	Attack
};

class Battle
{
public:
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

	void updateMovementPhase(float deltaTime);
	void updateAttackPhase();
	bool allEntitiesAttacked(std::vector<std::unique_ptr<BattleEntity>>& playerEntities) const;
	BattlePlayer& getPlayer(FactionName factionName);
};