#include "Player.h"
#include "Game.h"

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

void CPlayer::Init(CGame* pgame)
{
	m_pgame = pgame; 
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
	m_ziStart.Init("textures\\startbildschrim.png");
	m_zoStart.InitFull(&m_ziStart);
	m_zoStart.SetLayer(0.9f);
	m_zv.AddOverlay(&m_zoStart);

	//Button 1 und 2
	m_zoButtonStart.Init("textures\\START.png", C2dRect(0.3f, 0.2f, 0.1f, 0.7f), true);
	m_zoStart.AddOverlay(&m_zoButtonStart);
	m_zoButtonStart.SetLayer(0.3f);
	m_zosStart.Add(&m_zoButtonStart);

	m_zoButtonPlaneSelection.Init("textures\\SELECTION.png", C2dRect(0.3f, 0.2f, 0.6f, 0.7f), false);
	m_zoStart.AddOverlay(&m_zoButtonPlaneSelection);
	m_zoButtonPlaneSelection.SetLayer(0.3f);
	m_zosStart.Add(&m_zoButtonPlaneSelection);

	/*
	m_zoButtonOptions.Init("textures\\test 2.jpg", C2dRect(0.3f, 0.2f, 0.6f, 0.7f), false);
	m_zoStart.AddOverlay(&m_zoButtonOptions);
	m_zoButtonOptions.SetLayer(0.3f);
	m_zosStart.Add(&m_zoButtonOptions);
	*/

	// ----------------
	// Pausebildschirm:
	// ----------------

	m_ziPause.Init("textures\\Pause.jpg");
	m_zoPause.InitFull(&m_ziPause);
	m_zoPause.SetLayer(0.9f);
	m_zv.AddOverlay(&m_zoPause);
	m_zoPause.SwitchOff();

	m_zoButtonGoOn.Init("textures\\RESUME.png", C2dRect(0.3f, 0.2f, 0.1f, 0.7f), false);
	m_zoPause.AddOverlay(&m_zoButtonGoOn);
	m_zoButtonGoOn.SetLayer(0.3f);
	
	m_zoBack2Start.Init("textures\\MAINMENU.png", C2dRect(0.3f, 0.2f, 0.6f, 0.7f), false);
	m_zoPause.AddOverlay(&m_zoBack2Start);
	m_zoBack2Start.SetLayer(0.1f);

	m_zosPause.Add(&m_zoButtonGoOn);
	m_zosPause.Add(&m_zoBack2Start);



	// ---------------------
	// In-Game: 
	// ---------------------
	//Startbildschrim zurück

	// ----------------
	// Plane Selection: 
	// ----------------

	m_zoPlaneSelection.Init("textures\\PLANESELECTION.jpg", C2dRect(0.5,1.0f, 0.0f, 0.0f));
	m_zoPlaneSelection.SetLayer(0.9);
	m_zoPlaneSelection.SwitchOff();
	m_zv.AddOverlay(&m_zoPlaneSelection);

	m_zPlaneselecion.Init(&m_zv, &m_zdc);
	AddPlacement(&m_zPlaneselecion);
	/*
	m_zoPrev.SetTexts("Prev", "Prev");
	m_zoNext.SetTexts("Next", "Next");
	m_zoPrev.SetColorWriting(CColor(eColor_White));
	m_zoNext.SetColorWriting(CColor(eColor_White));
	m_zoPrev.SetColor(CColor(eColor_Black));
	m_zoNext.SetColor(CColor(eColor_Black));
	m_zoPrev.Init(C2dRect(0.3f, 0.1f, 0.05f, 0.05f), 4);
	m_zoNext.Init(C2dRect(0.3f, 0.1f, 0.45f, 0.05f), 4);
	*/

	m_zoNext.Init("textures\\NEXT.png", C2dRect(0.3f, 0.2f, 0.1f, 0.1f), false);
	m_zoNext.SetLayer(0.3f);
	m_zosPlaneSelection.Add(&m_zoNext);
	m_zoPlaneSelection.AddOverlay(&m_zoNext);

	m_zoPrev.Init("textures\\PREV.png", C2dRect(0.3f, 0.2f, 0.1f, 0.4f), false);
	m_zoPrev.SetLayer(0.3f);
	m_zosPlaneSelection.Add(&m_zoPrev);
	m_zoPlaneSelection.AddOverlay(&m_zoPrev);

	m_zoBack.Init("textures\\START.png", C2dRect(0.3f, 0.2f, 0.1f, 0.7f), false);
	m_zoBack.SetLayer(0.3f);
	m_zosPlaneSelection.Add(&m_zoBack);
	m_zoPlaneSelection.AddOverlay(&m_zoBack);





	m_zosPlaneSelection.Add(&m_zoPrev);
	m_zosPlaneSelection.Add(&m_zoNext);


	m_zpCamera.AddPlacement(&m_zpPlane2Select);
	for (int i = 0; i < 6; i++)
	{
		m_zpModel[i].Init(i);
		m_zpPlane2Select.AddPlacement(&m_zpModel[i]);
	}
	m_zpPlane2Select.SwitchOff();


	// --------------------
	// Optionen-Bildschirm: 
	// --------------------
	
	m_zoGameOver.InitFull("textures\\death.png");
	m_zoGameOver.SetLayer(0.9);
	m_zoGameOver.SwitchOff();

	m_zoRestart.Init("textures\\test.jpg", C2dRect(0.3f, 0.2f, 0.1f, 0.7f), false);
	m_zoGameOver.AddOverlay(&m_zoRestart);
	m_zoRestart.SetLayer(0.3f);
	m_zosStart.Add(&m_zoRestart);

	m_zoEnd.Init("textures\\test 2.jpg", C2dRect(0.3f, 0.2f, 0.6f, 0.7f), false);
	m_zoGameOver.AddOverlay(&m_zoEnd);
	m_zoEnd.SetLayer(0.3f);
	m_zosStart.Add(&m_zoEnd);


	m_zosGameOver.Add(&m_zoRestart);
	m_zosGameOver.Add(&m_zoEnd);



}

void CPlayer::Tick(float fTime, float fTimeDelta)
{
	//Pause
	//m_fTimeWithPausings = fTime - m_fTimePausings;

			/*

	if (m_zeStatus == eSelection)
	{
		m_zoPause.SwitchOff();
		m_zoStart.SwitchOff();
		m_zoCrosshair.SwitchOff();
		m_zoCirclehair.SwitchOff();

		COverlay* pzoPicked = m_zdc.PickOverlayPreselected(m_zosPlaneSelection);

		if (m_zoNext.IsDown(m_zdc))
		{
			m_iFlugGeo++;
			m_iFlugGeo %= m_iFlugGeos;
		// 	for (int i = 0; i < m_iFlugGeos; i++)
		//		m_zpModel[i].SwitchOff();
		//	m_zpModel[m_iFlugGeo].SwitchOn();
		}
		if (m_zoPrev.IsDown(m_zdc))
		{
			m_iFlugGeo--;
			if (m_iFlugGeo < 0)
				m_iFlugGeo = m_iFlugGeos - 1;
		//	for (int i = 0; i < m_iFlugGeos; i++)
		// 		m_zpModel[i].SwitchOff();
		// 	m_zpModel[m_iFlugGeo].SwitchOn();
		}




	}

		*/

	if (m_zeStatus == eStart)
	{
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
				//m_zoMap.SwitchOn();
				m_zeStatus = eInGame;
			}
			if (pzoPicked == &m_zoButtonPlaneSelection)
			{
				m_zdc.Show();

				//m_zoMa.SwitchOn();
				m_zeStatus = eSelection;
				m_mLastCamPos = m_zpCamera.GetMat();
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
		//m_fTimePausings += fTimeDelta;
		//fTimeDelta = 0.0f;
		m_zoPause.SwitchOff();
		m_zoStart.SwitchOff();
		m_zoPlaneSelection.SwitchOn();
		m_zoGameOver.SwitchOff();
		m_zvMinimap.SwitchOff();
		m_zoCrosshair.SwitchOff();
		m_zoCirclehair.SwitchOff();
		m_pgame->m_zpBlackSphere.SwitchOn();
		m_zpCamera.TranslateY(1000);
		m_pgame->m_zpBlackSphere.TranslateY(1000);
		m_zpPlane2Select.SwitchOn();


		for (int i = 0; i < 6; i++)
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
				m_zpCamera.SetMat(m_mLastCamPos);
				m_zeStatus = eInGame;
			}


		}
		m_zpPlane2Select.RotateY(fTime);
		m_zpPlane2Select.TranslateDelta(10.0f, 0, -30.0f);

	}
	else if (m_zeStatus == eInGame)
	{
		m_zoPause.SwitchOff();
		m_zoStart.SwitchOff();
		m_zoPlaneSelection.SwitchOff();
		m_zoGameOver.SwitchOff();
		m_zvMinimap.SwitchOn();
		m_zoCrosshair.SwitchOn();
		m_zoCirclehair.SwitchOn();
		m_pgame->m_zpBlackSphere.SwitchOff();



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

