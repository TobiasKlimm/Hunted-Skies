#pragma once

#pragma once
#include "Vektoria\Root.h"

using namespace Vektoria;

#define MAX_ROTATION_SPEED  0.25f
#define BULLETSPEED 1000
#define DAMAGE  10.0f


class CAirplaneModel :
	public CPlacement
{
public:
	void Init(int planeID);

	void Tick(float fTime, float fTimeDelta);
private:
	int m_planeID;
	CFileWavefront objLoader;
	CGeoTriangleTable* m_pzgPlane = nullptr;
	CGeoTriangleTable* m_pzgPropeller1 = nullptr;
	CPlacement m_zpPropeller1;
	CGeoTriangleTable* m_pzgPropeller2 = nullptr;
	CPlacement m_zpPropeller2;

	CMaterial m_zmPlaneMaterial;
	CMaterial m_zmPropellerMaterial;
};