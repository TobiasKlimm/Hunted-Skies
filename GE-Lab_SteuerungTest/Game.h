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
	float x_initial = 0;
	float y_initial = 0;
	float x_rotation = 0;
	float y_rotation = 0;

	CRoot m_zr;
	CFrame m_zf;
	CViewport m_zv;
	CDeviceKeyboard m_zdk;
	CDeviceCursor m_zdc;
	CDeviceMouse m_zdm;
	CDeviceGameController m_zdgc;


	CScene m_zs;
	CPlacement m_zpCamera;
	CCamera m_zc;

	CPlacement m_zpSphere;
	CGeoSphere m_zgSphere;

	CPlacement m_zpSphere2;
	CGeoSphere m_zgSphere2;

	CPlacement m_zpTerrain;
	CGeoTriangleTable* m_pzgTerrain = nullptr;
	CFileWavefront m_TerrainFile;

	CPlacement m_zpPlane;
	CGeoTriangleTable* m_pzgPlane = nullptr;
	CFileWavefront m_PlaneFile;

	CPlacement m_zpPlaneCenter;
	CPlacement m_zpPlaneTip;
	CPlacement m_zpCameraPivot;

	CWriting m_zw; CWritingFont m_zwf;
	CImage m_ziCrosshair; CImage m_ziCirclehair;
	COverlay m_zoCrosshair; COverlay m_zoCirclehair;
	C2dRect m_zCrosshairRect;

	CPlacement m_zpBulletTemplate;
	CGeoSphere m_zgBullet;
	CPlacements m_zpsBullets;
};