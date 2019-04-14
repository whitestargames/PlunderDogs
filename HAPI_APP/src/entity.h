#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include <memory>
#include <string>
#include "Timer.h"
#include "Global.h"

struct Tile;
struct Weapons;
class Map;
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
struct EntityBattleProperties
{
	class Weapon
	{
		struct HighlightNode
		{
			HighlightNode();
			std::unique_ptr<Sprite> sprite;
			bool activate;
		};

	public:
		Weapon();
		void render() const;
		void generateTargetArea(const Map& map, const Tile& source);//using same convention as movement // from source should be able to get position
	private:
		std::vector<HighlightNode> m_spriteTargetStorage;
		void clearGunArea();
	};

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

	void update(float deltaTime, const Map& map, EntityProperties& entityProperties);

	void render(std::shared_ptr<HAPISPACE::Sprite>& sprite, const Map& map);

	void generateMovementGraph(const Map& map, const Tile& source, const Tile& destination);
	void generateWeaponArea(const Map& map, const Tile& source);
	void clearMovementPath();
	void moveEntity(Map& map, const Tile& tile, int movementPointsAvailable);

	

	std::pair<int, int> m_currentPosition;
	std::deque<std::pair<eDirection, std::pair<int, int>>> m_pathToTile;
	Timer m_movementTimer;
	bool m_movedToDestination;
	MovementPath m_movementPath;
	Weapon m_weapon;
	int m_movementPathSize;
	eDirection m_direction;
};



struct BattleEntity
{
	BattleEntity(std::pair<int, int> startingPosition);

	void setPosition(const Map& map);

	EntityProperties m_entityProperties;
	EntityBattleProperties m_battleProperties;
};

