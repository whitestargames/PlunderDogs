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

struct MoveCounter;
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
	void setMapDrawOffset(std::pair<int, int> offset) { m_map.setDrawOffset(offset); }

	void fireEntityWeaponAtPosition(BattleEntity& player, const Tile& tileOnAttackPosition, const std::vector<const Tile*>& targetArea);

	void insertEntity(std::pair<int, int> startingPosition, const EntityProperties& entityProperties, PlayerName playerName);
	void nextTurn();

private:
	std::vector<std::unique_ptr<BattleEntity>> m_player1Entities;
	std::vector<std::unique_ptr<BattleEntity>> m_player2Entities;
	Map m_map;
	BattlePhase m_currentPhase;


	PlayerName m_currentPlayerTurn;
	BattleUI m_battleUI;
	MoveCounter m_moveCounter;


	void updateMovementPhase(float deltaTime);
	void updateAttackPhase();
	bool allEntitiesAttacked(std::vector<std::unique_ptr<BattleEntity>>& playerEntities) const;
};