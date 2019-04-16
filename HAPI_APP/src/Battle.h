#pragma once

#include "Map.h"
#include "entity.h"
#include "BattleUI.h"
#include "PlayerName.h"

enum class BattlePhase
{
	ShipPlacement = 0,
	Movement,
	Attack
};

class Battle
{
public:
	Battle(std::vector<EntityProperties*>& player1, std::vector<EntityProperties*>& player2);

	const Map& getMap() const;

	BattlePhase getCurrentPhase() const;
	PlayerName getCurentPlayer() const;

	void render() const;
	void update(float deltaTime);
	void moveEntityToPosition(BattleEntity& entity, const Tile& destination);
	void activateEntityWeapon(EntityBattleProperties& battleProperties);
	void fireEntityWeaponAtPosition(BattleEntity& player, const BattleEntity& enemy, const std::vector<const Tile*>& targetArea);

	void insertEntity(std::pair<int, int> startingPosition, const EntityProperties& entityProperties, PlayerName playerName);
	void nextTurn();

private:
	std::vector<std::unique_ptr<BattleEntity>> m_player1;
	std::vector<std::unique_ptr<BattleEntity>> m_player2;
	Map m_map;
	BattleUI m_battleUI;
	BattlePhase m_currentPhase;
	PlayerName m_currentPlayerTurn;
};