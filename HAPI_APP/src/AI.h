#pragma once

#include <memory>
#include "Global.h"

class Battle;
struct Tile;
class Map;
struct BattleEntity;
struct BattlePlayer;
struct EntityProperties;

namespace AI
{
//Internal use
	//Finds closest living enemy, returns nullptr if none found
	const Tile* findClosestEnemy(const Battle& battle, const Map& map, std::pair<int, int> alliedShipPosition, FactionName faction);
	//Finds nearest firing position to ship, if none are found will return the tile the ship is on
	std::pair<const Tile*,eDirection> findFiringPosition(const Map& mapPtr, const Tile* targetShip, const Tile* alliedShip, eWeaponType weapon, int range);
	void attemptMove(Map& map, std::shared_ptr<BattleEntity> currentShip, std::pair<const Tile*, eDirection> targetTile);
	void attemptShot(Battle& battle, const Map& mapPtr, std::shared_ptr<BattleEntity> firingShip);
//External use
	void handleMovementPhase(const Battle& battle, Map& map, BattlePlayer& player);
	void handleShootingPhase(Battle& battle, const Map& map, BattlePlayer& player);
	void handleDeploymentPhase(Battle& battle, Map& map, BattlePlayer& player);
	void handleShipSelection(std::vector<EntityProperties>& shipPool, std::vector<EntityProperties*>& selectedShips);
};