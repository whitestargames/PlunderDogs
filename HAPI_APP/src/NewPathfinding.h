#pragma once
#include <vector>
#include <queue>
#include "Global.h"

struct Tile;
class Map;

namespace BFS
{
	//A recursive algorithm for finding the optimal path to an
	std::queue<posi> findPath(const Map& map, posi startPos, posi endPos, bool ignoreEndDirection = false, float maxMovement = FLT_MAX);
};

