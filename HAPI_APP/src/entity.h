#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include <memory>
#include <string>
#include "Timer.h"
#include "Global.h"
#include "PlayerName.h"

struct MoveCounter
{
	MoveCounter()
		: m_counter(0)
	{}

	int m_counter;
};

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

class EntityBattleProperties
{
	class MovementPath
	{
		struct PathNode
		{
			PathNode();

			std::unique_ptr<Sprite> sprite;
			bool activate;
			std::pair<int, int> m_position;
		};

	public:
		MovementPath();

		void render(const Map& map) const;
		void generatePath(const Map& map, const Tile& source, const Tile& destination);
		void eraseNode(std::pair<int, int> position, const Map& map);
		void clearPath();

	private:
		std::vector<PathNode> m_movementPath;
		unsigned int getDirectionCost(int currentDirection, int newDirection);
	};

public:
	EntityBattleProperties(std::pair<int, int> startingPosition);

	eDirection getCurrentDirection() const;
	bool isMovedToDestination() const;
	std::pair<int, int> getCurrentPosition() const;
	bool isWeaponFired() const;

	void update(float deltaTime, const Map& map, EntityProperties& entityProperties, MoveCounter& gameCounter);
	void render(std::shared_ptr<HAPISPACE::Sprite>& sprite, const Map& map);

	void generateMovementGraph(const Map& map, const Tile& source, const Tile& destination);
	void clearMovementPath();

	void moveEntity(Map& map, const Tile& tile);
	void takeDamage(EntityProperties& entityProperties, int damageAmount);
	void fireWeapon();
	void onNewTurn();

private:
	std::pair<int, int> m_currentPosition;
	std::deque<std::pair<eDirection, std::pair<int, int>>> m_pathToTile;
	Timer m_movementTimer;
	MovementPath m_movementPath;
	int m_movementPathSize;
	eDirection m_currentDirection;
	bool m_weaponFired;
	bool m_movedToDestination;

	void handleRotation(EntityProperties& entityProperties, const Map& map);
};

struct BattleEntity
{
	BattleEntity(std::pair<int, int> startingPosition, const EntityProperties& entityProperties, Map& map, PlayerName playerName);

	EntityProperties m_entityProperties;
	EntityBattleProperties m_battleProperties;
	const PlayerName m_playerName;
};