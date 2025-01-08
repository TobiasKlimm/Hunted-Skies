#include "Airplane.h"
#include <cmath>

CAirplane::CAirplane()
{
}

CAirplane::~CAirplane()
{
}

void CAirplane::Init(float damage)
{
	m_damage = damage;
	this->AddPlacement(&m_zpPlaneCenter);
	m_planeModel.Init(m_planeID);

	switch (m_planeID)
	{
	case 1:
	{
		m_flySpeed = 100;
		m_minFlySpeed = 150;
		m_maxFlySpeed = 200;
		m_flySpeedChangeRate = 35;
		m_zaFlugzeugsound.Init3D("sound\\Propeller.wav",3.0F);
		

		break;
	}
	case 2:
	{
		m_flySpeed = 30;
		m_minFlySpeed = 30;
		m_maxFlySpeed = 130;
		m_flySpeedChangeRate = 15;
		m_zaFlugzeugsound.Init3D("sound\\Propeller.wav", 3.0F);

		break;
	}
	case 3:
	{
		m_flySpeed = 120;
		m_minFlySpeed = 150;
		m_maxFlySpeed = 250;
		m_flySpeedChangeRate = 40;
		m_zaFlugzeugsound.Init3D("sound\\Propeller.wav", 3.0F);
		break;
	}
	case 4:
	{
		m_flySpeed = 60;
		m_minFlySpeed = 30;
		m_maxFlySpeed = 130;
		m_flySpeedChangeRate = 15;
		m_zaFlugzeugsound.Init3D("sound\\Propeller.wav", 3.0F);
		break;
	}
	case 5:
	{
		m_flySpeed = 120;
		m_minFlySpeed = 150;
		m_maxFlySpeed = 250;
		m_flySpeedChangeRate = 40;
		break;
	}
	case 6:
	{
		m_flySpeed = 120;
		m_minFlySpeed = 150;
		m_maxFlySpeed = 250;
		m_flySpeedChangeRate = 40;
		m_zaFlugzeugsound.Init3D("sound\\Propeller.wav", 3.0F);
		break;
	}
	}


	m_zpPlaneCenter.AddAudio(&m_zaFlugzeugsound);
	m_zaFlugzeugsound.Loop();



	m_zpPlaneTip.TranslateZDelta(-2);
	m_zpPlaneTip.RotateXDelta(-PI / 14.0f);

	m_zpPlaneCenter.AddPlacement(&m_planeModel);
	m_zpPlaneCenter.AddPlacement(&m_zpPlaneTip);

	m_zpPlaneTip.AddPlacement(&m_BulletManager);
	m_BulletManager.Init(BULLETSPEED, m_damage);



	//Abstands Vektor
	
	m_zhvAbstand.Init(0.0, 0.0, 0.0, 1.0);
	
}

void CAirplane::Tick(float fTime, float fTimeDelta)
{
	m_planeModel.Tick(fTime, fTimeDelta);
	m_BulletManager.Tick(fTime, fTimeDelta);
	this->SetTranslationSensitivity(m_flySpeed);
	m_vDirection = m_zpPlaneTip.GetPosGlobal()-m_zpPlaneCenter.GetPosGlobal();
	m_vDirection.Normal();





	//-----------------------
	//Collision Detection
	//-----------------------
	float x = m_zpPlaneCenter.GetPosGlobal().x;
	float y = m_zpPlaneCenter.GetPosGlobal().y;
	float z = m_zpPlaneCenter.GetPosGlobal().z;
	int MaxAbstand =MAX_DISTANCE;

	//Collision Warnung für zu weit weg
	if (abs(x - m_zhvAbstand.x) > MaxAbstand || abs(y - m_zhvAbstand.y) > MaxAbstand || abs(z - m_zhvAbstand.z) > MaxAbstand)
	{
		LogDebug("Warnung sie verlassen das Kriegsgebiet");
	}
	else
	{

	}

	//Collision für Objekte
	if (x == m_lastx && y == m_lasty && z == m_lastz)
	{
		//LogDebug("TOT");
		RegisterHit(10000);
		
		
	}
	else
	{
		m_lastx = x;
		m_lasty = y;
		m_lastz = z;
	}




}

float CAirplane::ClampValue(float value, float minValue, float maxValue) {
	return max(minValue, min(value, maxValue));
}

void CAirplane::MovePlane(float& x, float& y, float fTimeDelta)
{
	//minimiert die maximale Rotation pro Tick
	m_Xrotation = ClampValue(x, m_Xrotation - MAX_ROTATION_SPEED * fTimeDelta, m_Xrotation + MAX_ROTATION_SPEED * fTimeDelta);
	m_Yrotation = ClampValue(y, m_Yrotation - MAX_ROTATION_SPEED * fTimeDelta, m_Yrotation + MAX_ROTATION_SPEED * fTimeDelta);

	m_planeModel.RotateY(-m_Xrotation / 2);
	m_planeModel.RotateZDelta(-m_Xrotation * 5);
	m_planeModel.RotateXDelta(-m_Yrotation);
	//Move Plane and Camera
	float RotationX = x * 15;
	float RotationY = -y * 15;
	float MoveAD = 0, MoveWS = -1, MoveUD = 0;
	this->MoveWithCollisionDetection(fTimeDelta, true, MoveAD, MoveWS, MoveUD, RotationX, RotationY, m_zgsCollisionObjects);
}

void CAirplane::Shoot(float randFac)
{
	CHVector vRand;
	vRand.RandomDir();
	m_BulletManager.Shoot(m_zpPlaneTip.GetDirectionGlobal() + vRand * randFac);
}

void CAirplane::SetSpeed(float change)
{
	LogDebug("%f", m_flySpeedChangeRate * change);
	m_flySpeed = ClampValue(m_flySpeed + m_flySpeedChangeRate * change, m_minFlySpeed, m_maxFlySpeed);
}
