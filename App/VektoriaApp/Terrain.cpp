#include "Terrain.h"

void CTerrain::Init()
{
	this->AddPlacement(&m_zpLandscape);
	m_zpLandscape.AddGeo(&m_zgWater);
	m_zpLandscape.AddGeo(&m_zgTerrain);

	// Islands erzeugen
	m_zpLandscape.AddPlacement(&m_zpIsland1);
	m_zpLandscape.AddPlacement(&m_zpIsland2);
	m_zpLandscape.AddPlacement(&m_zpIsland3);



	m_zpIsland1.Translate(6000, 0, -4000);


	m_zpIsland2.RotateY(THIRDPI);
	m_zpIsland2.TranslateDelta(7500, 0, 2500);


	m_zpIsland3.RotateY(-HALFPI);
	m_zpIsland3.TranslateDelta(-7890, 0, -6300);


	m_pperlin = new CPerlin(PERLIN_SEED, 3.0f, 15, 0.5f, 8.0f, 1.50f, 1.5f, ePerlinInterpol_Standard, false);
	m_pblobAllPositive = new CBlob(0.5f, 0.5f, 0.6f, 0.6f, -0.1f, eBlobShapeGround_Rect, eBlobShapeSide_All, NULL);
	m_pblobAllNegative = new CBlob(0.5f, 0.5f, 0.6f, 0.6f, +0.1f, eBlobShapeGround_Rect, eBlobShapeSide_All, NULL);
	m_pblobTerrainPositive = new CBlob(0.5f, 0.5f, 0.5f, 0.5f, TERRAIN_HEIGHT, eBlobShapeGround_Radial, eBlobShapeSide_Hill, m_pperlin);
	m_pblobTerrainNegative = new CBlob(0.5f, 0.5f, 0.5f, 0.5f, -TERRAIN_HEIGHT, eBlobShapeGround_Radial, eBlobShapeSide_Hill, m_pperlin);
	m_pblobTerrainCraterPositive = new CBlob(0.2f, 0.2f, 0.2f, 0.2f, TERRAIN_HEIGHT, eBlobShapeGround_Donut, eBlobShapeSide_Hill, m_pperlin);
	m_pblobAll = new CBlob(0.5f, 0.5f, 0.51f, 0.51f, -0.2f, eBlobShapeGround_Rect, eBlobShapeSide_All, nullptr);


	m_zgTerrainOri.AddBlob(m_pblobAllPositive);
	m_zgTerrainOri.AddBlob(m_pblobTerrainPositive);
	m_zgTerrainOri.AddBlob(m_pblobTerrainCraterPositive);
	m_zgTerrainOri.AddBlob(m_pblobAll);

	m_zgTerrain.AddBlob(m_pblobAllPositive);
	m_zgTerrain.AddBlob(m_pblobTerrainPositive);
	m_zgTerrain.AddBlob(m_pblobTerrainCraterPositive);
	m_zgTerrain.AddBlob(m_pblobAll);


	m_zgTerrainOri.CreateField(TERRAIN_SIZE, TERRAIN_SIZE, 1000, 1000, 0.0f, 0.0f, 10.0f, 10.0f);
	m_zgTerrain.Init(TERRAIN_SIZE, TERRAIN_SIZE, NULL, 100, 100, 0.0f, 0.0f, 10.0f, 10.0f);
	m_zgTerrain.SetDrawingOff();




	//Terrain mit verschiedenen Materialien erstellen
	//Cuts

	m_cutOverSea.SetHeightLimits(-F_MAX, 0.0f);
	m_cutOverSea.SetFlattenLowerOn();
	m_cutOverSea.SetFlattenSmartOn();



	m_cutUnderSea.SetHeightLimits(0.0f, F_MAX);
	m_cutUnderSea.SetFlattenUpperOn();
	m_cutUnderSea.SetFlattenSmartOn();


	m_cutSeaToSand.SetHeightLimits(0.0f, TERRAIN_SANDLIMIT);
	m_cutSeaToSand.SetHeightInverseOn();
	m_cutSeaToSand.SetFlattenLowerOn();
	m_cutSeaToSand.SetFlattenSmartOn();


	m_cutSandtoBeach.SetHeightLimits(TERRAIN_SANDLIMIT, TERRAIN_BEACHLIMIT);
	m_cutSandtoBeach.SetHeightInverseOn();

	m_cutSeaToBeach.SetHeightLimits(0.0f, TERRAIN_BEACHLIMIT);
	m_cutSeaToBeach.SetHeightInverseOn();
	m_cutSeaToBeach.SetFlattenLowerOn();
	m_cutSeaToBeach.SetFlattenSmartOn();


	m_cutBeachToSnowLimit.SetHeightLimits(TERRAIN_BEACHLIMIT, TERRAIN_SNOWLIMIT);
	m_cutBeachToSnowLimit.SetHeightInverseOn();

	m_cutSnowLimitToHeavySnowLimit.SetHeightLimits(TERRAIN_SNOWLIMIT, TERRAIN_SNOWLIMITHEAVY);
	m_cutSnowLimitToHeavySnowLimit.SetHeightInverseOn();

	m_cutOverHeavySnowLimit.SetHeightLimits(TERRAIN_SNOWLIMITHEAVY, F_MAX);
	m_cutOverHeavySnowLimit.SetHeightInverseOn();

	m_cutOverSnowLimit.SetHeightLimits(TERRAIN_SNOWLIMIT, F_MAX);
	m_cutOverSnowLimit.SetHeightInverseOn();


	m_cutUnder15Degrees.SetSlopeLimits(0.0f, PI / 12.0f);
	m_cutUnder15Degrees.SetSlopeInverseOn();

	m_cutUnder45Degrees.SetSlopeLimits(0.0f, QUARTERPI);
	m_cutUnder45Degrees.SetSlopeInverseOn();

	m_cut15DegreesTo30Degrees.SetSlopeLimits(PI / 12.0f, PI / 6.0f);
	m_cut15DegreesTo30Degrees.SetSlopeInverseOn();

	m_cut30DegreesTo45Degrees.SetSlopeLimits(PI / 6.0f, QUARTERPI);
	m_cut30DegreesTo45Degrees.SetSlopeInverseOn();

	m_cut15DegreesTo45Degrees.SetSlopeLimits(PI / 12.0f, QUARTERPI);
	m_cut15DegreesTo45Degrees.SetSlopeInverseOn();

	m_cutOver45Degrees.SetSlopeLimits(QUARTERPI, TWOPI);
	m_cutOver45Degrees.SetSlopeInverseOn();


	//Cuts dem Terrain Hinzufügen
	m_zgTerrainSand.AddCut(&m_cutSeaToSand);
	m_zgTerrainSand.AddCut(&m_cutUnder15Degrees);

	m_zgTerrainSandMossy.AddCut(&m_cutSandtoBeach);
	m_zgTerrainSandMossy.AddCut(&m_cutUnder15Degrees);

	m_zgTerrainFlora.AddCut(&m_cutBeachToSnowLimit);
	m_zgTerrainFlora.AddCut(&m_cutUnder45Degrees);

	m_zgTerrainLow.AddCut(&m_cutSeaToBeach);
	m_zgTerrainLow.AddCut(&m_cut15DegreesTo45Degrees);

	m_zgTerrainSnow.AddCut(&m_cutSnowLimitToHeavySnowLimit);
	m_zgTerrainSnow.AddCut(&m_cutUnder45Degrees);


	m_zgTerrainSnowHeavy.AddCut(&m_cutOverSnowLimit);
	m_zgTerrainSnowHeavy.AddCut(&m_cutUnder45Degrees);

	m_zgTerrainRock.AddCut(&m_cutOverSea);
	m_zgTerrainRock.AddCut(&m_cutOver45Degrees);

	m_zgTerrainMirror.AddCut(&m_cutUnderSea);




	m_zpLandscape.AddGeo(&m_zgTerrainFlora);
	m_zpLandscape.AddGeo(&m_zgTerrainLow);
	m_zpLandscape.AddGeo(&m_zgTerrainRock);
	m_zpLandscape.AddGeo(&m_zgTerrainMirror);
	m_zpLandscape.AddGeo(&m_zgTerrainSnowHeavy);
	m_zpLandscape.AddGeo(&m_zgTerrainSnow);
	m_zpLandscape.AddGeo(&m_zgTerrainOri);
	m_zpLandscape.AddGeo(&m_zgTerrainSand);
	m_zpLandscape.AddGeo(&m_zgTerrainSandMossy);


	m_zpIsland1.AddGeo(&m_zgTerrainFlora);
	m_zpIsland1.AddGeo(&m_zgTerrainLow);
	m_zpIsland1.AddGeo(&m_zgTerrainRock);
	m_zpIsland1.AddGeo(&m_zgTerrainMirror);
	m_zpIsland1.AddGeo(&m_zgTerrainSnowHeavy);
	m_zpIsland1.AddGeo(&m_zgTerrainSnow);
	m_zpIsland1.AddGeo(&m_zgTerrainOri);
	m_zpIsland1.AddGeo(&m_zgTerrainSand);
	m_zpIsland1.AddGeo(&m_zgTerrainSandMossy);

	m_zpIsland2.AddGeo(&m_zgTerrainFlora);
	m_zpIsland2.AddGeo(&m_zgTerrainLow);
	m_zpIsland2.AddGeo(&m_zgTerrainRock);
	m_zpIsland2.AddGeo(&m_zgTerrainMirror);
	m_zpIsland2.AddGeo(&m_zgTerrainSnowHeavy);
	m_zpIsland2.AddGeo(&m_zgTerrainSnow);
	m_zpIsland2.AddGeo(&m_zgTerrainOri);
	m_zpIsland2.AddGeo(&m_zgTerrainSand);
	m_zpIsland2.AddGeo(&m_zgTerrainSandMossy);


	m_zpIsland3.AddGeo(&m_zgTerrainFlora);
	m_zpIsland3.AddGeo(&m_zgTerrainLow);
	m_zpIsland3.AddGeo(&m_zgTerrainRock);
	m_zpIsland3.AddGeo(&m_zgTerrainMirror);
	m_zpIsland3.AddGeo(&m_zgTerrainSnowHeavy);
	m_zpIsland3.AddGeo(&m_zgTerrainSnow);
	m_zpIsland3.AddGeo(&m_zgTerrainOri);
	m_zpIsland3.AddGeo(&m_zgTerrainSand);
	m_zpIsland3.AddGeo(&m_zgTerrainSandMossy);





	m_zgTerrainMirror.InitFromOther(m_zgTerrainOri, &m_zmEarth, true);
	m_zgTerrainFlora.InitFromOther(m_zgTerrainOri, &m_zmEarth, false);
	m_zgTerrainLow.InitFromOther(m_zgTerrainOri, &m_zmEarth, false);
	m_zgTerrainRock.InitFromOther(m_zgTerrainOri, &m_zmRock, false);
	m_zgTerrainSnowHeavy.InitFromOther(m_zgTerrainOri, &m_zmSnowHeavy, false);
	m_zgTerrainSnow.InitFromOther(m_zgTerrainOri, &m_zmSnow, false);
	m_zgTerrainSand.InitFromOther(m_zgTerrainOri, &m_zmSand, false);
	m_zgTerrainSandMossy.InitFromOther(m_zgTerrainOri, &m_zmSandMossy, false);

	// Lade die Texturen für Wasser und Land:
	m_zmRock.LoadPreset("Rock");
	m_zmRock.SetHeightStrength(0.0f);
	m_zmSand.LoadPreset("Sand");
	m_zmEarth.LoadPreset("RockMossy");
	m_zmMirror.LoadPreset("RockMossy");
	m_zmMirror.SetShadingOff();
	m_zmSnow.LoadPreset("RockSnowy");
	m_zmSnowHeavy.LoadPreset("RockVerySnowy");
	m_zmSandMossy.LoadPreset("SandMossy");

	// Wasser
	m_zmWater.LoadPreset("Water"); //Laden des Wassers
    m_zmWater.SetAni(8, 8, 7);	//Animierte Wassertextur hat 8x8 Bilde
	m_zmWater.Translate(CColor(0.8f, 0.0f, 0.8f)); //Blauwert vom Wasser erhöhen
	m_zmWater.SetTransparency(0.2f);//Durchsichtigkeit des Wassers
	m_zmWater.SetTransparencyOn();
	m_zmWater.MakeTextureHaze("textures//WaterHaze.jpg");
	m_zmWater.SetPostprocessingOn();
	//m_zmWater.SetHazeOn();
	m_zmWater.SetHazeStrength(1.8);

	// Erzeuge mit dem Terraindaten eine Terraingeometrie: 
	//m_zgTerrain.InitFromOther(m_zgTerrainOri, &m_zmEarth);

	// Erzeuge die Wasseroberfläche:
	m_zgWater.Init(WATER_SIZE, WATER_SIZE, &m_zmWater, 10, 10, 0.0f, 0.0f, 500.0f, 500.0f);

	// Füge das Terrain dem Kollisionscontainer hinzu:
	m_zgsTerrain.Add(&m_zgTerrain);




	
	
}
