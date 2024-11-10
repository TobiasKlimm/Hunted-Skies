#pragma once
#define TURRET_COUNT 6

#include "Vektoria\Root.h"

using namespace Vektoria;
class Turret
{
public:
	/// Turrets
	CFileWavefront m_TurretFile;
	CGeoTriangleTable* m_zgTurret = nullptr;
	CPlacement m_zpTurret;
	CPlacement m_zpTurretPointing;

	/// Bullets
	CPlacements m_zpsBullets;
	CPlacement m_zpBulletTemplate;
	CGeoSphere m_zgBullet;
	CMaterial m_zmBullet;
};

