#include "AirplaneModel.h"

void CAirplaneModel::Init(int planeID)
{
	m_planeID = planeID;
	switch (m_planeID)
	{
	case 1:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\F4\\PlaneModel.obj", true);

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\F4\\textures\\Diffuse.png");
		m_zmPlaneMaterial.MakeTextureBump("models\\Airplanes\\F4\\textures\\Normal.png");
		m_zmPlaneMaterial.MakeTextureSpecular("models\\Airplanes\\F4\\textures\\Roughness.png");
		break;
	}
	case 2:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\StandardFlieger\\PlaneModel.obj");
		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\StandardFlieger\\textures\\Diffuse.png");

		m_pzgPropeller1 = objLoader.LoadGeoTriangleTable("models\\Airplanes\\StandardFlieger\\PropellerModel.obj");
		m_zpPropeller1.AddGeo(m_pzgPropeller1);
		m_zmPropellerMaterial.Copy(m_zmPlaneMaterial);
		break;
	}
	case 3:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\ORCA\\PlaneModel.obj");

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\ORCA\\textures\\Diffuse.jpg");
		m_zmPlaneMaterial.MakeTextureBump("models\\Airplanes\\ORCA\\textures\\Normal.jpg");
		m_zmPlaneMaterial.MakeTextureSpecular("models\\Airplanes\\ORCA\\textures\\Roughness.jpg");
		break;
	}
	case 4:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\P-40 Warhawk\\PlaneModel.obj");

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\P-40 Warhawk\\textures\\Diffuse.jpg");
		m_zmPlaneMaterial.MakeTextureBump("models\\Airplanes\\P-40 Warhawk\\textures\\Normal.jpg");
		m_zmPlaneMaterial.MakeTextureSpecular("models\\Airplanes\\P-40 Warhawk\\textures\\Roughness.jpg");

		m_pzgPropeller1 = objLoader.LoadGeoTriangleTable("models\\Airplanes\\StandardFlieger\\PropellerModel.obj");
		m_zpPropeller1.AddGeo(m_pzgPropeller1);
		m_zmPropellerMaterial.Copy(m_zmPlaneMaterial);
		break;
	}
	case 5:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\MiG-35\\PlaneModel.obj");

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\MiG-35\\textures\\Diffuse.png");
		m_zmPlaneMaterial.MakeTextureBump("models\\Airplanes\\MiG-35\\textures\\Normal.png");
		m_zmPlaneMaterial.MakeTextureSpecular("models\\Airplanes\\MiG-35\\textures\\Roughness.png");
		break;
	}
	case 6:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\F-117 Nighthawk\\PlaneModel.obj");

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\F-117 Nighthawk\\textures\\Diffuse.png");
		m_zmPlaneMaterial.MakeTextureBump("models\\Airplanes\\F-117 Nighthawk\\textures\\Normal.png");
		m_zmPlaneMaterial.MakeTextureSpecular("models\\Airplanes\\F-117 Nighthawk\\textures\\Specular.png");
		break;
	}
	}
	this->AddGeo(m_pzgPlane);
	this->AddPlacement(&m_zpPropeller1);
	this->AddPlacement(&m_zpPropeller2);
	m_pzgPlane->SetMaterial(&m_zmPlaneMaterial);
	if(m_pzgPropeller1)
	m_pzgPropeller1->SetMaterial(&m_zmPropellerMaterial);
	if (m_pzgPropeller2)
	m_pzgPropeller2->SetMaterial(&m_zmPropellerMaterial);
}

void CAirplaneModel::Tick(float fTime, float fTimeDelta)
{
	m_zpPropeller1.RotateZ(fTime * 100);
	m_zpPropeller2.RotateZ(fTime * 100);
}
