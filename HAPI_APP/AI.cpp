#include "AI.h"

constexpr int MAX_INT{ 2147483647 };

const Tile* AI::findClosestEnemy(
	const Battle* battlePtr, const Map* mapPtr, const std::shared_ptr<BattleEntity> alliedShip, FactionName faction)
{
	const Tile* closestEnemy{ nullptr };
	int closestDistance{ MAX_INT };
	std::pair<int, int> alliedPos{ 
		mapPtr->getTileScreenPos(alliedShip->m_battleProperties.getCurrentPosition()) };
	for (int i = 0; i < 4; i++)
	{
		if (i == static_cast<int>(faction))
			continue;
		const auto factionShipList = *battlePtr->getFactionShips(static_cast<FactionName>(i));
		for (int j = 0; j < factionShipList.size(); j++)
		{
			if (factionShipList[j]->m_battleProperties.isDead)
				continue;
			//Find the distance^2 from the allied ship to the enemy ship, 
			//then set closestEnemy to that enemy if it's the closest yet found
			std::pair<int, int> enemyPos = mapPtr->getTileScreenPos(
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

void AI::handleMovementPhase(Battle* battlePtr, Map* mapPtr, FactionName faction)
{
	auto ships = *battlePtr->getFactionShips(faction);

	//loop through all the ships in the faction
	for (int i = 0; i < ships.size(); i++)
	{
		//find the nearest enemy ship
		const Tile* enemyPosition{ findClosestEnemy(battlePtr, mapPtr, ships[i], faction) };

		//find the nearest tile and facing that can fire upon the chosen enemy ship
		std::pair<Tile*, eDirection>  firingPosition{ findFiringPosition(enemyPosition,ships[i]->m_entityProperties.m_weaponType, ships[i]->m_entityProperties.m_range) };
		//move as far as possible on the path to the chosen position
		attemptMove(mapPtr->getTile(ships[i]->m_battleProperties.getCurrentPosition()), firingPosition);
	}
}

void AI::handleShootingPhase(Battle* battlePtr, Map* mapPtr, FactionName faction)
{  
	std::vector<BattleEntity> ships{ battlePtr->getFactionShips(faction) };
	//loop through all the ships in the faction
	for (auto it = ships.begin(); it != ships.end(); it++)
	{
		//check if the ship is able to fire upon any enemies and fire if possible
		attemptShot(mapPtr->getTile(it->m_battleProperties.getCurrentPosition()));
	}
}
