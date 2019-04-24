#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include <memory>
#include "Timer.h"
#include "Global.h"


struct MoveCounter
{
	MoveCounter()
		: m_counter(0)
	{}

	int m_counter;
};
enum class EntityType
{
	eCruiser,
	eBattleShip,
	eDestroyer,
	eGunBoat
};

struct Tile;
struct Weapons;
class Map;

struct EntityProperties
{
	EntityProperties(FactionName factionName, EntityType entityType);

	std::shared_ptr<HAPISPACE::Sprite> m_sprite;
	int m_movementPoints;
	int m_healthMax;
	int m_currentHealth;
	int m_range;
	int m_damage;
	int m_weaponType;
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
	EntityBattleProperties(std::pair<int, int> startingPosition, eDirection startingDirection = eNorth);

	eDirection getCurrentDirection() const;
	bool isMovedToDestination() const;
	std::pair<int, int> getCurrentPosition() const;
	bool isWeaponFired() const;
	bool isDead() const;

	void update(float deltaTime, const Map& map, EntityProperties& entityProperties, MoveCounter& gameCounter);
	void render(std::shared_ptr<HAPISPACE::Sprite>& sprite, const Map& map);

	void generateMovementGraph(const Map& map, const Tile& source, const Tile& destination);
	void clearMovementPath();

	void moveEntity(Map& map, const Tile& tile);
	void moveEntity(Map& map, const Tile& tile, eDirection endDirection);
	void takeDamage(EntityProperties& entityProperties, int damageAmount, FactionName entityFaction);
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
	bool m_isDead;

	void handleRotation(EntityProperties& entityProperties);
};

struct BattleEntity
{
	BattleEntity(std::pair<int, int> startingPosition, const EntityProperties& entityProperties, Map& map, FactionName playerName, eDirection startingDirection = eNorth);

	EntityProperties m_entityProperties;
	EntityBattleProperties m_battleProperties;
	//TODO: Might change location of this value
	const FactionName m_factionName;
};

struct Player
{
	Player(FactionName name);

	std::vector<EntityProperties> m_entities;
	std::vector<EntityProperties*> m_selectedEntities;
	const FactionName m_factionName;
};

struct BattlePlayer
{
	BattlePlayer(FactionName name);

	std::vector<std::unique_ptr<BattleEntity>> m_entities;
	const FactionName m_factionName;
};