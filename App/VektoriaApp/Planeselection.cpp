#include "Planeselection.h"


void Planeselection::Init(CViewport* pviewport, CDeviceCursor * zdc)
{
	AddPlacement(&m_zpAirplane);

	for (int i = 0; i < m_iFlugGeos; i++)
	{
		m_zpModel[i].Init(i);
		m_zpAirplane.AddPlacement(&m_zpModel[i]);
	}
	m_zdc = zdc;
}