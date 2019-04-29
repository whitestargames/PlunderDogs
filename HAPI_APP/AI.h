#pragma once

#include "Global.h"
#include "Battle.h"
#include "Map.h"
#include "entity.h"

namespace AI
{
//Internal use
	//Finds closest living enemy, returns nullptr if none found
	const Tile* findClosestEnemy(const Battle* battlePtr, const Map* mapPtr, const std::shared_ptr<BattleEntity> alliedShip, FactionName faction);
	//Finds nearest firing position to ship, if none are found will return the tile the ship is on
	std::pair<const Tile*,eDirection> findFiringPosition(Map* mapPtr, const Tile* targetShip, const Tile* alliedShip, eWeaponType weapon, int range);
	void attemptShot(Battle* battlePtr, Map* mapPtr, std::shared_ptr<BattleEntity> firingShip);
	void attemptMove(std::shared_ptr<BattleEntity> currentShip, std::pair<const Tile*, eDirection> targetTile);
//External use
	void handleMovementPhase(Battle* battlePtr, Map* mapPtr, FactionName faction);
	void handleShootingPhase(Battle* battlePtr, Map* mapPtr, FactionName faction);
	void handleDeploymentPhase(Battle* battlePtr, Map* mapPtr, FactionName faction);
};

