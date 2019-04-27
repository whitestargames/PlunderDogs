#include "AI.h"

constexpr int MAX_INT{ 2147483647 };

Tile* AI::findClosestEnemy(
	const Battle* battlePtr, const Map* mapPtr, const BattleEntity* alliedShip, FactionName faction) const
{
	Tile* closestEnemy{ nullptr };
	int closestDistance{ MAX_INT };
	for (int i = 0; i < 4; i++)
	{
		if (i == static_cast<int>(faction))
			continue;
		auto factionShipList = battlePtr->getFactionShips(static_cast<FactionName>(i));
		for (auto it : *factionShipList)
		{
			std::pair<int, int> enemyPos = it.m_battleProperties.getCurrentPosition();
			if ()
		}
	}
	return closestEnemy;
}

void AI::handleMovementPhase(Battle* battlePtr, Map* mapPtr, FactionName faction)
{
	//Container of ships - Battle::getCurentFaction()
	std::vector<BattleEntity> ships{ battlePtr->getFactionShips(faction) };
	//loop through all the ships in the faction
	for (auto it = ships.begin(); it != ships.end(); it++)
	{
		//find the nearest enemy ship
		Tile* enemyPosition{ findClosestEnemy(battlePtr, mapPtr, mapPtr->getTile(it->m_battleProperties.getCurrentPosition())) };
		//find the nearest tile and facing that can fire upon the chosen enemy ship
		std::pair<Tile*, eDirection>  firingPosition{ findFiringPosition(enemyPosition,it->m_entityProperties.m_weaponType, it->m_entityProperties.m_range) };
		//move as far as possible on the path to the chosen position
		attemptMove(mapPtr->getTile(it->m_battleProperties.getCurrentPosition), firingPosition);
	}
}

void AI::handleShootingPhase(Battle* battlePtr, Map* mapPtr, FactionName faction)
{  
	//loop through all the ships in the faction
	for (auto it)
	{
		//check if the ship is able to fire upon any enemies and fire if possible
	}
}

AI::AI()
{
}


AI::~AI()
{
}
