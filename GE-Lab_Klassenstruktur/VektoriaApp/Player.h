#pragma once
#include "Vektoria\Root.h"
#include "Airplane.h"
using namespace Vektoria;

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
    CAirplane* GetAirplane()
    {
        return &m_airplane;
    }
private:
    float crosshairSize = 0.002f;


    CViewport m_zv;
    CPlacement m_zpCamera;
    CCamera m_zcCamera;

    CAirplane m_airplane;

    CDeviceCursor m_zdc;
    CDeviceGameController m_zdgc;
    CDeviceMouse m_zdm;
    CDeviceKeyboard m_zdk;

    CWriting m_zw; CWritingFont m_zwf;
    CImage m_ziCrosshair; CImage m_ziCirclehair;
    COverlay m_zoCrosshair; COverlay m_zoCirclehair;
    C2dRect m_zCrosshairRect;
};