#pragma once
#include "Vektoria\Root.h"
#include "Enemy.h"
#include "BulletManager.h"
#include "AirplaneModel.h"

using namespace Vektoria;

#define MAX_ROTATION_SPEED  0.25f
#define BULLETSPEED 1000


class CAirplane :
	public CEnemy
{
public:
	CAirplane();
	~CAirplane();

	void Init(float damage);

	void Tick(float fTime, float fTimeDelta);

	float ClampValue(float value, float minValue, float maxValue);

	void MovePlane(float& x, float& y, float fTimeDelta);

	void Shoot(float randFac);

	void SetSpeed(float change);

	CPlacement* GetCenter()
	{
		return &m_zpPlaneCenter;
	}
	CBulletManager* GetBulletManager()
	{
		return &m_BulletManager;
	}
	CHVector GetDirection()
	{
		return m_vDirection;
	}
	float GetFlySpeed() 
	{
		return m_flySpeed;
	}

	CGeos m_zgsCollisionObjects;

private:
	float m_Xrotation = 0.0f, m_Yrotation = 0.0f;
	CBulletManager m_BulletManager;
	CAirplaneModel m_planeModel;
	CPlacement m_zpPlaneCenter;
	CPlacement m_zpPlaneTip;
	CHVector m_vDirection;
	CAudio m_zaFlugzeugsound;
	



	unsigned m_planeID = 1;

	float m_damage;
	float m_flySpeed;
	float m_minFlySpeed;
	float m_maxFlySpeed;
	float m_flySpeedChangeRate;


};