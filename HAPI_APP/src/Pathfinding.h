#pragma once
#include <deque>

class Map;



namespace PathFinding
{
	std::deque<std::pair<int, int>> getPathToTile(const Map &map, std::pair<int, int> src, std::pair<int, int> dest);
}