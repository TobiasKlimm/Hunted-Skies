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

void CGame::Init(HWND hwnd, void(*procOS)(HWND hwnd, unsigned int uWndFlags), CSplash * psplash)
{
	m_zr.Init(psplash);
	m_zf.Init(hwnd, procOS);
	LockCursorToWindow(hwnd);
	m_zv.InitFull(&m_zc);
	m_zr.AddFrame(&m_zf);
	m_zf.AddViewport(&m_zv);

	m_zf.SetFullscreenOn();
	m_zf.ReSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	m_zr.AddScene(&m_zs);
	m_zpCamera.AddCamera(&m_zc);
	m_zc.Init(PI/3);

	m_zs.SetSkyOn(&m_zpCamera);
	m_zs.SetSkyFlowOn(0);

	m_zwf.LoadPreset("OCRAExtendedRed");
	//m_zw.Init(C2dRect(0.2f, 0.1f, 0.0f, 0.0f), 10, &m_zwf);
	m_zv.AddWriting(&m_zw);


	m_zf.AddDeviceCursor(&m_zdc);
	m_zf.AddDeviceKeyboard(&m_zdk);
	m_zf.AddDeviceMouse(&m_zdm);
	m_zf.AddDeviceGameController(&m_zdgc);
	m_zdc.Hide();


	m_zgSphere.Init(5.5F, NULL);
	m_zpSphere.AddGeo(&m_zgSphere);
	m_zs.AddPlacement(&m_zpSphere);
	m_zpSphere.TranslateZ(-20);

	m_zgSphere2.Init(50.0f, NULL);
	m_zpSphere2.AddGeo(&m_zgSphere2);
	m_zs.AddPlacement(&m_zpSphere2);
	m_zpSphere2.TranslateZ(-50);

	m_pzgTerrain = m_TerrainFile.LoadGeoTriangleTable("models\\Terrain.obj", true);
	m_zpTerrain.AddGeo(m_pzgTerrain);
	m_zpTerrain.ScaleDelta(50);
	m_zpTerrain.TranslateZDelta(-200);
	m_zs.AddPlacement(&m_zpTerrain);

	m_pzgPlane = m_PlaneFile.LoadGeoTriangleTable("models\\Airplane.obj", true);
	m_zpPlane.AddGeo(m_pzgPlane);
	m_zpPlane.ScaleDelta(1);

	m_zs.AddPlacement(&m_zpPlaneCenter);
	m_zpPlaneCenter.SetTranslationSensitivity(50);
	m_zpPlaneCenter.AddPlacement(&m_zpCameraPivot);
	m_zpCameraPivot.AddPlacement(&m_zpCamera);
	m_zpPlaneCenter.AddPlacement(&m_zpPlane);
	m_zpPlane.AddPlacement(&m_zpPlaneTip);

	m_zpPlaneTip.TranslateZDelta(-2);
	m_zpPlaneTip.TranslateYDelta(-0.4f);
	m_zpCamera.TranslateZDelta(40);
	m_zpCamera.TranslateYDelta(4);
	m_zpCamera.RotateXDelta(-PI/12);
	m_zCrosshairRect.Init(crosshairSize * 9, crosshairSize * 16,0,0);
	m_zoCrosshair.Init("textures\\crosshair.jpg", m_zCrosshairRect,true);
	m_zv.AddOverlay(&m_zoCrosshair);
	CenterSquare(0.5, 0.5, crosshairSize, m_zoCrosshair);
	m_zoCirclehair.Init("textures\\circlehair.jpg", m_zCrosshairRect, true);
	m_zv.AddOverlay(&m_zoCirclehair);

	m_zgBullet.Init(0.3, NULL);
	m_zpBulletTemplate.AddGeo(&m_zgBullet);
	m_zpBulletTemplate.SetPhysics(2.0f, 0.1f, 0.0f, 200000.1f, true);
	m_zpBulletTemplate.SwitchOff();
	m_zs.AddPlacement(&m_zpBulletTemplate);
	m_zpsBullets.RingMake(500, m_zpBulletTemplate);
	m_zs.AddPlacements(m_zpsBullets);
}

float CGame::ClampValue(float value, float minValue, float maxValue) {
	return max(minValue, min(value, maxValue));
}

void CGame::CenterSquare(float x, float y, float size, COverlay& rect) {
	float halfSize = size / 2.0f;
	rect.SetPos(x-(halfSize*9),y-(halfSize*16));
}

void CGame::PlaneSteering(float& x, float& y, float fTimeDelta)
{
	//GetMausbewegung
	x += m_zdm.GetRelativeX();
	y -= m_zdm.GetRelativeY();
	//GetControllerInput und rechne Stickdrift weg
	float controllerXInput = m_zdgc.GetRelativeX() * controllerSensitivity*fTimeDelta;
	if (abs(controllerXInput) < 0.0005f)
		controllerXInput = 0;
	float controllerYInput = m_zdgc.GetRelativeY() * controllerSensitivity*fTimeDelta;
	if (abs(controllerYInput) < 0.0005f)
		controllerYInput = 0;
	x += controllerXInput;
	y += controllerYInput;
	//Maximiere Ausschlag des Kreises
	x = ClampValue(x, -0.35, 0.35);
	y = ClampValue(y, -0.35, 0.35);
	//Verschiebe Kreis
	CenterSquare( x+0.5, y+0.5, crosshairSize, m_zoCirclehair);
	CenterSquare( x/7+0.5, y/5+0.5, crosshairSize, m_zoCrosshair);
	//minimiert die maximale Rotation pro Tick
	x_rotation = ClampValue(x, x_rotation - planeRotationSpeed*fTimeDelta, x_rotation + planeRotationSpeed*fTimeDelta);
	y_rotation = ClampValue(y, y_rotation - planeRotationSpeed*fTimeDelta, y_rotation + planeRotationSpeed*fTimeDelta);
	//Rotiert das Flugzeug
	m_zpPlane.RotateY(-x_rotation / 2);
	m_zpPlane.RotateZDelta(-x_rotation * 5);
	m_zpPlane.RotateXDelta(-y_rotation );
	//Rotiert die Kamera
	m_zpCameraPivot.RotateY(-x_rotation);
	m_zpCameraPivot.RotateXDelta(y_rotation / 2);
	
	//Setzt den Kreis langsam in Richtung Mitte des Bildsschirms zurück
	if (x != 0) {
		x -= x * fTimeDelta*1.5;
	}
	if (y != 0) {
		y -= y  * fTimeDelta*1.5;
	}
	//Move Plane and Camera
	float RotationX = x * 15;
	float RotationY = -y * 15;
	float MoveAD = 0, MoveWS = -1, MoveUD = 0;
	m_zpPlaneCenter.Move(fTimeDelta, true, MoveAD, MoveWS, MoveUD, RotationX, RotationY);
}

void CGame::Tick(float fTime, float fTimeDelta)
{
	PlaneSteering(x_initial, y_initial, fTimeDelta);

	if (m_zdm.ButtonPressedLeft()) {
		// … und Wenn noch Platz im Ringpuffer ist:
		if (m_zpsBullets.RingIsNotFull()) {
			// dann erwecke die nächste schlafende Feuerkugel:
			CPlacement* pzp = m_zpsBullets.RingInc();
			// Kopiere die Matrix (und damit die Startposition)
			// des Terrainplacements in die Feuerkugel
			pzp->SetMat(m_zpPlaneTip.GetMatGlobal());
			// Und schieß die Kugel mit einer Zufallsrichtung
			// nach oben:
			CHVector v;
			v.Copy(m_zpCamera.GetDirectionGlobal());
			pzp->SetPhysicsVelocity(v * 600.0f);
		}
	}
	// Wenn die Feuerkugel weiter als 1km weg ist,
	CPlacement* pzpOldestBullet = m_zpsBullets.RingAskLast();
	if (pzpOldestBullet)
		if (pzpOldestBullet->GetPos().Length() > 1000.0f)
			// dann überführe sie in das passive Reich
			// der Schläfer:
			m_zpsBullets.RingDec();



	//m_zdk.PlaceWASD(m_zpCamera, fTimeDelta);

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


