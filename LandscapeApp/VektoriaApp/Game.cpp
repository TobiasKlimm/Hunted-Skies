#include "Game.h"

CGame::CGame(void)
{
}

CGame::~CGame(void)
{
}

void CGame::Init(HWND hwnd, void(*procOS)(HWND hwnd, unsigned int uWndFlags), CSplash* psplash)
{



	
	// Das kennen wir ja schon:
	m_zr.Init(psplash);
	m_zf.Init(hwnd, procOS);
	m_zv.InitFull(&m_zc);
	m_zr.AddFrame(&m_zf);
	m_zf.AddViewport(&m_zv);
	m_zf.AddDeviceKeyboard(&m_zdk);
	m_zr.AddScene(&m_zs);
	m_zv.SetHazeOn();

	////LANDSCAPE


	m_pgtriangletable = m_filewavefront.LoadGeoTriangleTable("Models\\Cannon\\cannon_01_4k.obj");
	m_zs.AddPlacement(&m_zpCannon);
	m_zpCannon.AddGeo(m_pgtriangletable);

	m_zpCannon.ScaleDelta(10.0f);
	m_zpCannon.TranslateDelta(-300, 0, 700);



	m_pgtriangletable1 = m_filewavefront1.LoadGeoTriangleTable("Models\\ship_pinnace_4k.blend\\ship_pinnace_4k.obj");
	m_zs.AddPlacement(&m_zpShip);
	m_zpShip.AddGeo(m_pgtriangletable1);

	m_zpShip.ScaleDelta(10.0f);
	m_zpShip.RotateYDelta(HALFPI);
	m_zpShip.TranslateDelta(-350, 0, 1000);


	// Erzeuge einen Himmel mit Sonne, Mond und Sternen:
	m_zs.SetSkyOn(&m_zpCamera);
	m_zs.SetSkyTimeOfDay(0.0);
	m_zs.SetSkyTimeOfYear(0.75);
	m_zs.SetSkyFlowOn(1200);
	m_zs.SetSkyMistOn(true);
	m_zs.SetSkyMistStrength(0.00017f);
	m_zs.SetSkyMistStartDistance(18000.00003f);


	// Hänge die Geometrien an ein geeignetes Placement:
	m_zs.AddPlacement(&m_zpLandscape);
	m_zpLandscape.AddGeo(&m_zgTerrain);
	m_zpLandscape.AddGeo(&m_zgWater);


	//Islands erzeugen
	m_zs.AddPlacement(&m_zpIsland1);
	m_zs.AddPlacement(&m_zpIsland2);
	m_zs.AddPlacement(&m_zpIsland3);


	m_zpIsland2.RotateY(THIRDPI);
	m_zpIsland2.Translate(4000, 0, 3500);

	m_zpIsland3.RotateY(-HALFPI);
	m_zpIsland3.Translate(-5700, 0, -3500);


	m_zpIsland1.AddGeo(&m_zgWater);


	m_zpIsland1.AddPlacement(&m_zpHauptInsel);
	m_zpIsland2.AddPlacement(&m_zpHauptInsel);
	m_zpIsland3.AddPlacement(&m_zpHauptInsel);

	m_pperlin = new CPerlin(PERLIN_SEED, 3.0f, 15, 0.5f, 8.0f, 1.50f, 1.5f, ePerlinInterpol_Standard, false);
	m_pblobAllPositive = new CBlob(0.5f, 0.5f, 0.6f, 0.6f, -0.1f, eBlobShapeGround_Rect, eBlobShapeSide_All, NULL);
	m_pblobAllNegative = new CBlob(0.5f, 0.5f, 0.6f, 0.6f, +0.1f, eBlobShapeGround_Rect, eBlobShapeSide_All, NULL);
	m_pblobTerrainPositive = new CBlob(0.5f, 0.5f, 0.5f, 0.5f, TERRAIN_HEIGHT, eBlobShapeGround_Radial, eBlobShapeSide_Hill, m_pperlin);
	m_pblobTerrainNegative = new CBlob(0.5f, 0.5f, 0.5f, 0.5f, -TERRAIN_HEIGHT, eBlobShapeGround_Radial, eBlobShapeSide_Hill, m_pperlin);
	m_pblobTerrainCraterPositive = new CBlob(0.2f, 0.2f, 0.2f, 0.2f, TERRAIN_HEIGHT, eBlobShapeGround_Donut, eBlobShapeSide_Hill, m_pperlin);
	m_pblobAll = new CBlob(0.5f, 0.5f, 0.51f, 0.51f, -0.2, eBlobShapeGround_Rect, eBlobShapeSide_All, nullptr);

	m_zgTerrainOri.AddBlob(m_pblobAllPositive);
	m_zgTerrainOri.AddBlob(m_pblobTerrainPositive);
	m_zgTerrainOri.AddBlob(m_pblobTerrainCraterPositive);
	m_zgTerrainOri.AddBlob(m_pblobAll);

	m_zgTerrainOri.CreateField(TERRAIN_SIZE, TERRAIN_SIZE, 1000, 1000, 0.0f, 0.0f, 1.0f, 1.0f);


	//
	//Terrain mit verschiedenen Materialien erstellen

	m_cutUnderSea.SetHeightLimits(-F_MAX, 0.0f);
	m_cutUnderSea.SetFlattenLowerOn();
	m_cutUnderSea.SetFlattenSmartOn();
	m_cutUnderSea.SetFlattenUpperOn();
	m_zgTerrain.AddCut(&m_cutUnderSea);

	/*m_cutSeaToSand.SetHeightLimits(0.0f,TERRAIN_SANDLIMIT);
	m_cutSeaToSand.SetHeightInverseOn();
	m_cutSeaToSand.SetFlattenLowerOn();
	m_cutSeaToSand.SetFlattenSmartOn();


	m_cutSandtoBeach.SetHeightLimits(TERRAIN_SANDLIMIT, TERRAIN_BEACHLIMIT);
	m_cutSandtoBeach.SetHeightInverseOn();

	m_cutSeaToBeach.SetHeightLimits(0.0f, TERRAIN_BEACHLIMIT);
	m_cutSeaToBeach.SetHeightInverseOn();
	m_cutSeaToBeach.SetFlattenLowerOn();
	m_cutSeaToBeach.SetFlattenSmartOn();*/
	//
	//


	// Lade die Texturen für Wasser und Land:

	m_zmRock.LoadPreset("Rock");//Laden der Steinstruktur
	m_zmRock.SetHeightStrength(1.5);
	m_zmSand.LoadPreset("Sand");
	m_zmEarth.LoadPreset("RockMossy");



	//Wasser
	m_zmWater.LoadPreset("Water");//Laden des Wassers
	m_zmWater.SetAni(8, 8, 7);	//Animierte Wassertextur hat 8x8 Bilder 
	m_zmWater.Translate(CColor(0.8f, 0.0f, 0.0f)); //Blauwert vom Wasser erhöhen
	m_zmWater.SetTransparency(0.8f);//Durchsichtigkeit des Wassers
	m_zmWater.SetTransparencyOn();
	m_zmWater.MakeTextureHaze("textures//WaterH.jpg");
	m_zmWater.SetPostprocessingOn();
	m_zmWater.SetHazeOn();
	m_zmWater.SetHazeStrength(1.8);





	// Erzeuge mit dem Terraindaten eine Terraingeometrie: 
	m_zgTerrain.InitFromOther(m_zgTerrainOri, &m_zmEarth);


	// Erzeuge die Wasseroberfläche:
	m_zgWater.Init(WATER_SIZE, WATER_SIZE, &m_zmWater, 10, 10, 0.0f, 0.0f, 500.0f, 500.0f);



	// Füge das Terrain dem Kollisionscontainer hinzu:
	m_zgsTerrain.Add(&m_zgTerrain);


	// Initialisiere die Kamera mit Outdoor-BVH-
	// Schattenfrustumcasting (OBVHSFC) zur Beschleunigung:
	m_zs.SetFrustumCullingOn();
	m_zc.Init(HALFPI, 0.3, 170000.0f, true, m_zs.GetSkyLightPlacement());

	// Initialisiere die Kamera:
	m_zs.AddPlacement(&m_zpCamera);
	m_zpCamera.AddCamera(&m_zc);
	// Die Kamera soll sich 30 m/s bewegen:
	m_zpCamera.SetTranslationSensitivity(1000);
	// Stelle die Kamera an einen geeigneten Anfangsort: 
	m_zpCamera.Translate(0.0f, 20.0f, 1000.0f);




	////END LANDSCAPE


	////OVERLAY
	m_zwf2.LoadPreset("RodWhite");
	m_zw2.Init(C2dRect(0.04f, 0.04f, 0.0f, 0.0f), 5, &m_zwf2);
	m_zwf2.SetTransparency(0.0f);
	m_zw2.PrintF("Ammo:");
	m_zv.AddWriting(&m_zw2);
	m_zwf2.SetTransparencyKind(eTransparencyKind_BinaryByChromaKey);


	m_zwf.LoadPreset("RodWhite");
	m_zw.Init(C2dRect(0.04f, 0.04f, 0.0f, 0.05f), 5, &m_zwf);
	m_zw.PrintF("Fuel:");
	m_zv.AddWriting(&m_zw);
	m_zwf.SetTransparencyKind(eTransparencyKind_BinaryByChromaKey);


	m_zwf3.LoadPreset("RodWhite");
	m_zw3.Init(C2dRect(0.04f, 0.04f, 0.0f, 0.1f), 6, &m_zwf3);
	m_zwf3.SetTransparency(0.0f);
	m_zw3.PrintF("Speed:");
	m_zv.AddWriting(&m_zw3);
	m_zwf3.SetTransparencyKind(eTransparencyKind_BinaryByChromaKey);

	////END OVERLAY


}

void CGame::Tick(float fTime, float fTimeDelta)
{
	//-------------------------------------------
	// Veränderungen während der Simulationszeit:
	//-------------------------------------------

	// Hier kommen die Veränderungen pro Renderschritt hinein: 



	// Traversiere am Schluss den Szenengraf und rendere:
	m_zdk.PlaceWASD(m_zpCamera, fTimeDelta, true);
	m_zr.Tick(fTimeDelta);

}

void CGame::Fini()
{
	//-------------------------------
	// Finalisiere die Knotenobjekte:
	//-------------------------------

	// Hier die Finalisierung Deiner Vektoria-Objekte einfügen:

	// Auch die Root sollte finalisiert werden:   
	m_zr.Fini();
	delete m_pperlin;
	delete m_pblob;

}

void CGame::WindowReSize(int iNewWidth, int iNewHeight)
{
	// Windows ReSize wird immer automatisch aufgerufen, wenn die Fenstergröße verändert wurde.
	// Hier kannst Du dann die Auflösung des Viewports neu einstellen:
	// z.B. mittels: m_zf.ReSize(iNewWidth, iNewHeight);
}

float CGame::GetTimeDeltaMin()
{
	return m_zr.GetTimeDeltaMin();
}

float CGame::GetVersion()
{
	return m_zr.GetVersion();
}




