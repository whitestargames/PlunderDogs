#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include <memory>
#include <string>
#include "Timer.h"

struct EntityDetails
{
	EntityDetails();

	std::pair<int, int> m_currentPosition;
	std::pair<int, int> m_oldPosition;
	std::deque<std::pair<int, int>> m_pathToTile;
	Timer m_movementTimer;
	bool m_moving;
};

struct Tile;
class Map;
struct Entity
{
	class MovementPath
	{
	public:
		MovementPath();

		void render() const;
		void generatePath(Map& map, const Tile& source, const Tile& destination);
		void eraseNode(std::pair<int, int> position, Map& map);

	private:
		std::vector<std::pair<std::unique_ptr<Sprite>, bool>> m_movementPath;
		std::unique_ptr<Sprite> m_mouseCursor;

		void clearPath();
		
	};

	Entity(const std::string& spriteName);
	
	void update(float deltaTime, EntityDetails& entityDetails, Map& map);
	void render(Map& map, const EntityDetails& entityDetails);

	std::unique_ptr<HAPISPACE::Sprite> m_sprite;
	int m_movementPoints;
	MovementPath m_movementPath;
};