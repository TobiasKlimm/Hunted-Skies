#pragma once
#include "Vektoria\Root.h"
#include "Airplane.h"
using namespace Vektoria;

#define SHOOT_FREQUENCY 0.2f
#define DAMAGE 0.5f
#define MAX_TURN_RATE 70.0f



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