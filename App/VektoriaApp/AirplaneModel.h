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

	void ReInit(int planeID);

	void Tick(float fTime, float fTimeDelta);
private:
	int m_planeID;
	CFileWavefront objLoader;
	CGeoTriangleTable* m_pzgPlane = nullptr;
	
	
	//Propeller
	CGeoTriangleTable* m_pzgPropeller = nullptr;
	CPlacement m_zpPropeller1;
	CPlacement m_zpPropeller2;
	CPlacement m_zpPropellerMiddle;
	
	//Triebwerk
	CGeoSphere m_zgTriebwerk;
	CPlacement m_zpTriebwerkL;
	CPlacement m_zpTriebwerkR;

	//Triebwerke Stealth Jet
	CGeoCylinder m_zgTriebwerkStealth;
	CPlacement m_zpTriebwerkStealthL;
	CPlacement m_zpTriebwerkStealthR;

	//Materials
	CMaterial m_zmPlaneMaterial;
	CMaterial m_zmPropellerMaterial;
	CMaterial m_zmTriebwerke;
	CMaterial m_zmTriebwerkStealth;
	CEmitter m_zeTriebwerke;



	
};