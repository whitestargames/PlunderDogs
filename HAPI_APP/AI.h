#pragma once

#include "Global.h"
#include "Battle.h"
#include "Map.h"
#include "entity.h"

namespace AI
{
//Internal use
	const Tile* findClosestEnemy(const Battle* battlePtr, const Map* mapPtr, const std::shared_ptr<BattleEntity> alliedShip, FactionName faction);
	std::pair<Tile*,eDirection> findFiringPosition(const Tile* targetShip, eWeaponType weapon, int range);
	void attemptShot(const Tile* firingShip, const Tile* targetShip);
	void attemptMove(Tile* currentTile, std::pair<Tile*, eDirection> targetTile);
//External use
	void handleMovementPhase(Battle* battlePtr, Map* mapPtr, FactionName faction);
	void handleShootingPhase(Battle* battlePtr, Map* mapPtr, FactionName faction);
	void handleDeploymentPhase(Battle* battlePtr, Map* mapPtr, FactionName faction);
};

