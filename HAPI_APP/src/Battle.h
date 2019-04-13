#pragma once

#include "Map.h"
#include "entity.h"
#include "BattleUI.h"

struct Weapon
{
	// names temp mocking together 
	// basic cannon uses the cone from the map 
	void generateWeaponArea();// this will depend on weapon will take an enum corresponds to type of ship enemy has
	void selectTarget();// will be in the battle phase  checks entity there 
	void updateDamage();// does an amount of damage determined by weapon type 

};




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