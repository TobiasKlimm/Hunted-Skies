#pragma once
#include "Vektoria\Root.h"

using namespace Vektoria;

class CEnemy :
	public CPlacement
{
public:
	void Init();
	void Waaaa();
	CHVector m_position;

private:
	float m_health;
	float m_damage;
};