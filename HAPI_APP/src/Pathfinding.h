#pragma once
#include <iostream>
#include <stack>
#include <set>
#include <vector>
#include "Global.h"

class Map;
struct Tile;



namespace PathFinding
{
	std::vector<std::pair<eDirection, std::pair<int, int>>> getPathToTile(Map &map, std::pair<int, int> src, std::pair<int, int> dest);
}