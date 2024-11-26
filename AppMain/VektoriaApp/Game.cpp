#include "Game.h"

CGame::CGame(void)
{
}

CGame::~CGame(void)
{
}



void LockCursorToWindow(HWND hwnd)
{
	RECT rect;
	// Get the client area of the window
	if (GetClientRect(hwnd, &rect))
	{
		// Convert client coordinates to screen coordinates
		POINT upperLeft = { rect.left, rect.top };
		POINT lowerRight = { rect.right, rect.bottom };

		ClientToScreen(hwnd, &upperLeft);
		ClientToScreen(hwnd, &lowerRight);

		rect.left = upperLeft.x;
		rect.top = upperLeft.y;
		rect.right = lowerRight.x;
		rect.bottom = lowerRight.y;

		// Lock the cursor to this rectangle
		ClipCursor(&rect);
	}
}

float CGame::ClampValue(float value, float minValue, float maxValue) {
	return max(minValue, min(value, maxValue));
}

void CGame::CenterSquare(float x, float y, float size, COverlay& rect) {
	float halfSize = size / 2.0f;
	rect.SetPos(x - (halfSize * 9), y - (halfSize * 16));
}

void CGame::PlaneSteering(float& x, float& y, float fTimeDelta)
{
	//GetMausbewegung
	x += m_zdm.GetRelativeX();
	y -= m_zdm.GetRelativeY();

	//GetControllerInput und rechne Stickdrift weg
	float controllerXInput = m_zdgc.GetRelativeX() / controllerSensitivity;
	if (abs(controllerXInput) < 0.0001f)
		controllerXInput = 0;
	float controllerYInput = m_zdgc.GetRelativeY() / controllerSensitivity;
	if (abs(controllerYInput) < 0.0001f)
		controllerYInput = 0;
	x += controllerXInput;
	y += controllerYInput;

	//Maximiere Ausschlag des Kreises
	x = ClampValue(x, -0.4, 0.4);
	y = ClampValue(y, -0.4, 0.4);

	//Verschiebe Kreis
	CenterSquare(x + 0.5, y + 0.5, crosshairSize, m_zoCirclehair);
	CenterSquare(x / 7 + 0.5, y / 5 + 0.5, crosshairSize, m_zoCrosshair);

	//minimiert die maximale Rotation pro Tick
	x_rotation = ClampValue(x, x - planeRotationSpeed, x + planeRotationSpeed);
	y_rotation = ClampValue(y, y - planeRotationSpeed, y + planeRotationSpeed);

	//Rotiert das Flugzeug
	m_zpPlane.RotateY(-x / 2);
	m_zpPlane.RotateZDelta(-x * 5);
	m_zpPlane.RotateXDelta(-y);

	//Rotiert die Kamera
	//m_zpCameraPivot.RotateY(-x_rotation * 2);
	//m_zpCameraPivot.RotateXDelta(y_rotation / 2);

	//Setzt den Kreis langsam in Richtung Mitte des Bildsschirms zurück
	if (x != 0) {
		x -= x / 500;
	}
	if (y != 0) {
		y -= y / 900;
	}

	//Move Plane and Camera
	float RotationX = x * 15;
	float RotationY = -y * 15;
	float MoveAD = 0, MoveWS = -1, MoveUD = 0;
	m_zpPlaneCenter.Move(fTimeDelta, true, MoveAD, MoveWS, MoveUD, RotationX, RotationY);
}

void CGame::Init(HWND hwnd, void(*procOS)(HWND hwnd, unsigned int uWndFlags), CSplash* psplash)
{
	m_zr.Init(psplash, false, false, false);
	m_zf.Init(hwnd, procOS);
	m_zv.InitFull(&m_zc);
	m_zr.AddFrame(&m_zf);
	m_zf.AddViewport(&m_zv);
	m_zr.AddScene(&m_zs);
	m_zv.SetHazeOn();

	///////////////////////////////////////////////////
	//////////////////// FULLSCREEN ///////////////////
	/////////////////////////////////////////////////
	LockCursorToWindow(hwnd);
	//m_zf.SetFullscreenOn();
	//m_zf.ReSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	// Initialisiere die Kamera mit Outdoor-BVH-
	// Schattenfrustumcasting (OBVHSFC) zur Beschleunigung:
	m_zs.SetFrustumCullingOn();
	m_zc.Init(PI / 3, 0.3, 170000.0f, true, m_zs.GetSkyLightPlacement());

	// Initialisiere die Kamera:
	m_zs.AddPlacement(&m_zpCamera);
	m_zpCamera.AddCamera(&m_zc);


	///////////////////////////////////////////////////
	//////////////////// LANDSCAPE ////////////////////
	///////////////////////////////////////////////////


	// Himmel mit Sonne, Mond und Sternen:
	m_zs.SetSkyOn(&m_zpCamera);
	m_zs.SetSkyRandomWeatherOn();
	m_zs.SetSkyFlowOn(80.0f);
	// m_zs.SetSkyTimeOfYear(0.25);
	
	// m_zs.SetSkyFlowOn(0); // Day length
	//m_zs.SetSkyRandomWeatherOn();
	m_zs.SetSkyMistOn();

	//m_zs.SetSkyMistColor();
	m_zs.SetSkyMistStrength(0.00400f);
	m_zs.SetSkyMistHeightMin(0.0f);
	m_zs.SetSkyMistHeightMax(7.0f);
	m_zs.SetSkyMistStartDistance(25000);
	

	/*
	m_zs.SetSkyMistOn(true);
	m_zs.SetSkyMistHeightMax(2);
	m_zs.SetSkyMistStrength(0.00017f);
	m_zs.SetSkyMistStartDistance(07500);
	*/
	// Hänge die Geometrien an ein geeignetes Placement:
	m_zpLandscape.AddGeo(&m_zgTerrain);
	m_zpLandscape.AddGeo(&m_zgWater);
	
	
	//m_zpLandscape.AddGeo(&m_zgWaterSchoener);

	// Islands erzeugen
	m_zs.AddPlacement(&m_zpLandscape);


	m_zs.AddPlacement(&m_zpIsland1);
	m_zs.AddPlacement(&m_zpIsland2);
	m_zs.AddPlacement(&m_zpIsland3);



	//Die wo weit weg war
	m_zpIsland1.TranslateDelta(6000, 0, 4000);



	m_zpIsland2.RotateY(THIRDPI);
	m_zpIsland2.TranslateDelta(1500, 0, 2500);


	m_zpIsland3.RotateY(-HALFPI);
	m_zpIsland3.TranslateDelta(-1890, 0, -2300);

	m_pperlin = new CPerlin(PERLIN_SEED, 3.0f, 15, 0.5f, 8.0f, 1.50f, 1.5f, ePerlinInterpol_Standard, false);
	m_pblobAllPositive = new CBlob(0.5f, 0.5f, 0.6f, 0.6f, -0.1f, eBlobShapeGround_Rect, eBlobShapeSide_All, NULL);
	m_pblobAllNegative = new CBlob(0.5f, 0.5f, 0.6f, 0.6f, +0.1f, eBlobShapeGround_Rect, eBlobShapeSide_All, NULL);
	m_pblobTerrainPositive = new CBlob(0.5f, 0.5f, 0.5f, 0.5f, TERRAIN_HEIGHT, eBlobShapeGround_Radial, eBlobShapeSide_Hill, m_pperlin);
	m_pblobTerrainNegative = new CBlob(0.5f, 0.5f, 0.5f, 0.5f, -TERRAIN_HEIGHT, eBlobShapeGround_Radial, eBlobShapeSide_Hill, m_pperlin);
	m_pblobTerrainCraterPositive = new CBlob(0.2f, 0.2f, 0.2f, 0.2f, TERRAIN_HEIGHT, eBlobShapeGround_Donut, eBlobShapeSide_Hill, m_pperlin);
	m_pblobAll = new CBlob(0.5f, 0.5f, 0.51f, 0.51f, -0.4, eBlobShapeGround_Rect, eBlobShapeSide_All, nullptr);


	m_zgTerrainOri.AddBlob(m_pblobAllPositive);
	m_zgTerrainOri.AddBlob(m_pblobTerrainPositive);
	m_zgTerrainOri.AddBlob(m_pblobTerrainCraterPositive);
	m_zgTerrainOri.AddBlob(m_pblobAll);


	m_zgTerrainOri.CreateField(TERRAIN_SIZE, TERRAIN_SIZE, TERRAIN_VERTICES, TERRAIN_VERTICES, 0.0f, 0.0f, 10.0f, 10.0f);
	m_zgWater.CreateField(33000, 33000, 10, 10, 0.0f, 0.0f, 30.0f, 30.0f);

	// Erzeuge die Wasseroberfläche:
	m_zgWater.Init(WATER_SIZE, WATER_SIZE, &m_zmWater, 10, 10, 0.0f, 0.0f, 500.0f, 500.0f);
	
	
	//Terrain mit verschiedenen Materialien erstellen,
	//Cuts






	m_cutOverSea.SetHeightLimits(-F_MAX, 0.0f);
	m_cutOverSea.SetFlattenLowerOn();
	m_cutOverSea.SetFlattenSmartOn();



	m_cutUnderSea.SetHeightLimits(0.0f, F_MAX);
	m_cutUnderSea.SetFlattenUpperOn();
	m_cutUnderSea.SetFlattenSmartOn();


	m_cutSeaToSand.SetHeightLimits(0.0f,TERRAIN_SANDLIMIT);
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

	m_cutUnder45Degrees.SetSlopeLimits(0.0f,  QUARTERPI);
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

	//m_zgWaterSchoener.AddCut(&m_cutUnderSea);



	m_zpLandscape.AddGeo(&m_zgTerrainFlora);
	m_zpLandscape.AddGeo(&m_zgTerrainLow);
	m_zpLandscape.AddGeo(&m_zgTerrainRock);
	m_zpLandscape.AddGeo(&m_zgTerrainMirror);
	m_zpLandscape.AddGeo(&m_zgTerrainSnowHeavy);
	m_zpLandscape.AddGeo(&m_zgTerrainSnow);
	m_zpLandscape.AddGeo(&m_zgTerrainOri);
	m_zpLandscape.AddGeo(&m_zgTerrainSand);
	m_zpLandscape.AddGeo(&m_zgTerrainSandMossy);
	//m_zpLandscape.AddGeo(&m_zgWaterSchoener);


	//Insel in der anderen drinne 
	m_zpIsland1.AddGeo(&m_zgTerrainFlora);
	m_zpIsland1.AddGeo(&m_zgTerrainLow);
	m_zpIsland1.AddGeo(&m_zgTerrainRock);
	m_zpIsland1.AddGeo(&m_zgTerrainMirror);
	m_zpIsland1.AddGeo(&m_zgTerrainSnowHeavy);
	m_zpIsland1.AddGeo(&m_zgTerrainSnow);
	m_zpIsland1.AddGeo(&m_zgTerrainOri);
	m_zpIsland1.AddGeo(&m_zgTerrainSand);
	m_zpIsland1.AddGeo(&m_zgTerrainSandMossy);
	//m_zpIsland1.AddGeo(&m_zgWaterSchoener);





	m_zpIsland2.AddGeo(&m_zgTerrainFlora);
	m_zpIsland2.AddGeo(&m_zgTerrainLow);
	m_zpIsland2.AddGeo(&m_zgTerrainRock);
	m_zpIsland2.AddGeo(&m_zgTerrainMirror);
	m_zpIsland2.AddGeo(&m_zgTerrainSnowHeavy);
	m_zpIsland2.AddGeo(&m_zgTerrainSnow);
	m_zpIsland2.AddGeo(&m_zgTerrainOri);
	m_zpIsland2.AddGeo(&m_zgTerrainSand);
	m_zpIsland2.AddGeo(&m_zgTerrainSandMossy);
	//m_zpIsland2.AddGeo(&m_zgWaterSchoener);


	m_zpIsland3.AddGeo(&m_zgTerrainFlora);
	m_zpIsland3.AddGeo(&m_zgTerrainLow);
	m_zpIsland3.AddGeo(&m_zgTerrainRock);
	m_zpIsland3.AddGeo(&m_zgTerrainMirror);
	m_zpIsland3.AddGeo(&m_zgTerrainSnowHeavy);
	m_zpIsland3.AddGeo(&m_zgTerrainSnow);
	m_zpIsland3.AddGeo(&m_zgTerrainOri);
	m_zpIsland3.AddGeo(&m_zgTerrainSand);
	m_zpIsland3.AddGeo(&m_zgTerrainSandMossy);
	//m_zpIsland3.AddGeo(&m_zgWaterSchoener);

	


	m_zgTerrainMirror.InitFromOther(m_zgTerrainOri, &m_zmEarth, true);
	m_zgTerrainFlora.InitFromOther(m_zgTerrainOri, &m_zmEarth, false);
	m_zgTerrainLow.InitFromOther(m_zgTerrainOri, &m_zmEarth, false);
	m_zgTerrainRock.InitFromOther(m_zgTerrainOri, &m_zmRock, false);
	m_zgTerrainSnowHeavy.InitFromOther(m_zgTerrainOri, &m_zmSnowHeavy, false);
	m_zgTerrainSnow.InitFromOther(m_zgTerrainOri, &m_zmSnow, false);
	m_zgTerrainSand.InitFromOther(m_zgTerrainOri, &m_zmSand, false);
	m_zgTerrainSandMossy.InitFromOther(m_zgTerrainOri, &m_zmSandMossy, false);
	
	
	
	//m_zgWaterSchoener.InitFromOther(m_zgTerrainOri, &m_zmWater, false);
	//CHMat mMachsPlatt; 
	//mMachsPlatt.ScaleY(0.0f);
	//m_zgWaterSchoener.Transform(mMachsPlatt);


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
	m_zmWater.SetAni(8, 8, 10);	//Animierte Wassertextur hat 8x8 Bilder
	m_zmWater.Translate(CColor(0.8f, 0.0f, 0.8f)); //Blauwert vom Wasser erhöhen
	m_zmWater.SetTransparency(0.6f);//Durchsichtigkeit des Wassers
	m_zmWater.SetTransparencyOn();
	m_zmWater.MakeTextureHaze("textures//WaterHaze.jpg");
	m_zmWater.SetPostprocessingOn();
	m_zmWater.SetHazeOn();
	m_zmWater.SetHazeStrength(7.0);

	// Erzeuge mit dem Terraindaten eine Terraingeometrie: 
	//m_zgTerrain.InitFromOther(m_zgTerrainOri, &m_zmEarth);


	// Füge das Terrain dem Kollisionscontainer hinzu:
	m_zgsTerrain.Add(&m_zgTerrain);

	///////////////////////////////////////////////////
	//////////////////// PLANE STEERING ///////////////
	///////////////////////////////////////////////////
	m_zs.AddPlacement(&m_zpPlaneCenter);
	m_zpPlaneCenter.SetTranslationSensitivity(100);
	m_zpPlaneCenter.AddPlacement(&m_zpCameraPivot);
	m_zpPlaneCenter.AddPlacement(&m_zpPlane);
	m_zpPlane.AddPlacement(&m_zpPlaneTip);

	m_zpPlaneTip.TranslateZDelta(-2);
	m_zpPlaneTip.TranslateYDelta(-0.4f);

	////////////////////////////////////////////////////
	////////////////////// WASD STEERING ///////////////
	////////////////////////////////////////////////////
	//m_zpCamera.SetTranslationSensitivity(750.0f);
	//m_zpCamera.SetRotationSensitivity(1);
	//m_zpCamera.Translate(1000, 0, 1000);

	///////////////////////////////////////////////////
	//////////////////// PLACEMENTS ///////////////////
	///////////////////////////////////////////////////
	/// Cannon
	m_zgCannon = m_CannonFile.LoadGeoTriangleTable("models\\Cannon\\cannon_01_4k.obj");
	m_zs.AddPlacement(&m_zpCannon);
	m_zpCannon.AddGeo(m_zgCannon);
	m_zpCannon.ScaleDelta(10.0f);
	m_zpCannon.TranslateDelta(-300, 0, 700);

	/// Ship
	m_zgShip = m_ShipFile.LoadGeoTriangleTable("models\\ship_pinnace_4k.blend\\ship_pinnace_4k.obj");
	m_zs.AddPlacement(&m_zpShip);
	m_zpShip.AddGeo(m_zgShip);
	m_zpShip.ScaleDelta(5.0f);
	m_zpShip.RotateYDelta(HALFPI);
	m_zpShip.TranslateDelta(-350, 0, 1000);
	m_zmShip.LoadPreset("WoodPlanksSpaced");
	m_zgShip->SetMaterial(&m_zmShip);

	/// Plane
	m_pzgPlane = m_PlaneFile.LoadGeoTriangleTable("models\\Plane\\source\\mesh.obj",true);
	CHMat m;
	m.RotateY(PI);
	m_pzgPlane->Transform(m);
	m_zpPlane.AddGeo(m_pzgPlane);
	m_zpPlane.ScaleDelta(1);
	m_zmPlane.MakeTextureDiffuse("models\\Plane\\textures\\COLOR.png");
	m_pzgPlane->SetMaterial(&m_zmPlane);






	///// Carrier 1
	//m_zgCarrier1 = m_Carrier1File.LoadGeoTriangleTable("models\\Carrier\\Carrier.obj"); 
	//m_zs.AddPlacement(&m_zpCarrier1);
	//m_zpCarrier1.AddGeo(m_zgCarrier1);
	//m_zpCarrier1.ScaleDelta(4.0f);
	//m_zpCarrier1.RotateYDelta(HALFPI);
	//m_zpCarrier1.TranslateDelta(-5000, 20, 1000);


	//m_zmCarrier1.LoadPreset("MetalRustyFlaking");
	//m_zgCarrier1->SetMaterial(&m_zmCarrier1);

	////m_zmCarrier1.MakeTextureDiffuse("models\\Carrier\\Carrier.mtl");
	////m_zmCarrier1.Load("models\\Carrier\\Carrier.mtl");
	////m_zgCarrier1->SetMaterial(&m_zmCarrier1);
	////m_zgCarrier1->SetMaterial(&m_zmCarrier1);

	/////Carrier 2
	//m_zgCarrier2 = m_Carrier2File.LoadGeoTriangleTable("models\\Carrier\\Carrier.obj");
	//m_zs.AddPlacement(&m_zpCarrier2);
	//m_zpCarrier2.AddGeo(m_zgCarrier2);
	//m_zpCarrier2.ScaleDelta(4.0f);
	//m_zpCarrier2.RotateYDelta(HALFPI);
	//m_zpCarrier2.TranslateDelta(-5500, 20, 1500);
	////m_zmCarrier2.Load("models\\Carrier\\Carrier.mtl");
	////m_zgCarrier2->SetMaterial(&m_zmCarrier2);
	//
	/////Battleship1
	//m_zgBattleship1 = m_Battleship1File.LoadGeoTriangleTable("models\\Battleship1\\Yamato[1].obj");
	//m_zs.AddPlacement(&m_zpBattleship1);
	//m_zpBattleship1.AddGeo(m_zgBattleship1);
	//m_zpBattleship1.ScaleDelta(2.5f);
	////m_zpBattleship1.RotateYDelta(HALFPI);
	//m_zpBattleship1.TranslateDelta(-5000, 0, 2000);



	/////Battleship2	
	//m_zgBattleship2 = m_Battleship2File.LoadGeoTriangleTable("models\\Battleship1\\Yamato[1].obj");
	//m_zs.AddPlacement(&m_zpBattleship2);
	//m_zpBattleship2.AddGeo(m_zgBattleship2);
	//m_zpBattleship2.ScaleDelta(2.5f);
	////m_zpBattleship1.RotateYDelta(HALFPI);
	//m_zpBattleship2.TranslateDelta(-3500, 0, 500);



	/////Destroyer1
	//m_zgDestroyer1 = m_Destroyer1File.LoadGeoTriangleTable("models\\Destroyer\\HMS_Daring_Type_45.obj");
	//m_zs.AddPlacement(&m_zpDestroyer1);
	//m_zpDestroyer1.AddGeo(m_zgDestroyer1);
	//m_zpDestroyer1.ScaleDelta(1.5f);
	//m_zpDestroyer1.RotateYDelta(HALFPI);
	//m_zpDestroyer1.TranslateDelta(-4500, 0, 700);


	/////Destroyer2
	//m_zgDestroyer2 = m_Destroyer2File.LoadGeoTriangleTable("models\\Destroyer\\HMS_Daring_Type_45.obj");
	//m_zs.AddPlacement(&m_zpDestroyer2);
	//m_zpDestroyer2.AddGeo(m_zgDestroyer2);
	//m_zpDestroyer2.ScaleDelta(1.5f);
	//m_zpDestroyer2.RotateYDelta(HALFPI);
	//m_zpDestroyer2.TranslateDelta(-4500, 0, -700);



	/////Lighthouse
	//m_zgLighthouse = m_LighthouseFile.LoadGeoTriangleTable("models\\Lighthouse\\LightHouse.obj");
	//m_zs.AddPlacement(&m_zpLighthouse);
	//m_zpLighthouse.AddGeo(m_zgLighthouse);
	//m_zpLighthouse.ScaleDelta(4.0f);
	////m_zpBattleship1.RotateYDelta(HALFPI);
	//m_zpLighthouse.TranslateDelta(1000,0, 2000);


	/// Turrets
	m_zgTurret = m_TurretFile.LoadGeoTriangleTable("models\\turret\\turret.obj");

	for (int i = 0; i < TURRET_COUNT; i++)
	{
		m_zs.AddPlacement(&m_zpTurrets[i]);
		m_zpTurrets[i].AddPlacement(&m_zpTurretPointing[i]);

		m_zpTurretPointing[i].AddGeo(m_zgTurret);
		m_zpTurrets[i].ScaleDelta(25);
		m_zpTurrets[i].RotateYDelta(HALFPI);
	}

	m_zpTurrets[0].TranslateDelta(650, 0, 450);
	m_zpTurrets[1].TranslateDelta(300, 0, -250);
	m_zpTurrets[2].TranslateDelta(650, 0, -450);
	m_zpTurrets[3].TranslateDelta(0, 0, -650);
	m_zpTurrets[4].TranslateDelta(-550, 0, -150);
	m_zpTurrets[5].TranslateDelta(-450, 0, 350);

	float height;

	for (int i = 0; i < TURRET_COUNT; i++)
	{
		height = m_zgTerrain.GetHeight(m_zpTurrets[i].GetPos().x, m_zpTurrets[i].GetPos().z);
		m_zpTurrets[i].TranslateYDelta(height);
	}

	/// Bullets
	//m_zmBullet.LoadPreset("AshesGlowing");
	m_zgBullet.Init(0.1f, &m_zmBullet);
	m_zpBulletTemplate.AddGeo(&m_zgBullet);
	m_zpBulletTemplate.SetPhysics(2.0f, 0.1f, 0.0f, 200000.1f, true); // Welche Phsyics?
	m_zpBulletTemplate.SwitchOff();
	m_zs.AddPlacement(&m_zpBulletTemplate);
	m_zmBullet.LoadPreset("MetalRustyFlaking");



	/// Initialisiere den Schalt-Ringpuffer:
	m_zpsBullets.RingMake(10000, m_zpBulletTemplate);
	m_zs.AddPlacements(m_zpsBullets);

	///////////////////////////////////////////////////
	//////////////////// Overlay //////////////////////
	///////////////////////////////////////////////////
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


	/////////////////////////////////////////////////
	//////////////////// DEVICES ////////////////////
	/////////////////////////////////////////////////
	m_zf.AddDeviceCursor(&m_zdc);
	m_zf.AddDeviceKeyboard(&m_zdk);
	m_zf.AddDeviceMouse(&m_zdm);
	m_zf.AddDeviceGameController(&m_zdgc);
	m_zdc.Hide();

	m_zpCameraPivot.AddPlacement(&m_zpCamera);
	m_zpCamera.TranslateZDelta(40);
	m_zpCamera.TranslateYDelta(4);
	m_zpCamera.RotateXDelta(-PI / 12);

	// Crosshair
	m_zCrosshairRect.Init(crosshairSize * 9, crosshairSize * 16, 0, 0);
	m_zoCrosshair.Init("textures\\crosshair.jpg", m_zCrosshairRect, true);
	m_zv.AddOverlay(&m_zoCrosshair);
	CenterSquare(0.5, 0.5, crosshairSize, m_zoCrosshair);
	m_zoCirclehair.Init("textures\\circlehair.jpg", m_zCrosshairRect, true);
	m_zv.AddOverlay(&m_zoCirclehair);
}

void CGame::Tick(float fTime, float fTimeDelta)
{
	//-------------------------------------------
	// Veränderungen während der Simulationszeit:
	//-------------------------------------------

	// Hier kommen die Veränderungen pro Renderschritt hinein: 
	/////////////////////////////////////////////
	//////////////////// STEERING ///////////////
	/////////////////////////////////////////////
	/// PLANE STEERING
	PlaneSteering(x_initial, y_initial, fTimeDelta);



	//m_zpBattleship1.TranslateXDelta(0.01);
	//m_zpBattleship2.TranslateXDelta(0.01);
	//m_zpCarrier1.TranslateXDelta(0.01);
	//m_zpCarrier2.TranslateXDelta(0.01);
	/// WASD STEERING
	//m_zdk.PlaceWASD(m_zpCamera, fTimeDelta, false);

	/////////////////////////////////////////////
	////////////// TURRET TURNING ///////////////
	/////////////////////////////////////////////
	float shootInterval = 1.0f; // Mindestzeit in Sekunden zwischen den Schüssen

	for (int i = 0; i < TURRET_COUNT; i++)
	{
		CHVector v1 = m_zpPlaneCenter.GetPosGlobal();
		CHVector v2 = m_zpTurretPointing[i].GetPosGlobal();

		CHVector vDir2PlaneNormal(v1 - v2);
		vDir2PlaneNormal.Normal();
		CHVector vDir2Plane(v1 - v2);

		float fa = vDir2PlaneNormal.AngleXZ();
		float faUpDown = asinf(vDir2PlaneNormal.y);

		UM_SETINRANGE(faUpDown, -QUARTERPI, QUARTERPI);

		m_zpTurretPointing[i].RotateZ(faUpDown);
		m_zpTurretPointing[i].RotateYDelta(-fa - HALFPI);

		/*
		if(i==0)
		{
			LogDebugVector("Plane ", v1);
			LogDebugVector("Turret", v2);
			LogDebugVector("Dir2Pl", vDir2Plane);
			LogDebug("fa: %f", UM_RAD2DEG(fa));
		}
		*/

		/////////////////////////////////////////////
		///////////// TURRET SHOOTING ///////////////
		///////////////////////////////////////////// 
		static float fLastShotTime[TURRET_COUNT] = {0}; // Array für jeden Turret

		// Wenn das Flugzeug nah genug ist:
		if (vDir2Plane.Length() < ENTFERNUNG_FLUGZEUGKANONE && fTime - fLastShotTime[i] >= shootInterval) {
			// … und Wenn noch Platz im Ringpuffer ist:
			if (m_zpsBullets.RingIsNotFull())
			{
				CPlacement* pzp = m_zpsBullets.RingInc();

				// schieß die Kugel
				pzp->SetMat(m_zpTurrets[i].GetMatGlobal());
				pzp->SetPhysicsVelocity(vDir2Plane);

				fLastShotTime[i] = fTime; // Zeitpunkt des Schusses aktualisieren
			}
		}
	}

	// Wenn die Kugel weiter als 1km weg ist,
	CPlacement* pzpOldestBullet = m_zpsBullets.RingAskLast();
	if (pzpOldestBullet)
		if (pzpOldestBullet->GetPos().Length() > 1000.0f)
			// dann überführe sie in das passive Reich der Schläfer:
			m_zpsBullets.RingDec();

	// Traversiere am Schluss den Szenengraf und rendere:
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




