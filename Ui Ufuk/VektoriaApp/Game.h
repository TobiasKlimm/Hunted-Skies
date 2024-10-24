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
	void Init(HWND hwnd, void(*procOS)(HWND hwnd, unsigned int uWndFlags), CSplash * psplash);	

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
	CFrame m_zf;
	CViewport m_zv;
	CScene m_zs;
	CCamera m_zc;
	CPlacement m_zpCamera;
	CImage m_zi; 
	CLightParallel m_zlp;
	CDeviceKeyboard m_zdk;

	//Overlay
	CWritingFont m_zwf;
	CWritingFont m_zwf2;
	CWritingFont m_zwf3;
	CWriting m_zw;
	CWriting m_zw2;
	CWriting m_zw3;




	//Object

	CFileWavefront m_fileWavefront;
	CGeoTriangleTable* m_mpgTest = nullptr;
	CPlacement m_zpTest;


	//Terrain hier rein

	CGeoTerrain m_zgTerrrainOri;	//Gesamtes Terrain
	CGeoTerrain m_zgTerrrain;		//Inselausschnitt
	CGeoTerrain m_zgWater;			//Wasserausschnitt
	CPlacement m_zpLandscape;		//Insel & Wasserplatzierung
	CPerlin* m_pperlin = nullptr;	//Perlin Noise
	CBlob* m_mpblob = nullptr;		//Erhebung aus dem Meer
	CBlob* m_mpblob2 = nullptr;		
	CBlob* m_mpblob3 = nullptr;
	CBlob* m_mpblob4 = nullptr;
	CCut m_cutUnderSea;				//Schnitt unter N.N
	CMaterial m_zmWater;			//Wellen
	CMaterial m_zmEarth;			//Erde
	CGeos m_zgsCol, m_zgsHeight;	//Kollisionscontainer
	CGeoTerrains m_zgsTerrain;		//Terrainkollisionskontainer

	

	// Hier ist Platz für Deine weiteren Vektoriaobjekte:


};


