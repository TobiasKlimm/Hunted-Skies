#pragma once
#pragma once

#define TERRAIN_HEIGHT 500
#define TERRAIN_SIZE 2500
#define WATER_SIZE 50000
#define TERRAIN_BEACHLIMIT 12
#define TERRAIN_SANDLIMIT 4
#define TERRAIN_SNOWLIMIT 130
#define TERRAIN_SNOWLIMITHEAVY 150
#define TERRAIN_UV 50
#define PERLIN_SEED 42

#include "Vektoria\Root.h"

using namespace Vektoria;

class CTerrain : public CPlacement
{
public:
	void Init();
	CGeoTerrain* GetTerrainWater()
	{
		return &m_zgWater;
	}
	CGeoTerrain* GetTerrainGeo()
	{
		return &m_zgTerrain;
	}

private:
	CGeoTerrain m_zgTerrainOri; // entire terrain
	CGeoTerrain m_zgTerrain; // island section
	CGeoTerrain m_zgWater;// water section

	// MATERIALS
	CMaterial m_zmRock;
	CMaterial m_zmSnow;
	CMaterial m_zmSand;
	CMaterial m_zmSnowHeavy;
	CMaterial m_zmSandMossy;
	CMaterial m_zmMirror;
	CMaterial m_zmWater; // wave material
	CMaterial m_zmEarth; // earth material

	CPlacement m_zpLandscape; // island and waterplacement
	CPerlin* m_pperlin = nullptr; // perlin Noise

	CPlacement m_zpMainIsland;
	CPlacement m_zpIsland1;
	CPlacement m_zpIsland2;
	CPlacement m_zpIsland3;

	CBlob* m_pblobAllPositive = nullptr;
	CBlob* m_pblobAllNegative = nullptr;
	CBlob* m_pblobTerrainPositive = nullptr;
	CBlob* m_pblobTerrainNegative = nullptr;
	CBlob* m_pblobTerrainCraterPositive = nullptr;

	CBlob* m_pblobAll = nullptr;

	// RISING FROM THE SEA
	CGeos m_zgsCollision, m_zgsHeight; // collision scontainer
	CGeoTerrains m_zgsTerrain; // terrain collision container

	// MATERIAL CUTS
	CCut m_cutOverSea;
	CCut m_cutSeaToSand;
	CCut m_cutSandtoBeach;
	CCut m_cutSeaToBeach;
	CCut m_cutUnderSea; // Cut under normal zero
	CCut m_cutBeachToSnowLimit;
	CCut m_cutSnowLimitToHeavySnowLimit;
	CCut m_cutOverHeavySnowLimit;
	CCut m_cutOverSnowLimit;

	CCut m_cutUnder15Degrees;
	CCut m_cut15DegreesTo30Degrees;
	CCut m_cut30DegreesTo45Degrees;
	CCut m_cut15DegreesTo45Degrees;
	CCut m_cutOver45Degrees;
	CCut m_cutUnder45Degrees;

	CGeoTerrain m_zgTerrainFlora;
	CGeoTerrain m_zgTerrainLow;
	CGeoTerrain m_zgTerrainMirror;
	CGeoTerrain m_zgTerrainRock;
	CGeoTerrain m_zgTerrainSnow;
	CGeoTerrain m_zgTerrainSand;
	CGeoTerrain m_zgTerrainSandMossy;
	CGeoTerrain m_zgTerrainSnowHeavy;
};