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
	m_zr.Init(psplash);
	m_zf.Init(hwnd, procOS);
	m_zv.InitFull(&m_zc);
	m_zr.AddFrame(&m_zf);
	m_zf.AddViewport(&m_zv);
	m_zr.AddScene(&m_zs);
	m_zv.SetHazeOn();

	///////////////////////////////////////////////////
	//////////////////// FULLSCREEN ///////////////////
	///////////////////////////////////////////////////
	LockCursorToWindow(hwnd);
	m_zf.SetFullscreenOn();
	m_zf.ReSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

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
	m_zs.SetSkyTimeOfDay(0.0);
	m_zs.SetSkyTimeOfYear(0.0);
	m_zs.SetSkyFlowOn(0); // Day length
	m_zs.SetSkyMistOn(true);
	m_zs.SetSkyMistStrength(0.00017f);
	m_zs.SetSkyMistStartDistance(18000.00003f);

	// Hänge die Geometrien an ein geeignetes Placement:
	m_zs.AddPlacement(&m_zpLandscape);
	m_zpLandscape.AddGeo(&m_zgTerrain);
	m_zpLandscape.AddGeo(&m_zgWater);

	// Islands erzeugen
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

	// Lade die Texturen für Wasser und Land:
	m_zmRock.LoadPreset("Rock");
	m_zmRock.SetHeightStrength(1.5);
	m_zmSand.LoadPreset("Sand");
	m_zmEarth.LoadPreset("RockMossy");

	// Wasser
	m_zmWater.LoadPreset("Water"); //Laden des Wassers
	m_zmWater.SetAni(8, 8, 7);	//Animierte Wassertextur hat 8x8 Bilder 
	m_zmWater.Translate(CColor(0.8f, 0.0f, 0.8f)); //Blauwert vom Wasser erhöhen
	m_zmWater.SetTransparency(0.2f);//Durchsichtigkeit des Wassers
	m_zmWater.SetTransparencyOn();
	m_zmWater.MakeTextureHaze("textures//WaterHaze.jpg");
	m_zmWater.SetPostprocessingOn();
	m_zmWater.SetHazeOn();
	m_zmWater.SetHazeStrength(1.8);

	// Erzeuge mit dem Terraindaten eine Terraingeometrie: 
	m_zgTerrain.InitFromOther(m_zgTerrainOri, &m_zmEarth);

	// Erzeuge die Wasseroberfläche:
	m_zgWater.Init(WATER_SIZE, WATER_SIZE, &m_zmWater, 10, 10, 0.0f, 0.0f, 500.0f, 500.0f);

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
	m_pgCannon = m_CannonFile.LoadGeoTriangleTable("models\\Cannon\\cannon_01_4k.obj");
	m_zs.AddPlacement(&m_zpCannon);
	m_zpCannon.AddGeo(m_pgCannon);
	m_zpCannon.ScaleDelta(10.0f);
	m_zpCannon.TranslateDelta(-300, 0, 700);

	/// Ship
	m_zgShip = m_ShipFile.LoadGeoTriangleTable("models\\ship_pinnace_4k.blend\\ship_pinnace_4k.obj");
	m_zs.AddPlacement(&m_zpShip);
	m_zpShip.AddGeo(m_zgShip);
	m_zpShip.ScaleDelta(10.0f);
	m_zpShip.RotateYDelta(HALFPI);
	m_zpShip.TranslateDelta(-350, 0, 1000);

	/// Plane
	m_pzgPlane = m_PlaneFile.LoadGeoTriangleTable("models\\Aviones\\Fighter1\\Arsenal_VG33.obj");
	m_zpPlane.AddGeo(m_pzgPlane);
	m_zpPlane.ScaleDelta(1);
	
	/// Turrets
	m_zgTurret = m_TurretFile.LoadGeoTriangleTable("models\\turret\\turret.obj");

	for (int i = 0; i < TURRET_COUNT; i++)
	{
		m_zs.AddPlacement(&m_zpTurrets[i]);
		m_zpTurrets[i].AddGeo(m_zgTurret);
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

		//  Welches Placement muss übergeben werden?
		m_zpTurrets[i].SetPointing(&m_zpPlaneCenter);
	}


	/// Bullets
	m_zgBullet.Init(0.5f, &m_zmBullet);
	m_zpBulletTemplate.AddGeo(&m_zgBullet);
	m_zpBulletTemplate.SetPhysics(2.0f, 0.1f, 0.0f, 200000.1f, true); // Welche Phsyics?
	m_zpBulletTemplate.SwitchOff();
	m_zs.AddPlacement(&m_zpBulletTemplate);

	// Initialisiere den Schalt-Ringpuffer:
	m_zpsBullets.RingMake(15, m_zpBulletTemplate);
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

	/// WASD STEERING
	//m_zdk.PlaceWASD(m_zpCamera, fTimeDelta, false);

	/////////////////////////////////////////////
	///////////// TURRET SHOOTING ///////////////
	/////////////////////////////////////////////
	// Wenn die Leertaste heruntergedrückt wurde ...:
	if (m_zdk.KeyDown(DIK_SPACE)) {
		// … und Wenn noch Platz im Ringpuffer ist:
		if (m_zpsBullets.RingIsNotFull()) 
		{
			CPlacement* pzp = m_zpsBullets.RingInc();

			pzp->SetMat(m_zpTurrets[0].GetMatGlobal());
		
			// Und schieß die Kugel
			CHVector v;
			v.Copy(m_zpPlane.GetPosGlobal());
			v.MakeDirection();

			pzp->SetPhysicsVelocity(v);
			//pzp->SetPhysicsVelocity(m_zpPlane.GetPosGlobal()); // Welcher Vector muss hier übergeben werden?

		}
	}
	// Wenn die Feuerkugel weiter als 1km weg ist,
	CPlacement* pzpOldestBullet = m_zpsBullets.RingAskLast();
	if (pzpOldestBullet)
		if (pzpOldestBullet->GetPos().Length() > 1000.0f)
			// dann überführe sie in das passive Reich
			// der Schläfer:
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




