#include "NewPathfinding.h"
#include "Map.h""

struct tileData
{
	bool isTraversable;
	bool isOccupied;
	bool dir [6];

	tileData(bool traversable, bool occupied) : isTraversable(traversable), isOccupied(occupied), dir{ 0,0,0,0,0,0 } {}
};

struct finderMap
{
	int width;
	std::vector<tileData> data;

	finderMap(const Map& map) : width(map.getDimensions().first)
	{
		data.reserve(map.getDimensions().first * map.getDimensions().second);
		for (const Tile& it : map.getData())
		{
			bool traversable = (it.m_type == eSea || it.m_type == eOcean);
			bool occupied = static_cast<bool>(it.m_entityOnTile);
			data.emplace_back(traversable, occupied);
		}
	}

	tileData& nextTile(tileData& currentTile, eDirection facing)
	{

	}
};

std::vector<Tile*> BreadthFirst::findPath(Map& map, posi startPos, posi endPos, float maxMovement)
{
	finderMap exploreArea(map);
	exploreArea
}

bool pathExplorer(std::vector<tileData>& explorationArea, std::vector<Tile*>& path, tileData& currentTile, eDirection facing)
{

}


/*
//A 1 byte store for all the data, very compact but annoying to use
struct byteStore
{
	char byte;
	byteStore() : byte(0) {}

	//First bit
	bool traversable() { return byte & 128; }
	void setTraversable(bool isTraversable)
	{
		if (isTraversable)
			byte |= (1 << 7);
		else
			byte &= ~(1 << 7);
	}
	//Second bit
	bool occupied() { return byte & 64; }
	void setOccupied(bool isOccupied)
	{
		if (isOccupied)
			byte |= (1 << 6);
		else
			byte &= ~(1 << 6);
	}
	//Available directions
	bool north() { return byte & 32; }
	bool northEast() { return byte & 16; }
	bool southEast() { return byte & 8; }
	bool south() { return byte & 4; }
	bool southWest() { return byte & 2; }
	bool northWest() { return byte & 1; }

	void setNorth(bool north)
	{
		if (north)
			byte |= (1 << 5);
		else
			byte &= ~(1 << 5);
	}
	void setNorthEast(bool northEast)
	{
		if (northEast)
			byte |= (1 << 4);
		else
			byte &= ~(1 << 4);
	}
	void setSouthEast(bool southEast)
	{
		if (southEast)
			byte |= (1 << 3);
		else
			byte &= ~(1 << 3);
	}
	void setSouth(bool south)
	{
		if (south)
			byte |= (1 << 2);
		else
			byte &= ~(1 << 2);
	}
	void setSouthWest(bool southWest)
	{
		if (southWest)
			byte |= (1 << 1);
		else
			byte &= ~(1 << 1);
	}
	void setNorthWest(bool northWest)
	{
		if (northWest)
			byte |= 1;
		else
			byte &= ~1;
	}
};
*/