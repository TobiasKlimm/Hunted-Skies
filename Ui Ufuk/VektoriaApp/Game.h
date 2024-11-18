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


	//---------------------------------------------------------------------
	//STEUERUNG
	//Zentriert ein Quadrat mit den Koordinaten x und y als linkes oberes Eck
	void CenterSquare(float x, float y, float size, COverlay& rect);

	//Steuerung des Flugzeugs
	void PlaneSteering(float& x, float& y, float fTimeDelta);

	//Gibt den value maximal als maxValue und mindestens als minValue zurück
	float ClampValue(float value, float minValue, float maxValue);

	//Größe des Crosshairs und des Kreises in Abhängigkeit des Bildschirms
	float crosshairSize = 0.002;
	//Schnelligkeit der Flugzeugrotation
	float planeRotationSpeed = 0.0005;
	//Sensitivität des Controllers
	float controllerSensitivity = 1500;


private:

	//---------------------------------------------------------------------
	//Steuerung
	float x_initial = 0;
	float y_initial = 0;
	float x_rotation = 0;
	float y_rotation = 0;
	CDeviceCursor m_zdc;
	CDeviceMouse m_zdm;
	CDeviceGameController m_zdgc;
	CDeviceKeyboard m_zdk;


	//---------------------------------------------------------------------
	// Die Wurzel des Szenengrafen: 
	CRoot m_zr; 
	CFrame m_zf;
	CViewport m_zv;
	CScene m_zs;
	CCamera m_zc;
	CPlacement m_zpCamera;
	CImage m_zi; 
	CLightParallel m_zlp;


	//---------------------------------------------------------------------
	//Overlay
	CWritingFont m_zwf;
	CWritingFont m_zwf2;
	CWritingFont m_zwf3;
	CWriting m_zw;
	CWriting m_zw2;
	CWriting m_zw3;
	CCamera m_zc2;
	CPlacement m_zpCamera2;

	//---------------------------------------------------------------------
	//Flugzeug
	CPlacement m_zpPlane;
	CGeoTriangleTable* m_pzgPlane = nullptr;
	CFileWavefront m_PlaneFile;
	CPlacement m_zpPlaneCenter;
	CPlacement m_zpPlaneTip;
	CPlacement m_zpCameraPivot;


	//---------------------------------------------------------------------
	//Crosshair
	CImage m_ziCrosshair; CImage m_ziCirclehair;
	COverlay m_zoCrosshair; COverlay m_zoCirclehair;
	C2dRect m_zCrosshairRect;


	//---------------------------------------------------------------------
	//Object
	CFileWavefront m_fileWavefront;
	CGeoTriangleTable* m_mpgTest = nullptr;
	CPlacement m_zpTest;


	//---------------------------------------------------------------------
	//MiniMap
	CViewport m_zv2;
	CPlacement m_zp2;
	CCamera m_zcMiniMap;
	CPlacement m_zpMiniMap;
	CImage m_ziMap;
	COverlay m_zoMap;
	C2dRect m_z2dMap;
	
	//---------------------------------------------------------------------
	//Startbildschrim
	CImage m_ziStart;
	COverlay m_zoStart;

	//---------------------------------------------------------------------
	//Button
	CImage m_ziButton;
	COverlay m_zoButton;
	C2dRect m_z2dButton;

	CImage m_ziButton2;
	COverlay m_zoButton2;
	C2dRect m_z2dButton2;

	//---------------------------------------------------------------------
	//Menutext auf den Buttons
	/*CWritingFont m_zwfm1;
	CWritingFont m_zwfm2;
	CWriting m_zwm1;
	CWriting m_zwm2;
	*/

	//Transparente Buttons
	CImage m_ziButtonTrans1;
	COverlay m_zoButtonTrans1;
	C2dRect m_z2dButtonTrans1;

	CImage m_ziButtonTrans2;
	COverlay m_zoButtonTrans2;
	C2dRect m_z2dButtonTrans2;

	//---------------------------------------------------------------------
	//Pause
	float m_fTimeLastPausingStart = 0.0f;
	float m_fTimePausings = 0.0f;
	float m_fTimeWithPausings = 0.0f;
	bool m_bPaused = true; 

	//---------------------------------------------------------------------
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


