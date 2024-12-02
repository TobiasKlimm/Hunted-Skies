#include "Airplane.h"

CAirplane::CAirplane()
{

}

CAirplane::~CAirplane()
{
	delete m_pzgPlane;
	delete m_pzgPropeller;
}

void CAirplane::Init()
{
	this->AddPlacement(&m_zpPlaneCenter);
	CFileWavefront objLoader;

	


	m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Flugzeug 2\\ORCA_4.5GEN_JET.obj");
	m_zpPlane.AddGeo(m_pzgPlane);
	CHMat o;
	o.RotateY(PI);
	m_pzgPlane->Transform(o);
	m_zpPlane.AddGeo(m_pzgPlane);


	m_zpPlane.ScaleDelta(1);
	m_zmPlane.MakeTextureDiffuse("models\\Flugzeug 2\\ORCA_Full_body_Textures\\ORCA_Full_body_Diffuse_map_.jpg");
	m_zmPlane.MakeTextureBump("models\\Flugzeug 2\\ORCA_Full_body_Textures\\ORCA_FULL_BODY_Normal_map.jpg");
	m_zmPlane.MakeTextureSpecular("models\\Flugzeug 2\\ORCA_Full_body_Textures\\ORCA_Full_body_Speculer_map.jpg");



	//m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Flugzeug\\Flugzeug.obj");
	//CHMat m;
	//m.RotateY(PI);
	//m_pzgPlane->Transform(m);
	//m_zpPlane.AddGeo(m_pzgPlane);
	//m_zpPlane.ScaleDelta(1);
	//m_zmPlane.MakeTextureDiffuse("models\\Flugzeug\\COLOR.png");
	//m_pzgPlane->SetMaterial(&m_zmPlane);




	//m_pzgPropeller = objLoader.LoadGeoTriangleTable("models\\Flugzeug\\Flugzeugpropeller.obj", true);
	//m_zpPropellerModel.AddGeo(m_pzgPropeller);
	//CHMat n;
	//n.RotateY(PI);
	//m_pzgPropeller->Transform(n);
	//m_zpPropellerModel.AddGeo(m_pzgPropeller);
	//m_zpPropellerModel.ScaleDelta(1);
	//m_zmPropeller.MakeTextureDiffuse("models\\Flugzeug\\metal_plate_02_diff_4k.jpg");
	//m_pzgPropeller->SetMaterial(&m_zmPlane);

	









	m_zpPlane.AddPlacement(&m_zpPlaneModel);
	m_zpPlane.AddPlacement(&m_zpPropellerModel);

	m_zpPlaneTip.TranslateZDelta(-2);
	m_zpPlaneTip.TranslateYDelta(-0.4f);
	m_zpPlaneTip.RotateXDelta(-PI / 14);

	m_zpPlaneCenter.AddPlacement(&m_zpPlane);
	m_zpPlaneCenter.AddPlacement(&m_zpPlaneTip);

	m_zpPlaneTip.AddPlacement(&m_BulletManager);
	m_BulletManager.Init(0.05, 1000);
}

void CAirplane::Tick(float fTime, float fTimeDelta)
{
	m_BulletManager.Tick(fTime, fTimeDelta);
	m_zpPropellerModel.RotateZ(fTime * 100);
	this->SetTranslationSensitivity(m_flySpeed);
}

float CAirplane::ClampValue(float value, float minValue, float maxValue) {
	return max(minValue, min(value, maxValue));
}

void CAirplane::MovePlane(float& x, float& y, float fTimeDelta)
{
	static float x_rotation = 0, y_rotation = 0;

	//minimiert die maximale Rotation pro Tick
	x_rotation = ClampValue(x, x_rotation - MAX_ROTATION_SPEED * fTimeDelta, x_rotation + MAX_ROTATION_SPEED * fTimeDelta);
	y_rotation = ClampValue(y, y_rotation - MAX_ROTATION_SPEED * fTimeDelta, y_rotation + MAX_ROTATION_SPEED * fTimeDelta);

	m_zpPlane.RotateY(-x_rotation / 2);
	m_zpPlane.RotateZDelta(-x_rotation * 5);
	m_zpPlane.RotateXDelta(-y_rotation);
	//Move Plane and Camera
	float RotationX = x * 15;
	float RotationY = -y * 15;
	float MoveAD = 0, MoveWS = -1, MoveUD = 0;
	this->MoveWithCollisionDetection(fTimeDelta, true, MoveAD, MoveWS, MoveUD, RotationX, RotationY, m_zgsCollisionObjects);
}

void CAirplane::Shoot(float fTimeDelta)
{
	m_BulletManager.Shoot(m_zpPlaneTip.GetDirectionGlobal(), fTimeDelta);
}

void CAirplane::SetSpeed(float speed)
{
	m_flySpeed = speed;
}

float CAirplane::GetSpeed()
{
	return m_flySpeed;
}
