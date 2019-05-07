#pragma once
#include <utility>

enum FactionName
{
	eYellow = 0,
	eBlue,
	eGreen,
	eRed
};

enum eDirection
{
	eNorth,
	eNorthEast,
	eSouthEast,
	eSouth,
	eSouthWest,
	eNorthWest, 
	Max = eNorthWest
};

struct posi
{
	int x;
	int y;
	eDirection dir;

	posi(int numX = 0, int numY = 0, eDirection direction = eNorth) : x(numX), y(numY), dir(direction) {}
	posi(std::pair<int, int> pair, eDirection direction = eNorth) : x(pair.first), y(pair.second), dir(direction) {}

	std::pair<int, int> pair() { return { x, y }; }

	inline bool operator==(const posi& rhs) const 
	{
		bool ans{ false };
		posi lhs = *this;
		if (lhs.x == rhs.x && lhs.y == rhs.y && lhs.dir == rhs.dir)
			ans = true;
		return ans;
	}
	inline bool operator!=(const posi& rhs) const { return !(*this == rhs); }
};

enum OverWorldWindow
{
	eBattle = 0,
	eMainMenu,
	eShipSelection,
	eLevelSelection,
	ePlayerSelection, 
	eUpgrade
};

enum ePlayerType
{
	eNone = 0,
	eHuman,
	eAI
};

enum eTileType
{
	eGrass = 0,
	eSparseForest,
	eForest,
	eFoothills,
	eWoodedFoothills,
	eMountain,
	eSea,
	eOcean,
	eGrasslandTown,
	eWalledGrasslandTown,
	eStoneGrasslandTown,
	eFarm,
	eWoodedSwamp,
	eSwampPools,
	eSwamp,
	eSwampWater,
	eSnow,
	eSparseSnowForest,
	eSnowForest,
	eSnowFoothills,
	eSnowWoodedFoothills,
	eIceburgs,
	eSnowTown,
	eSnowCastle,
	eSand,
	eSandFoothills,
	eSandDunes,
	eMesa,
	eOasis,
	eSandTown,
	eWalledSandTown,
	eJungle,
	eLeftPort,
	eRightPort,
	eLighthouse,
	eGrasslandRuin,
	eSwampRuins
};


enum eShipSpriteFrame
{
	eMaxHealth = 0,
	eLowDamage,
	eHighDamage,
	eDead
};

enum eWeaponType
{
	eSideCannons = 0,
	eStraightShot,
	eShotgun, 
	eFlamethrower
};


enum eLightIntensity
{
	eMaximum = 0,
	//eHigh,
	//eLow,
	eMinimum
};