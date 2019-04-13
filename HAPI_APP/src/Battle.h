#pragma once

#include "Map.h"
#include "entity.h"
#include "BattleUI.h"


enum class BattlePhase
{
	Movement = 0,
	Attack
};

class Battle
{
public:
	Battle();

	const Map& getMap() const;

	BattlePhase getCurrentPhase() const;

	void render() const;
	void update(float deltaTime);
	void moveEntityToPosition(BattleEntity& entity, const Tile& destination);
	void activateEntityWeapon(BattleEntity& entity);

private:
	std::vector<std::unique_ptr<BattleEntity>> m_entities;
	Map m_map;
	BattleUI m_battleUI;
	BattlePhase m_currentPhase;

	void insertEntity(std::pair<int, int> startingPosition);
	void changePhase(BattlePhase newPhase);

	//Movement Phase
	void updateMovementPhase(float deltaTime);
};