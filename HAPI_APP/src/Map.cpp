#include "Map.h"
#include <memory>
#include <math.h>
#include <algorithm>
#include <HAPISprites_Lib.h>
#include "Utilities/Utilities.h"
#include "entity.h"
#include "Textures.h"

typedef std::pair<int, int> intPair;

constexpr int FRAME_HEIGHT{ 28 };
constexpr float FRAME_CENTRE_X{ 15.5 };
constexpr float FRAME_CENTRE_Y{ 32.5 };

void Map::drawMap() const 
{

	
	
	intPair textureDimensions = intPair(
		m_data[0].m_daySprite->FrameWidth(), 
		FRAME_HEIGHT);

	int access{ 0 };
	for (int y = 0; y < m_mapDimensions.second; y++)
	{
		const float yPosEven = (float)(0.5 + y) * textureDimensions.second;
		const float yPosOdd = (float)y * textureDimensions.second;

		for (int x = 1; x < m_mapDimensions.first; x += 2)
		{
			const float xPos = (float)x * textureDimensions.first * 3 / 4;
			switch (m_timeOfDay)
			{
			case eGameTime::eMorning:
				m_data[access + x].m_daySprite->GetTransformComp().SetPosition(HAPISPACE::VectorF(
					(xPos - m_drawOffset.first)*m_drawScale,
					(yPosOdd - m_drawOffset.second)*m_drawScale));
				m_data[access + x].m_daySprite->GetTransformComp().SetScaling(
					HAPISPACE::VectorF(m_drawScale, m_drawScale));
				m_data[access + x].m_daySprite->Render(SCREEN_SURFACE);
				break;
			case eGameTime::eAfternoon:
				m_data[access + x].m_aftersprite->GetTransformComp().SetPosition(HAPISPACE::VectorF(
					(xPos - m_drawOffset.first)*m_drawScale,
					(yPosOdd - m_drawOffset.second)*m_drawScale));
				m_data[access + x].m_aftersprite->GetTransformComp().SetScaling(
					HAPISPACE::VectorF(m_drawScale, m_drawScale));
				m_data[access + x].m_aftersprite->Render(SCREEN_SURFACE);
				break;
			case eGameTime::eEvening:
				m_data[access + x].m_eveningSprite->GetTransformComp().SetPosition(HAPISPACE::VectorF(
					(xPos - m_drawOffset.first)*m_drawScale,
					(yPosOdd - m_drawOffset.second)*m_drawScale));
				m_data[access + x].m_eveningSprite->GetTransformComp().SetScaling(
					HAPISPACE::VectorF(m_drawScale, m_drawScale));
				m_data[access + x].m_eveningSprite->Render(SCREEN_SURFACE);
				break;
			case eGameTime::eNight:
				m_data[access + x].m_nightSprite->GetTransformComp().SetPosition(HAPISPACE::VectorF(
					(xPos - m_drawOffset.first)*m_drawScale,
					(yPosOdd - m_drawOffset.second)*m_drawScale));
				m_data[access + x].m_nightSprite->GetTransformComp().SetScaling(
					HAPISPACE::VectorF(m_drawScale, m_drawScale));
				m_data[access + x].m_nightSprite->Render(SCREEN_SURFACE);
				break;
			}
			//Is Odd
			
		}
		for (int x = 0; x < m_mapDimensions.first; x += 2)
		{
			const float xPos = (float)x * textureDimensions.first * 3 / 4;
			//Is even
			switch (m_timeOfDay)
			{
			case eGameTime::eMorning:
				m_data[access + x].m_daySprite->GetTransformComp().SetPosition(HAPISPACE::VectorF(
					(xPos - m_drawOffset.first)*m_drawScale,
					(yPosEven - m_drawOffset.second)*m_drawScale));
				m_data[access + x].m_daySprite->GetTransformComp().SetScaling(
					HAPISPACE::VectorF(m_drawScale, m_drawScale));
				m_data[access + x].m_daySprite->Render(SCREEN_SURFACE);
				break;
			case eGameTime::eAfternoon:
				m_data[access + x].m_aftersprite->GetTransformComp().SetPosition(HAPISPACE::VectorF(
					(xPos - m_drawOffset.first)*m_drawScale,
					(yPosEven - m_drawOffset.second)*m_drawScale));
				m_data[access + x].m_aftersprite->GetTransformComp().SetScaling(
					HAPISPACE::VectorF(m_drawScale, m_drawScale));
				m_data[access + x].m_aftersprite->Render(SCREEN_SURFACE);
				break;
			case eGameTime::eEvening:
				m_data[access + x].m_eveningSprite->GetTransformComp().SetPosition(HAPISPACE::VectorF(
					(xPos - m_drawOffset.first)*m_drawScale,
					(yPosEven - m_drawOffset.second)*m_drawScale));
				m_data[access + x].m_eveningSprite->GetTransformComp().SetScaling(
					HAPISPACE::VectorF(m_drawScale, m_drawScale));
				m_data[access + x].m_eveningSprite->Render(SCREEN_SURFACE);
				break;
			case eGameTime::eNight:
				m_data[access + x].m_nightSprite->GetTransformComp().SetPosition(HAPISPACE::VectorF(
					(xPos - m_drawOffset.first)*m_drawScale,
					(yPosEven - m_drawOffset.second)*m_drawScale));
				m_data[access + x].m_nightSprite->GetTransformComp().SetScaling(
					HAPISPACE::VectorF(m_drawScale, m_drawScale));
				m_data[access + x].m_nightSprite->Render(SCREEN_SURFACE);
				break;
			}
		
		}
		access += m_mapDimensions.first;
	}
}

intPair Map::offsetToCube(intPair offset) const
{
	int cubeX = offset.first;
	int cubeY = - offset.first - (offset.second - (offset.first + (offset.first & 1)) / 2);
	int cubeZ = -cubeX - cubeY;
	return intPair(cubeX, cubeY);
}

intPair Map::cubeToOffset(intPair cube) const
{
	int offsetX = cube.first;
	int offsetY = -cube.first - cube.second + (cube.first + (cube.first & 1)) / 2;
	return intPair(offsetX, offsetY);
}

int Map::cubeDistance(intPair a, intPair b) const
{
	const int x = abs(a.first - b.first);
	const int y = abs(a.second - b.second);
	const int z = abs(a.first + a.second - b.first - b.second);
	return std::max(x, std::max(y, z));
}

bool Map::inCone(intPair orgHex, intPair testHex, eDirection dir) const
{
	const intPair diff(testHex.first - orgHex.first, testHex.second - orgHex.second);
	const int zDiff = -diff.first - diff.second;
	if (dir == eNorth || dir == eSouth)//Axis x = 0
	{
		//Deadzones y pos and z neg or y neg and z pos
		if ((diff.second > 0 && zDiff < 0) || (diff.second < 0 && zDiff > 0))
			return false;
	}
	else if (dir == eNorthEast || dir == eSouthWest)//Axis y = 0
	{
		//Deadzones x pos and z neg or x neg and z pos
		if ((diff.first > 0 && zDiff < 0) || (diff.first < 0 && zDiff > 0))
			return false;
	}
	else if (dir == eNorthWest || dir == eSouthEast)//Axis z = 0
	{
		//Deadzones x pos and y neg or x neg and y pos
		if ((diff.first > 0 && diff.second < 0) || (diff.first < 0 && diff.second > 0))
			return false;
	}
	return true;
}

float Map::tileDistanceMag(intPair tileCoord, intPair mouseClick) const
{
	std::pair<float, float> tileCentre = getTileScreenPos(tileCoord);
	tileCentre.first += FRAME_CENTRE_X * m_drawScale;
	tileCentre.second += FRAME_CENTRE_Y * m_drawScale;

	const float diffX = tileCentre.first - static_cast<float>(mouseClick.first);
	const float diffY = tileCentre.second - static_cast<float>(mouseClick.second);

	return (diffX * diffX) + (diffY * diffY);
}

Tile* Map::getTile(intPair coordinate)
{
	//Bounds check
	if (coordinate.first < m_mapDimensions.first &&
		coordinate.second < m_mapDimensions.second &&
		coordinate.first >= 0 &&
		coordinate.second >= 0)
	{	 
		return &m_data[coordinate.first + coordinate.second * m_mapDimensions.first];
	}
	/*
	HAPI_Sprites.UserMessage(
		std::string("getTile request out of bounds: " + std::to_string(coordinate.first) +
			", " + std::to_string(coordinate.second) + " map dimensions are: " +
			std::to_string(m_mapDimensions.first) +", "+ std::to_string(m_mapDimensions.second)),
		"Map error");
	*/
	return nullptr;
}

std::vector<Tile*> Map::getAdjacentTiles(intPair coord)
{
	const size_t allAdjacentTiles = 6;
	std::vector<Tile*> result;
	result.reserve(size_t(allAdjacentTiles));
	if (coord.first & 1)//Is an odd tile
	{
		result.push_back(getTile(intPair(coord.first, coord.second - 1)));		//N
		result.push_back(getTile(intPair(coord.first + 1, coord.second - 1)));	//NE
		result.push_back(getTile(intPair(coord.first + 1, coord.second)));		//SE
		result.push_back(getTile(intPair(coord.first, coord.second + 1)));		//S
		result.push_back(getTile(intPair(coord.first - 1, coord.second)));		//SW
		result.push_back(getTile(intPair(coord.first - 1, coord.second - 1)));	//NW
	}
	else//Is even
	{
		result.push_back(getTile(intPair(coord.first, coord.second - 1)));		//N
		result.push_back(getTile(intPair(coord.first + 1, coord.second)));		//NE
		result.push_back(getTile(intPair(coord.first + 1, coord.second + 1)));	//SE
		result.push_back(getTile(intPair(coord.first, coord.second + 1)));		//S
		result.push_back(getTile(intPair(coord.first - 1, coord.second + 1)));	//SW
		result.push_back(getTile(intPair(coord.first - 1, coord.second)));		//NW
	}
	return result;
}

std::vector<Tile*> Map::getTileRadius(intPair coord, int range)
{
	if (range < 1)
		HAPI_Sprites.UserMessage("getTileRadius range less than 1", "Map error");
	
	int reserveSize{ 0 };
	for (int i = 1; i <= range; i++)
	{
		reserveSize += 6 * i;
	}
	std::vector<Tile*> tileStore;

	tileStore.reserve((size_t)reserveSize);

	intPair cubeCoord(offsetToCube(coord));

	for (int y = std::max(0, coord.second - range);
		y < std::min(m_mapDimensions.second, coord.second + range + 1);
		y++)
	{
		for (int x = std::max(0, coord.first - range);
			x < std::min(m_mapDimensions.first, coord.first + range + 1);
			x++)
		{
			if (!(coord.first == x && coord.second == y))//If not the tile at the centre
			{
				if (cubeDistance(cubeCoord, offsetToCube(intPair(x, y))) <= range)
				{
					tileStore.push_back(getTile(intPair(x, y)));
				}
			}
		}
	}
	return tileStore;
}

std::vector< Tile*> Map::getTileCone(intPair coord, int range, eDirection direction)
{
	if (range < 1)
		HAPI_Sprites.UserMessage("getTileCone range less than 1", "Map error");

	int reserveSize{ 0 };
	for (int i = 2; i < range + 2; i++)
	{
		reserveSize += 2 * i;
	}
	std::vector<Tile*> tileStore;
	tileStore.reserve((size_t)reserveSize);

	const intPair cubeCoord(offsetToCube(coord));

	for (int y = std::max(0, coord.second - range - 1);
		y < std::min(m_mapDimensions.second, coord.second + range + 2);
		y++)
	{
		for (int x = std::max(0, coord.first - range - 1);
			x < std::min(m_mapDimensions.first, coord.first + range + 2);
			x++)
		{
			if (!(coord.first == x && coord.second == y))//If not the tile at the centre
			{
				intPair tempCube(offsetToCube(intPair(x, y)));
				if (cubeDistance(cubeCoord, tempCube) <= range)
				{
					if (inCone(cubeCoord, tempCube, direction))
					{
						Tile* tile = getTile(intPair(x, y));

						if (tile && (tile->m_type == eTileType::eSea || tile->m_type == eTileType::eOcean))
						{
							tileStore.push_back(getTile(intPair(x, y)));
						}
					}
				}
			}
		}
	}
	return tileStore;
}

std::vector<const Tile*> Map::getTileCone(intPair coord, int range, eDirection direction) const
{
	if (range < 1)
		HAPI_Sprites.UserMessage("getTileCone range less than 1", "Map error");

	int reserveSize{ 0 };
	for (int i = 2; i < range + 2; i++)
	{
		reserveSize += 2 * i;
	}
	std::vector<const Tile*> tileStore;
	tileStore.reserve((size_t)reserveSize);

	const intPair cubeCoord(offsetToCube(coord));

	for (int y = std::max(0, coord.second - range - 1);
		y < std::min(m_mapDimensions.second, coord.second + range + 2);
		y++)
	{
		for (int x = std::max(0, coord.first - range - 1);
			x < std::min(m_mapDimensions.first, coord.first + range + 2);
			x++)
		{
			if (!(coord.first == x && coord.second == y))//If not the tile at the centre
			{
				intPair tempCube(offsetToCube(intPair(x, y)));
				if (cubeDistance(cubeCoord, tempCube) <= range)
				{
					if (inCone(cubeCoord, tempCube, direction))
					{
						const Tile* tile = getTile(intPair(x, y));
						if (tile && (tile->m_type == eTileType::eSea || tile->m_type == eTileType::eOcean))
						{
							tileStore.push_back(getTile(intPair(x, y)));
						}
					}
				}
			}
		}
	}
	return tileStore;
}

bool Map::moveEntity(intPair originalPos, intPair newPos)
{
	Tile* oldTile = getTile(originalPos);
	Tile* newTile = getTile(newPos);
	
	if (!oldTile || !newTile)
	{
		return false;
	}

	if (newTile->m_entityOnTile != nullptr || oldTile->m_entityOnTile == nullptr)
	{
		return false;
	}

	newTile->m_entityOnTile = oldTile->m_entityOnTile;
	oldTile->m_entityOnTile = nullptr;
	return true;
}

eGameTime Map::getTimeOfDay() const
{
	return m_timeOfDay;
}

void Map::setTimeOfDay(eGameTime timeOfDay)
{
	m_timeOfDay = timeOfDay;
}

void Map::insertEntity(BattleEntity& newEntity)
{
	Tile* tile = getTile(newEntity.m_battleProperties.getCurrentPosition());	
	if (tile && !tile->m_entityOnTile)
	{
		tile->m_entityOnTile = &newEntity;
	}
}

intPair Map::getTileScreenPos(intPair coord) const
{
	intPair textureDimensions = intPair(
		m_data[0].m_daySprite->FrameWidth(),
		FRAME_HEIGHT);

	const float xPos = static_cast<float>(coord.first * textureDimensions.first) * 3 / 4;
	const float yPos = static_cast<float>((((1 + coord.first) % 2) + 2 * coord.second)
		* textureDimensions.second) / 2;

	return intPair(
		(xPos - m_drawOffset.first)* m_drawScale,
		(yPos - m_drawOffset.second)* m_drawScale);
}

intPair Map::getMouseClickCoord(intPair mouseCoord) const
{
	intPair textureDimensions = intPair(
		m_data[0].m_daySprite->FrameWidth(),
		FRAME_HEIGHT);
	
	const float translatedX = (static_cast<float>(mouseCoord.first - FRAME_CENTRE_X) / m_drawScale) + static_cast<float>(m_drawOffset.first);
	const float translatedY = (static_cast<float>(mouseCoord.second - FRAME_CENTRE_Y) / m_drawScale) + static_cast<float>(m_drawOffset.second);
	const int predictedTileX = static_cast<const int>(translatedX * 4 / (3 * textureDimensions.first));
	const int predictedTileY = static_cast<const int>(translatedY / textureDimensions.second);
	
	float distance{ 10000000 };//An arbitrary big number
	intPair closestTile(predictedTileX, predictedTileY);
	//Iterate through the 9 tiles around the guess to find the actual closest tile to the click
	for (int y = predictedTileY - 1; y <= predictedTileY + 1; y++)
	{
		for (int x = predictedTileX - 1; x <= predictedTileX + 1; x++)
		{
			const float tempDist = tileDistanceMag(intPair(x, y), mouseCoord);
			if (tempDist < distance)
			{
				distance = tempDist;
				closestTile = intPair(x, y);
			}
		}
	}
	return closestTile;
}

Map::Map(intPair size, const std::vector<std::vector<int>>& tileData) :
	m_mapDimensions(size),
	m_data(),
	m_drawOffset(intPair(10, 60)),
	m_windDirection(eNorth),
	m_windStrength(0.4),
	m_drawScale(2),
	m_timeOfDay(eMorning)
{
	m_data.reserve(m_mapDimensions.first * m_mapDimensions.second);

	//TODO: Will be loaded in through Tiled
	m_spawnPositions.emplace_back(11, 6);
	m_spawnPositions.emplace_back(8, 18);
	m_spawnPositions.emplace_back(28, 28);
	m_spawnPositions.emplace_back(20, 10);

	for (int y = 0; y < m_mapDimensions.second; y++)
	{
		for (int x = 0; x < m_mapDimensions.first; x++)
		{
			const int tileID = tileData[y][x];
			assert(tileID != -1);
			m_data.emplace_back(static_cast<eTileType>(tileID), 
				Textures::m_hexTiles, Textures::m_afternoonHexTiles, 
				Textures::m_eveningHexTiles, Textures::m_nightHexTiles, intPair(x, y));

			if (!m_data[x + y * m_mapDimensions.first].m_daySprite)
			{
				HAPI_Sprites.UserMessage("Could not load tile spritesheet", "Error");
				return;
			}
			m_data[x + y * m_mapDimensions.first].m_daySprite->SetFrameNumber(tileID);
			m_data[x + y * m_mapDimensions.first].m_aftersprite->SetFrameNumber(tileID);
			m_data[x + y * m_mapDimensions.first].m_eveningSprite->SetFrameNumber(tileID);
			m_data[x + y * m_mapDimensions.first].m_nightSprite->SetFrameNumber(tileID);
		}
	}
}

const Tile * Map::getTile(std::pair<int, int> coordinate) const
{
	//Bounds check
	if (coordinate.first < m_mapDimensions.first &&
		coordinate.second < m_mapDimensions.second &&
		coordinate.first >= 0 &&
		coordinate.second >= 0)
	{
		return &m_data[coordinate.first + coordinate.second * m_mapDimensions.first];
	}
	/*
	HAPI_Sprites.UserMessage(
		std::string("getTile request out of bounds: " + std::to_string(coordinate.first) +
			", " + std::to_string(coordinate.second) + " map dimensions are: " +
			std::to_string(m_mapDimensions.first) +", "+ std::to_string(m_mapDimensions.second)),
		"Map error");
	*/
	return nullptr;
}

std::vector<const Tile*> Map::getAdjacentTiles(std::pair<int, int> coord) const
{
	const size_t allAdjacentTiles = 6;
	std::vector<const Tile*> result;
	result.reserve(size_t(allAdjacentTiles));
	if (coord.first & 1)//Is an odd tile
	{
		result.push_back(getTile(intPair(coord.first, coord.second - 1)));		//N
		result.push_back(getTile(intPair(coord.first + 1, coord.second - 1)));	//NE
		result.push_back(getTile(intPair(coord.first + 1, coord.second)));		//SE
		result.push_back(getTile(intPair(coord.first, coord.second + 1)));		//S
		result.push_back(getTile(intPair(coord.first - 1, coord.second)));		//SW
		result.push_back(getTile(intPair(coord.first - 1, coord.second - 1)));	//NW
	}
	else//Is even
	{
		result.push_back(getTile(intPair(coord.first, coord.second - 1)));		//N
		result.push_back(getTile(intPair(coord.first + 1, coord.second)));		//NE
		result.push_back(getTile(intPair(coord.first + 1, coord.second + 1)));	//SE
		result.push_back(getTile(intPair(coord.first, coord.second + 1)));		//S
		result.push_back(getTile(intPair(coord.first - 1, coord.second + 1)));	//SW
		result.push_back(getTile(intPair(coord.first - 1, coord.second)));		//NW
	}
	return result;
}

std::vector<const Tile*> Map::getTileRadius(std::pair<int, int> coord, int range) const
{
	if (range < 1)
		HAPI_Sprites.UserMessage("getTileRadius range less than 1", "Map error");

	int reserveSize{ 0 };
	for (int i = 1; i <= range; i++)
	{
		reserveSize += 6 * i;
	}
	std::vector<const Tile*> tileStore;

	tileStore.reserve((size_t)reserveSize);

	intPair cubeCoord(offsetToCube(coord));

	for (int y = std::max(0, coord.second - range);
		y < std::min(m_mapDimensions.second, coord.second + range + 1);
		y++)
	{
		for (int x = std::max(0, coord.first - range);
			x < std::min(m_mapDimensions.first, coord.first + range + 1);
			x++)
		{
			if (!(coord.first == x && coord.second == y))//If not the tile at the centre
			{
				if (cubeDistance(cubeCoord, offsetToCube(intPair(x, y))) <= range)
				{
					tileStore.push_back(getTile(intPair(x, y)));
				}
			}
		}
	}
	return tileStore;
}

std::vector<Tile*> Map::getTileLine(
	std::pair<int, int> coord, int range, eDirection direction)
{
	std::vector<Tile*> tileStore;
	tileStore.reserve(range);
	Tile* pushBackTile{ getTile(coord) };
	for (int i = 0; i < range; i++)
	{
		if (pushBackTile)
			pushBackTile = getAdjacentTiles(pushBackTile->m_tileCoordinate)[direction];
		tileStore.emplace_back(pushBackTile);
	}
	return tileStore;
}

std::vector<const Tile*> Map::getTileLine(
	std::pair<int, int> coord, int range, eDirection direction)const
{
	std::vector<const Tile*> tileStore;
	tileStore.reserve(range);
	const Tile* pushBackTile{ getTile(coord) };
	assert(pushBackTile);
	for (int i = 0; i < range; i++)
	{
		if (pushBackTile)
			pushBackTile = getAdjacentTiles(pushBackTile->m_tileCoordinate)[direction];
		tileStore.emplace_back(pushBackTile);
	}
	return tileStore;
}

std::vector<std::pair<int, int>> Map::getSpawnPositions() const
{
	return m_spawnPositions;
}
