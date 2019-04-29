#include <vector>
#include "AI.h"
#include "MouseSelection.h"
#include "pathfinding.h"
#include "Battle.h"
#include "Map.h"
#include "entity.h"

constexpr int MAX_INT{ 2147483647 };

const Tile* AI::findClosestEnemy(
	const Battle* battlePtr, const Map* mapPtr, const std::shared_ptr<BattleEntity> alliedShip, FactionName faction)
{
	const Tile* closestEnemy{ nullptr };
	int closestDistance{ MAX_INT };
	std::pair<int, int> alliedPos{ MouseSelection::coordToHexPos(
		alliedShip->m_battleProperties.getCurrentPosition()) };
	for (int i = 0; i < 4; i++)
	{
		if (i == static_cast<int>(faction))
			continue;
		const auto factionShipList = *battlePtr->getFactionShips(static_cast<FactionName>(i));
		for (int j = 0; j < factionShipList.size(); j++)
		{
			if (factionShipList[j]->m_battleProperties.isDead()) continue;
			//Find the distance^2 from the allied ship to the enemy ship, 
			//then set closestEnemy to that enemy if it's the closest yet found
			std::pair<int, int> enemyPos = MouseSelection::coordToHexPos(
				factionShipList[j]->m_battleProperties.getCurrentPosition());
			std::pair<int, int> diff(
				{ enemyPos.first - alliedPos.first, enemyPos.second - alliedPos.second });
			int enemyDistance = diff.first * diff.first + diff.second * diff.second;
			if (enemyDistance < closestDistance)
			{
				closestDistance = enemyDistance;
				closestEnemy = mapPtr->getTile(factionShipList[j]->m_battleProperties.getCurrentPosition());
			}
		}
	}
	return closestEnemy;
}
//Support function for AI::findFiringPositions. 
//Finds the closest tile a specified distance away from a target tile
const Tile* firePosRadial(
	Map* mapPtr, const Tile* targetShip, const Tile* alliedShip, int range)
{
	const Tile* closestTile{ alliedShip };
	int closestDistance{ MAX_INT };
	std::pair<int, int> alliedPos{ MouseSelection::coordToHexPos(alliedShip->m_tileCoordinate) };
	//TODO: can't use const Tile* for some reason
	std::vector<Tile*> availableTiles{ mapPtr->getTileRing(targetShip->m_tileCoordinate, range) };
	for (Tile* it : availableTiles)
	{
		//Ensure it's a valid tile, if not skip this one
		if (!it) continue;
		if (it->m_type != eSea && it->m_type != eOcean) continue;
		if (it->m_entityOnTile) continue;
		//Determine distance
		std::pair<int, int> tempPos = MouseSelection::coordToHexPos(it->m_tileCoordinate);
		std::pair<int, int> diff(
			{ tempPos.first - alliedPos.first, tempPos.second - alliedPos.second });
		int tempDistance = diff.first * diff.first + diff.second * diff.second;
		//If distance is smallest, set as new target tile
		if (tempDistance < closestDistance)
		{
			closestDistance = tempDistance;
			closestTile = it;
		}
	}
	return closestTile;
}
//Support function for AI::findFiringPositions. 
//Finds the closest tile for line firing by projecting lines of length "range"
const Tile* firePosLine(
	Map* mapPtr, const Tile* targetShip, const Tile* alliedShip, int range)
{
	const Tile* closestTile{ alliedShip };
	int closestDistance{ MAX_INT };
	std::pair<int, int> alliedPos{ MouseSelection::coordToHexPos(alliedShip->m_tileCoordinate) };
	//TODO: can't use const Tile* for some reason
	std::vector<Tile*> availableTiles;
	availableTiles.reserve(range);
	//Iterate through all 6 possible lines
	for (int i = 0; i < 6; i++)
	{
		availableTiles = mapPtr->getTileLine(targetShip->m_tileCoordinate, range, static_cast<eDirection>(i));
		for (Tile* it : availableTiles)
		{
			//Ensure it's a valid tile, if not skip this one
			if (!it) continue;
			if (it->m_type != eSea && it->m_type != eOcean) continue;
			if (it->m_entityOnTile) continue;
			//Determine distance
			std::pair<int, int> tempPos = MouseSelection::coordToHexPos(it->m_tileCoordinate);
			std::pair<int, int> diff(
				{ tempPos.first - alliedPos.first, tempPos.second - alliedPos.second });
			int tempDistance = diff.first * diff.first + diff.second * diff.second;
			//If distance is smallest, set as new target tile
			if (tempDistance < closestDistance)
			{
				closestDistance = tempDistance;
				closestTile = it;
			}
		}
	}
	return closestTile;
}

std::pair<const Tile*, eDirection> findFiringPosition(
	Map* mapPtr, const Tile* targetShip, const Tile* alliedShip, eWeaponType weapon, int range)
{
	const Tile* closestTile{ alliedShip };
	int facingDirection{ 0 };
	switch (weapon)
	{
	case eSideCannons:
	{
		closestTile = firePosRadial(mapPtr, targetShip, alliedShip, range);
		facingDirection = static_cast<int>(MouseSelection::calculateDirection(alliedShip, targetShip).second);
		facingDirection++;
		if (facingDirection > 5) 
			facingDirection -= 6;
	}
	case eStraightShot:
	{
		closestTile = firePosLine(mapPtr, targetShip, alliedShip, range);
		facingDirection = static_cast<int>(MouseSelection::calculateDirection(alliedShip, targetShip).second);
	}
	case eShotgun:
	{
		closestTile = firePosRadial(mapPtr, targetShip, alliedShip, range);
		facingDirection = static_cast<int>(MouseSelection::calculateDirection(alliedShip, targetShip).second);
	}
	case eFlamethrower:
	{
		facingDirection = static_cast<int>(MouseSelection::calculateDirection(alliedShip, targetShip).second);
		facingDirection += 3;
		if (facingDirection > 5)
			facingDirection -= 6;
	}
	}
	return { closestTile, static_cast<eDirection>(facingDirection) };
}

std::pair<const Tile*, eDirection> AI::findFiringPosition(Map * mapPtr, const Tile * targetShip, const Tile * alliedShip, eWeaponType weapon, int range)
{
	return std::pair<const Tile*, eDirection>();
}

void AI::attemptMove(Map* mapPtr, std::shared_ptr<BattleEntity> currentShip, std::pair<const Tile*, eDirection> targetTile)
{
	//Call generate path
	Tile* tile{ mapPtr->getTile(currentShip->m_battleProperties.getCurrentPosition()) };
	int pathLength = currentShip->m_battleProperties.generateMovementGraph(*mapPtr, *tile, *targetTile.first);
	auto pathToTile = PathFinding::getPathToTile(
		*mapPtr,
		currentShip->m_battleProperties.getCurrentPosition(),
		targetTile.first->m_tileCoordinate);
	//Loop calling moveEntity for each tile in the path end to start until one of them works
	for (pathLength; pathLength > 0; pathLength--)
	{
		const Tile* attemptedDest = mapPtr->getTile(pathToTile[pathLength].second);
		if (currentShip->m_battleProperties.moveEntity(*mapPtr, *attemptedDest, pathToTile[pathLength].first))
			break;
	}
	currentShip->m_battleProperties.setMoved();
}

void AI::attemptShot(Battle* battlePtr, Map* mapPtr, std::shared_ptr<BattleEntity> firingShip)
{
	std::vector< const Tile*> firingArea;
	switch (firingShip->m_entityProperties.m_weaponType)
	{
	case eSideCannons:
	{
		firingArea = mapPtr->cGetTileCone(firingShip->m_battleProperties.getCurrentPosition(), firingShip->m_entityProperties.m_range, firingShip->m_battleProperties.getCurrentDirection());
		for (int i = 0; i < firingArea.size(); i++)
		{
			if (!firingArea[i]->m_entityOnTile) continue;
			if (firingArea[i]->m_entityOnTile->m_factionName == firingShip->m_factionName || firingArea[i]->m_entityOnTile->m_battleProperties.isDead()) continue;
			battlePtr->fireEntityWeaponAtPosition(*firingShip, *firingArea[i], firingArea);
		}
	}
	case eStraightShot:
	{
		firingArea = mapPtr->cGetTileLine(firingShip->m_battleProperties.getCurrentPosition(), firingShip->m_entityProperties.m_range, firingShip->m_battleProperties.getCurrentDirection());
		for (int i = 0; i < firingArea.size(); i++)
		{
			if (!firingArea[i]->m_entityOnTile) continue;
			if (firingArea[i]->m_entityOnTile->m_factionName == firingShip->m_factionName || firingArea[i]->m_entityOnTile->m_battleProperties.isDead()) continue;
			battlePtr->fireEntityWeaponAtPosition(*firingShip, *firingArea[i], firingArea);
		}
	}
	case eShotgun:
	{
		firingArea = mapPtr->cGetTileRadius(firingShip->m_battleProperties.getCurrentPosition(), firingShip->m_entityProperties.m_range);
		for (int i = 0; i < firingArea.size(); i++)
		{
			if (!firingArea[i]->m_entityOnTile) continue;
			if (firingArea[i]->m_entityOnTile->m_factionName == firingShip->m_factionName || firingArea[i]->m_entityOnTile->m_battleProperties.isDead()) continue;
			battlePtr->fireEntityWeaponAtPosition(*firingShip, *firingArea[i], firingArea);
		}
	}
	case eFlamethrower:
	{
		eDirection backwardsDirection{ eNorth };
		switch(firingShip->m_battleProperties.getCurrentDirection())
		{
		case eNorth: backwardsDirection = eSouth;
			break;
		case eNorthEast: backwardsDirection = eSouthWest;
			break;
		case eSouthEast: backwardsDirection = eNorthWest;
			break;
		case eSouth: backwardsDirection = eNorth;
			break;
		case eSouthWest: backwardsDirection = eNorthEast;
			break;
		case eNorthWest: backwardsDirection = eSouthEast;
			break;
		}
			
		firingArea = mapPtr->cGetTileLine(firingShip->m_battleProperties.getCurrentPosition(), firingShip->m_entityProperties.m_range, backwardsDirection);
		for (int i = 0; i < firingArea.size(); i++)
		{
			if (!firingArea[i]->m_entityOnTile) continue;
			if (firingArea[i]->m_entityOnTile->m_factionName == firingShip->m_factionName || firingArea[i]->m_entityOnTile->m_battleProperties.isDead()) continue;
			battlePtr->fireEntityWeaponAtPosition(*firingShip, *firingArea[i], firingArea);
		}
		
	}
	}
	firingShip->m_battleProperties.fireWeapon();
}

void AI::handleMovementPhase(Battle* battlePtr, Map* mapPtr, FactionName faction)
{
	auto ships = *battlePtr->getFactionShips(faction);

	//loop through all the ships in the faction
	for (int i = 0; i < ships.size(); i++)
	{
		if (ships[i]->m_battleProperties.isDead()) continue;
		//find the nearest enemy ship
		const Tile* enemyPosition{ findClosestEnemy(battlePtr, mapPtr, ships[i], faction) };

		//find the nearest tile and facing that can fire upon the chosen enemy ship
		std::pair<const Tile*, eDirection>  firingPosition{ AI::findFiringPosition(
			mapPtr, 
			enemyPosition, 
			mapPtr->getTile(ships[i]->m_battleProperties.getCurrentPosition()), 
			ships[i]->m_entityProperties.m_weaponType, 
			ships[i]->m_entityProperties.m_range) };

		//move as far as possible on the path to the chosen position
		attemptMove(mapPtr, ships[i], firingPosition);
	}
}

void AI::handleShootingPhase(Battle* battlePtr, Map* mapPtr, FactionName faction)
{
	auto ships = *battlePtr->getFactionShips(faction);
	//loop through all the ships in the faction
	for (int i = 0; i < ships.size(); i++)
	{
		if (ships[i]->m_battleProperties.isDead()) continue;
		//check if the ship is able to fire upon any enemies and fire if possible
		attemptShot(battlePtr, mapPtr, ships[i]);
	}
}

void AI::handleDeploymentPhase(Battle * battlePtr, Map * mapPtr, FactionName faction)
{
}
