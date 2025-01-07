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

class CTerrain :
	public CPlacement
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
	CGeoTerrain m_zgTerrainOri; // Gesamtes Terrain
	CGeoTerrain m_zgTerrain; // Inselausschnitt
	CGeoTerrain m_zgWater;// Wasserausschnitt

	CMaterial m_zmRock;
	CMaterial m_zmSnow;
	CMaterial m_zmSand;
	CMaterial m_zmSnowHeavy;
	CMaterial m_zmSandMossy;
	CMaterial m_zmMirror;
	CMaterial m_zmWater; // Wellenmaterial
	CMaterial m_zmEarth; // Erdmaterial

	CPlacement m_zpLandscape; // Insel- & Wasserplacement
	CPerlin* m_pperlin = nullptr;// Perlin Noise
	CPerlin* m_pperlin1 = nullptr;

	CPlacement m_zpHauptInsel;
	CPlacement m_zpIsland1;
	CPlacement m_zpIsland2;
	CPlacement m_zpIsland3;
	CBlob* m_pblobAllPositive = nullptr;
	CBlob* m_pblobAllNegative = nullptr;
	CBlob* m_pblobTerrainPositive = nullptr;
	CBlob* m_pblobTerrainNegative = nullptr;
	CBlob* m_pblobTerrainCraterPositive = nullptr;


	CBlob* m_pblob9 = nullptr;
	CBlob* m_pblob8 = nullptr;
	CBlob* m_pblob6 = nullptr;
	CBlob* m_pblob7 = nullptr;
	CBlob* m_pblob5 = nullptr;
	CBlob* m_pblob3 = nullptr;
	CBlob* m_pblob4 = nullptr;
	CBlob* m_pblob2 = nullptr;
	CBlob* m_pblob1 = nullptr;
	CBlob* m_pblob = nullptr;
	CBlob* m_pblobAll = nullptr;


	// Erhebung aus dem Meer
	CGeos m_zgsCollision, m_zgsHeight; // Kollisionscontainer
	CGeoTerrains m_zgsTerrain; // Terrainkollisionscontainer




	CCut m_cutOverSea;
	CCut m_cutSeaToSand;
	CCut m_cutSandtoBeach;
	CCut m_cutSeaToBeach;
	CCut m_cutUnderSea; // Schnitt unter N.N.
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