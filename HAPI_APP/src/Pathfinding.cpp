#include "Pathfinding.h"
#include "Map.h"
#include "entity.h"


struct Cell
{
	Cell()
		: m_parent_i(0),
		m_parent_j(0),
		m_f(0),
		m_g(0),
		m_h(0),
		direction(eDirection::eNorth)
	{}

	int m_parent_i;
	int m_parent_j;
	double m_f;
	double m_g;
	double m_h;
	eDirection direction;
};

void tracePath(const std::vector<std::vector<Cell> >& cellDetails, 
	std::pair<int, int> dest, std::vector<pPair>& path);
bool isValid(int row, int col, int sizeX, int sizeY);
bool isUnBlocked(Map &map, std::pair<int, int> coord);
bool isDestination(int row, int col, std::pair<int, int> dest);
double calculateHeuristicValue(int row, int col, std::pair<int, int> dest);
void findAvailableTiles(std::pair<int, int> src, Map &map, int depth);

void tracePath(const std::vector<std::vector<Cell>>& cellDetails, std::pair<int, int> dest, std::vector<pPair>& path)
{
	int row = dest.first;
	int col = dest.second;

	while (!(cellDetails[row][col].m_parent_i == row && cellDetails[row][col].m_parent_j == col))
	{
		path.emplace_back(std::make_pair(cellDetails[row][col].direction, std::make_pair(row, col)));
		int tempRow = cellDetails[row][col].m_parent_i;
		int tempCol = cellDetails[row][col].m_parent_j;

		row = tempRow;
		col = tempCol;
	}
	path.emplace_back(std::make_pair(cellDetails[row][col].direction, std::make_pair(row, col)));
}

bool isValid(int row, int col, int sizeX, int sizeY)
{
	//returns true if the row and col are in range
	return (row >= 0) && (row < sizeX) &&
		(col >= 0) && (col < sizeY);
}

bool isUnBlocked(Map & map, std::pair<int, int> coord)
{
	if (map.getTile(coord) != nullptr)
		if (map.getTile(coord)->m_type == eTileType::eSea ||
			map.getTile(coord)->m_type == eTileType::eOcean)
			return true;
	return false;
}

bool isDestination(int row, int col, std::pair<int, int> dest)
{
	if (row == dest.first && col == dest.second)
		return true;
	else
		return false;
}

double calculateHeuristicValue(int row, int col, std::pair<int, int> dest)
{
	//calculated using Diagonal Distance

	return ((double)sqrt((row - dest.first)*(row - dest.first)
		+ (col - dest.second)*(col - dest.second)));
}

std::vector<pPair> PathFinding::getPathToTile(Map &map, std::pair<int, int> src, std::pair<int, int> dest)
{
	int sizeX = map.getDimensions().first;
	int sizeY = map.getDimensions().second;
	Entity *ship = map.getTile(src)->m_entityOnTile;

	if (!isValid(dest.first, dest.second, sizeX, sizeY))//TODO
	{
		std::cout << "Destination is invalid" << std::endl;
		return std::vector<pPair>();
	}

	if (!isUnBlocked(map, src) || !isUnBlocked(map, dest))
	{
		std::cout << "Source or Destination blocked" << std::endl;
		return std::vector<pPair>();
	}

	if (isDestination(src.first, src.second, dest))
	{
		std::cout << "Destination Already reached" << std::endl;
		return std::vector<pPair>();
	}

	std::vector<std::vector<Cell>> cellDetails;
	cellDetails.resize(sizeX);
	for (int i = 0; i < cellDetails.size(); i++)
		cellDetails[i].resize(sizeY);

	int i = 0, j = 0;

	for (i = 0; i < sizeX; i++)
	{
		for (j = 0; j < sizeY; j++)
		{
			cellDetails[i][j].m_f = FLT_MAX;
			cellDetails[i][j].m_g = FLT_MAX;
			cellDetails[i][j].m_h = FLT_MAX;
			cellDetails[i][j].m_parent_i = -1;
			cellDetails[i][j].m_parent_j = -1;
		}
	}

	i = src.first, j = src.second;
	cellDetails[i][j].m_f = 0.0;
	cellDetails[i][j].m_g = 0.0;
	cellDetails[i][j].m_h = 0.0;
	cellDetails[i][j].m_parent_i = i;
	cellDetails[i][j].m_parent_j = j;

	//bool closedList[size][size];
	std::vector < std::vector<bool>> closedList;
	closedList.resize(sizeX);
	for (int i = 0; i < closedList.size(); i++)
		closedList[i].resize(sizeY);

	//open list contains std::pair<int, int> <f,<i,j>>
	//f = h+g
	//i and j are the rows and cols
	std::set<pPair> openList;
	openList.insert(std::make_pair(0.0, std::make_pair(i, j)));

	bool destFound = false;
	std::vector<pPair> path;
	while (!openList.empty())
	{
		pPair p = *openList.begin();
		openList.erase(openList.begin());

		i = p.second.first;
		j = p.second.second;
		closedList[i][j] = true;
		if (isValid(i, j, sizeX, sizeY))//TODO
		{
			std::vector<Tile*> adjacentCells = map.getAdjacentTiles(std::pair<int, int>(i, j));

			double sucG, sucH, sucF;

			for (int cellIndex = 0; cellIndex < adjacentCells.size(); cellIndex++)
			{
				 //Cell doesn't exist
				if (!adjacentCells[cellIndex])
					continue;

				int x = adjacentCells[cellIndex]->m_tileCoordinate.first;
				int y = adjacentCells[cellIndex]->m_tileCoordinate.second;
				//If adjacent cell is valid
				if (isValid(x, y, sizeX, sizeY))//TODO
				{
					//If cell is destination point
					if (isDestination(x, y, dest))
					{
						
						cellDetails[x][y].m_parent_i = i;
						cellDetails[x][y].m_parent_j = j;
						cellDetails[x][y].direction = (eDirection)cellIndex;
						tracePath(cellDetails, dest, path);
						destFound = true;
						return path;
					}

					else if (!closedList[x][y] && isUnBlocked(map, std::pair<int, int>(x, y)))
					{
						sucG =  1.0;
						sucH = calculateHeuristicValue(x, y, dest);
						sucF = sucG + sucH;

						if (cellDetails[x][y].m_f == FLT_MAX || cellDetails[x][y].m_f > sucF)
						{
							openList.insert(std::make_pair(sucF, std::make_pair(x, y)));
							cellDetails[x][y].m_f = sucF;
							cellDetails[x][y].m_g = sucG;
							cellDetails[x][y].m_h = sucH;
							cellDetails[x][y].m_parent_i = i;
							cellDetails[x][y].m_parent_j = j;
							cellDetails[x][y].direction = (eDirection)cellIndex;
						}
					}
				}
			}
		}
	}
	if (!destFound)
		std::cout << "Failed to find destination" << std::endl;
	return std::vector<pPair>();
}

void findAvailableTiles(std::pair<int, int> src, Map &map, int depth)
{
	int sizeX = map.getDimensions().first;
	int sizeY = map.getDimensions().second;
	int currentDepth = 0;
	int i{ 0 };
	int j{ 0 };

	auto ship = map.getTile(src)->m_entityOnTile;
	
	//ship->setDirection(eDirection::eNorth);
	std::vector < std::vector<bool>> closedList;
	closedList.resize(sizeX);
	for (int i = 0; i < closedList.size(); i++)
		closedList[i].resize(sizeY);

	closedList[src.first][src.second] = true;


	unsigned int dir = 0;
	int totalRange = 0;
	std::set<std::pair<int, int>> openList;
	openList.insert(src);

	std::pair<int, int> p = *openList.begin();
	openList.erase(openList.begin());

	i = p.first;
	j = p.second;
	std::vector<Tile*> adjacentCells = map.getAdjacentTiles(std::pair<int, int>(i, j));
	std::vector<std::pair<int, int>> range;
	for (int cellIndex = 0; cellIndex < adjacentCells.size(); cellIndex++)
	{
		int index = cellIndex;
		////dir = std::abs((int)ship->getDirection() - index);
		//if ((int)index == 5)
		//	//dir = std::abs((int)ship->getDirection() - 1);
		//else if (index == 4)
		//	//dir = std::abs((int)ship->getDirection() - 2);

		if (adjacentCells[index] != nullptr)
		{
			int x = adjacentCells[index]->m_tileCoordinate.first;
			int y = adjacentCells[index]->m_tileCoordinate.second;
			totalRange = depth - dir;
			for (int rangeToSearch = 0; rangeToSearch < totalRange; rangeToSearch++)
			{

				if (isValid(x, y, sizeX, sizeY))
				{
					if (!closedList[x][y] && isUnBlocked(map, std::pair<int, int>(x, y)))
					{
						openList.insert(std::pair<int, int>(x, y));
						closedList[x][y] = true;
						range.push_back(std::pair<int, int>(x, y));
						adjacentCells = map.getAdjacentTiles(std::pair<int, int>(x, y));

						x = adjacentCells[index]->m_tileCoordinate.first;
						y = adjacentCells[index]->m_tileCoordinate.second;


					}
				}
			}

			adjacentCells = map.getAdjacentTiles(std::pair<int, int>(i, j));
		}
	}

}