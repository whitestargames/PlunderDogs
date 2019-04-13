#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include <memory>
#include <string>
#include "Timer.h"
#include "Global.h"

struct Tile;
class Map;
struct EntityBattleProperties
{
	class MovementPath
	{
		struct MovementPathNode
		{
			MovementPathNode();

			std::unique_ptr<Sprite> sprite;
			bool activate;
		};

	public:
		MovementPath();

		void render() const;
		void generatePath(const Map& map, const Tile& source, const Tile& destination);
		void eraseNode(std::pair<int, int> position, const Map& map);
		void clearPath();

	private:
		std::vector<MovementPathNode> m_movementPath;
		unsigned int getDirectionCost(int currentDirection, int newDirection);
		
	};

	EntityBattleProperties(std::pair<int, int> startingPosition);

	void update(float deltaTime, const Map& map);

	void render(std::shared_ptr<HAPISPACE::Sprite>& sprite, const Map& map);

	void generateMovementGraph(const Map& map, const Tile& source, const Tile& destination);
	void clearMovementPath();
	void moveEntity(Map& map, const Tile& tile, int movementPointsAvailable);

	

	std::pair<int, int> m_currentPosition;
	std::deque<std::pair<eDirection, std::pair<int, int>>> m_pathToTile;
	Timer m_movementTimer;
	bool m_movedToDestination;
	MovementPath m_movementPath;
	int m_movementPathSize;
	eDirection m_direction;
	
};

struct EntityProperties
{
	EntityProperties();

	std::shared_ptr<HAPISPACE::Sprite> m_sprite;
	int m_movementPoints;
	int m_healthMax;
	int m_currentHealth;
	int m_range;
	int m_damage;
	
	
};

struct BattleEntity
{
	BattleEntity(std::pair<int, int> startingPosition);

	void setPosition(const Map& map);

	EntityProperties m_entityProperties;
	EntityBattleProperties m_battleProperties;
};

struct Weapons
{
	// names temp mocking together 
	// basic cannon uses the cone from the map 
	void generateWeaponArea();// this will depend on weapon will take an enum corresponds to type of ship enemy has
	void selectTarget();// will be in the battle phase  checks entity there 
	void updateDamage();// does an amount of damage determined by weapon type 

};