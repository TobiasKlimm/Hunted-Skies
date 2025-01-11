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

	m_zvMinimap->StyleRadar();
	m_zvMinimap->SetShadowRenderingOff();
	//m_zvMinimap->SetBrightnessDelta(CColor(0.9,0.9,0.9));
	m_zvMinimap->SetBloomOn();
	m_zvMinimap->SetBloomDepthDecay(10);
	//m_zvMinimap->SetOutliningThreshold(0.5);
	//m_zvMinimap->SetOutliningOn();
	//m_zvMinimap->SetOutliningStrength(100);
	m_zr.AddFrame(&m_zf);
	m_zr.AddScene(&m_zs);
	m_zf.AddViewport(m_zv);
	m_zf.AddViewport(m_zvMinimap);
	m_zf.AddDeviceMouse(m_player.GetMouse());
	m_zf.AddDeviceKeyboard(m_player.GetKeyboard());
	m_zf.AddDeviceCursor(m_player.GetCursor());
	m_zf.AddDeviceGameController(m_player.GetGameController());

	//Carrier
	m_zgCarrier = m_zfCarrier.LoadGeoTriangleTable("models\\Carrier\\source\\untitled.obj",true);
	m_zpCarrier.AddGeo(m_zgCarrier);
	m_zs.AddPlacement(&m_zpCarrier);
	m_zgCarrier->SetMaterial(&m_zmCarrier);
	m_zmCarrier.MakeTextureDiffuse("models\\Carrier\\textures\\Nimitz_Albedo.png");
	m_zmCarrier.MakeTextureBump("models\\Carrier\\textures\\Nimitz_Normal_(1).png");
	m_zmCarrier.MakeTextureSpecular("models\\Carrier\\textures\\Nimitz_Metalness.png");
	m_zpCarrier.ScaleDelta(2);
	m_zpCarrier.RotateYDelta(-HALFPI);
	m_zpCarrier.TranslateDelta(2000,0,-1000);


	//Himmel &Tag Nacht Zyklus
	m_zs.SetSkyOn(m_player.GetCameraPlacement());
	m_zs.SetSkyFlowOn(100);
	//m_zs.m_psceneweather->m_azmSky[0].SetPostprocessingOff();
	//m_zs.m_psceneweather->m_azmSky[1].SetPostprocessingOff();
	//m_zs.m_psceneweather->m_azmSky[2].SetPostprocessingOff();

	//Player
	m_player.Init(this);
	m_zs.AddPlacement(&m_player);
	m_zs.AddPlacements(*m_player.GetAirplane()->GetBulletManager()->GetBullets());

	//Terrain
	m_terrain.Init();
	m_zs.AddPlacement(&m_terrain);

	//Collision Kugel Blender für weite Entfernungen;
	m_zgCollisionKugelBlender = m_zfCollisionKugelBlender.LoadGeoTriangleTable("models\\Collision Ball\\untitled.obj");
	m_zpCollisionKugelBlender.AddGeo(m_zgCollisionKugelBlender);
	m_zs.AddPlacement(&m_zpCollisionKugelBlender);
	m_zpCollisionKugelBlender.Scale(700);
	m_zpCollisionKugelBlender.TranslateYDelta(100);
	m_zgCollisionKugelBlender->Flip();
	m_zpCollisionKugelBlender.SetDrawingOff();

	//Terraincollision Airplane
	m_zgsTerrainCollision.Add(m_terrain.GetTerrainGeo());
	m_zgsTerrainCollision.Add(m_terrain.GetTerrainWater());
	m_zgsTerrainCollision.Add(m_zgCarrier);
	m_zgsTerrainCollision.Add(m_zgCollisionKugelBlender);

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
	m_botplanes.GetAirplane()->GetBulletManager()->m_collisionTargets.Add(m_player.GetAirplane());
	m_botplanes.GetAirplane()->GetBulletManager()->SetTerrain(m_terrain.GetTerrainGeo());
	m_botplanes.GetAirplane()->SetHealth(100);
	m_zs.AddPlacement(&m_botplanes);
	m_zs.AddPlacements(*m_botplanes.GetAirplane()->GetBulletManager()->GetBullets());
	m_player.GetAirplane()->GetBulletManager()->m_collisionTargets.Add(&m_botplanes);

	//MUSIC
	/*m_zaTrackOne.Init("sounds\\TrackOne.wav");
	m_zaTrackOne.SetVolume(1.0F);
	m_zs.AddAudio(&m_zaTrackOne);
	m_zaTrackOne.Loop();*/

	m_zaWingsOfValor.Init("sounds\\WingsOfValor.wav");
	m_zs.AddAudio(&m_zaWingsOfValor);
	m_zaWingsOfValor.Loop();
	m_zaWingsOfValor.SetVolume(0.9F);

	////
	//// Turret Explosion
	////
	m_zmExplosion.MakeTextureBillboard("textures\\explosion_sprite_sheet.png");
	m_zmExplosion.SetChromaKeyingOn();
	m_zmExplosion.SetBot(17, 1);
	

	m_zgExplosion.Init(50.f, 50.f, &m_zmExplosion);
	m_zpExplosion.AddGeo(&m_zgExplosion);
	m_zpExplosion.TranslateY(10.f);

	m_zbpExplosion.AddPlacement(&m_zpExplosion);
	m_zs.AddPlacement(&m_zbpExplosion);
	m_zbpExplosion.SetBillboard();

	
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
			m_zbpExplosion.Translate(m_turrets[i].GetPosGlobal());
			m_zpExplosion.SwitchOn();
			m_zmExplosion.SetPic(0,0);

			CHVector vrand;
			vrand.RandomDir();
			vrand *= 1000;
			float fHeightTerrain = m_terrain.GetTerrainGeo()->GetHeight(vrand.x, vrand.z);
			LogDebug("New Turret spawned at Height: %f", fHeightTerrain);

			if (fHeightTerrain > 0.0f) {
				m_turrets[i].SwitchOn();
				vrand.y = fHeightTerrain;
				m_turrets[i].SetPosition(vrand);
				m_turrets[i].SetHealth(100);
			}
		}
		m_turrets[i].Tick(fTime, fTimeDelta, m_player.GetAirplane()->GetDirection(), m_player.GetAirplane()->GetFlySpeed());
	}

	if (m_zpExplosion.IsOn())
			{
		m_zmExplosion.SetPic(m_iExplosion, 0);
				EndExplosion(fTime);
			}
	m_botplanes.Tick(fTime, fTimeDelta);

	m_zr.Tick(fTimeDelta);
}

void CGame::EndExplosion(float fTime) {
	if (fTime - m_fBotTime > 0.1f)
	{
		m_iExplosion++;
		if (m_iExplosion > 17) {
			m_zpExplosion.SwitchOff();
			m_iExplosion = 0;
		}
		m_fBotTime = fTime; 
	}
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