#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include <memory>
#include <string>
#include "Timer.h"

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
			bool render;
		};
	public:
		MovementPath();

		void render() const;
		void generatePath(Map& map, const Tile& source, const Tile& destination);
		void eraseNode(std::pair<int, int> position, const Map& map);
		void clearPath();

	private:
		std::vector<MovementPathNode> m_movementPath;
		std::unique_ptr<Sprite> m_mouseCursor;
	};

	EntityBattleProperties(std::pair<int, int> startingPosition);

	void update(float deltaTime, const Map& map);
	void render(std::unique_ptr<HAPISPACE::Sprite>& sprite, const Map& map);

	void generateMovementGraph(Map& map, const Tile& source, const Tile& destination);
	void clearMovementPath();
	void moveEntity(Map& map, const Tile& tile, int movementPoints);

	std::pair<int, int> m_currentPosition;
	std::pair<int, int> m_oldPosition;
	std::deque<std::pair<int, int>> m_pathToTile;
	Timer m_movementTimer;
	bool m_movedToDestination;
	MovementPath m_movementPath;
};

struct EntityProperties
{
	EntityProperties(const std::string& spriteName);

	std::unique_ptr<HAPISPACE::Sprite> m_sprite;
	int m_movementPoints;
};

struct BattleEntity
{
	BattleEntity(std::pair<int, int> startingPosition, const std::string& spriteName);

	void setPosition(const Map& map);

	EntityProperties m_entity;
	EntityBattleProperties m_battleProperties;
};