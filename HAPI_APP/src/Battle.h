#pragma once

#include "Map.h"
#include "entity.h"
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
	Battle(std::deque<EntityProperties*>& selectedEntities);

	const Map& getMap() const;

	BattlePhase getCurrentPhase() const;

	void start();

	void render() const;
	void update(float deltaTime);
	void moveEntityToPosition(BattleEntity& entity, const Tile& destination);
	void activateEntityWeapon(BattleEntity& entity);
	void insertEntity(std::pair<int, int> startingPosition, const EntityProperties& entityProperties);
	void nextPhase();

private:
	std::vector<std::unique_ptr<BattleEntity>> m_entities;
	Map m_map;
	BattleUI m_battleUI;
	BattlePhase m_currentPhase;

	//Movement Phase
	void updateMovementPhase(float deltaTime);
};