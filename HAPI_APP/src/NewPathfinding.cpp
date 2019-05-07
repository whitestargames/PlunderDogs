#include "NewPathfinding.h"
#include "Map.h"
#include <vector>
#include <algorithm>

#define NO_TILE posi(-1, -1, eNorth)

struct tileData
{
	const bool isTraversable;
	const bool isOccupied;
	//The node that was first used to access the corresponding direction during the BFS
	//One for each direction in order
	posi parent[6];

	tileData(bool traversable, bool occupied) :
		isTraversable(traversable), isOccupied(occupied),
		parent{ NO_TILE, NO_TILE, NO_TILE, NO_TILE, NO_TILE, NO_TILE } {}
};

struct finderMap
{
	int width;
	std::vector<tileData> data;

	finderMap(const Map& map);
	tileData& access(posi tile) { return data[tile.x + tile.y * width]; }

	posi nextTile(const posi& currentTile) const;
	posi turnLeft(const posi& currentTile) const;
	posi turnRight(const posi& currentTile) const;
};

bool pathExplorer(finderMap& exploreArea, std::queue<posi>& queue, posi destination, bool destDirIgnore)
{
	//Dequeue a tile
	posi tile = queue.front();
	queue.pop();
	//Check if this is the destination
	if (destDirIgnore && tile.pair() == destination.pair())
		return true;
	else if (tile == destination)
		return true;
	//Check if any movements are unexplored and movable, if so set those to true and set their parent to this tile
	//Then enqueue forward, left, and right as appropriate:
	//Forward
	posi forward = exploreArea.nextTile(tile);
	if (forward != NO_TILE && 
		exploreArea.access(forward).parent[forward.dir] == NO_TILE && 
		exploreArea.access(forward).isTraversable && 
		!exploreArea.access(forward).isOccupied)
	{
		exploreArea.access(forward).parent[forward.dir] = tile;
		queue.emplace(forward);
	}
	//Left
	posi left = exploreArea.turnLeft(tile);
	if (exploreArea.access(left).parent[left.dir] == NO_TILE)
	{
		exploreArea.access(left).parent[left.dir] = tile;
		queue.emplace(left);
	}
	//Right
	posi right = exploreArea.turnRight(tile);
	if (exploreArea.access(right).parent[right.dir] == NO_TILE)
	{
		exploreArea.access(right).parent[right.dir] = tile;
		queue.emplace(right);
	}
	//If queue is not empty run algorithm again
	bool final{ false };
	if (!queue.empty())
		final = pathExplorer(exploreArea, queue, destination, destDirIgnore);
	return final;
}

std::queue<posi> BFS::findPath(const Map& map, posi startPos, posi endPos, bool ignoreEndDirection, float maxMovement)
{
	//No bullshit
	if (!map.getTile(endPos) || 
		map.getTile(endPos)->m_entityOnTile || 
		(map.getTile(endPos)->m_type != eSea && map.getTile(endPos)->m_type != eOcean))
		return std::queue<posi>();
	//Initialise variables
	finderMap exploreArea(map);
	std::queue<posi> exploreQueue;
	//Add first element and set it to explored
	exploreQueue.emplace(startPos);
	//Start recursion
	if (!pathExplorer(exploreArea, exploreQueue, endPos, ignoreEndDirection))
		return std::queue<posi>();
	//Trace path back from destination via parents
	std::vector<posi> pathToTile;
	pathToTile.reserve(30);
	posi trace = endPos;
	if (ignoreEndDirection)
	{
		//Find which direction the algorithm found endPos in
		for (int i = 0; i < 6; i++)
		{
			if (exploreArea.access(trace).parent[i] != NO_TILE)
			{
				trace.dir = static_cast<eDirection>(i);
				break;
			}
		}
		while (trace.pair() != startPos.pair())
		{
			pathToTile.emplace_back(trace);
			trace = exploreArea.access(trace).parent[trace.dir];
		}
	}
	else
	{
		while (trace != startPos)
		{
			pathToTile.emplace_back(trace);
			trace = exploreArea.access(trace).parent[trace.dir];
		}
	}
	//Invert for convenience and fetch tile* for each address
	std::queue<posi> finalPath;
	for (int i = pathToTile.size() - 1; i >= 0; i--)
	{
		finalPath.emplace(pathToTile[i]);
	}
	return finalPath;
}

finderMap::finderMap(const Map& map) : width(map.getDimensions().first)
{
	data.reserve(map.getDimensions().first * map.getDimensions().second);
	for (const Tile& it : map.getData())
	{
		bool traversable = (it.m_type == eSea || it.m_type == eOcean);
		bool occupied = static_cast<bool>(it.m_entityOnTile);
		data.emplace_back(traversable, occupied);
	}
}

posi finderMap::nextTile(const posi& currentTile) const
{
	int x = currentTile.x;
	int y = currentTile.y;
	posi nextAddress;
	nextAddress.dir = currentTile.dir;
	if (x & 1)//odd
	{
		switch (currentTile.dir)
		{
		case eNorth:
			nextAddress.x = x;
			nextAddress.y = y - 1;
			break;
		case eNorthEast:
			nextAddress.x = x + 1;
			nextAddress.y = y - 1;
			break;
		case eSouthEast:
			nextAddress.x = x + 1;
			nextAddress.y = y;
			break;
		case eSouth:
			nextAddress.x = x;
			nextAddress.y = y + 1;
			break;
		case eSouthWest:
			nextAddress.x = x - 1;
			nextAddress.y = y;
			break;
		case eNorthWest:
			nextAddress = x - 1;
			nextAddress.y = y - 1;
			break;
		}
	}
	else//even
	{
		switch (currentTile.dir)
		{
		case eNorth:
			nextAddress = x;
			nextAddress.y = y - 1;
			break;
		case eNorthEast:
			nextAddress = x + 1;
			nextAddress.y = y;
			break;
		case eSouthEast:
			nextAddress = x + 1;
			nextAddress.y = y + 1;
			break;
		case eSouth:
			nextAddress = x;
			nextAddress.y = y + 1;
			break;
		case eSouthWest:
			nextAddress = x - 1;
			nextAddress.y = y + 1;
			break;
		case eNorthWest:
			nextAddress = x - 1;
			nextAddress.y = y;
			break;
		}
	}
	//Bounds checking
	if (nextAddress.x < 0 || nextAddress.y < 0 || nextAddress.x >= width || (nextAddress.x + nextAddress.y * width) >= data.size())
		nextAddress = NO_TILE;
	return nextAddress;
}

posi finderMap::turnLeft(const posi& currentTile) const
{
	posi nextTile = currentTile;
	switch (currentTile.dir)
	{
	case eNorth:
		nextTile.dir = eNorthWest;
		break;
	case eNorthEast:
		nextTile.dir = eNorth;
		break;
	case eSouthEast:
		nextTile.dir = eNorthEast;
		break;
	case eSouth:
		nextTile.dir = eSouthEast;
		break;
	case eSouthWest:
		nextTile.dir = eSouth;
		break;
	case eNorthWest:
		nextTile.dir = eSouthWest;
		break;
	}
	return nextTile;
}

posi finderMap::turnRight(const posi& currentTile) const
{
	posi nextTile = currentTile;
	switch (currentTile.dir)
	{
	case eNorth:
		nextTile.dir = eNorthEast;
		break;
	case eNorthEast:
		nextTile.dir = eSouthEast;
		break;
	case eSouthEast:
		nextTile.dir = eSouth;
		break;
	case eSouth:
		nextTile.dir = eSouthWest;
		break;
	case eSouthWest:
		nextTile.dir = eNorthWest;
		break;
	case eNorthWest:
		nextTile.dir = eNorth;
		break;
	}
	return nextTile;
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