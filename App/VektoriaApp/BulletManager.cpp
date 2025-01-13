#include "BulletManager.h"

void CBulletManager::Init(float bulletSpeed, float damage)
{
	m_damage = damage;
	m_bulletSpeed = bulletSpeed;

	m_zmBulletMaterial.LoadPreset("Sun");
	m_zgBullet.Init(0.2f, 0.2f,2.0f, &m_zmBulletMaterial);
	//m_zgBullet.SetMaterial(&m_zmBulletMaterial);
	m_zpBulletTemplate.AddGeo(&m_zgBullet);
	m_zpBulletTemplate.SetPhysics(0.05f, 0.01f, 0.0f, 50.1f, true);
	m_zpBulletTemplate.SwitchOff();
	m_zpsBullets.RingMake(100, m_zpBulletTemplate);
	this->AddPlacement(&m_zpBulletTemplate);

	m_zaShot.Init3D("sounds\\PlaneShot.wav",10);
	//AddAudio(&m_zaShot);
}

void CBulletManager::Tick(float fTime, float fTimeDelta) {
	m_killedEnemy = false;
	UpdateBullets();
}

void CBulletManager::Shoot(const CHVector& direction) {
	if (m_zpsBullets.RingIsFull())
		m_zpsBullets.RingDec();

	m_position.Rotate(direction, GetDirection());
	m_position.CopyTranslation(GetPosGlobal());
	CPlacement* pzp = m_zpsBullets.RingInc();
	// Kopiere die Matrix (und damit die Startposition)
	pzp->SetMat(m_position);
	//schießt die Kugel in die Richtung die das Flugueug zeigt
	pzp->SetPhysicsVelocity(direction * m_bulletSpeed);

	m_zaShot.Start();
}

void CBulletManager::UpdateBullets() {
	//Iteriere durch alle Bullets
	for (unsigned i = 0; i < m_zpsBullets.GetCount(); i++) {
		CPlacement* currentBullet = m_zpsBullets.Get(i);
		CHVector currentPos = currentBullet->GetPosGlobal();
		//Wenn Bullet nicht on, resete Position
		if (currentBullet->IsOn() == false)
			lastPosBullets[i] = CHVector(0, 0, 0, 1);
		//Wenn Bullet on, aber noch keine letzte Position, setze diese
		else if (lastPosBullets[i] == CHVector(0, 0, 0, 1))
			lastPosBullets[i] = currentPos;
		//Sonst
		else {
			//Collision Detection
			CRay m_zrBulletRay;
			m_zrBulletRay.InitFromTo(lastPosBullets[i], currentPos);
			CHitPoint hp; 
			for (unsigned j = 0; j < m_collisionTargets.GetCount(); j++) {
				CPlacement* currentTarget = m_collisionTargets.Get(j);
				CGeo* pzg = (CGeo*)currentTarget->GetChild(0);
				if (currentTarget->IsOn() &&

					pzg->Intersects(m_zrBulletRay, hp)){

//					currentTarget->GetAABB()->Intersects(m_zrBulletRay)) {

					currentBullet->SwitchOff();
					currentTarget->GetParent()->GetName();;
					CEnemy* currentEnemy = static_cast<CEnemy*>(currentTarget);

					if (currentEnemy->RegisterHit(m_damage)) {
						m_killedEnemy = true;
						
					}
				}
			}
			//Wenn Kugeln zu weit weg, switch off
			if (currentPos.Dist(m_position.GetPos()) > 1000.0f)
				currentBullet->SwitchOff();
			//Update letzte Position
			lastPosBullets[i] = currentPos;

			if (m_zgTerrain != nullptr) {
				float yTerrainHeight = m_zgTerrain->GetHeight(currentPos.x, currentPos.z);
				if (yTerrainHeight > currentPos.y)
					currentBullet->SwitchOff();
			}


		/*	float x = currentBullet->GetPosGlobal().x;
			float y = currentBullet->GetPosGlobal().y;
			float z = currentBullet->GetPosGlobal().z;
			float y_terrain = m_zgTerrain->GetHeight(x, z);
			if (y < y_terrain) {
				LogDebug("Hit at %f %f %f", x, y_terrain, z);
			}*/

		}
	}
}

