#include "Planeselection.h"


void Planeselection::Init(CViewport* pviewport, CDeviceCursor * zdc)
{
	AddPlacement(&m_zpFlugzeug);

	for (int i = 0; i < m_iFlugGeos; i++)
	{
		m_zpModel[i].Init(i);
		m_zpFlugzeug.AddPlacement(&m_zpModel[i]);
	}




	m_zdc = zdc;


}

void Planeselection::Tick(float fTime, float fTimeDelta)
{



}




