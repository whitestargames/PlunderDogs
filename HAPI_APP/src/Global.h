#pragma once

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

enum OverWorldWindow
{
	eBattle = 0,
	eShipSelection,
	eLevelSelection,
	ePlayerSelection, 
	eUpgrade

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
	eShotGun, 
	eFlamethrower
};


enum eTimeOfDay
{
	eMorning = 0,
	eAfternoon,
	eEvening,
	eNight
};