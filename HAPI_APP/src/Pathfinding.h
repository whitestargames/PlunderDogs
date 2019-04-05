#pragma once
#include <iostream>
#include <stack>
#include <set>
#include <vector>

class Map;
struct Tile;



namespace PathFinding
{
	std::vector<std::pair<double, std::pair<int, int>>> getPathToTile(Map &map, std::pair<int, int> src, std::pair<int, int> dest);
}