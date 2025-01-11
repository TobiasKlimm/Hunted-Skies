#include "Bot.h"

void CBot::Init(CPlacement* target)
{
	m_zpTarget = target;
	AddPlacement(&m_airplane);

	m_airplane.Init(DAMAGE,1);
	SetName("BotPlane");
}

void CBot::Tick(float fTime, float fTimeDelta)
{
	m_airplane.Tick(fTime, fTimeDelta);
	ControlPlane(fTimeDelta);
}

void CBot::ControlPlane(float fTimeDelta)
{
	CHVector vDir = m_airplane.GetDirection(); // Normalized direction vector

	// Calculate the direction to the player
	CHVector vToPlayer = m_zpTarget->GetPosGlobal() - m_airplane.GetPosGlobal();
	vToPlayer.Normal(); // Normalize the vector

	// Calculate dot and cross products
	float dot = vDir*vToPlayer; // Dot product for angle
	float angle = acos(ClampValue(dot, -1.0f, 1.0f)); // Angular difference in radians
	CHVector vCross;
	vCross.x = vDir.y * vToPlayer.z - vDir.z * vToPlayer.y;
	vCross.y = vDir.z * vToPlayer.x - vDir.x * vToPlayer.z;
	vCross.z = vDir.x * vToPlayer.y - vDir.y * vToPlayer.x; // Cross product for rotation axis


	vCross.Normal();

	// Calculate yaw (x) and pitch (y) adjustments
	//float x = vCross.y >= 0 ? angle : -angle; // Horizontal adjustment (yaw)
	float y = -vCross.z; ; // Vertical adjustment (pitch)
	float x = vCross.y >= 0 ? -angle : angle;

	// Scale adjustments based on the maximum allowed turn rate and time delta
	x = ClampValue(x, -MAX_TURN_RATE * fTimeDelta, MAX_TURN_RATE * fTimeDelta);
	y = ClampValue(y, -MAX_TURN_RATE * fTimeDelta, MAX_TURN_RATE * fTimeDelta);

	// Move the airplane
	m_airplane.MovePlane(x, y, fTimeDelta);

	if (abs(dot - 1.0f) < 0.0015f) {
		m_timePassed += fTimeDelta;
		// Führe die Funktion aus, während genug Zeit vergangen ist
		if (m_timePassed <= SHOOT_FREQUENCY)
			return;
		m_timePassed = 0.0;
		m_airplane.Shoot(0.01f);
	}
}

float CBot::ClampValue(float value, float minValue, float maxValue) {
	return max(minValue, min(value, maxValue));
}
