#pragma once
#include "Vektoria\Root.h"
#include "BulletManager.h"

using namespace Vektoria;

#define MAX_ROTATION_SPEED  0.25


class CAirplane :
	public CPlacement
{
public:
	CAirplane();
	~CAirplane();

	void Init();

	void Tick(float fTime, float fTimeDelta);

	float ClampValue(float value, float minValue, float maxValue);

	void MovePlane(float& x, float& y, float fTimeDelta);

	void Shoot(float fTimeDelta);

	void SetSpeed(float speed);

	float GetSpeed();

	CPlacement* GetCenter()
	{
		return &m_zpPlaneCenter;
	}
	CBulletManager* GetBulletManager()
	{
		return &m_BulletManager;
	}
	
	CGeos m_zgsCollisionObjects;

private:
	CBulletManager m_BulletManager;
	CPlacement m_zpPlaneModel;
	CGeoTriangleTable* m_pzgPlane = nullptr;
	CPlacement m_zpPropellerModel;
	CGeoTriangleTable* m_pzgPropeller = nullptr;

	CPlacement m_zpPlane;
	CPlacement m_zpPlaneCenter;
	CPlacement m_zpPlaneTip;
	CMaterial m_zmPlane;
	CMaterial m_zmPropeller;


	float m_flySpeed = 150;


};