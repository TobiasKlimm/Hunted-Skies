#pragma once
#include "Vektoria\Root.h"
#include "Airplane.h"
#include "Planeselection.h"
using namespace Vektoria;

#define CROSSHAIRSIZE 0.003f
#define MINIMAPSIZE 0.02f
#define SHOOT_FREQUENCY 0.05f
#define DAMAGE 10.0f

enum Status
{
    eStart,
    eInGame,
    ePaused,
    eSelection,
    eOptions
};


class CPlayer :
    public CPlacement
{
public:
    //pack dann in Init::: CGeos* CollisionObjects, CGeoTerrains* CollsisionTerrains
    void Init();

    void Tick(float fTime, float fTimeDelta);

    void InitCam();

    void ControlPlane(float fTimeDelta);

    void CenterSquare(float x, float y, float size, COverlay& rect);

    float ClampValue(float value, float minValue, float maxValue);

    CViewport* GetViewport()
    {
        return &m_zv;
    }

    CViewport* GetViewportMinimap()
    {
        return &m_zvMinimap;
    }

    CCamera* GetCamera()
    {
        return &m_zcCamera;
    }

    CPlacement* GetCameraPlacement()
    {
        return &m_zpCamera;
    }

    CDeviceMouse* GetMouse()
    {
        return &m_zdm;
    }
    CDeviceKeyboard* GetKeyboard()
    {
        return &m_zdk;
    }
    CDeviceCursor* GetCursor()
    {
        return &m_zdc;
    }
    CAirplane* GetAirplane()
    {
        return &m_airplane;
    }

    Status m_zeStatus = eStart;

private:
    CViewport m_zv;
    CPlacement m_zpCamera;
    CCamera m_zcCamera;
    float m_timePassed = 0.0;

    unsigned m_score = 0;

    CAirplane m_airplane;

    //Input
    CDeviceCursor m_zdc;
    CDeviceGameController m_zdgc;
    CDeviceMouse m_zdm;
    CDeviceKeyboard m_zdk;

    //---------------------------------------------------------------------
    //Minimap
    CViewport m_zvMinimap;
    CCamera m_zcMinimap;
    CPlacement m_zpMinimap;

    //---------------------------------------------------------------------
    //Overlay
    COverlay m_zoCirclehair; CImage m_ziCirclehair;
    COverlay m_zoCrosshair; CImage m_ziCrosshair;
    C2dRect m_zCrosshairRect;

    //Stats
    CWritingFont m_zwf;
    CWriting m_zwScore;
    CWriting m_zw2;
    CWriting m_zw3;

    //Pause
    //---------------------------------------------------------------------
    //Startbildschrim
    COverlay m_zoStart; CImage m_ziStart;
    COverlay m_zoButtonStart;
    COverlay m_zoButtonOptions;
    COverlay m_zoButtonPlaneSelection;

    //Pausebildschirm
    COverlay m_zoPause; CImage m_ziPause;
    COverlay m_zoButtonGoOn; 

    // Optionenbildschirm
    COverlay m_zoOptions;


    //Planeselection-Bildschirm
    Planeselection m_zPlaneselecion;
    COverlay m_zoPlaneSelection;
   
    // In-Game-Overlays:
    COverlay m_zoBack2Start;

    //Container für Startbildschirm
    COverlays m_zosStart;
    COverlays m_zosInGame;
    COverlays m_zosPause;
    COverlays m_zosPlaneSelection; 
    COverlays m_zosOptions;




    //---------------------------------------------------------------------
    //Pause
    //float m_fTimeLastPausingStart = 0.0f;
    //float m_fTimePausings = 0.0f;
    //float m_fTimeWithPausings = 0.0f;
};