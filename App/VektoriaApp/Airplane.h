#pragma once
#include "Vektoria\Root.h"
#include "Enemy.h"
#include "BulletManager.h"
#include "AirplaneModel.h"

using namespace Vektoria;

#define MAX_ROTATION_SPEED  0.25f
#define BULLETSPEED 1000
#define DEAD 1000;



class CAirplane :
	public CEnemy
{
public:
	CAirplane();
	~CAirplane();

	void Init(float damage, unsigned planeID = 0);

	void ReInit(unsigned planeID);

	void Tick(float fTime, float fTimeDelta);

	float ClampValue(float value, float minValue, float maxValue);

	void MovePlane(float& x, float& y, float fTimeDelta);

	void Shoot(float randFac);

	void SetSpeed(float change);

	void ReduceSpeedWhenOutOfFuel();

	void StopSounds();

	void StartSounds();

	void SelectSounds(int iSelected);

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
	CGeoCylinder GetHitboxGeo()
	{
		return m_HitboxGeo;
	}


	CGeos m_zgsCollisionObjects;
	float m_Xrotation = 0.0f, m_Yrotation = 0.0f;
private:
	CGeoCylinder m_HitboxGeo;
	CBulletManager m_BulletManager;
	CAirplaneModel m_planeModel[7];
	CPlacement m_zpPlaneCenter;
	CPlacement m_zpPlaneTip;
	CHVector m_vDirection;
	
	CAABB m_zHitBox;


	unsigned m_planeID;

	float m_damage;
	float m_flySpeed;
	float m_minFlySpeed;
	float m_maxFlySpeed;
	float m_flySpeedChangeRate;


	
	/// SOUNDS ///
	CAudio m_zaPlaneShot; 
	CAudio m_zaPlaneSound[7];
};