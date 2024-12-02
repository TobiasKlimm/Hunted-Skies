#include "Player.h"

void CPlayer::Init()
{
	this->AddPlacement(&m_airplane);
	m_airplane.TranslateY(100);
	m_airplane.Init();
	m_airplane.AddPlacement(&m_zpCamera);

	m_zwf.LoadPreset("OCRAExtendedRed");
	m_zw.Init(C2dRect(0.2f, 0.1f, 0.0f, 0.0f), 10, &m_zwf);
	m_zv.AddWriting(m_airplane.GetBulletManager()->GetWriting());

	m_zCrosshairRect.Init(crosshairSize * 9, crosshairSize * 16, 0, 0);
	m_zoCrosshair.Init("textures\\crosshair.bmp", m_zCrosshairRect, true);
	m_zv.AddOverlay(&m_zoCrosshair);
	CenterSquare(0.5, 0.5, crosshairSize, m_zoCrosshair);
	m_zoCirclehair.Init("textures\\circlehair.bmp", m_zCrosshairRect, true);
	m_zv.AddOverlay(&m_zoCirclehair);

	m_zv.SetMistOn();
	m_zv.SetMistStartDistance(1000);
	m_zv.SetMistStrength(0.0002);
	m_zv.SetMistHeightMax(500);
	m_zv.SetMistHeightMin(-500);

}

void CPlayer::Tick(float fTime, float fTimeDelta)
{
	m_airplane.Tick(fTime, fTimeDelta);
	ControlPlane(fTimeDelta);
	if (m_zdm.ButtonPressedLeft()) {
		m_airplane.Shoot(fTimeDelta);

	}
}

void CPlayer::InitCam() {
	m_zcCamera.Init(PI / 3);
	m_zpCamera.AddCamera(&m_zcCamera);
	m_zpCamera.TranslateZDelta(40);
	m_zpCamera.TranslateYDelta(4);
	m_zpCamera.RotateXDelta(-PI / 12);
	m_zv.InitFull(&m_zcCamera);
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
	CenterSquare(x + 0.5f, y + 0.5f, crosshairSize, m_zoCirclehair);
	CenterSquare(-x / 5 + 0.5f, -y / 5 + 0.5f, crosshairSize, m_zoCrosshair);
	//Setzt y und y (und damit den Kreis langsam in Richtung Mitte des Bildsschirms) auf 0 zurück zurück
	if (x != 0) {
		x -= x * fTimeDelta * 1.5f;
	}
	if (y != 0) {
		y -= y * fTimeDelta * 1.5f;
	}
	if (m_zdk.KeyPressed(DIK_W) && m_airplane.GetSpeed() <= 400)
		m_airplane.SetSpeed(m_airplane.GetSpeed() + 20 * fTimeDelta);
	if (m_zdk.KeyPressed(DIK_S) && m_airplane.GetSpeed() >= 50)
		m_airplane.SetSpeed(m_airplane.GetSpeed() - 20 * fTimeDelta);

	m_airplane.MovePlane(x, y, fTimeDelta);
}

void CPlayer::CenterSquare(float x, float y, float size, COverlay& rect) {
	float halfSize = size / 2.0f;
	rect.SetPos(x - (halfSize * 9), y - (halfSize * 16));
}

float CPlayer::ClampValue(float value, float minValue, float maxValue) {
	return max(minValue, min(value, maxValue));
}

