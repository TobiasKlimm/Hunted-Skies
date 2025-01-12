#include "Enemy.h"

bool CEnemy::RegisterHit(float damage)
{
	LogDebug("%s HIT, Health: %f",this->GetName(),(m_health-damage));
	m_health -= damage;
	if (m_health <= 0) {
		m_health = 0;
		LogDebug("KILL");
		SwitchOff();
		return true;
	}
	else
		return false;
}
