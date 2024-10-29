#pragma once

//-----------------------------------------------------------------------
// CGame
// 
// Erklärung : Eine weitestgehend leere Klasse, 
//             sinnfällig in eine Windows-Applikation eingebettet,
//             um ein Spiel bzw. eine 3D-Simulation zu erzeugen
// Autor     : Prof. Dr. Tobias Breiner
// Ort       : Pfungstadt
// Zeit      : seit Aug. 2011 (mit seitdem ständigen Verbesserungs-Updates)
// Copyright : Tobias Breiner  
// Disclaimer: Nutzung auf eigene Gefahr, keine Gewähr, no warranty!
//------------------------------------------------------------------------
#define TERRAIN_HEIGHT 500
#define TERRAIN_SIZE 2500
#define WATER_SIZE 20000
#define TERRAIN_BEACHLIMIT 12
#define TERRAIN_SANDLIMIT 4
#define PERLIN_SEED 42

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

using namespace Vektoria;


class CGame
{
public:
	// Wird vor Begin einmal aufgerufen (Konstruktor):
	CGame(void);

	// Wird nach Ende einmal aufgerufen (Destruktor):
	~CGame(void);

	// Wird zu Begin einmal aufgerufen:
	void Init(HWND hwnd, void(*procOS)(HWND hwnd, unsigned int uWndFlags), CSplash* psplash);

	// Wird während der Laufzeit bei jedem Bildaufbau aufgerufen:
	void Tick(float fTime, float fTimeDelta);

	// Wird am Ende einmal aufgerufen:
	void Fini();

	// Wird immer dann aufgerufen, wenn der Benutzer die Fenstergröße verändert hat:
	void WindowReSize(int iNewWidth, int iNewHeight);

	// Holt das minimale Zeitdelta zur eventuellen Frame-Rate-Beschränkung:
	float GetTimeDeltaMin();

	// Holt die Versionsnummer:
	float GetVersion();


private:

	// Die Wurzel des Szenengrafen: 
	CRoot m_zr;
	CScene m_zs;
	CFrame m_zf;
	CViewport m_zv;
	CCamera m_zc;
	CDeviceKeyboard m_zdk;
	CPlacement m_zpCamera;
	CGeoTerrain m_zgTerrainOri; // Gesamtes Terrain
	CGeoTerrain m_zgTerrain; // Inselausschnitt
	CGeoTerrain m_zgWater;// Wasserausschnitt



	CMaterial m_zmRock;
	CMaterial m_zmSnow;
	CMaterial m_zmSand;



	CPlacement m_zpLandscape; // Insel- & Wasserplacement
	CPerlin* m_pperlin = nullptr;// Perlin Noise
	CPerlin* m_pperlin1 = nullptr;




	//Placements

	CFileWavefront m_filewavefront;
	CGeoTriangleTable* m_pgtriangletable = nullptr;
	CPlacement m_zpCannon;




	CFileWavefront m_filewavefront1;
	CGeoTriangleTable* m_pgtriangletable1 = nullptr;
	CPlacement m_zpShip;



	//CGeoTerrain m_zgTerrainMirror;
	//CGeoTerrain m_zgTerrainMirrorLOD1;




	//Insel Test
	CPlacement m_zpHauptInsel;
	CPlacement m_zpIsland1;
	CPlacement m_zpIsland2;
	CPlacement m_zpIsland3;
	CBlob* m_pblobAllPositive = nullptr;
	CBlob* m_pblobAllNegative = nullptr;
	CBlob* m_pblobTerrainPositive = nullptr;
	CBlob* m_pblobTerrainNegative = nullptr;
	CBlob* m_pblobTerrainCraterPositive = nullptr;


	CBlob* m_pblob9 = nullptr;
	CBlob* m_pblob8 = nullptr;
	CBlob* m_pblob6 = nullptr;
	CBlob* m_pblob7 = nullptr;
	CBlob* m_pblob5 = nullptr;
	CBlob* m_pblob3 = nullptr;
	CBlob* m_pblob4 = nullptr;
	CBlob* m_pblob2 = nullptr;
	CBlob* m_pblob1 = nullptr;
	CBlob* m_pblob = nullptr;
	CBlob* m_pblobAll = nullptr;


	// Erhebung aus dem Meer
	CCut m_cutUnderSea; // Schnitt unter N.N.
	CMaterial m_zmWater; // Wellenmaterial
	CMaterial m_zmEarth; // Erdmaterial
	CGeos m_zgsCollision, m_zgsHeight; // Kollisionscontainer
	CGeoTerrains m_zgsTerrain; // Terrainkollisionscontainer




	CCut m_cutOverSea;
	CCut m_cutSeaToSand;
	CCut m_cutSandtoBeach;
	CCut m_cutSeaToBeach;



	// Hier ist Platz für Deine weiteren Vektoriaobjekte:


};


