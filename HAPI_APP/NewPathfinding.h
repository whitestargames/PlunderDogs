#pragma once
#include <vector>
#include "Global.h"

struct Tile;
class Map;

namespace BreadthFirst
{
	//A recursive algorithm for finding the optimal path to an
	std::vector<Tile*> findPath(Map& map, posi startPos, posi endPos, float maxMovement = FLT_MAX);
};

