#pragma once
#include "Vektoria\Root.h"
#include "Airplane.h"
using namespace Vektoria;

#define CROSSHAIRSIZE 0.003f
#define MINIMAPSIZE 0.02f
#define SHOOT_FREQUENCY 0.05f
#define DAMAGE 10.0f
#define MAX_DISTANCE 2500;

enum Status
{
    eStart,
    eInGame,
    ePaused
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
    CPlacement m_zpCameraBack;
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
    
    //Pausebildschirm
    COverlay m_zoPause; CImage m_ziPause;
    
    //Buttons
    COverlay m_zoButtonStart;
    COverlay m_zoButtonOptionen;

    //Container für Startbildschirm
    COverlays m_zos;
    COverlays m_zosInGame;
    COverlay m_zoBack2Start;
    COverlays m_zosButtons;


    // Bloodscreen
    COverlay m_zo;
    CImage m_zi;

    //------------------------------------------------------------------------
    // Abstandswarnung
    //Abstandsvektor f?r Warnung
    CHVector m_zhvAbstand;
    float m_distanceX = 0;
    float m_distanceY = 0;
    float m_distanceZ = 0;

    //Warnung
    COverlay m_zoAbstand;
    CImage m_ziAbstand;
    float m_WarningTransparency = 1.0;
    float m_bloodTransparency = 1.0;
    float m_lastHealth = 100;

    //You have died screen
    COverlay m_zoDied;
    CImage m_ziDied;

    //Collision Detection
    CHVector m_lastPos;
};