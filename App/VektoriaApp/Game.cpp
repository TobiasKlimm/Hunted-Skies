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
	m_zr.Init(psplash, false, false, true);
	m_zf.Init(hwnd, procOS);
	m_player.InitCam();

	/*m_zf.SetFullscreenOn();
	m_zf.ReSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	LockCursorToWindow(hwnd);*/

	CViewport* m_zv = m_player.GetViewport();
	CViewport* m_zvMinimap = m_player.GetViewportMinimap();

	m_zr.AddFrame(&m_zf);
	m_zr.AddScene(&m_zs);
	m_zf.AddViewport(m_zv);
	m_zf.AddViewport(m_zvMinimap);
	m_zf.AddDeviceMouse(m_player.GetMouse());
	m_zf.AddDeviceKeyboard(m_player.GetKeyboard());
	m_zf.AddDeviceCursor(m_player.GetCursor());

	m_zs.SetSkyOn(m_player.GetCameraPlacement());
	m_zs.SetSkyFlowOn(0);

	//Player
	m_player.Init(this);
	m_zs.AddPlacement(&m_player);
	m_zs.AddPlacements(*m_player.GetAirplane()->GetBulletManager()->GetBullets());

	//Terrain
	m_terrain.Init();
	m_zs.AddPlacement(&m_terrain);

	//Terraincollision Airplane
	m_zgsTerrainCollision.Add(m_terrain.GetTerrainGeo());
	m_zgsTerrainCollision.Add(m_terrain.GetTerrainWater());
	m_player.GetAirplane()->m_zgsCollisionObjects = m_zgsTerrainCollision;
	//Terraincollision Bullets Airplane
	m_player.GetAirplane()->GetBulletManager()->SetTerrain(m_terrain.GetTerrainGeo());

	//Turrets
	for (int i = 0; i < 10; i++) {
		m_turrets[i].Init(m_player.GetAirplane());
		m_turrets[i].GetBulletManager()->m_collisionTargets.Add(m_player.GetAirplane());
		m_turrets[i].GetBulletManager()->SetTerrain(m_terrain.GetTerrainGeo());
		m_turrets[i].SwitchOff();
		m_zs.AddPlacement(&m_turrets[i]);
		m_zs.AddPlacements(*m_turrets[i].GetBulletManager()->GetBullets());
		m_player.GetAirplane()->GetBulletManager()->m_collisionTargets.Add(&m_turrets[i]);
	}

	//BotPlanes
	m_botplanes.Init(m_player.GetAirplane());
	m_zs.AddPlacement(&m_botplanes);
	m_zs.AddPlacements(*m_botplanes.GetAirplane()->GetBulletManager()->GetBullets());
	m_botplanes.GetAirplane()->GetBulletManager()->m_collisionTargets.Add(m_player.GetAirplane());
	m_player.GetAirplane()->GetBulletManager()->m_collisionTargets.Add(&m_botplanes);


	
	// Schwarze Kugel für Planeselection initialisieren: 

	m_zmBlackSphere.MakeTextureSky("textures//black_image.jpg");
	m_zs.AddPlacement(&m_zpBlackSphere);
	m_zpBlackSphere.AddGeo(&m_zgBlackSphere);
	m_zpBlackSphere.SetSky();
	m_zgBlackSphere.Init(3000, &m_zmBlackSphere);
	m_zgBlackSphere.Flip();





	


}

void CGame::Tick(float fTime, float fTimeDelta)
{
	m_player.Tick(fTime, fTimeDelta);
	//If Game Paused do not update game objects
	if (m_player.m_zeStatus != eInGame) {
		m_zr.Tick(0);
		return; //<- wois ich net ob des so muss
	}

	//Turet Spawning
	for (int i = 0; i < 10; i++) {
		if (!m_turrets[i].IsOn()) {
			CHVector vrand;
			vrand.RandomDir();
			vrand *= 1000;
			float fHeightTerrain = m_terrain.GetTerrainGeo()->GetHeight(vrand.x, vrand.z);
			LogDebug("Turret Height: %f", fHeightTerrain);

			if (fHeightTerrain > 0.0f) {
				m_turrets[i].SwitchOn();
				vrand.y = fHeightTerrain;
				m_turrets[i].SetPosition(vrand);
			}
		}
		m_turrets[i].Tick(fTime, fTimeDelta, m_player.GetAirplane()->GetDirection(), m_player.GetAirplane()->GetFlySpeed());
	}

	m_botplanes.Tick(fTime, fTimeDelta);

	m_zr.Tick(fTimeDelta);
}

void CGame::Fini()
{
	//-------------------------------
	// Finalisiere die Knotenobjekte:
	//-------------------------------

	// Hier die Finalisierung Deiner Vektoria-Objekte einf�gen:

	// Auch die Root sollte finalisiert werden:   
	m_zr.Fini();
}

void CGame::WindowReSize(int iNewWidth, int iNewHeight)
{
	// Windows ReSize wird immer automatisch aufgerufen, wenn die Fenstergr��e ver�ndert wurde.
	// Hier kannst Du dann die Aufl�sung des Viewports neu einstellen:
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


