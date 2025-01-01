#include "Planeselection.h"


void Planeselection::Init(CViewport* pviewport, CDeviceCursor * zdc)
{
	AddPlacement(&m_zpFlugzeug);

	for (int i = 0; i < m_iFlugGeos; i++)
	{
		m_zpModel[i].Init(i);
		m_zpFlugzeug.AddPlacement(&m_zpModel[i]);
	}

	m_zoPrev.SetTexts("Prev", "Prev");
	m_zoNext.SetTexts("Next", "Next");
	m_zoPrev.SetColorWriting(CColor(eColor_White));
	m_zoNext.SetColorWriting(CColor(eColor_White));
	m_zoPrev.SetColor(CColor(eColor_Black));
	m_zoNext.SetColor(CColor(eColor_Black));
	m_zoPrev.Init(C2dRect(0.3f, 0.1f, 0.05f, 0.05f), 4);
	m_zoNext.Init(C2dRect(0.3f, 0.1f, 0.45f, 0.05f), 4);

	pviewport->AddOverlay(&m_zoPrev);
	pviewport->AddOverlay(&m_zoNext);



	m_zdc = zdc;


}

void Planeselection::Tick(float fTime, float fTimeDelta)
{
	if (m_zoNext.IsDown(*m_zdc))
	{
		m_iFlugGeo++;
		m_iFlugGeo %= m_iFlugGeos;
		for (int i = 0; i < m_iFlugGeos; i++)
			m_zpModel[i].SwitchOff();
		m_zpModel[m_iFlugGeo].SwitchOn();
	}
	if (m_zoPrev.IsDown(*m_zdc))
	{
		m_iFlugGeo--;
		if (m_iFlugGeo < 0)
			m_iFlugGeo = m_iFlugGeos - 1;
		for (int i = 0; i < m_iFlugGeos; i++)
			m_zpModel[i].SwitchOff();
		m_zpModel[m_iFlugGeo].SwitchOn();
	}




}




