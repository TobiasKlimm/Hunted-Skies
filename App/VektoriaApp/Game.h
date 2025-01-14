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
	// Wird vor Begin einmal aufgerufen (Konstruktor):
	CGame(void);

	// Wird nach Ende einmal aufgerufen (Destruktor):
	~CGame(void);

	// Wird zu Begin einmal aufgerufen:
	void Init(HWND hwnd, void(*procOS)(HWND hwnd, unsigned int uWndFlags), CSplash* psplash);

	// Wird w�hrend der Laufzeit bei jedem Bildaufbau aufgerufen:
	void Tick(float fTime, float fTimeDelta);

	void EndExplosion(float fTime);

	// Wird am Ende einmal aufgerufen:
	void Fini();

	// Wird immer dann aufgerufen, wenn der Benutzer die Fenstergr��e ver�ndert hat:
	void WindowReSize(int iNewWidth, int iNewHeight);

	// Holt das minimale Zeitdelta zur eventuellen Frame-Rate-Beschr�nkung:
	float GetTimeDeltaMin();

	// Holt die Versionsnummer:
	float GetVersion();

	void ScreenOverlaysInit(CViewport* m_zv);

private:
	bool m_bTurretDestroyed;
	float m_fBotTime = 0.0f; 
	CRoot m_zr;
	CFrame m_zf;
	CScene m_zs;




	// 2. Szenenobjekte: 
	/*
	CScene m_zsPlaneSelection;
	CPlacement m_zpPlane2; 
	CPlacement m_zpCamera2;
	CCamera m_zc2;
	CLightParallel m_zl2; 
	CViewport m_zvPlaneSelection; 
	*/

	CLightParallel m_zlp2; 
	CGeoSphere m_zgBlackSphere; 
	CPlacement m_zpBlackSphere; 
	CMaterial m_zmBlackSphere; 




	CPlayer m_player;
	CTerrain m_terrain;

	CTurret m_turrets[MAX_TURRETS];
	CBot m_botplanes[MAX_BOTS];

	CGeos m_zgsTerrainCollision;

	//CAudio m_zaTrackOne;
	CAudio m_zaWingsOfValor;

	////
	//// Turret Explposion
	////
	CPlacement m_zbpExplosion;
	CPlacement m_zpExplosion;
	CGeoQuad m_zgExplosion;
	CMaterial m_zmExplosion;
	int m_iExplosion = 0;
	int Elapsedticks = 0; 


	//Carrier
	CMaterial m_zmCarrier;
	CPlacement m_zpCarrier;
	CGeoTriangleTable* m_zgCarrier=nullptr;
	CFileWavefront m_zfCarrier;



	//Collision Kugel Blender
	CPlacement m_zpCollisionKugelBlender;
	CGeoTriangleTable* m_zgCollisionKugelBlender = nullptr;
	CFileWavefront m_zfCollisionKugelBlender;


	//Collision Kugel
	CGeoSphere m_zgSphereCollision;
	CPlacement m_zpSphereCollision;

	//Ships
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
	
	//SOUNDS
	CAudio m_zaExplosion;

};







