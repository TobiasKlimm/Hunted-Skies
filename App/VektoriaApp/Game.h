#pragma once
//-----------------------------------------------------------------------
// CGame
// 
// Erkl�rung : Eine weitestgehend leere Klasse, 
//             sinnf�llig in eine Windows-Applikation eingebettet,
//             um ein Spiel bzw. eine 3D-Simulation zu erzeugen
// Autor     : Prof. Dr. Tobias Breiner
// Ort       : Pfungstadt
// Zeit      : seit Aug. 2011 (mit seitdem st�ndigen Verbesserungs-Updates)
// Copyright : Tobias Breiner  
// Disclaimer: Nutzung auf eigene Gefahr, keine Gew�hr, no warranty!
//------------------------------------------------------------------------


#ifdef _WIN64
#ifdef _DEBUG
#pragma comment (lib, "Vektoria_Debug64.lib")
#pragma comment (lib, "VektoriaMath_Debug64.lib")
#else
#pragma comment (lib, "Vektoria_Release64.lib")
#pragma comment (lib, "VektoriaMath_Release64.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment (lib, "Vektoria_Debug.lib")
#pragma comment (lib, "VektoriaMath_Debug.lib")
#else
#pragma comment (lib, "Vektoria_Release.lib")
#pragma comment (lib, "VektoriaMath_Release.lib")
#endif
#endif


#include "Vektoria\Root.h"
#include "Terrain.h"

#include "Bot.h"
#include "Turret.h"
#include "Player.h"

using namespace Vektoria;

#define MAX_BOTS 5
#define MAX_TURRETS 10
#define MAX_RANDOMSHIPS 40

class CGame
{
	friend class CPlayer; 
public:
	CGame(void);
	~CGame(void);
	void Init(HWND hwnd, void(*procOS)(HWND hwnd, unsigned int uWndFlags), CSplash* psplash);
	void Tick(float fTime, float fTimeDelta);
	void EndExplosion(float fTime);
	void Fini();
	void WindowReSize(int iNewWidth, int iNewHeight);
	float GetTimeDeltaMin();
	float GetVersion();

private:
	bool m_bTurretDestroyed;
	float m_fBotTime = 0.0f; 
	CRoot m_zr;
	CFrame m_zf;
	CScene m_zs;
	CLightParallel m_zlp2;
	CPlayer m_player;
	CTerrain m_terrain;
	CTurret m_turrets[MAX_TURRETS];
	CBot m_botplanes[MAX_BOTS];
	CGeos m_zgsTerrainCollision;
	CAudio m_zaWingsOfValor;

	// TURRET EXPLOSION
	CPlacement m_zbpExplosion;
	CPlacement m_zpExplosion;
	CGeoQuad m_zgExplosion;
	CMaterial m_zmExplosion;
	int m_iExplosion = 0;
	int Elapsedticks = 0; 

	// CARRIER
	CMaterial m_zmCarrier;
	CPlacement m_zpCarrier;
	CGeoTriangleTable* m_zgCarrier=nullptr;
	CFileWavefront m_zfCarrier;

	// COLLISION SPHERE
	CPlacement m_zpCollisionSphereBlender;
	CGeoTriangleTable* m_zgCollisionSphereBlender = nullptr;
	CFileWavefront m_zfCollisionSphereBlender;
	CGeoSphere m_zgSphereCollision;
	CPlacement m_zpSphereCollision;

	// SHIPS
	CPlacement m_zpDestroyer[4];
	CGeoTriangleTable* m_zgDestroyer;
	CMaterial m_zmDestroyer;
	CFileWavefront m_zfDestroyer;
	float m_vector;
	float m_vecotr1;
	int m_destroyerx=0;
	int m_destroyerx1 = 0;

	CPlacement m_zpRandomShip[MAX_RANDOMSHIPS];
	CGeoTriangleTable* m_zgRandomShip;
	CMaterial m_zmRandomShip;
	CFileWavefront m_zfRandomShip;

	CGeoTriangleTable* m_zgRandomShip1;
	CMaterial m_zmRandomShip1;
	CFileWavefront m_zfRandomShip1;

	CGeoTriangleTable* m_zgRandomShip2;
	CMaterial m_zmRandomShip2;
	CFileWavefront m_zfRandomShip2;

	int m_movefaktor=40;
	
	// EXPLOSIN SOUND
	CAudio m_zaExplosion;
};







