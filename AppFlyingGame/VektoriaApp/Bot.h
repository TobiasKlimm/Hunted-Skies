#pragma once
#include "Vektoria\Root.h"
#include "Airplane.h"
using namespace Vektoria;

#define SHOOT_FREQUENCY 0.1f
#define DAMAGE 1.0f



class CBot :
    public CPlacement
{
public:
    void Init(CPlacement* target);

	void Tick(float fTime, float fTimeDelta);

	void ControlPlane(float fTimeDelta);

	float ClampValue(float value, float minValue, float maxValue);

	CAirplane* GetAirplane()
	{
		return &m_airplane;
	}
private:
	float m_timePassed = 0.0;

	CAirplane m_airplane;
	CPlacement* m_zpTarget;

	CHVector m_vDir;
	CHVector m_vDirNormal;
};