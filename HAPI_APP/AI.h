#pragma once

#include "Global.h"
#include "Battle.h"
#include "Map.h"
#include "entity.h"

class AI
{
private:
	const Tile* findClosestEnemy(const Battle* battlePtr, const Map* mapPtr, const BattleEntity* alliedShip, FactionName faction) const;
	std::pair<Tile*,eDirection> findFiringPosition(const Tile* targetShip, eWeaponType weapon, int range) const;
	void attemptShot(const Tile* firingShip, const Tile* targetShip);
	void attemptMove(Tile* currentTile, std::pair<Tile*, eDirection> targetTile);

public:
	void handleMovementPhase(Battle* battlePtr, Map* mapPtr, FactionName faction);
	void handleShootingPhase(Battle* battlePtr, Map* mapPtr, FactionName faction);
	void handleDeploymentPhase(Battle* battlePtr, Map* mapPtr, FactionName faction);

	AI();
	~AI();
};

