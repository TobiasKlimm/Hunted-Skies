#pragma once
#include "Vektoria\Root.h"

using namespace Vektoria;

class CEnemy :
	public CPlacement
{
public:
	bool RegisterHit(float damage);
	CHVector GetPosition() {
		return m_position;
	}
	void UpdatePosition() {
		m_position = GetPosGlobal();
	}
	void SetPosition(CHVector position) {
		m_position = position;
	}
	void SetHealth(float health) {
		m_health = health;
	}
	float GetHealth()
	{
		return m_health;
	}
	void AddHealth(float health)
	{
		m_health += health;
	}
private:
	float m_health =0;
	CHVector m_position;

};