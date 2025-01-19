#pragma once
#include "Vektoria\Root.h"

using namespace Vektoria;

#define MAX_ROTATION_SPEED  0.25f
#define BULLETSPEED 1000
#define DAMAGE  10.0f


class CAirplaneModel : public CPlacement
{
public:
	void Init(int planeID);
	void ReInit(int planeID);
	void Tick(float fTime, float fTimeDelta);

private:
	int m_planeID;
	CFileWavefront objLoader;
	CGeoTriangleTable* m_pzgPlane = nullptr;

	// PROPELLER
	CGeoTriangleTable* m_pzgPropeller = nullptr;
	CPlacement m_zpPropeller1;
	CPlacement m_zpPropeller2;
	CPlacement m_zpPropellerMiddle;

	// ENGINE
	CGeoSphere m_zgEngine;
	CPlacement m_zpEngineL;
	CPlacement m_zpEngineR;

	// ENGINE STEALTH JET
	CGeoCylinder m_zgEngineStealth;
	CPlacement m_zpEngineStealthL;
	CPlacement m_zpEngineStealthR;

	// MATERIALS
	CMaterial m_zmPlaneMaterial;
	CMaterial m_zmPropellerMaterial;
	CMaterial m_zmEngine;
	CMaterial m_zmEngineStealth;
	CEmitter m_zeEngine;
};