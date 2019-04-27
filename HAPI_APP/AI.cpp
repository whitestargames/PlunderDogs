#include "AI.h"



void AI::handleMovementPhase(Battle* battlePtr, Map* mapPtr, FactionName faction)
{
	//Container of ships - Battle::getCurentFaction()
	std::vector<BattleEntity> ships{ battlePtr->getFactionShips(faction) };
	//loop through all the ships in the faction
	for (auto it = ships.begin(); it != ships.end(); it++)
	{
		//find the nearest enemy ship
		Tile* enemyPosition{ findClosestEnemy(mapPtr->getTile(it->m_battleProperties.getCurrentPosition())) };
		//find the nearest tile and facing that can fire upon the chosen enemy ship
		std::pair<Tile*, eDirection>  firingPosition{ findFiringPosition(enemyPosition,it->m_entityProperties.m_weaponType, it->m_entityProperties.m_range) };
		//move as far as possible on the path to the chosen position
		attemptMove(mapPtr->getTile(it->m_battleProperties.getCurrentPosition), firingPosition);
	}
}

void AI::handleShootingPhase(FactionName faction)
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
