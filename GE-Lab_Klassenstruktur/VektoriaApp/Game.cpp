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

void CGame::Init(HWND hwnd, void(*procOS)(HWND hwnd, unsigned int uWndFlags), CSplash* psplash)
{
	m_zr.Init(psplash);
	m_zf.Init(hwnd, procOS);
	m_player.InitCam();
	LockCursorToWindow(hwnd);

	//m_zf.SetFullscreenOn();
	//m_zf.ReSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	m_zr.AddFrame(&m_zf);
	m_zr.AddScene(&m_zs);
	m_zf.AddViewport(m_player.GetViewport());
	m_zf.AddDeviceMouse(m_player.GetMouse());
	m_zf.AddDeviceKeyboard(m_player.GetKeyboard());

	m_zs.SetSkyOn(m_player.GetCameraPlacement());
	m_zs.SetSkyFlowOn(0);

	m_player.Init();
	m_zs.AddPlacement(&m_player);
	m_zs.AddPlacements(*m_player.GetAirplane()->GetBulletManager()->GetBullets());

	m_terrain.Init();
	m_zs.AddPlacement(&m_terrain);

	m_turret.Init(m_player.GetAirplane());
	m_zs.AddPlacement(&m_turret);
	m_zs.AddPlacements(*m_turret.GetBulletManager()->GetBullets());

	m_zgsTerrainCollision.Add(m_terrain.GetTerrainGeo());
	m_zgsTerrainCollision.Add(m_terrain.GetTerrainWater());
	m_player.GetAirplane()->m_zgsCollisionObjects = m_zgsTerrainCollision;
	m_player.GetAirplane()->GetBulletManager()->m_collisionTargets.Add(&m_turret);
}

void CGame::Tick(float fTime, float fTimeDelta)
{
	//-------------------------------------------
	// Veränderungen während der Simulationszeit:
	//-------------------------------------------

	// Hier kommen die Veränderungen pro Renderschritt hinein: 
	m_player.Tick(fTime, fTimeDelta);
	m_turret.Tick(fTime, fTimeDelta);


	//m_turrets.Tick(fTime, fTimeDelta);

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


