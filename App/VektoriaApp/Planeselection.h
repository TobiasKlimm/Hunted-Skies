#pragma once
#include "Vektoria\Root.h"
#include "AirplaneModel.h"

using namespace Vektoria;

class Planeselection : public CPlacement
{
	private:
	
	CAirplaneModel m_zpModel[7];

	int m_iFlugGeo = 0;

	int m_iFlugGeos = 0;

	CDeviceCursor* m_zdc;
	CPlacement m_zpFlugzeug;



	public:

		void Init(CViewport * pviewport, CDeviceCursor * zdc);

	
		void Tick(float fTime, float fTimeDelta);



};

