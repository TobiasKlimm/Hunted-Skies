#include "Enemy.h"

bool CEnemy::RegisterHit(float damage)
{
	LogDebug("%s HIT, Health: %f",this->GetName(),m_health);
	m_health -= damage;
	if (m_health <= 0) {
		//explosion
		LogDebug("KILL");
		SwitchOff();
		m_health = 100;
		return true;
	}
	else
		return false;
}
