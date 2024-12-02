#include "Game.h"

CGame::CGame(void)
{
}

CGame::~CGame(void)
{
}


//---------------------------------------------------------------------
//Fullscreen
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

//---------------------------------------------------------------------
//
float CGame::ClampValue(float value, float minValue, float maxValue) {
	return max(minValue, min(value, maxValue));
}

void CGame::CenterSquare(float x, float y, float size, COverlay& rect) {
	float halfSize = size / 2.0f;
	rect.SetPos(x - (halfSize * 9), y - (halfSize * 16));
}


//---------------------------------------------------------------------
//Flugzeugsteuerung
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

void CGame::Init(HWND hwnd, void(*procOS)(HWND hwnd, unsigned int uWndFlags), CSplash * psplash)
{
	//---------------------------------
	// Initialisiere die Knotenobjekte:
	//---------------------------------

	// Zuallererst muss die Root mit dem Splash-Screen initialisiert werden, 
	// damit die Engine freigeschaltet wird:

	//---------------------------------------------------------------------
	//MiniMap, muss vor allem Initialisiert werden, da ein zweiter Viewport erstellt wird
	
	m_zv2.Init(&m_zcMiniMap,0.15f, 0.25f, 0.82f, 0.05f);
	m_zcMiniMap.InitOrtho(100.0f);
	m_zs.AddPlacement(&m_zpMiniMap);
	m_zpMiniMap.AddCamera(&m_zcMiniMap);
	m_zf.AddViewport(&m_zv2);
	m_zv2.SwitchOn();

	//---------------------------------------------------------------------
	//Startbildschrim Inits und Overlayadds overlay mit switchoff
	m_ziStart.Init("textures\\HalloWelt.jpg");
	m_zoStart.InitFull(&m_ziStart);
	m_zv.AddOverlay(&m_zoStart);

	//---------------------------------------------------------------------
	//Startbildschrim zurück
	m_zoBack2Start.Init("textures\\HalloWelt.jpg", C2dRect(0.08f, 0.05f, 0.9f, 0.9f));
	m_zv.AddOverlay(&m_zoBack2Start);
	m_zoBack2Start.SetLayer(0.1f);


	//---------------------------------------------------------------------
	//Pausebildschirm
	m_ziPause.Init("textures\\Pause.png");
	m_zoPause.InitFull(&m_ziPause);
	m_zv.AddOverlay(&m_zoPause);

	//---------------------------------------------------------------------
	//Button 1 und 2
	m_ziButton.Init("textures\\test.jpg");
	m_z2dButton.Init(0.3f, 0.2f, 0.4f, 0.1f);
	m_zoButton.Init(&m_ziButton,m_z2dButton, false);
	m_zv.AddOverlay(&m_zoButton);
	m_zoButton.SetLayer(0.3f);

	m_ziButton2.Init("textures\\test 2.jpg");
	m_z2dButton2.Init(0.3f, 0.2f, 0.39f, 0.39f);
	m_zoButton2.Init(&m_ziButton2, m_z2dButton2, false);
	m_zv.AddOverlay(&m_zoButton2);
	m_zoButton2.SetLayer(0.3f);

	//---------------------------------------------------------------------
	//Container für die Buttons
	m_zos.Add(&m_zoButton);
	m_zos.Add(&m_zoButton2);
	m_zos.Add(&m_zoBack2Start);
	m_zosInGame.Add(&m_zoBack2Start);


	//---------------------------------------------------------------------
	//Schwarzer Rand der Map ist transparent gesetzt damit er nicht das Wasser überdeckt
	m_ziMap.Init("textures\\black_image.jpg");
	m_z2dMap.Init(0.16f, 0.27f, 0.815f, 0.04f);
	m_zoMap.Init(&m_ziMap, m_z2dMap, false);
	m_zv.AddOverlay(&m_zoMap);
	m_zoMap.SetLayer(0.5f);
	m_zoMap.SetTransparency(0.7f);
	/*m_zoMap.SwitchOff();*/
	
	/*m_zv.SetWireframeOn();*/

	//---------------------------------------------------------------------
	//Grundinits wie Window, Kamera Viewport usw
	m_zr.Init(psplash);
	m_zf.Init(hwnd, procOS);
	LockCursorToWindow(hwnd);
	m_zv.InitFull(&m_zc);
	m_zc.Init(PI / 3, 0.3, 170000.0f, true, m_zs.GetSkyLightPlacement());;
	/*m_zf.SetFullscreenOn();
	m_zf.ReSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));*/
	m_zlp.Init(CHVector(-0.8f, 0.02f, 0.2f), CColor(1.0f, 1.0f, 1.0f));
	m_zr.AddFrame(&m_zf);
	m_zf.AddViewport(&m_zv);
	m_zs.AddLightParallel(&m_zlp);
	m_zr.AddScene(&m_zs);
	m_zs.AddPlacement(&m_zpCamera);
	m_zpCamera.AddCamera(&m_zc);
	m_zf.AddDeviceKeyboard(&m_zdk);
	


	//---------------------------------------------------------------------
	//Overlay

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
	

	//---------------------------------------------------------------------
	//Object rein

	m_mpgTest = m_fileWavefront.LoadGeoTriangleTable("C:\\Users\\Fozer\\OneDrive\\Desktop\\Programme\\Vektoria\\Vektoria\\VektoriaV23.19\\App - Kopie (2)\\modells\\test.obj");

	m_zs.AddPlacement(&m_zpTest);
	m_zpTest.AddGeo(m_mpgTest);
	m_zpTest.TranslateY(50);
	m_zpTest.TranslateZ(-10.0f);
	m_zpTest.Scale(15);



	//---------------------------------------------------------------------
	//STEUERUNG
	m_zf.AddDeviceCursor(&m_zdc);
	m_zf.AddDeviceKeyboard(&m_zdk);
	m_zf.AddDeviceMouse(&m_zdm);
	m_zf.AddDeviceGameController(&m_zdgc);
	/*m_zdc.Hide();*/


	m_pzgPlane = m_PlaneFile.LoadGeoTriangleTable("Models\\Aviones\\Fighter1\\Arsenal_VG33.obj", true);
	m_zpPlane.AddGeo(m_pzgPlane);
	m_zpPlane.ScaleDelta(1);

	m_zs.AddPlacement(&m_zpPlaneCenter);
	m_zpPlaneCenter.SetTranslationSensitivity(200);
	m_zpPlaneCenter.AddPlacement(&m_zpCameraPivot);
	m_zpCameraPivot.AddPlacement(&m_zpCamera);
	m_zpPlaneCenter.AddPlacement(&m_zpPlane);
	m_zpPlane.AddPlacement(&m_zpPlaneTip);

	m_zpPlaneTip.TranslateZDelta(-2);
	m_zpPlaneTip.TranslateYDelta(-0.4f);
	m_zpCamera.TranslateZDelta(40);
	m_zpCamera.TranslateYDelta(4);
	m_zpCamera.RotateXDelta(-PI / 12);
	m_zCrosshairRect.Init(crosshairSize * 9, crosshairSize * 16, 0, 0);
	m_zoCrosshair.Init("textures\\crosshair.jpg", m_zCrosshairRect, true);
	m_zv.AddOverlay(&m_zoCrosshair);
	CenterSquare(0.5, 0.5, crosshairSize, m_zoCrosshair);
	m_zoCirclehair.Init("textures\\circlehair.jpg", m_zCrosshairRect, true);
	m_zv.AddOverlay(&m_zoCirclehair);


	//---------------------------------------------------------------------
	//Hier ist der Teil für das Terrain

	//Haze_Postprocessing Filter für Wasser
	m_zv.SetHazeOn();

	//Erzeugt den Himmel
	m_zs.SetSkyOn(&m_zpCamera);
	m_zs.SetSkyFlowOn(0);
	for (int i = 0; i < 3; i++)
		m_zs.m_psceneweather->m_azmSky[i].SetPostprocessingOn();

	//Beschränkung der Kamera
	m_zpCamera.SetMoveRange(CAABB(
		CHVector(-50000.0f, 4.0f, -50000.0f, 1.0f),
		CHVector(+50000.0f, 20000.0f, +50000.0f, 1.0f)));

	//Erzeugt einen Cut, der alles unter N.N wegschneidet
	m_cutUnderSea.SetHeightLimits(-F_MAX, 0.0f);

	//Die Vertices der den Rand überlappenden Polygone werden auf N.N angehoben
	m_cutUnderSea.SetFlattenLowerOn();

	//XZ-Ebene mittel Bezier Curves
	m_cutUnderSea.SetFlattenSmartOn();

	//Füge den Cut dem Terrain hizu:
	/*m_cutUnderSea.SetFlattenSmartOn();*/

	//Perlin-noise wird hier erzeugt
	m_pperlin = new CPerlin(42, 3.0f, 12, 0.5f, 5.0f, 0.0f, 0.0f, ePerlinInterpol_Standard, false);

	//Texturen für Wassen und land werden hier geladen:
	m_zmWater.LoadPreset("WaterHaze");
	m_zmEarth.LoadPreset("RockMossy");

	//Mische den Perlin-Noise mit einem radialen Blob
	m_mpblob = new CBlob(0.5f, 0.5f, 0.5f, 0.5f, 300, eBlobShapeGround_Radial, eBlobShapeSide_Hill, m_pperlin);
	m_mpblob2 = new CBlob(0.8f, 0.8f, 0.2f, 0.2f, 400, eBlobShapeGround_Radial, eBlobShapeSide_Hill, nullptr);
	m_mpblob3 = new CBlob(0.2f, 0.2f, 0.2f, 0.2f, 200, eBlobShapeGround_Radial, eBlobShapeSide_Hill, m_pperlin);
	m_mpblob4 = new CBlob(0.5f, 0.5f, 0.5f, 0.5f, -0.1, eBlobShapeGround_Rect, eBlobShapeSide_All, nullptr);


	//Füge den Blob einem Blaupausen-Terrain hinzu:
	m_zgTerrrainOri.AddBlob(m_mpblob);
	m_zgTerrrainOri.AddBlob(m_mpblob2);
	m_zgTerrrainOri.AddBlob(m_mpblob3);
	m_zgTerrrainOri.AddBlob(m_mpblob4);


	m_zgTerrrain.AddCut(&m_cutUnderSea);

	//Erzeuge die Vertex-Daten des Blueprint Terrains
	m_zgTerrrainOri.CreateField(
		2000, 2000, // Ein Quadratkilometer Terrain
		1000, 1000, // 2000 mal 2000 Vertices0000
		0.0f, 0.0f, // Die UV-Textur beginnt bei (0,0) …
		1.0f, 1.0f);// … und geht bis (1,1)

	// Erzeuge mit dem Terraindaten eine Terraingeometrie: 
	m_zgTerrrain.InitFromOther(m_zgTerrrainOri, &m_zmEarth);

	// Erzeuge die Wasseroberfläche:
	m_zgWater.Init(
		70000, 70000, // Das Meer ist groß (70 *70 qkm)
		&m_zmWater,
		// Ein bewegtes Wassermaterial mit Wellen
		10, 10,
		0.0f, 0.0f,
		500.0f,
		500.0f);
	// Brauchen nur wenige Polygone (hier 100)
	// Eine UV-Verschiebung brauchen wir nicht
	// Aber die Textur soll sich 500 Mal …  
   // …in jeder Richtung wiederholen
  
	 // Hänge die Geometrien an ein geeignetes Placement:
	m_zs.AddPlacement(&m_zpLandscape);
	m_zpLandscape.AddGeo(&m_zgTerrrain);
	m_zpLandscape.AddGeo(&m_zgWater);

	// Füge das Terrain dem Kollisionscontainer hinzu:
	m_zgsTerrain.Add(&m_zgTerrrain);

	// Hier kommen alle weiteren Initialisierungen hinein: 
	
	m_bPaused = true; 
}

void CGame::Tick(float fTime, float fTimeDelta)
{
	//-------------------------------------------
	// Veränderungen während der Simulationszeit:
	//-------------------------------------------
	

	//---------------------------------------------------------------------
	//Pause
	m_fTimeWithPausings = fTime - m_fTimePausings;	
	if (m_zdk.KeyDown(DIK_P))
	{
		m_fTimeLastPausingStart = fTime;
		m_bPaused = true;
		m_zoButton.SwitchOn();
		m_zoButton2.SwitchOn();
		m_zoPause.SwitchOn();
		m_zoStart.SwitchOff();
		m_zv2.SwitchOff();
		m_zoBack2Start.SwitchOn();
		m_zdc.Show();

	}


	if (m_bPaused)
	{

		if (m_zdc.ButtonDownLeft())
		{
			COverlay* pzoPicked = m_zdc.PickOverlayPreselected(m_zos);
			if (pzoPicked == &m_zoBack2Start)
			{
				m_zoPause.SwitchOff();
				m_zoMap.SwitchOff();
				m_zoStart.SwitchOn();
				m_bPaused = false; 
			}
		}
	}

	//---------------------------------------------------------------------
	//Pause
	if (m_zdc.ButtonDownLeft())
	{
		m_bPaused = false;
		m_zoButton.SwitchOff();
		m_zoButton2.SwitchOff();
		m_zoPause.SwitchOff();
		m_zoStart.SwitchOff();
		m_zv2.SwitchOn();
		m_zoBack2Start.SwitchOff();
		m_zdc.Hide();
	}


	if (m_bPaused)
	{
		m_fTimePausings += fTimeDelta;
		fTimeDelta = 0.0f; 
	}

	//---------------------------------------------------------------------
	//Button

	if (m_zdk.KeyDown(DIK_A))
	{
		m_zoButton.SwitchOff();
	}

	if (m_zdk.KeyDown(DIK_S))
	{
		m_zoButton.SwitchOn();
	}

	if (m_zdk.KeyDown(DIK_A))
	{
		m_zoButton2.SwitchOff();
	}

	if (m_zdk.KeyDown(DIK_S))
	{
		m_zoButton2.SwitchOn();
	}



	// Hier kommen die Veränderungen pro Renderschritt hinein: 

	CHVector vPlane(m_zpPlaneCenter.GetPosGlobal());
	vPlane.y += 100; 
	m_zpMiniMap.RotateX(-HALFPI);
	m_zpMiniMap.TranslateDelta(vPlane);


	// Traversiere am Schluss den Szenengraf und rendere:
	if(!m_bPaused)
		PlaneSteering(x_initial, y_initial, fTimeDelta);

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
	delete m_mpblob;
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


