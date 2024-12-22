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
	
	
	//Propeller
	CGeoTriangleTable* m_pzgPropeller1 = nullptr;
	CPlacement m_zpPropeller1;
	CGeoTriangleTable* m_pzgPropeller2 = nullptr;
	CPlacement m_zpPropeller2;
	
	//Triebwerk
	CGeoSphere m_zgTriebwerkL;
	CGeoSphere m_zgTriebwerkR;
	CPlacement m_zpTriebwerkL;
	CPlacement m_zpTriebwerkR;


	//Triebwerke Stealth Jet

	CGeoCylinder m_zgTriebwerkStealthL;
	CGeoCylinder m_zgTriebwerkStealthR;

	CPlacement m_zpTriebwerkStealthL;
	CPlacement m_zpTriebwerkStealthR;
	

	//Materials
	CMaterial m_zmPlaneMaterial;
	CMaterial m_zmPropellerMaterial;
	CMaterial m_zmTriebwerke;
	CMaterial m_zmTriebwerkStealth;
	CEmitter m_zeTriebwerke;
};