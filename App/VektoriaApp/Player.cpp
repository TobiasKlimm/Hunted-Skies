#include "Player.h"

void CPlayer::InitCam() {
	//Hauptviewport Initialisierung
	m_zcCamera.Init(PI / 3);
	m_zpCamera.AddCamera(&m_zcCamera);
	m_zpCamera.TranslateZDelta(40);
	m_zpCamera.TranslateYDelta(4);
	m_zpCamera.RotateXDelta(-PI / 12);
	m_zv.InitFull(&m_zcCamera);

	//Minimap Initialierung
	m_zcMinimap.InitOrtho(200.0f);
	m_zpMinimap.AddCamera(&m_zcMinimap);
	this->AddPlacement(&m_zpMinimap);
	m_zvMinimap.Init(&m_zcMinimap, MINIMAPSIZE * 9, MINIMAPSIZE * 16, 0.81f, 0.01f);
	m_zvMinimap.SwitchOff();
}

void CPlayer::Init()
{
	this->AddPlacement(&m_airplane);
	m_airplane.TranslateY(500);
	m_airplane.Init(DAMAGE);
	m_airplane.AddPlacement(&m_zpCamera);

	m_zCrosshairRect.Init(CROSSHAIRSIZE * 9, CROSSHAIRSIZE * 16, 0, 0);
	m_zoCrosshair.Init("textures\\crosshair.bmp", m_zCrosshairRect, true);
	m_zv.AddOverlay(&m_zoCrosshair);
	CenterSquare(0.5, 0.5, CROSSHAIRSIZE, m_zoCrosshair);
	m_zoCirclehair.Init("textures\\circlehair.bmp", m_zCrosshairRect, true);
	m_zv.AddOverlay(&m_zoCirclehair);

	//---------------------------------------------------------------------
	//Overlay
	m_zwf.LoadPreset("RodWhite");
	m_zwf.SetTransparencyKind(eTransparencyKind_BinaryByChromaKey);
	m_zwScore.Init(C2dRect(0.1f, 0.04f, 0.0f, 0.0f), 11, &m_zwf);
	m_zv.AddWriting(&m_zwScore);
	m_zwScore.PrintF("Score: 0");

	m_zw2.Init(C2dRect(0.1f, 0.04f, 0.0f, 0.05f), 11, &m_zwf);
	m_zv.AddWriting(&m_zw2);

	m_zw3.Init(C2dRect(0.1f, 0.04f, 0.0f, 0.1f), 11, &m_zwf);
	m_zv.AddWriting(&m_zw3);

	//-----------------
	// Startbildschirm:
	//-----------------

	//Startbildschrim Inits und Overlayadds overlay mit switchoff
	m_ziStart.Init("textures\\HalloWelt.jpg");
	m_zoStart.InitFull(&m_ziStart);
	m_zoStart.SetLayer(0.9f);
	m_zv.AddOverlay(&m_zoStart);

	//Button 1 und 2
	m_zoButtonStart.Init("textures\\test.jpg", C2dRect(0.3f, 0.2f, 0.4f, 0.1f), false);
	m_zoStart.AddOverlay(&m_zoButtonStart);
	m_zoButtonStart.SetLayer(0.3f);
	m_zosStart.Add(&m_zoButtonStart);

	m_zoPlaneSelection.Init("textures\\test 2.jpg", C2dRect(0.3f, 0.2f, 0.39f, 0.39f), false);
	m_zoStart.AddOverlay(&m_zoButtonPlaneSelection);
	m_zoPlaneSelection.SetLayer(0.3f);
	m_zosStart.Add(&m_zoButtonPlaneSelection);

	m_zoButtonOptions.Init("textures\\test 2.jpg", C2dRect(0.3f, 0.2f, 0.39f, 0.59f), false);
	m_zoStart.AddOverlay(&m_zoButtonOptions);
	m_zoButtonOptions.SetLayer(0.3f);
	m_zosStart.Add(&m_zoButtonOptions);


	// ----------------
	// Pausebildschirm:
	// ----------------

	m_ziPause.Init("textures\\Pause.png");
	m_zoPause.InitFull(&m_ziPause);
	m_zoPause.SetLayer(0.9f);
	m_zv.AddOverlay(&m_zoPause);

	m_zoButtonGoOn.Init("textures\\test 2.jpg", C2dRect(0.3f, 0.2f, 0.39f, 0.39f), false);
	m_zoPause.AddOverlay(&m_zoButtonGoOn);
	m_zoButtonGoOn.SetLayer(0.9f);

	m_zosPause.Add(&m_zoButtonGoOn);
	m_zoPause.SwitchOff();

	
	// ---------------------
	// In-Game: 
	// ---------------------
	//Startbildschrim zurück
	m_zoBack2Start.Init("textures\\HalloWelt.jpg", C2dRect(0.08f, 0.05f, 0.9f, 0.9f));
	m_zv.AddOverlay(&m_zoBack2Start);
	m_zoBack2Start.SetLayer(0.1f);
	m_zoBack2Start.SwitchOff();
	m_zosInGame.Add(&m_zoBack2Start);


	// ----------------
	// Plane Selection: 
	// ----------------

	m_zoPlaneSelection.InitFull("textures\\white_image.jpg");
	m_zoPlaneSelection.SetLayer(0.9);
	m_zoPlaneSelection.SwitchOff();
	m_zPlaneselecion.Init(&m_zv, &m_zdc);
	AddPlacement(&m_zPlaneselecion);
	m_zosPlaneSelection.Add(&m_zoPlaneSelection);


	// --------------------
	// Optionen-Bildschirm: 
	// --------------------
	
	m_zoOptions.InitFull("textures\\white_image.jpg");
	m_zoOptions.SetLayer(0.9);
	m_zoOptions.SwitchOff();
	m_zPlaneselecion.Init(&m_zv, &m_zdc);
	AddPlacement(&m_zPlaneselecion);
	m_zosOptions.Add(&m_zoOptions);



}

void CPlayer::Tick(float fTime, float fTimeDelta)
{
	//Pause
	//m_fTimeWithPausings = fTime - m_fTimePausings;

	if (m_zeStatus == eSelection)
	{



	}


	if (m_zeStatus == eStart)
	{
		if (m_zdc.ButtonDownLeft())
		{
			m_zoPause.SwitchOff();
			m_zoStart.SwitchOff();
			m_zoPlaneSelection.SwitchOff();
			m_zoOptions.SwitchOff();
			m_zoBack2Start.SwitchOff();
			m_zvMinimap.SwitchOff();


			COverlay* pzoPicked = m_zdc.PickOverlayPreselected(m_zosStart);
			if (pzoPicked == &m_zoButtonStart)
			{
				m_zvMinimap.SwitchOn();
				m_zdc.Hide();
				//m_zoMap.SwitchOn();
				m_zeStatus = eInGame;
			}
			if (pzoPicked == &m_zoButtonPlaneSelection)
			{
				m_zoPlaneSelection.SwitchOn();
				m_zdc.Show();

				//m_zoMap.SwitchOn();
				m_zeStatus = eSelection;
			}
			if (pzoPicked == &m_zoOptions)
			{
				m_zoOptions.SwitchOn();
				m_zdc.Show();

				//m_zoMap.SwitchOn();
				m_zeStatus = eOptions;
			}
		}
	}

	if (m_zeStatus == ePaused)
	{
		//m_fTimePausings += fTimeDelta;
		//fTimeDelta = 0.0f;
		m_zoPause.SwitchOn();
		m_zoStart.SwitchOff();
		m_zoPlaneSelection.SwitchOff();
		m_zoOptions.SwitchOff();
		m_zoBack2Start.SwitchOff();
		m_zvMinimap.SwitchOff();

		if (m_zdc.ButtonDownLeft())
		{
			COverlay* pzoPicked = m_zdc.PickOverlayPreselected(m_zosPause);
			if (pzoPicked == &m_zoBack2Start)
			{
				m_zoPause.SwitchOff();
				//m_zoMinimap.SwitchOff();
				m_zoStart.SwitchOn();
				m_zeStatus = eStart;
			}

			if (pzoPicked == &m_zoButtonStart)
			{
				m_zeStatus = eInGame;
			}
		}
	}

	if (m_zeStatus == eSelection)
	{
		//m_fTimePausings += fTimeDelta;
		//fTimeDelta = 0.0f;
		m_zoPause.SwitchOff();
		m_zoStart.SwitchOff();
		m_zoPlaneSelection.SwitchOn();
		m_zoOptions.SwitchOff();
		m_zoBack2Start.SwitchOff();
		m_zvMinimap.SwitchOff();

		if (m_zdc.ButtonDownLeft())
		{
			COverlay* pzoPicked = m_zdc.PickOverlayPreselected(m_zosPlaneSelection);
			if (pzoPicked == &m_zoBack2Start)
			{
				m_zeStatus = eStart;
			}

			if (pzoPicked == &m_zoButtonStart)
			{
				m_zeStatus = eInGame;
			}
		}
	}




	if (m_zeStatus == eInGame)
	{
		m_zoPause.SwitchOff();
		m_zoStart.SwitchOff();
		m_zoPlaneSelection.SwitchOff();
		m_zoOptions.SwitchOff();
		m_zoBack2Start.SwitchOff();
		m_zvMinimap.SwitchOn();



		if (m_zdk.KeyDown(DIK_P))
		{
			m_zdc.Show();
			m_zeStatus = ePaused;
		}


		//Score System
		if (m_airplane.GetBulletManager()->m_killedEnemy) {
			m_score++;
			m_airplane.AddHealth(10);
			m_zwScore.PrintF("Score: %d", m_score);
		}
		m_zw2.PrintF("Health: %f", m_airplane.GetHealth());
		m_zw3.PrintF("Speed: %f", m_airplane.GetFlySpeed());

		//Minimap Movement
		CHVector vPlane(m_airplane.GetPosGlobal());
		vPlane.y += 100;
		m_zpMinimap.RotateX(-HALFPI);
		m_zpMinimap.TranslateDelta(vPlane);

		m_airplane.Tick(fTime, fTimeDelta);
		ControlPlane(fTimeDelta);
	}
}


void CPlayer::ControlPlane(float fTimeDelta) {
	static float x = 0, y = 0;
	//GetMausbewegung
	x += m_zdm.GetRelativeX();
	y -= m_zdm.GetRelativeY();
	//Maximiere Ausschlag des Kreises
	x = ClampValue(x, -0.35f, 0.35f);
	y = ClampValue(y, -0.35f, 0.35f);
	//Verschiebe Kreis und das Crosshair
	CenterSquare(x + 0.5f, y + 0.5f, CROSSHAIRSIZE, m_zoCirclehair);
	CenterSquare(-x / 5 + 0.5f, -y / 5 + 0.5f, CROSSHAIRSIZE, m_zoCrosshair);
	//Setzt y und y (und damit den Kreis langsam in Richtung Mitte des Bildsschirms) auf 0 zurück zurück
	if (x != 0) {
		x -= x * fTimeDelta * 1.5f;
	}
	if (y != 0) {
		y -= y * fTimeDelta * 1.5f;
	}
	m_airplane.MovePlane(x, y, fTimeDelta);

	if (m_zdk.KeyPressed(DIK_W))
		m_airplane.SetSpeed(fTimeDelta);
	if (m_zdk.KeyPressed(DIK_S))
		m_airplane.SetSpeed(-fTimeDelta);
	if (m_zdm.ButtonPressedLeft()) {
		m_timePassed += fTimeDelta;
		// Führe die Funktion aus, während genug Zeit vergangen ist
		if (m_timePassed <= SHOOT_FREQUENCY)
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

