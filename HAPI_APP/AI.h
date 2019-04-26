#pragma once

#include "Global.h"
#include "Battle.h"
#include "Map.h"
#include "entity.h"

class AI
{
private:
	Tile* findClosestEnemy(const Tile* alliedShip) const;
	Tile* getFiringPosition(const Tile* targetShip, eWeaponType weapon, int range) const;
	void attemptShot(const Tile* firingShip, const Tile* targetShip);
	void attemptMove(Tile* currentTile, Tile* targetTile);

public:
	void handleMovementPhase(FactionName faction);
	void handleShootingPhase(FactionName faction);

	AI();
	~AI();
};

