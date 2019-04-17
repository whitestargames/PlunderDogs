#pragma once

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
	ePreBattle,
	eLevelSelection,
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
	eDeadYellow = 0,
	eDeadBlue,
	eDeadGreen,
	eDeadRed,
	eDeadBlack,
	eDeadWhite,
	eLowHealthYellow,
	eLowHealthBlue,
	eLowHealthGreen,
	eLowHealthRed,
	eLowHealthBlack,
	eLowHealthWhite,
	eMediumHealthYellow,
	eMediumHealthBlue,
	eMediumHealthGreen,
	eMediumHealthRed,
	eMediumHealthBlack,
	eMediumHealthWhite,
	eMaxHealthYellow,
	eMaxHealthBlue,
	eMaxHealthGreen,
	eMaxHealthRed,
	eMaxHealthBlack,
	eMaxHealthWhite


};

enum eWeaponType
{
	eSideCannons = 0,
	eStraightShot,
	eStraightShotExplosive
};