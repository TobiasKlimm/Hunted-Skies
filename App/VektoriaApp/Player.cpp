#include "Player.h"
#define MAX_DISTANCE 2000;

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

void CPlayer::Init()
{
	this->AddPlacement(&m_airplane);
	m_airplane.RotateY(HALFPI); //Ausrichtung Startbahn flugzeugträger
	m_airplane.TranslateDelta(2000, 20, -1000);
	m_airplane.Init(DAMAGE);
	m_airplane.AddPlacement(&m_zpCamera);
	m_airplane.AddPlacement(&m_zpCameraBack);

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

	//Startbildschrim Inits und Overlayadds overlay mit switchoff
	m_ziStart.Init("textures\\HalloWelt.jpg");
	m_zoStart.InitFull(&m_ziStart);
	m_zv.AddOverlay(&m_zoStart);

	//Startbildschrim zurï¿½ck
	m_zoBack2Start.Init("textures\\HalloWelt.jpg", C2dRect(0.08f, 0.05f, 0.9f, 0.9f));
	m_zv.AddOverlay(&m_zoBack2Start);
	m_zoBack2Start.SetLayer(0.1f);

	//Pausebildschirm
	m_ziPause.Init("textures\\Pause.png");
	m_zoPause.InitFull(&m_ziPause);
	m_zv.AddOverlay(&m_zoPause);

	//Button 1 und 2
	m_zoButtonStart.Init("textures\\test.jpg", C2dRect(0.3f, 0.2f, 0.4f, 0.1f), false);
	m_zv.AddOverlay(&m_zoButtonStart);
	m_zoButtonStart.SetLayer(0.3f);

	m_zoButtonOptionen.Init("textures\\test 2.jpg", C2dRect(0.3f, 0.2f, 0.39f, 0.39f), false);
	m_zv.AddOverlay(&m_zoButtonOptionen);
	m_zoButtonOptionen.SetLayer(0.3f);

	//Container fuer die Buttons
	m_zos.Add(&m_zoButtonStart);
	m_zos.Add(&m_zoButtonOptionen);
	m_zos.Add(&m_zoBack2Start);
	m_zosInGame.Add(&m_zoBack2Start);

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
	m_zoAbstand.Init(&m_ziAbstand, C2dRect(1.0f, 1.0f, 0.0f, 0.0f), false);
	m_zoAbstand.SetTransparency(0.5f);
	m_zoAbstand.SwitchOff();
	m_zv.AddOverlay(&m_zoAbstand);
}

void CPlayer::Tick(float fTime, float fTimeDelta)
{
	//Pause
	//m_fTimeWithPausings = fTime - m_fTimePausings;

	if (m_zeStatus == eStart)
	{
		if (m_zdc.ButtonDownLeft())
		{
			COverlay* pzoPicked = m_zdc.PickOverlayPreselected(m_zos);
			if (pzoPicked == &m_zoButtonStart)
			{
				m_zoButtonStart.SwitchOff();
				m_zoButtonOptionen.SwitchOff();
				m_zoPause.SwitchOff();
				m_zoStart.SwitchOff();
				m_zvMinimap.SwitchOn();
				m_zoBack2Start.SwitchOff();
				m_zdc.Hide();
				//m_zoMap.SwitchOn();
				m_zeStatus = eInGame;
			}
		}
	}

	if (m_zeStatus == ePaused)
	{
		//m_fTimePausings += fTimeDelta;
		//fTimeDelta = 0.0f;

		if (m_zdc.ButtonDownLeft())
		{
			COverlay* pzoPicked = m_zdc.PickOverlayPreselected(m_zos);
			if (pzoPicked == &m_zoBack2Start)
			{
				m_zoPause.SwitchOff();
				//m_zoMinimap.SwitchOff();
				m_zoStart.SwitchOn();
				m_zeStatus = eStart;
			}

			if (pzoPicked == &m_zoButtonStart)
			{
				m_zoPause.SwitchOff();
				//m_zoMap.SwitchOn();
				m_zoButtonStart.SwitchOff();
				m_zoButtonOptionen.SwitchOff();
				m_zoBack2Start.SwitchOff();
				m_zeStatus = eInGame;
			}
		}
	}

	if (m_zeStatus == eInGame)
	{
		float airplaneHealth = m_airplane.GetHealth();
		if (m_zdk.KeyDown(DIK_P))
		{
			//m_fTimeLastPausingStart = fTime;
			m_zoButtonStart.SwitchOn();
			m_zoButtonOptionen.SwitchOn();
			m_zoPause.SwitchOn();
			m_zoStart.SwitchOff();
			m_zvMinimap.SwitchOff();
			m_zoBack2Start.SwitchOn();
			m_zdc.Show();
			m_zeStatus = ePaused;
		}

		//Score System
		if (m_airplane.GetBulletManager()->m_killedEnemy) {
			m_score++;
			m_airplane.AddHealth(10);
			m_zwScore.PrintF("Score: %d", m_score);
		}
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
			m_zoDied.SwitchOn();
			//REST
		}


		//Collision Warnung fuer zu weit weg
		float MaxAbstand = MAX_DISTANCE;

		CHVector PlayerPos = m_airplane.GetPosGlobal();
		if (PlayerPos.Length()> MaxAbstand) {
			m_zo.SwitchOff();
			m_zoAbstand.SwitchOn();
			LogDebug("Warnung sie verlassen das Kriegsgebiet");
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
			m_airplane.RegisterHit(10000);
		m_lastPos = PlayerPos;
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
	//Setzt y und y (und damit den Kreis langsam in Richtung Mitte des Bildsschirms) auf 0 zurueck
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
		// Fuehre die Funktion aus, waehrend genug Zeit vergangen ist
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