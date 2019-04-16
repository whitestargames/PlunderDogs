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
	eLevelSelection
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

enum eSpriteFrame
{
	eDeadYellow = 0,
	eDeadBlue,
	eDeadGreen,
	eDeadRed,
	eDeadBlack,
	eDeadWhite,
	eHighDamageYellow,
	eHighDamageBlue,
	eHighDamageGreen,
	eHighDamageRed,
	eHighDamageBlack,
	eHighDamageWhite,
	eLowDamageYellow,
	eLowDamageBlue,
	eLowDamageGreen,
	eLowDamageRed,
	eLowDamageBlack,
	eLowDamageWhite,
	eNoDamageYellow,
	eNoDamageBlue,
	eNoDamageGreen,
	eNoDamageRed,
	eNoDamageBlack,
	eNoDamageWhite


};

enum weaponType
{
	eSideCannons = 0,
	eStraightShot,
	eStraightShotExplosive
};