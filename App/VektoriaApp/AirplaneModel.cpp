#include "AirplaneModel.h"

void CAirplaneModel::Init(int planeID)
{


	//Triebwerke
	m_zmTriebwerke.LoadPreset("Sun");
	m_zmTriebwerkStealth.LoadPreset("Sun");

	m_zmTriebwerkStealth.SetGlowStrength(3.0);
	m_zmTriebwerke.SetGlowStrength(3.0f);

	m_zgTriebwerkL.Init(0.27, &m_zmTriebwerke, 50, 50);

	m_zgTriebwerkR.Init(0.27, &m_zmTriebwerke, 50, 50);

	m_zgTriebwerkStealthR.Init(0.09, 4, &m_zmTriebwerkStealth, 50, true, true);
	m_zgTriebwerkStealthL.Init(0.09, 4, &m_zmTriebwerkStealth, 50, true, true);
	
	
	
	
	//Flugzeuge
	m_planeID = planeID;
	switch (m_planeID)
	{
	case 1:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\F4\\PlaneModel.obj", true);

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\F4\\textures\\Diffuse.png");
		m_zmPlaneMaterial.MakeTextureBump("models\\Airplanes\\F4\\textures\\Normal.png");
		m_zmPlaneMaterial.MakeTextureSpecular("models\\Airplanes\\F4\\textures\\Roughness.png");

		m_zpTriebwerkL.AddGeo(&m_zgTriebwerkL);
		m_zpTriebwerkR.AddGeo(&m_zgTriebwerkR);
		m_zpTriebwerkL.Translate(0.5, 0.08, 3.5);
		m_zpTriebwerkR.Translate(-0.5, 0.08, 3.5);
		

		

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
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\ORCA\\NewOne.obj");

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\ORCA\\textures\\Diffuse.jpg");
		m_zmPlaneMaterial.MakeTextureBump("models\\Airplanes\\ORCA\\textures\\Normal.jpg");
		m_zmPlaneMaterial.MakeTextureSpecular("models\\Airplanes\\ORCA\\textures\\Roughness.jpg");

		m_zpTriebwerkL.AddGeo(&m_zgTriebwerkL);
		m_zpTriebwerkR.AddGeo(&m_zgTriebwerkR);
		m_zpTriebwerkL.Translate(0.4, 0.4, 5.7);
		m_zpTriebwerkR.Translate(-0.4, 0.4, 5.7);
		break;
	}
	case 4:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\P-40 Warhawk\\PlaneModel.obj",true);

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\P-40 Warhawk\\textures\\Diffuse.jpg");
		m_zmPlaneMaterial.MakeTextureBump("models\\Airplanes\\P-40 Warhawk\\textures\\Normal.jpg");
		m_zmPlaneMaterial.MakeTextureSpecular("models\\Airplanes\\P-40 Warhawk\\textures\\Roughness.jpg");

		m_pzgPropeller1 = objLoader.LoadGeoTriangleTable("models\\Airplanes\\StandardFlieger\\PropellerModel.obj",true);
		m_zpPropeller1.AddGeo(m_pzgPropeller1);
		m_zmPropellerMaterial.Copy(m_zmPlaneMaterial);
	
		break;
	}
	case 5:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\MiG-35\\newone.obj");//Geht nicht

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


		
		m_zpTriebwerkStealthL.AddGeo(&m_zgTriebwerkStealthL);
		m_zpTriebwerkStealthL.Translate(-0.52, -2.0, 5);
		m_zpTriebwerkStealthL.RotateZDelta(0.5 * PI);
		

		


		break;
	}
	case 7:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\B17\\B17Tri.obj");

		//m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\B17\Texture\\B17.jpeg");
		//m_zmPlaneMaterial.MakeTextureBump("models\\Airplanes\\F-117 Nighthawk\\textures\\Normal.png");
		//m_zmPlaneMaterial.MakeTextureSpecular("models\\Airplanes\\F-117 Nighthawk\\textures\\Specular.png");
		break;
	}
	}
	AddGeo(m_pzgPlane);
	AddPlacement(&m_zpPropeller1);
	AddPlacement(&m_zpPropeller2);
	AddPlacement(&m_zpTriebwerkL);
	AddPlacement(&m_zpTriebwerkR);
	AddPlacement(&m_zpTriebwerkStealthL);


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
