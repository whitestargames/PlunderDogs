#include "NewPathfinding.h"
#include "Map.h"
#include <queue>
#include <algorithm>

struct tileData
{
	bool isTraversable;
	bool isOccupied;
	bool dir[6];

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

	tileData& access(posi tile) { return data[tile.x + tile.y * width]; }

	int nextTile(int currentTile, eDirection facing)
	{
		int x = currentTile % width;
		int y = static_cast<int>(currentTile / width);
		int nextAddress{ 0 };

		if (x & 1)//odd
		{
			switch (facing)
			{
			case eNorth:
				nextAddress = x + (y - 1) * width;
				break;
			case eNorthEast:
				nextAddress = (x + 1) + (y - 1) * width;
				break;
			case eSouthEast:
				nextAddress = (x + 1) + y * width;
				break;
			case eSouth:
				nextAddress = x + (y + 1) * width;
				break;
			case eSouthWest:
				nextAddress = (x - 1) + y * width;
				break;
			case eNorthWest:
				nextAddress = (x - 1) + (y - 1) * width;
				break;
			}
		}
		else//even
		{
			switch (facing)
			{
			case eNorth:
				nextAddress = x + (y - 1) * width;
				break;
			case eNorthEast:
				nextAddress = (x + 1) + y * width;
				break;
			case eSouthEast:
				nextAddress = (x + 1) + (y + 1) * width;
				break;
			case eSouth:
				nextAddress = x + (y + 1) * width;
				break;
			case eSouthWest:
				nextAddress = (x - 1) + (y + 1) * width;
				break;
			case eNorthWest:
				nextAddress = (x - 1) + y * width;
				break;
			}
			return nextAddress;
		}
	};

	bool pathExplorer(std::vector<tileData>& exploreArea, std::queue<std::pair<int, eDirection>> queue, std::vector<Tile*>& path)
	{
		//dequeue a tile
		//Check if this is the destination if so pushback path and return true
		//Check if it is not traversable or is occupied and if either return false
		//Check if any of FLR are unexplored if so set those to true then enqueue forward, left, and right as appropriate. 
		//Each time, if one of them returns true also return true and pushback path with current tile
	}

	std::vector<Tile*> BreadthFirst::findPath(Map& map, posi startPos, posi endPos, float maxMovement)
	{
		finderMap exploreArea(map);
		std::queue<posi> exploreQueue;
		std::vector<Tile*> pathToTile;
		pathToTile.reserve(30);
		//Add first element and set it to explored
		exploreArea.access(startPos).dir[static_cast<int>(startPos.dir)] = true;
		exploreQueue.emplace(startPos);
		//Start recursion
		pathExplorer(exploreArea, exploreQueue, pathToTile);
		//Invert for convenience

		return pathToTile;
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