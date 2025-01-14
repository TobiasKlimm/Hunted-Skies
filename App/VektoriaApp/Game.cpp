#include "Game.h"
#include <random>
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
	
	m_zf.SetFullscreenOn();
	m_zf.ReSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	
	LockCursorToWindow(hwnd);

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
	m_zpCarrier.Scale(2);
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

	//Ships auf der Map um Flugzeugcarrier
	//Ship
	m_zgDestroyer = m_zfDestroyer.LoadGeoTriangleTable("models\\Ships\\destroyerClass\\source\\destroyer.obj",true);
	m_zmDestroyer.MakeTextureDiffuse("models\\Ships\\destroyerClass\\textures\\body.png");
	m_zgDestroyer->SetMaterial(&m_zmDestroyer);
	

	for (int i = 0; i < 4; i++)
	{
	CHVector fuer (150, 0, -20-m_destroyerx);
	CHVector nur (-150, 0, -20-m_destroyerx1);//x ist Vom carrier aus weiter vorner oder oder weiter hinten
		m_zpCarrier.AddPlacement(&m_zpDestroyer[i]);
		m_zpDestroyer[i].Scale(13);
		//m_zpDestroyer[i].RotateYDelta();
		m_zpDestroyer[i].AddGeo(m_zgDestroyer);
		if (i % 2 == 0)
		{
			m_zpDestroyer[i].TranslateDelta(fuer);
			m_destroyerx += 200;
			
		}
		else
		{
			m_zpDestroyer[i].TranslateDelta(nur);
			m_destroyerx1 += 200;
		}
	}


	//Random Schiffe auf der Map
	m_zgRandomShip = m_zfRandomShip.LoadGeoTriangleTable("models\\Ships\\destroyerClass\\source\\destroyer.obj", true);
	m_zmRandomShip.MakeTextureDiffuse("models\\Ships\\destroyerClass\\textures\\body.png");
	m_zgRandomShip->SetMaterial(&m_zmRandomShip);

	m_zgRandomShip1 = m_zfRandomShip1.LoadGeoTriangleTable("models\\Ships\\UsShip\\source\\ShipUs.obj", true);
	m_zmRandomShip1.MakeTextureDiffuse("models\\Ships\\UsShip\\textures\\DDG51_s.png");
	m_zgRandomShip1->SetMaterial(&m_zmRandomShip1);

	m_zgRandomShip2= m_zfRandomShip2.LoadGeoTriangleTable("models\\Ships\\Uboot\\UBoat.obj", true);
	m_zmRandomShip2.MakeTextureDiffuse("models\\Ships\\Uboot\\uboat.png");
	m_zgRandomShip2->SetMaterial(&m_zmRandomShip2);


	for (int i = 0; i < MAX_RANDOMSHIPS; i++)
	{
		std::random_device rd; // Liefert einen Seed
		std::mt19937 gen(rd()); // Mersenne Twister Engine
		std::uniform_int_distribution<> range_selector(0, 1); // Bereichsauswahl: 0 oder 1
		std::uniform_int_distribution<> negative_range(-3000, -2000); // Bereich: -3000 bis -2000
		std::uniform_int_distribution<> positive_range(2000, 3000);   // Bereich: 1000 bis 2000
		std::uniform_int_distribution<> ship_select(1, 3);
		// Zwei Zufallszahlen generieren
		int random_number1, random_number2;
		int random_Ship = ship_select(gen);
		// Erste Zufallszahl
		if (range_selector(gen) == 0) 
		{
			random_number1 = negative_range(gen);
		}
		else 
		{
			random_number1 = positive_range(gen);
		}

		// Zweite Zufallszahl
		if (range_selector(gen) == 0) 
		{
			random_number2 = negative_range(gen);
		}
		else 
		{
			random_number2 = positive_range(gen);
		}
		m_zs.AddPlacement(&m_zpRandomShip[i]);
		switch (random_Ship)
		{
			case 1:
			{
				m_zpRandomShip[i].AddGeo(m_zgRandomShip);
				break;
			}
			case 2:
			{
				m_zpRandomShip[i].AddGeo(m_zgRandomShip1);
				break;
			}
			case 3:
			{
				m_zpRandomShip[i].AddGeo(m_zgRandomShip2);
				break;
			}
		}
		m_zpRandomShip[i].Scale(25);
		m_zpRandomShip[i].TranslateDelta(random_number1+m_movefaktor, 0, random_number2+m_movefaktor);
		m_movefaktor += 40;
	}



	//Terraincollision Airplane
	m_zgsTerrainCollision.Add(m_terrain.GetTerrainGeo());
	m_zgsTerrainCollision.Add(m_terrain.GetTerrainWater());
	m_zgsTerrainCollision.Add(m_zgCarrier);
	m_zgsTerrainCollision.Add(m_zgCollisionKugelBlender);
	m_zgsTerrainCollision.Add(m_zgDestroyer);
	m_zgsTerrainCollision.Add(m_zgRandomShip);
	m_zgsTerrainCollision.Add(m_zgRandomShip1);
	m_zgsTerrainCollision.Add(m_zgRandomShip2);

	m_player.GetAirplane()->m_zgsCollisionObjects = m_zgsTerrainCollision;

	//Terraincollision Bullets Airplane
	m_player.GetAirplane()->GetBulletManager()->SetTerrain(m_terrain.GetTerrainGeo());

	//Turrets
	for (int i = 0; i < MAX_TURRETS; i++) {
		m_turrets[i].Init(m_player.GetAirplane());
		m_turrets[i].GetBulletManager()->m_collisionTargets.Add(m_player.GetAirplane());
		m_turrets[i].GetBulletManager()->SetTerrain(m_terrain.GetTerrainGeo());
		m_turrets[i].SwitchOff();
		m_zs.AddPlacement(&m_turrets[i]);
		m_zs.AddPlacements(*m_turrets[i].GetBulletManager()->GetBullets());
		m_player.GetAirplane()->GetBulletManager()->m_collisionTargets.Add(&m_turrets[i]);
	}

	//BotPlanes
	for (int i = 0; i < MAX_BOTS; i++) {
		m_botplanes[i].Init(m_player.GetAirplane());
		m_botplanes[i].GetAirplane()->GetBulletManager()->m_collisionTargets.Add(m_player.GetAirplane());
		m_botplanes[i].GetAirplane()->GetBulletManager()->SetTerrain(m_terrain.GetTerrainGeo());
		m_botplanes[i].GetAirplane()->SwitchOff();
		m_zs.AddPlacement(&m_botplanes[i]);
		m_zs.AddPlacements(*m_botplanes[i].GetAirplane()->GetBulletManager()->GetBullets());
		m_player.GetAirplane()->GetBulletManager()->m_collisionTargets.Add(m_botplanes[i].GetAirplane());
	}

	//MUSIC
	/*m_zaTrackOne.Init("sounds\\TrackOne.wav");
	m_zaTrackOne.SetVolume(1.0F);
	m_zs.AddAudio(&m_zaTrackOne);
	m_zaTrackOne.Loop();*/

	m_zaWingsOfValor.Init("sounds\\WingsOfValor.wav");
	m_zs.AddAudio(&m_zaWingsOfValor);
	m_zaWingsOfValor.Loop();
	m_zaWingsOfValor.SetVolume(0.75F);

	m_zaExplosion.Init3D("sounds\\ExplosionBlast.wav",1000);
	m_zpExplosion.AddAudio(&m_zaExplosion);
	//m_zaExplosion.SetVolume(0.9F);

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
	float zero = 0;
	float vorwaerts = -1;
	m_player.Tick(fTime, fTimeDelta);
	//If Game Paused do not update game objects
	if (m_player.m_zeStatus != eInGame) {
		//m_zpCarrier.Scale(2);
		//m_zpCarrier.RotateYDelta(-HALFPI);
		//m_zpCarrier.TranslateDelta(2000, 0, -1000);
		//m_zpCarrier.Move(fTimeDelta, true, zero, zero, vorwaerts, zero, zero);
		m_zr.Tick(0);
		return; //<- wois ich net ob des so muss
	}
	//m_zpCarrier.SetTranslationSensitivity(20);
	//m_zpCarrier.Move(fTimeDelta, true, zero, vorwaerts, zero, zero, zero);

	//Turet Spawning
	for (int i = 0; i < MAX_TURRETS; i++) {
		if (!m_turrets[i].IsOn()) {

			m_zbpExplosion.Translate(m_turrets[i].GetPosGlobal());
			m_zpExplosion.SwitchOn();
			m_bTurretDestroyed = true;
			m_zmExplosion.SetPic(0,0);

			CHVector vrand;
			vrand.RandomDir();
			vrand *= 1000;
			float fHeightTerrain = m_terrain.GetTerrainGeo()->GetHeight(vrand.x, vrand.z);
			LogDebug("New Turret spawned at Height: %f", fHeightTerrain);

			if (fHeightTerrain > 0.0f) {
				vrand.y = fHeightTerrain;
				m_turrets[i].SwitchOn();
				m_turrets[i].SetPosition(vrand);
				m_turrets[i].SetHealth(100);
			}
		}
		m_turrets[i].Tick(fTime, fTimeDelta, m_player.GetAirplane()->GetDirection(), m_player.GetAirplane()->GetFlySpeed());
	}

	for (int i = 0; i < MAX_BOTS; i++) {
		if (!m_botplanes[i].GetAirplane()->IsOn()) {
			m_zbpExplosion.Translate(m_botplanes[i].GetAirplane()->GetPosGlobal());
			m_zpExplosion.SwitchOn();
			m_bTurretDestroyed = true;
			m_zmExplosion.SetPic(0, 0);

			m_botplanes[i].GetAirplane()->m_bFirstMove = true;

			CHVector vrand2;
			vrand2.RandomDir();
			vrand2 *= 1000;
			vrand2.y = 150;
			LogDebug("New Bot spawned at %f %f %f",vrand2.x,vrand2.y,vrand2.z);

			m_botplanes[i].GetAirplane()->SwitchOn();
			m_botplanes[i].GetAirplane()->Translate(vrand2);
			m_botplanes[i].GetAirplane()->SetHealth(100);
		}
		else
			m_botplanes[i].Tick(fTime, fTimeDelta);
	}

	if (m_zpExplosion.IsOn())
	{
		m_zmExplosion.SetPic(m_iExplosion, 0);
		EndExplosion(fTime);
	}

	m_zr.Tick(fTimeDelta);
}

void CGame::EndExplosion(float fTime) {
	if (fTime - m_fBotTime > 0.1f)
	{
		if (m_bTurretDestroyed == true) {
			m_iExplosion = 0;
			m_zaExplosion.Start();
		}
		m_iExplosion++;
		if (m_iExplosion > 17) {
			m_zpExplosion.SwitchOff();
			m_iExplosion = 0;
		}
		m_bTurretDestroyed = false;
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