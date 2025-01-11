#include "Game.h"
#include "Player.h"
#define MAX_ZOOMIN	10;
#define MAX_ZOOMOUT 20;

void CPlayer::InitCam() {
	//Hauptviewport Initialisierung
	m_zcCamera.Init(PI / 3, 1.0f);
	m_zpCamera.AddCamera(&m_zcCamera);
	m_zpCamera.TranslateZDelta(40);
	m_zpCamera.TranslateYDelta(4);
	m_zpCamera.RotateXDelta(-PI / 12);

	//Backfacing Cam
	m_zpCameraBack.RotateYDelta(PI);
	m_zpCameraBack.TranslateZDelta(-40);
	m_zpCameraBack.TranslateYDelta(2);
	m_zpCameraBack.RotateXDelta(PI / 50);

	m_zv.InitFull(&m_zcCamera);

	//Minimap Initialierung
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
	//Ausrichtung Startbahn flugzeugträger
	m_airplane.RotateY(HALFPI);
	m_airplane.TranslateDelta(2000, 25, -1000);
	m_airplane.Init(DAMAGE,1);
	m_airplane.AddPlacement(&m_zpCamera);
	m_airplane.AddPlacement(&m_zpCameraBack);
	m_airplane.SetHealth(100);


	m_zCrosshairRect.Init(CROSSHAIRSIZE * 9, CROSSHAIRSIZE * 16, 0, 0);
	m_zoCrosshair.Init("textures\\crosshair.bmp", m_zCrosshairRect, true);
	m_zv.AddOverlay(&m_zoCrosshair);
	CenterSquare(0.5, 0.5, CROSSHAIRSIZE, m_zoCrosshair);
	m_zoCirclehair.Init("textures\\circlehair.bmp", m_zCrosshairRect, true);
	m_zv.AddOverlay(&m_zoCirclehair);


	//Highscore wird aus File geladen
	m_scoreFile.open("highscore.txt",std::ios::in);
	if (m_scoreFile.is_open()) {
		m_scoreFile >> m_highscore;
		m_scoreFile.close();
		LogDebug("Highscore Loaded: %d", m_highscore);
	}

	//---------------------------------------------------------------------
	//Overlay
	m_zwf.LoadPreset("RodWhite");
	m_zwf.SetTransparencyKind(eTransparencyKind_BinaryByChromaKey);
	m_zwScore.Init(C2dRect(0.1f, 0.04f, 0.0f, 0.0f), 11, &m_zwf);
	m_zv.AddWriting(&m_zwScore);

	m_zw2.Init(C2dRect(0.1f, 0.04f, 0.0f, 0.05f), 11, &m_zwf);
	m_zv.AddWriting(&m_zw2);

	m_zw3.Init(C2dRect(0.1f, 0.04f, 0.0f, 0.1f), 11, &m_zwf);
	m_zv.AddWriting(&m_zw3);

	m_zwHighScore.Init(C2dRect(0.1f, 0.04f, 0.0f, 0.0f), 14, &m_zwf);
	m_zv.AddWriting(&m_zwHighScore);
	m_zwHighScore.PrintF("Highscore: %d", m_highscore);


	//-----------------
	// Startbildschirm:
	//-----------------

	//Startbildschrim Inits und Overlayadds overlay mit switchoff
	m_ziStart.Init("textures\\startbildschrim.png");
	//m_zoStart.InitFull(&m_ziStart);
	m_zoStart.Init("textures\\startbildschrim.png", C2dRect(0.01f, 0.01f, 0.0f, 0.0f), true);
	m_zoStart.SetLayer(0.9f);
	m_zv.AddOverlay(&m_zoStart);

	//Button Start und PlaneSelection
	m_zoButtonStart.Init("textures\\START.png", C2dRect(0.3f, 0.2f, 0.1f, 0.7f), true);
	m_zoStart.AddOverlay(&m_zoButtonStart);
	m_zoButtonStart.SetLayer(0.3f);
	m_zosStart.Add(&m_zoButtonStart);

	m_zoButtonPlaneSelection.Init("textures\\SELECTION.png", C2dRect(0.3f, 0.2f, 0.6f, 0.7f), true);
	m_zoStart.AddOverlay(&m_zoButtonPlaneSelection);
	m_zoButtonPlaneSelection.SetLayer(0.4f);
	m_zosStart.Add(&m_zoButtonPlaneSelection);

	// ----------------
	// Pausebildschirm:
	// ----------------

	m_ziPause.Init("textures\\Pause.jpg");
	m_zoPause.InitFull(&m_ziPause);
	m_zoPause.SetLayer(0.9f);
	m_zv.AddOverlay(&m_zoPause);
	m_zoPause.SwitchOff();

	//Buttons Resume und BacktoMainMenu
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

	
	//m_zoPlaneSelection.Init("textures\\PLANESELECTION.jpg", C2dRect(0.5,1.0f, 0.0f, 0.0f));
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
	// Optionen-Bildschirm: 
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

	//Nebel in der Ferne
	m_zv.SetMistOn();
	m_zv.SetMistStartDistance(1000);
	m_zv.SetMistStrength(0.0002);
	m_zv.SetMistHeightMax(500);
	m_zv.SetMistHeightMin(-500);

	//m_zv.SetHazeOn();
	//m_zv.SetBloomOn();
	//m_zv.SetBloomStrengthNear(5.0f); //6.0
	//m_zv.SetBloomStrengthFar(0.0f);
	//m_zv.SetBloomWidth(1.0f); //3.m_zs

	//You have died Screen
	m_ziDied.Init("textures\\YOUVE DIED.jpeg");
	m_zoDied.Init(&m_ziDied, C2dRect(1.0f, 1.0f, 0.0f, 0.0f), false);
	m_zv.AddOverlay(&m_zoDied);
	m_zoDied.SwitchOff();

	//Abstands Warnung
	m_zhvAbstand.Init(0.0, 0.0, 0.0, 1.0);
	m_ziAbstand.Init("textures\\Warning.png");
	m_zoAbstand.Init(&m_ziAbstand, C2dRect(1.0f, 1.0f, 0.0f, 0.6f), false);
	m_zoAbstand.SetTransparency(0.5f);
	m_zoAbstand.SwitchOff();
	m_zv.AddOverlay(&m_zoAbstand);
}

void CPlayer::Tick(float fTime, float fTimeDelta)
{
	if (m_zeStatus == eStart)
	{
		m_zwHighScore.SwitchOn();
		m_zwScore.SwitchOff();
		m_zw2.SwitchOff();
		m_zw3.SwitchOff();
		m_zo.SwitchOff();
		m_zoPause.SwitchOff();
		m_zoStart.SwitchOn();
		m_zoPlaneSelection.SwitchOff();
		m_zoGameOver.SwitchOff();
		m_zvMinimap.SwitchOff();
		m_zoCrosshair.SwitchOff();
		m_zoCirclehair.SwitchOff();
		m_pgame->m_zpBlackSphere.SwitchOff();
		m_zpPlane2Select.SwitchOff();

		if (m_zdc.ButtonDownLeft())
		{
			COverlay* pzoPicked = m_zdc.PickOverlayPreselected(m_zosStart);
			if (pzoPicked == &m_zoButtonStart)
			{
				m_zvMinimap.SwitchOn();
				m_zdc.Hide();
				m_zwHighScore.SwitchOff();
				//m_zoMap.SwitchOn();
				m_zeStatus = eInGame;
			}
			if (pzoPicked == &m_zoButtonPlaneSelection)
			{
				m_zdc.Show();
				//m_zoMa.SwitchOn();
				m_zeStatus = eSelection;
				m_zpCamera.SubCamera(&m_zcCamera);
				m_zpPlaneSelection.AddCamera(&m_zcCamera);
			}
		}
	}
	else if (m_zeStatus == ePaused)
	{
		//m_fTimePausings += fTimeDelta;
		//fTimeDelta = 0.0f;
		m_zoPause.SwitchOn();
		m_zoStart.SwitchOff();
		m_zoPlaneSelection.SwitchOff();
		m_zoGameOver.SwitchOff();
		m_zvMinimap.SwitchOff();
		m_zoCrosshair.SwitchOff();
		m_zoCirclehair.SwitchOff();
		m_pgame->m_zpBlackSphere.SwitchOff();
		m_zpPlane2Select.SwitchOff();

		if (m_zdc.ButtonDownLeft())
		{
			COverlay* pzoPicked = m_zdc.PickOverlayPreselected(m_zosPause);
			if (pzoPicked == &m_zoBack2Start)
			{
				m_airplane.SetHealth(100);
				m_score = 0;
				m_airplane.RotateY(HALFPI);
				m_airplane.TranslateDelta(2000, 55, -1000);
				m_zeStatus = eStart;
			}

			if (pzoPicked == &m_zoButtonGoOn)
			{
				m_zeStatus = eInGame;
			}
		}
	}
	else if (m_zeStatus == eSelection)
	{
		m_zoPause.SwitchOff();
		m_zoStart.SwitchOff();
		m_zoPlaneSelection.SwitchOn();
		m_zoGameOver.SwitchOff();
		m_zvMinimap.SwitchOff();
		m_zoCrosshair.SwitchOff();
		m_zoCirclehair.SwitchOff();
		//m_pgame->m_zpBlackSphere.SwitchOn();
		//m_zpCamera.RotateY(HALFPI);
		//m_zpCamera.TranslateDelta(2004.553711,117.965439,-1515.466431);
		//m_pgame->m_zpBlackSphere.TranslateY(1000);
		m_zpPlane2Select.SwitchOn();

		for (int i = 0; i < 7; i++)
		{
			m_zpModel[i].SwitchOff();
			if(m_iFlugGeo == i)
				m_zpModel[i].SwitchOn();
		}

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
				m_airplane.ReInit(m_iFlugGeo);
				m_zeStatus = eStart;
			}
		}
		m_zpPlane2Select.RotateY(fTime);
		m_zpPlane2Select.RotateZDelta(.3);
		m_zpPlane2Select.TranslateDelta(10.0f, 0, -35.0f);
	}
	else if (m_zeStatus == eInGame)
	{
		m_zwScore.SwitchOn();
		m_zw2.SwitchOn();
		m_zw3.SwitchOn();
		m_zoPause.SwitchOff();
		m_zoStart.SwitchOff();
		m_zoPlaneSelection.SwitchOff();
		m_zoGameOver.SwitchOff();
		m_zvMinimap.SwitchOn();
		m_zoCrosshair.SwitchOn();
		m_zoCirclehair.SwitchOn();
		m_pgame->m_zpBlackSphere.SwitchOff();

		if (m_zdk.KeyDown(DIK_P) || m_zdgc.ButtonDown(45))
		{
			m_zdc.Show();
			m_zeStatus = ePaused;
		}

		float airplaneHealth = m_airplane.GetHealth();

		//Score System
		if (m_airplane.GetBulletManager()->m_killedEnemy) {
			m_score++;
			m_airplane.AddHealth(10);
		}
		m_zwScore.PrintF("Score: %d", m_score);
		m_zw2.PrintF("Health: %f", airplaneHealth);
		m_zw3.PrintF("Speed: %f", m_airplane.GetFlySpeed());

		//Backfacing Cam
		if (m_zdk.KeyDown(DIK_LSHIFT)) {
			m_zpCamera.SubCamera(&m_zcCamera);
			m_zpCameraBack.AddCamera(&m_zcCamera);
		}

		if (m_zdk.KeyUp(DIK_LSHIFT)) {
			m_zpCameraBack.SubCamera(&m_zcCamera);
			m_zpCamera.AddCamera(&m_zcCamera);
		}

		//----------------------------------------------------------------------------------
		//Airplane Zoom
		//----------------------------------------------------------------------------------
		float zoomSpeedIn = MAX_ZOOMIN;
		float zoomSpeedOut = MAX_ZOOMOUT;
		if (m_zdc.ButtonPressedRight() || m_zdgc.ButtonPressed(44))
		{
			m_zoom += fTimeDelta * zoomSpeedIn;
			if (m_zoom > 5.0f)
			{
				m_zoom = 5.0f;
			}
			m_zcCamera.SetFov((PI / 3) / m_zoom);
		}
		else if (!m_zdc.ButtonPressedRight() || !m_zdgc.ButtonPressed(44))
		{
			m_zoom -= fTimeDelta * zoomSpeedOut;
			if (m_zoom < 1.0f)
			{
				m_zcCamera.SetFov(PI / 3);
				m_zoom = 1.0f;
			}
			else
				m_zcCamera.SetFov((PI / 3) / m_zoom);
		}

		//Minimap Movement
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

		//DIED
		if (airplaneHealth == 0) {
			//Highscore
			m_scoreFile.open("highscore.txt", std::ios::out);
			if (m_scoreFile.is_open() && m_score > m_highscore) {
				m_scoreFile << m_score;
				m_highscore = m_score;
				m_zwHighScore.PrintF("Highscore: %d", m_score);
				LogDebug("Highscore Broken! NewHighscore: %d", m_score);
			}
			m_scoreFile.close();
			m_score = 0;
			m_zeStatus = eGameOver;
			m_zdc.Show();
		}

		//----------------------------------------------------------------------------------
		//Collision Warnung fuer zu weit weg
		//----------------------------------------------------------------------------------
		int MaxAbstand = MAX_DISTANCE;

		CHVector PlayerPos = m_airplane.GetPosGlobal();
		if (PlayerPos.Length()> MaxAbstand) {
			m_zo.SwitchOff();
			m_zoAbstand.SwitchOn();
			//LogDebug("Warnung sie verlassen das Kriegsgebiet");
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

		//Collision fuer Objekte
		if (m_lastPos == PlayerPos)
			m_airplane.RegisterHit(1);
		//LogDebug("%f,%f,%f", m_lastPos.x, m_lastPos.y, m_lastPos.z);
		m_lastPos = PlayerPos;

		//LogDebug("%f,%f,%f", m_airplane.GetPosGlobal().x, m_airplane.GetPosGlobal().y, m_airplane.GetPosGlobal().z);

	}
	else if (m_zeStatus == eGameOver)
	{
		m_zoAbstand.SwitchOff();
		m_zwScore.SwitchOff();
		m_zw2.SwitchOff();
		m_zw3.SwitchOff();
		m_zoGameOver.SwitchOn();
		m_zvMinimap.SwitchOff();
		m_zoCrosshair.SwitchOff();
		m_zoCirclehair.SwitchOff();
		if (m_zdc.ButtonDownLeft())
		{
			COverlay* pzoPicked = m_zdc.PickOverlayPreselected(m_zosGameOver);
			if (pzoPicked == &m_zoRestart)
			{
				y = 0;
				x = 0;
				m_airplane.m_Xrotation = 0.0f;
				m_airplane.m_Yrotation = 0.0f;
				m_lastPos = CHVector(0, 0, 0, 1);
				m_airplane.RotateY(HALFPI);
				m_airplane.TranslateDelta(2000, 55, -1000);
				m_airplane.SetHealth(100);
				m_zeStatus = eStart;
				m_airplane.SwitchOn();
			}
		}
	}
}


void CPlayer::ControlPlane(float fTimeDelta) {
	float controllerXInput = m_zdgc.GetRelativeX() / CONTROLLER_SENSITIVITY;
	if (abs(controllerXInput) < 0.0001f)
		controllerXInput = 0;
	float controllerYInput = m_zdgc.GetRelativeY() / CONTROLLER_SENSITIVITY;
	if (abs(controllerYInput) < 0.0001f)
		controllerYInput = 0;
	x += controllerXInput;
	y += controllerYInput;
	//GetMausbewegung
	x += m_zdm.GetRelativeX();
	y -= m_zdm.GetRelativeY();
	//Maximiere Ausschlag des Kreises
	x = ClampValue(x, -0.35f, 0.35f);
	y = ClampValue(y, -0.35f, 0.35f);
	//Verschiebe Kreis und das Crosshair
	CenterSquare(x + 0.5f, y + 0.5f, CROSSHAIRSIZE, m_zoCirclehair);
	CenterSquare(-x / 5 + 0.5f, -y / 5 + 0.5f, CROSSHAIRSIZE, m_zoCrosshair);
	//Setzt y und y (und damit den Kreis langsam in Richtung Mitte des Bildsschirms) auf 0 zurueck
	if (x != 0) {
		x -= x * fTimeDelta * 1.5f;
	}
	if (y != 0) {
		y -= y * fTimeDelta * 1.5f;
	}
	m_airplane.MovePlane(x, y, fTimeDelta);

	if (m_zdk.KeyPressed(DIK_W) || m_zdgc.ButtonPressed(7))
		m_airplane.SetSpeed(fTimeDelta);
	if (m_zdk.KeyPressed(DIK_S) )
	if (m_zdk.KeyPressed(DIK_S) || m_zdgc.ButtonPressed(6))
		m_airplane.SetSpeed(-fTimeDelta);
	if (m_zdm.ButtonPressedLeft() || m_zdgc.ButtonPressed(37)) {
		m_timePassed += fTimeDelta;
		// Fuehre die Funktion aus, waehrend genug Zeit vergangen ist
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