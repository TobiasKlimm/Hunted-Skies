#include "Game.h"
#include "Player.h"
#define MAX_ZOOMIN	10
#define MAX_ZOOMOUT 20

void CPlayer::InitCam() {

	// Initialize main viewport
	m_zcCamera.Init(PI / 3, 1.0f);
	m_zpCamera.AddCamera(&m_zcCamera);
	m_zpCamera.TranslateZDelta(40);
	m_zpCamera.TranslateYDelta(4);
	m_zpCamera.RotateXDelta(-PI / 12);

	// Backfacing Cam
	m_zpCameraBack.RotateYDelta(PI);
	m_zpCameraBack.TranslateZDelta(-40);
	m_zpCameraBack.TranslateYDelta(2);
	m_zpCameraBack.RotateXDelta(PI / 50);

	m_zv.InitFull(&m_zcCamera);

	// Initialize Minimap
	m_zcMinimap.InitOrtho(500.0f);
	m_zpMinimap.AddCamera(&m_zcMinimap);
	this->AddPlacement(&m_zpMinimap);
	m_zvMinimap.Init(&m_zcMinimap, MINIMAPSIZE * 9, MINIMAPSIZE * 16, 0.81f, 0.01f);
	m_zvMinimap.SwitchOff();
}

void CPlayer::Init(CGame* pgame)
{
	m_pgame = pgame; 
	AddPlacement(&m_airplane);

	// Runway alignment aircraft carrier
	m_airplane.RotateY(HALFPI);
	m_airplane.TranslateDelta(2000, 25, -1000);
	m_airplane.Init(DAMAGE,0);
	m_airplane.AddPlacement(&m_zpCamera);
	m_airplane.AddPlacement(&m_zpCameraBack);
	m_zCrosshairRect.Init(CROSSHAIRSIZE * 9, CROSSHAIRSIZE * 16, 0, 0);
	m_zoCrosshair.Init("textures\\crosshair.png", m_zCrosshairRect, true);
	m_zv.AddOverlay(&m_zoCrosshair);
	CenterSquare(0.5, 0.5, CROSSHAIRSIZE, m_zoCrosshair);
	m_zoCirclehair.Init("textures\\circlehair.png", m_zCrosshairRect, true);
	m_zv.AddOverlay(&m_zoCirclehair);

	// Load highscore from file
	m_scoreFile.open("highscore.txt",std::ios::in);
	if (m_scoreFile.is_open()) {
		m_scoreFile >> m_highscore;
		m_scoreFile.close();
		LogDebug("Highscore Loaded: %d", m_highscore);
	}

	// Overlay
	m_zwf.LoadPreset("RodWhite");
	m_zwf.SetTransparencyKind(eTransparencyKind_BinaryByChromaKey);
	m_zwScore.Init(C2dRect(0.1f, 0.04f, 0.0f, 0.0f), 11, &m_zwf);
	m_zv.AddWriting(&m_zwScore);

	m_zw2.Init(C2dRect(0.1f, 0.04f, 0.0f, 0.05f), 11, &m_zwf);
	m_zv.AddWriting(&m_zw2);

	m_zw3.Init(C2dRect(0.1f, 0.04f, 0.0f, 0.1f), 11, &m_zwf);
	m_zv.AddWriting(&m_zw3);
	
	// Fuel Overlay
	m_zwFuel.Init(C2dRect(0.1f, 0.04f, 0.0f, 0.15f), 11, &m_zwf);
	m_zv.AddWriting(&m_zwFuel);

	m_zwHighScore.Init(C2dRect(0.25f, 0.1f, 0.0f, 0.0f), 14, &m_zwf);
	m_zv.AddWriting(&m_zwHighScore);
	m_zwHighScore.PrintF("Highscore: %d", m_highscore);

	//-----------------
	// Startscreen:
	//-----------------
	// Start screen Inits and overlay ads overlay with switchoff
	m_ziStart.Init("textures\\startbildschrim.png");
	m_zoStart.InitFull(&m_ziStart);
	m_zoStart.SetLayer(0.9f);
	m_zv.AddOverlay(&m_zoStart);

	// Start and PlaneSelection button
	m_zoButtonStart.Init("textures\\START.png", C2dRect(0.3f, 0.2f, 0.1f, 0.7f), true);
	m_zoStart.AddOverlay(&m_zoButtonStart);
	m_zoButtonStart.SetLayer(0.3f);
	m_zosStart.Add(&m_zoButtonStart);

	m_zoButtonPlaneSelection.Init("textures\\SELECTION.png", C2dRect(0.3f, 0.2f, 0.6f, 0.7f), true);
	m_zoStart.AddOverlay(&m_zoButtonPlaneSelection);
	m_zoButtonPlaneSelection.SetLayer(0.4f);
	m_zosStart.Add(&m_zoButtonPlaneSelection);


	m_zoControllerButton.Init("textures\\ControllerButton.png", C2dRect(0.10f, 0.10f, 0.45f, 0.75f), true);
	m_zoStart.AddOverlay(&m_zoControllerButton);
	m_zoButtonPlaneSelection.SetLayer(0.5f);
	m_zosStart.Add(&m_zoControllerButton);

	m_zoControllerMainMenu.Init(&m_ziController, C2dRect(0.75f, 0.75, 0.125f, 0.0f), true);
	m_zv.AddOverlay(&m_zoControllerMainMenu);
	m_zoControllerMainMenu.SetLayer(0.6);
	m_zoControllerMainMenu.SwitchOff();
	



	// ----------------
	// Pausescreen:
	// ----------------
	m_ziPause.Init("textures\\Pause.jpg");
	m_zoPause.InitFull(&m_ziPause);
	m_zoPause.SetLayer(0.9f);
	m_zv.AddOverlay(&m_zoPause);
	m_zoPause.SwitchOff();

	// Buttons Resume and BacktoMainMenu
	m_zoButtonGoOn.Init("textures\\RESUME.png", C2dRect(0.3f, 0.2f, 0.1f, 0.7f), true);
	m_zoPause.AddOverlay(&m_zoButtonGoOn);
	m_zoButtonGoOn.SetLayer(0.3f);
	
	m_zoBack2Start.Init("textures\\MAINMENU.png", C2dRect(0.3f, 0.2f, 0.6f, 0.7f), true);
	m_zoPause.AddOverlay(&m_zoBack2Start);
	m_zoBack2Start.SetLayer(0.1f);

	m_zosPause.Add(&m_zoButtonGoOn);
	m_zosPause.Add(&m_zoBack2Start);

	// ----------------
	// Plane Selection: 
	// ----------------
	m_zoPlaneSelection.SetLayer(0.9);
	m_zoPlaneSelection.SwitchOff();
	m_zv.AddOverlay(&m_zoPlaneSelection);
	
	m_zPlaneselecion.Init(&m_zv, &m_zdc);
	AddPlacement(&m_zPlaneselecion);

	m_zoNext.Init("textures\\NEXT.png", C2dRect(0.3f, 0.2f, 0.1f, 0.1f), false);
	m_zoNext.SetLayer(0.3f);
	m_zosPlaneSelection.Add(&m_zoNext);
	m_zoPlaneSelection.AddOverlay(&m_zoNext);

	m_zoPrev.Init("textures\\PREV.png", C2dRect(0.3f, 0.2f, 0.1f, 0.4f), false);
	m_zoPrev.SetLayer(0.3f);
	m_zosPlaneSelection.Add(&m_zoPrev);
	m_zoPlaneSelection.AddOverlay(&m_zoPrev);

	m_zoBack.Init("textures\\MAINMENU.png", C2dRect(0.3f, 0.2f, 0.1f, 0.7f), false);
	m_zoBack.SetLayer(0.3f);
	m_zosPlaneSelection.Add(&m_zoBack);
	m_zoPlaneSelection.AddOverlay(&m_zoBack);

	m_zosPlaneSelection.Add(&m_zoPrev);
	m_zosPlaneSelection.Add(&m_zoNext);

	AddPlacement(&m_zpPlaneSelection);
	m_zpPlaneSelection.RotateY(HALFPI);
	m_zpPlaneSelection.TranslateDelta(2004.553711, 117.965439, -1515.466431);
	m_zpPlaneSelection.AddPlacement(&m_zpPlane2Select);

	for (int i = 0; i < 7; i++)
	{
		m_zpModel[i].Init(i);
		m_zpPlane2Select.AddPlacement(&m_zpModel[i]);
	}
	m_zpPlane2Select.SwitchOff();

	// --------------------
	// Options-Screen: 
	// --------------------
	m_zoGameOver.InitFull("textures\\YOUVE DIED.jpeg");
	m_zoGameOver.SetLayer(0.9);
	m_zoGameOver.SwitchOff();
	m_zv.AddOverlay(&m_zoGameOver);

	m_zoRestart.Init("textures\\MAINMENU.png", C2dRect(0.3f, 0.2f, 0.35f, 0.7f), true);
	m_zoGameOver.AddOverlay(&m_zoRestart);
	m_zoRestart.SetLayer(0.3f);
	m_zosGameOver.Add(&m_zoRestart);

	// Bloodscreen
	m_zi.Init("textures\\BloodOverlay.png");
	m_zo.Init(&m_zi, C2dRect(1.0f, 1.0f, 0.0f, 0.0f), false);
	m_zo.SetTransparency(1.0);
	m_zv.AddOverlay(&m_zo);

	// Fog in the distance
	m_zv.SetMistOn();
	m_zv.SetMistStartDistance(1000);
	m_zv.SetMistStrength(0.0002);
	m_zv.SetMistHeightMax(500);
	m_zv.SetMistHeightMin(-500);

	// You have died Screen
	m_ziDied.Init("textures\\YOUVE DIED.jpeg");
	m_zoDied.Init(&m_ziDied, C2dRect(1.0f, 1.0f, 0.0f, 0.0f), false);
	m_zv.AddOverlay(&m_zoDied);
	m_zoDied.SwitchOff();

	// Distance Warnung
	m_zhvAbstand.Init(0.0, 0.0, 0.0, 1.0);
	m_ziAbstand.Init("textures\\Warning.png");
	m_zoAbstand.Init(&m_ziAbstand, C2dRect(1.0f, 1.0f, 0.0f, 0.6f), false);
	m_zoAbstand.SetTransparency(0.5f);
	m_zoAbstand.SwitchOff();
	m_zv.AddOverlay(&m_zoAbstand);

	// Cursor 
	m_ziFlugzeugCursor.Init("textures\\FlugzeugCursor.png");
	m_ziEasterCursor.Init("textures\\bombe.png");
	m_ziPickingCursor.Init("textures\\explosion.png");

	m_zoFlugzeugCursor.Init(&m_ziFlugzeugCursor, C2dRect(0.04f, 0.04f), true);
	m_zoPickingCursor.Init(&m_ziPickingCursor, C2dRect(0.04f, 0.04f), true);
	m_zoEasterCursor.Init(&m_ziEasterCursor, C2dRect(0.04f, 0.04f), true);

	m_zoFlugzeugCursor.SetLayer(0.05f);
	m_zoPickingCursor.SetLayer(0.05f);
	m_zoEasterCursor.SetLayer(0.05f);


	//Controller Bild
	m_ziController.Init("textures\\PS5controller.png");
	m_zoController.Init(&m_ziController,C2dRect(0.25f,0.25,0.0f,0.5f),true);
	m_zv.AddOverlay(&m_zoController);
	m_zoController.SetLayer(0.4);
	m_zoController.SetTransparency(0.0f);
}

void CPlayer::Tick(float fTime, float fTimeDelta)
{
	if (m_zeStatus == eStart)
	{
		m_zwHighScore.SwitchOn();
		m_zwScore.SwitchOff();
		m_zw2.SwitchOff();
		m_zw3.SwitchOff();
		m_zwFuel.SwitchOff();
		m_zo.SwitchOff();
		m_zoPause.SwitchOff();
		m_zoStart.SwitchOn();
		m_zoPlaneSelection.SwitchOff();
		m_zoGameOver.SwitchOff();
		m_zvMinimap.SwitchOff();
		m_zoCrosshair.SwitchOff();
		m_zoCirclehair.SwitchOff();
		m_zpPlane2Select.SwitchOff();
		m_zoControllerButton.SwitchOn();
		m_zoController.SwitchOff();
		m_zoControllerMainMenu.SwitchOff();
		//m_zoControllerMainMenu.SwitchOff();


		if (m_zdc.ButtonDownLeft())
		{
			COverlay* pzoPicked = m_zdc.PickOverlayPreselected(m_zosStart);
			if (pzoPicked == &m_zoButtonStart)
			{
				m_zvMinimap.SwitchOn();
				m_zdc.Hide();
				m_zwHighScore.SwitchOff();
				m_fuel = 100;
				m_score = 0;
				m_airplane.RotateY(HALFPI);
				m_airplane.TranslateDelta(2000, 55, -1000);
				m_zeStatusLast = m_zeStatus;
				m_airplane.ReInit(m_iFlugGeo);
				m_airplane.StartSounds();

				m_zeStatus = eInGame;
			}
			if (pzoPicked == &m_zoButtonPlaneSelection)
			{
				m_zdc.Show();
				m_zeStatusLast = m_zeStatus;

				m_zeStatus = eSelection;
				m_zpCamera.SubCamera(&m_zcCamera);
				m_zpPlaneSelection.AddCamera(&m_zcCamera);
			}
		}
		if (m_zdc.ButtonPressedLeft())
		{
			COverlay* pzoPicked = m_zdc.PickOverlayPreselected(m_zosStart);
			if (pzoPicked == &m_zoControllerButton)
			{
				m_zoControllerMainMenu.SwitchOn();
			}
		}
		else if (m_zdc.ButtonUpLeft())
		{
			m_zoControllerMainMenu.SwitchOff();
		}
	}
	else if (m_zeStatus == ePaused)
	{
		m_zoPause.SwitchOn();
		m_zoStart.SwitchOff();
		m_zoPlaneSelection.SwitchOff();
		m_zoGameOver.SwitchOff();
		m_zvMinimap.SwitchOff();
		m_zoCrosshair.SwitchOff();
		m_zoCirclehair.SwitchOff();
		m_zpPlane2Select.SwitchOff();
		m_zoController.SwitchOff();
		m_zoControllerButton.SwitchOff();
		m_zoControllerMainMenu.SwitchOff();

		if (m_zdc.ButtonDownLeft())
		{
			COverlay* pzoPicked = m_zdc.PickOverlayPreselected(m_zosPause);
			if (pzoPicked == &m_zoBack2Start)
			{
				m_zeStatusLast = m_zeStatus;
				m_zeStatus = eStart;
			}

			if (pzoPicked == &m_zoButtonGoOn)
			{
				m_airplane.StartSounds();
				m_zeStatus = eInGame;
			}
		}
	}
	else if (m_zeStatus == eSelection)
	{
		if (m_zeStatusLast != eSelection)
		{
			m_airplane.SelectSounds(m_iFlugGeo);
			m_zeStatusLast = eSelection; 
		}
		m_zoPause.SwitchOff();
		m_zoStart.SwitchOff();
		m_zoPlaneSelection.SwitchOn();
		m_zoGameOver.SwitchOff();
		m_zvMinimap.SwitchOff();
		m_zoCrosshair.SwitchOff();
		m_zoCirclehair.SwitchOff();
		m_zoControllerButton.SwitchOff();
		m_zoControllerMainMenu.SwitchOff();
		m_zoController.SwitchOff();
		m_zpPlane2Select.SwitchOn();

		for (int i = 0; i < 7; i++)
		{
			m_zpModel[i].SwitchOff();
			if(m_iFlugGeo == i)
				m_zpModel[i].SwitchOn();
		}
		m_zpPlane2Select.RotateY(fTime);
		m_zpPlane2Select.RotateZDelta(.3);
		m_zpPlane2Select.TranslateDelta(10.0f, 0, -35.0f);

		if (m_zdc.ButtonDownLeft())
		{
			COverlay* pzoPicked = m_zdc.PickOverlayPreselected(m_zosPlaneSelection);
			if (pzoPicked == &m_zoNext)
			{
				m_iFlugGeo++;
				m_iFlugGeo %= m_iFlugGeos;
			}
			if (pzoPicked == &m_zoPrev)
			{
				m_iFlugGeo--;
				if (m_iFlugGeo < 0)
					m_iFlugGeo = m_iFlugGeos - 1;
			}
			if (pzoPicked == &m_zoBack)
			{
				m_zpPlane2Select.SwitchOff();
				m_zpPlaneSelection.SubCamera(&m_zcCamera);
				m_zpCamera.AddCamera(&m_zcCamera);

				m_airplane.RotateY(HALFPI);
				m_airplane.TranslateDelta(2000, 55, -1000);
				m_airplane.SetHealth(100);
				m_zeStatusLast = m_zeStatus;

				m_zeStatus = eStart;
			}
			m_airplane.SelectSounds(m_iFlugGeo);
		}
	}
	else if (m_zeStatus == eInGame)
	{
		m_zoControllerMainMenu.SwitchOff();
		m_zoControllerButton.SwitchOff();
		m_zoPickingCursor.SwitchOff();
		m_zoFlugzeugCursor.SwitchOff();
		m_zoEasterCursor.SwitchOff();
		m_zwScore.SwitchOn();
		m_zw2.SwitchOn();
		m_zw3.SwitchOn();
		m_zwFuel.SwitchOn();
		m_zoPause.SwitchOff();
		m_zoStart.SwitchOff();
		m_zoPlaneSelection.SwitchOff();
		m_zoGameOver.SwitchOff();
		m_zvMinimap.SwitchOn();
		m_zoCrosshair.SwitchOn();
		m_zoCirclehair.SwitchOn();

		if (m_zdk.KeyDown(DIK_P) || m_zdgc.ButtonDown(9))
		{
			m_zdc.Show();
			m_zeStatusLast = m_zeStatus;

			m_zeStatus = ePaused;
		}
		
		// Fuel
		bool bHasInitializedRotationY = false;
		if (m_fuel > 0)
		{
			m_fuel -= 2.f * fTimeDelta;
		}
		else // if no fuel
		{
			if (m_airplane.GetFlySpeed() > 40) // if still momentum
			{
				m_airplane.ReduceSpeedWhenOutOfFuel();
			}
			else // if no momentum
			{
				float RotationY;

				if (!bHasInitializedRotationY)
				{
					RotationY = -1.0f; // only the first time
					bHasInitializedRotationY = true;
				}
				else
				{
					RotationY = 0.0f; // from the second time
				}

				float RotationX = 0.0f;
				float MoveAD = 0.0f;
				float MoveWS = 0.0f;
				float MoveUD = -2.5f;

				m_airplane.MoveWithCollisionDetection(fTimeDelta, true, MoveAD, MoveWS, MoveUD, RotationX, RotationY, m_airplane.GetCollisionObjects());

				LogDebug("Mayday");
			}
		}
		m_zwFuel.PrintF("Fuel: %f", m_fuel);

		float airplaneHealth = m_airplane.GetHealth();

		// Score System
		if (m_airplane.GetBulletManager()->GetKilledEnemy()) {
			m_score++;
			m_fuel += 10;
		}

		m_zwScore.PrintF("Score: %d", m_score);
		m_zw2.PrintF("Health: %f", airplaneHealth);
		m_zw3.PrintF("Speed: %f", m_airplane.GetFlySpeed());

		// Backfacing Cam
		if (m_zdk.KeyUp(DIK_LSHIFT) || !m_zdgc.ButtonPressed(0)) {
			m_zpCameraBack.SubCamera(&m_zcCamera);
			m_zpCamera.AddCamera(&m_zcCamera);
		}
		if (m_zdk.KeyPressed(DIK_LSHIFT) || m_zdgc.ButtonDown(0)) {
			m_zpCamera.SubCamera(&m_zcCamera);
			m_zpCameraBack.AddCamera(&m_zcCamera);
		}

		//--------------------
		// Airplane Zoom
		//--------------------
		if (m_zdc.ButtonPressedRight() || m_zdgc.ButtonPressed(6))
		{
			m_zoom += fTimeDelta * MAX_ZOOMIN;
			if (m_zoom > 5.0f)
			{
				m_zoom = 5.0f;
			}
			m_zcCamera.SetFov((PI / 3) / m_zoom);
		}
		else if (!m_zdc.ButtonPressedRight() || !m_zdgc.ButtonPressed(6))
		{
			m_zoom -= fTimeDelta * MAX_ZOOMOUT;
			if (m_zoom < 1.0f)
			{
				m_zcCamera.SetFov(PI / 3);
				m_zoom = 1.0f;
			}
			else
				m_zcCamera.SetFov((PI / 3) / m_zoom);
		}

		if (m_zdk.KeyPressed(DIK_E) || m_zdgc.ButtonPressed(8))
		{
			m_zoController.SwitchOn();
			
		}
		else
		{
			m_zoController.SwitchOff();
		}

		// Minimap Movement
		CHVector vPlane(m_airplane.GetPosGlobal());
		vPlane.y += 100;
		m_zpMinimap.RotateX(-HALFPI);
		m_zpMinimap.TranslateDelta(vPlane);

		m_airplane.Tick(fTime, fTimeDelta);
		ControlPlane(fTimeDelta);

		// Blood Overlay if player was hit
		if (m_lastHealth != airplaneHealth) {
			m_zo.SwitchOn();
			float transparency = airplaneHealth / 100;
			m_zo.SetTransparency(transparency);
			m_bloodTransparency = transparency;
		}

		if (m_bloodTransparency != 1.0) {
			m_bloodTransparency = ClampValue(m_bloodTransparency + 0.45 * fTimeDelta, 0.0f, 1.0f);
			m_zo.SetTransparency(m_bloodTransparency);
			if (m_bloodTransparency == 1.0)
				m_zo.SwitchOff();
		}

		m_lastHealth = airplaneHealth;

		// DIED
		if (airplaneHealth == 0) {
			// Highscore
			m_scoreFile.open("highscore.txt", std::ios::out);
			if (m_scoreFile.is_open() && m_score > m_highscore) {
				m_scoreFile << m_score;
				m_highscore = m_score;
				m_zwHighScore.PrintF("Highscore: %d", m_score);
				LogDebug("Highscore Broken! NewHighscore: %d", m_score);
			}
			m_scoreFile.close();
			m_score = 0;

			m_airplane.RotateY(HALFPI);
			m_airplane.TranslateDelta(2000, 55, -1000);
			m_airplane.SetHealth(100);

			m_zeStatusLast = m_zeStatus;

			m_zeStatus = eGameOver;
			m_zdc.Show();
		}

		//-------------------------------------
		// Distance Warnung if to far away
		//-------------------------------------
		CHVector PlayerPos = m_airplane.GetPosGlobal();
		if (PlayerPos.Length()> MAX_DISTANCE) {
			m_zo.SwitchOff();
			m_zoAbstand.SwitchOn();
			m_WarningTransparency = ClampValue(m_WarningTransparency - 0.25 * fTimeDelta, 0.0f, 1.0f);
			m_zoAbstand.SetTransparency(m_WarningTransparency);
		}
		else if(m_WarningTransparency != 1.0) {
			m_WarningTransparency = ClampValue(m_WarningTransparency + 0.5 * fTimeDelta, 0.0f, 1.0f);
			m_zoAbstand.SetTransparency(m_WarningTransparency);
			if (m_WarningTransparency == 1.0) {
				m_zoAbstand.SwitchOff();
				m_zo.SwitchOn();
			}
		}

		// Collision for objekts
		if (m_lastPos == PlayerPos)
			m_airplane.RegisterHit(10);
		m_lastPos = PlayerPos;
		if (m_zeStatusLast != eInGame)
		{
			m_airplane.m_bFirstMove = true; 
			m_airplane.RotateY(HALFPI);
			m_airplane.TranslateDelta(2000, 55, -1000);
			LogDebug("%f,%f,%f", m_lastPos.x, m_lastPos.y, m_lastPos.z);
			m_fuel = 100;
			m_zeStatusLast = eInGame;
		}
	}
	else if (m_zeStatus == eGameOver)
	{
		m_zoController.SwitchOff();
		m_zo.SwitchOff();
		m_zoAbstand.SwitchOff();
		m_zwScore.SwitchOff();
		m_zw2.SwitchOff();
		m_zw3.SwitchOff();
		m_zwFuel.SwitchOff();
		m_zoGameOver.SwitchOn();
		m_zvMinimap.SwitchOff();
		m_zoCrosshair.SwitchOff();
		m_zoCirclehair.SwitchOff();
		if (m_zdc.ButtonDownLeft())
		{
			COverlay* pzoPicked = m_zdc.PickOverlayPreselected(m_zosGameOver);
			if (pzoPicked == &m_zoRestart)
			{
				m_airplane.SwitchOn();
				m_airplane.RotateY(HALFPI);
				m_airplane.TranslateDelta(2000, 55, -1000);
				m_zeStatusLast = m_zeStatus;
				m_zeStatus = eStart;
			}
		}
	}

	// Change cursor
	if (m_zeStatus != eInGame)
	{
		if(m_zeStatus != eSelection)
			m_airplane.StopSounds();
		m_zv.AddOverlay(&m_zoPickingCursor);
		m_zv.AddOverlay(&m_zoFlugzeugCursor);
		m_zv.AddOverlay(&m_zoEasterCursor);
		float fx, fy;
		m_zdc.GetFractional(fx, fy,true);
		if (m_zdc.ButtonPressedRight())
		{
			m_zoEasterCursor.SwitchOn();
			m_zoPickingCursor.SwitchOff();
			m_zoFlugzeugCursor.SwitchOff();
			m_zoEasterCursor.SetPos(fx, fy);
		}
		else if (m_zdc.ButtonPressedLeft())
		{
			m_zoEasterCursor.SwitchOff();
			m_zoPickingCursor.SwitchOn();
			m_zoFlugzeugCursor.SwitchOff();
			m_zoPickingCursor.SetPos(fx, fy);
		}
		else
		{
			m_zoEasterCursor.SwitchOff();
			m_zoPickingCursor.SwitchOff();
			m_zoFlugzeugCursor.SwitchOn();
			m_zoFlugzeugCursor.SetPos(fx, fy);
		}
	}
}

void CPlayer::ControlPlane(float fTimeDelta) {
	float controllerXInput = m_zdgc.GetRelativeX() / CONTROLLER_SENSITIVITY;
	if (abs(controllerXInput) < 0.0004f)
		controllerXInput = 0;
	float controllerYInput = m_zdgc.GetRelativeY() / CONTROLLER_SENSITIVITY;
	if (abs(controllerYInput) < 0.0004f)
		controllerYInput = 0;
	x += controllerXInput;
	y += controllerYInput;

	// GetCursorMovement
	x += m_zdm.GetRelativeX();
	y -= m_zdm.GetRelativeY();

	// Maximize deflection of the circle
	x = ClampValue(x, -0.35f, 0.35f);
	y = ClampValue(y, -0.35f, 0.35f);

	// Move circle and the crosshair
	CenterSquare(x + 0.5f, y + 0.5f, CROSSHAIRSIZE, m_zoCirclehair);
	CenterSquare(-x / 5 + 0.5f, -y / 5 + 0.5f, CROSSHAIRSIZE, m_zoCrosshair);

	// Resets x and y (and thus the circle slowly towards the center of the screen) to 0
	if (x != 0) {
		x -= x * fTimeDelta * 1.5f;
	}
	if (y != 0) {
		y -= y * fTimeDelta * 1.5f;
	}
	m_airplane.MovePlane(x, y, fTimeDelta);

	if (m_fuel > 0)
	{
		if (m_zdk.KeyPressed(DIK_W) || m_zdgc.ButtonPressed(5))
			m_airplane.SetSpeed(fTimeDelta);
		if (m_zdk.KeyPressed(DIK_S) || m_zdgc.ButtonPressed(4))
			m_airplane.SetSpeed(-fTimeDelta);
	}
	if (m_zdm.ButtonPressedLeft() || m_zdgc.ButtonPressed(7)) {
		m_timePassed += fTimeDelta;

		// Execute the function if enough time has passed
		if (m_timePassed <=  SHOOT_FREQUENCY)
			return;
		m_timePassed = 0.0;
		m_airplane.Shoot(0.001f);
	}
}

void CPlayer::CenterSquare(float x, float y, float size, COverlay& rect) {
	float halfSize = size / 2.0f;
	rect.SetPos(x - (halfSize * 9), y - (halfSize * 16));
}

float CPlayer::ClampValue(float value, float minValue, float maxValue) {
	return max(minValue, min(value, maxValue));
}