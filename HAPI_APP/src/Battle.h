#pragma once

#include "Map.h"
#include "entity.h"
#include "BattleUI.h"
#include "FactionName.h"

enum class BattlePhase
{
	ShipPlacement = 0,
	Movement,
	Attack
};

struct Player;
struct MoveCounter;
class Battle
{
public:
	Battle(std::vector<Player>& players);

	const Map& getMap() const;
	
	BattlePhase getCurrentPhase() const;
	FactionName getCurentFaction() const;

	void render() const;
	void update(float deltaTime);
	void moveEntityToPosition(BattleEntity& entity, const Tile& destination);
	void setMapDrawOffset(std::pair<int, int> offset) { m_map.setDrawOffset(offset); }

	void fireEntityWeaponAtPosition(BattleEntity& player, const Tile& tileOnAttackPosition, const std::vector<const Tile*>& targetArea);

	void insertEntity(std::pair<int, int> startingPosition, const EntityProperties& entityProperties, FactionName playerName);
	void nextTurn();

private:
	std::vector<BattlePlayer> m_players;
	Map m_map;
	BattlePhase m_currentPhase;
	FactionName m_currentFaction;
	BattleUI m_battleUI;
	MoveCounter m_moveCounter;

	void updateMovementPhase(float deltaTime);
	void updateAttackPhase();
	bool allEntitiesAttacked(std::vector<std::unique_ptr<BattleEntity>>& playerEntities) const;
};