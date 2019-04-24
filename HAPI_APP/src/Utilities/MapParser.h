#pragma once

#include <string>
#include <vector>

struct MapDetails
{
	MapDetails(std::pair<int, int> mapSize, const std::vector<std::vector<int>>& tileData);

	const std::pair<int, int> mapDimensions;
	const std::vector<std::vector<int>> tileData;
};

namespace MapParser
{
	MapDetails parseMap(const std::string& name);
}