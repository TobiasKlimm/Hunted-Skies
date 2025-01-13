#pragma once
#include "Vektoria\Root.h"
#include "Airplane.h"
#include "Planeselection.h"
using namespace Vektoria;

#define CROSSHAIRSIZE 0.003f
#define MINIMAPSIZE 0.02f
#define SHOOT_FREQUENCY 0.05f
#define DAMAGE 10.0f
#define MAX_DISTANCE 2500
#define CONTROLLER_SENSITIVITY 220

enum Status
{
    eStart,
    eInGame,
    ePaused,
    eSelection,
    eGameOver
};

class CGame; 
class CPlayer :
    public CPlacement
{
public:
    //pack dann in Init::: CGeos* CollisionObjects, CGeoTerrains* CollsisionTerrains
    void Init(CGame * pgame);

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
    CDeviceGameController* GetGameController()
    {
        return &m_zdgc;
    }
    CAirplane* GetAirplane()
    {
        return &m_airplane;
    }

    Status m_zeStatus = eStart;
    Status m_zeStatusLast = eStart;

private:
    CViewport m_zv;
    CPlacement m_zpCamera;
    CPlacement m_zpCameraBack;
    CCamera m_zcCamera;
    float m_timePassed = 0.0;

    unsigned m_score = 0;

    CAirplane m_airplane;

    std::fstream m_scoreFile;
    unsigned m_highscore;

    float x = 0, y = 0;


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
    CWriting m_zwHighScore;
    CWriting m_zw2;
    CWriting m_zw3;
    CWriting m_zwFuel;

    // 
    //---------------------------------------------------------------------
    //Startbildschrim
    COverlay m_zoStart; CImage m_ziStart;
    COverlay m_zoButtonStart;
 //   COverlay m_zoButtonOptions;
    COverlay m_zoButtonPlaneSelection;

    CMaterial m_zmButtonStart;


    //Pausebildschirm
    COverlay m_zoPause; CImage m_ziPause;
    COverlay m_zoButtonGoOn; 

    // GameOver-Bildschirm
    COverlay m_zoGameOver;
    COverlay m_zoRestart;
    COverlay m_zoEnd;
   



    //Planeselection-Bildschirm
    Planeselection m_zPlaneselecion;
    COverlay m_zoPlaneSelection;

    COverlay m_zoNext;
    COverlay m_zoPrev;
    COverlay m_zoBack;


    // In-Game-Overlays:
    COverlay m_zoBack2Start;
    CPlacement m_zpPlaneSelection;
    CPlacement m_zpPlane2Select; 

    CAirplaneModel m_zpModel[7];

    int m_iFlugGeo = 0;

    int m_iFlugGeos = 7;






    //Container fuer Startbildschirm
    COverlays m_zosStart;
    COverlays m_zosInGame;
    COverlays m_zosPause;
    COverlays m_zosPlaneSelection; 
    COverlays m_zosGameOver;


    CHMat m_mLastCamPos; 


    CGame *   m_pgame = nullptr; 


    // Bloodscreen
    COverlay m_zo;
    CImage m_zi;

    //------------------------------------------------------------------------
    // Abstandswarnung
    //Abstandsvektor fuer Warnung
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

    //Zoom
    float m_zoom = 0;

    // Fuel
    float m_fuel = 100;

    //Andere Cursor
    CImage m_ziPickingCursor;
    CImage m_ziEasterCursor;
    CImage m_ziFlugzeugCursor;

    COverlay m_zoPickingCursor;
    COverlay m_zoEasterCursor;
    COverlay m_zoFlugzeugCursor;
};