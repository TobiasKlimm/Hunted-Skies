#include "AirplaneModel.h"

void CAirplaneModel::Init(int planeID)
{
	//Triebwerke
	m_zmTriebwerke.LoadPreset("Sun");
	//m_zmTriebwerkStealth.LoadPreset("Sun");
	//m_zmTriebwerkStealth.SetGlowStrength(3.0);
	m_zmTriebwerke.SetGlowStrength(3.0f);

	m_zgTriebwerk.Init(0.27, &m_zmTriebwerke, 50, 50);

	m_zgTriebwerkStealth.Init(0.09, 4, &m_zmTriebwerke, 50, true, true);
	
	
	//Flugzeuge
	m_planeID = planeID;
	switch (m_planeID)
	{
	case 0:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\F4\\PlaneModel.obj", true);

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\F4\\textures\\Diffuse.png");
		m_zmPlaneMaterial.MakeTextureBump("models\\Airplanes\\F4\\textures\\Normal.png");
		m_zmPlaneMaterial.MakeTextureSpecular("models\\Airplanes\\F4\\textures\\Roughness.png");

		m_zpTriebwerkL.AddGeo(&m_zgTriebwerk);
		m_zpTriebwerkR.AddGeo(&m_zgTriebwerk);
		m_zpTriebwerkL.Translate(0.5, 0.08, 3.5);
		m_zpTriebwerkR.Translate(-0.5, 0.08, 3.5);
		

		

		break;
	}
	case 1:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\StandardFlieger\\PlaneModel.obj");
		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\StandardFlieger\\textures\\Diffuse.png");

		m_pzgPropeller = objLoader.LoadGeoTriangleTable("models\\Airplanes\\StandardFlieger\\PropellerModel.obj");
		m_zpPropellerMiddle.AddGeo(m_pzgPropeller);
		m_zmPropellerMaterial.Copy(m_zmPlaneMaterial);
		AddPlacement(&m_zpPropellerMiddle);
		break;
	}
	case 2:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\ORCA\\NewOne.obj");

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\ORCA\\textures\\Diffuse.jpg");
		m_zmPlaneMaterial.MakeTextureBump("models\\Airplanes\\ORCA\\textures\\Normal.jpg");
		m_zmPlaneMaterial.MakeTextureSpecular("models\\Airplanes\\ORCA\\textures\\Roughness.jpg");

		m_zpTriebwerkL.AddGeo(&m_zgTriebwerk);
		m_zpTriebwerkR.AddGeo(&m_zgTriebwerk);
		m_zpTriebwerkL.Translate(0.4, 0.4, 5.7);
		m_zpTriebwerkR.Translate(-0.4, 0.4, 5.7);
		break;
	}
	case 3:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\P-40 Warhawk\\PlaneModel.obj",true);

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\P-40 Warhawk\\textures\\Diffuse.jpg");
		m_zmPlaneMaterial.MakeTextureBump("models\\Airplanes\\P-40 Warhawk\\textures\\Normal.jpg");
		m_zmPlaneMaterial.MakeTextureSpecular("models\\Airplanes\\P-40 Warhawk\\textures\\Roughness.jpg");

		m_pzgPropeller = objLoader.LoadGeoTriangleTable("models\\Airplanes\\StandardFlieger\\PropellerModel.obj",true);
		m_zpPropellerMiddle.AddGeo(m_pzgPropeller);
		m_zmPropellerMaterial.Copy(m_zmPlaneMaterial);
		AddPlacement(&m_zpPropellerMiddle);
		break;
	}
	case 4:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\MiG-35\\newone.obj");//Geht nicht

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\MiG-35\\textures\\Diffuse.png");
		m_zmPlaneMaterial.MakeTextureBump("models\\Airplanes\\MiG-35\\textures\\Normal.png");
		m_zmPlaneMaterial.MakeTextureSpecular("models\\Airplanes\\MiG-35\\textures\\Roughness.png");
		break;
	}
	case 5:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\F-117 Nighthawk\\PlaneModel.obj");

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\F-117 Nighthawk\\textures\\Diffuse.png");
		m_zmPlaneMaterial.MakeTextureBump("models\\Airplanes\\F-117 Nighthawk\\textures\\Normal.png");
		m_zmPlaneMaterial.MakeTextureSpecular("models\\Airplanes\\F-117 Nighthawk\\textures\\Specular.png");

		m_zpTriebwerkStealthL.AddGeo(&m_zgTriebwerkStealth);
		m_zpTriebwerkStealthL.Translate(-0.52, -2.0, 5);
		m_zpTriebwerkStealthL.RotateZDelta(0.5 * PI);
		break;
	}
	case 6:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\B17\\PlaneModel.obj");

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\B17\\textures\\Diffuse.jpeg");

		m_pzgPropeller = objLoader.LoadGeoTriangleTable("models\\Airplanes\\B17\\PropellerModel.obj");
		m_zpPropellerMiddle.AddGeo(m_pzgPropeller);

		m_zpPropeller1.AddPlacement(&m_zpPropellerMiddle);
		m_zpPropeller2.AddPlacement(&m_zpPropellerMiddle);
		AddPlacement(&m_zpPropeller1);
		AddPlacement(&m_zpPropeller2);
		m_zmPropellerMaterial.Copy(m_zmPlaneMaterial);

		m_zpPropeller1.TranslateX(3.3);
		m_zpPropeller1.TranslateYDelta(0.5);
		m_zpPropeller2.TranslateX(-3.3);
		m_zpPropeller2.TranslateYDelta(0.5);

		break;
	}
	}
	AddGeo(m_pzgPlane);
	AddPlacement(&m_zpTriebwerkL);
	AddPlacement(&m_zpTriebwerkR);
	AddPlacement(&m_zpTriebwerkStealthL);


		m_pzgPlane->SetMaterial(&m_zmPlaneMaterial);
	if(m_pzgPropeller)
		m_pzgPropeller->SetMaterial(&m_zmPropellerMaterial);
}

void CAirplaneModel::ReInit(int planeID)
{
	SubGeo(m_pzgPlane);
	SubPlacement(&m_zpTriebwerkL);
	SubPlacement(&m_zpTriebwerkR);
	SubPlacement(&m_zpTriebwerkStealthL);
	SubPlacement(&m_zpPropeller1);
	SubPlacement(&m_zpPropeller2);
	SubPlacement(&m_zpPropellerMiddle);


	//Flugzeuge
	m_planeID = planeID;
	switch (m_planeID)
	{
	case 0:
	{
		CMaterial planeMaterial;
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\F4\\PlaneModel.obj", true);

		planeMaterial.MakeTextureDiffuse("models\\Airplanes\\F4\\textures\\Diffuse.png");
		planeMaterial.MakeTextureBump("models\\Airplanes\\F4\\textures\\Normal.png");
		planeMaterial.MakeTextureSpecular("models\\Airplanes\\F4\\textures\\Roughness.png");

		m_zpTriebwerkL.AddGeo(&m_zgTriebwerk);
		m_zpTriebwerkR.AddGeo(&m_zgTriebwerk);
		m_zpTriebwerkL.Translate(0.5, 0.08, 3.5);
		m_zpTriebwerkR.Translate(-0.5, 0.08, 3.5);

		m_zmPlaneMaterial.LoadPreset("AshesGlowing");
		m_zmPlaneMaterial.Copy(planeMaterial);
		break;
	}
	case 1:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\StandardFlieger\\PlaneModel.obj", true);
		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\StandardFlieger\\textures\\Diffuse.png");

		m_pzgPropeller = objLoader.LoadGeoTriangleTable("models\\Airplanes\\StandardFlieger\\PropellerModel.obj");
		m_zpPropellerMiddle.AddGeo(m_pzgPropeller);
		m_zmPropellerMaterial.Copy(m_zmPlaneMaterial);
		AddPlacement(&m_zpPropellerMiddle);
		break;
	}
	case 2:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\ORCA\\NewOne.obj", true);

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\ORCA\\textures\\Diffuse.jpg");
		m_zmPlaneMaterial.MakeTextureBump("models\\Airplanes\\ORCA\\textures\\Normal.jpg");
		m_zmPlaneMaterial.MakeTextureSpecular("models\\Airplanes\\ORCA\\textures\\Roughness.jpg");

		m_zpTriebwerkL.AddGeo(&m_zgTriebwerk);
		m_zpTriebwerkR.AddGeo(&m_zgTriebwerk);
		m_zpTriebwerkL.Translate(0.4, 0.4, 5.7);
		m_zpTriebwerkR.Translate(-0.4, 0.4, 5.7);
		break;
	}
	case 3:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\P-40 Warhawk\\PlaneModel.obj", true);

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\P-40 Warhawk\\textures\\Diffuse.jpg");
		m_zmPlaneMaterial.MakeTextureBump("models\\Airplanes\\P-40 Warhawk\\textures\\Normal.jpg");
		m_zmPlaneMaterial.MakeTextureSpecular("models\\Airplanes\\P-40 Warhawk\\textures\\Roughness.jpg");

		m_pzgPropeller = objLoader.LoadGeoTriangleTable("models\\Airplanes\\StandardFlieger\\PropellerModel.obj", true);
		m_zpPropellerMiddle.AddGeo(m_pzgPropeller);
		m_zmPropellerMaterial.Copy(m_zmPlaneMaterial);
		AddPlacement(&m_zpPropellerMiddle);
		break;
	}
	case 4:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\MiG-35\\newone.obj", true);//Geht nicht

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\MiG-35\\textures\\Diffuse.png");
		m_zmPlaneMaterial.MakeTextureBump("models\\Airplanes\\MiG-35\\textures\\Normal.png");
		m_zmPlaneMaterial.MakeTextureSpecular("models\\Airplanes\\MiG-35\\textures\\Roughness.png");
		break;
	}
	case 5:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\F-117 Nighthawk\\PlaneModel.obj", true);

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\F-117 Nighthawk\\textures\\Diffuse.png");
		m_zmPlaneMaterial.MakeTextureBump("models\\Airplanes\\F-117 Nighthawk\\textures\\Normal.png");
		m_zmPlaneMaterial.MakeTextureSpecular("models\\Airplanes\\F-117 Nighthawk\\textures\\Specular.png");

		m_zpTriebwerkStealthL.AddGeo(&m_zgTriebwerkStealth);
		m_zpTriebwerkStealthL.Translate(-0.52, -2.0, 5);
		m_zpTriebwerkStealthL.RotateZDelta(0.5 * PI);
		break;
	}
	case 6:
	{
		m_pzgPlane = objLoader.LoadGeoTriangleTable("models\\Airplanes\\B17\\PlaneModel.obj", true);

		m_zmPlaneMaterial.MakeTextureDiffuse("models\\Airplanes\\B17\\textures\\Diffuse.jpeg");

		m_pzgPropeller = objLoader.LoadGeoTriangleTable("models\\Airplanes\\B17\\PropellerModel.obj");
		m_zpPropellerMiddle.AddGeo(m_pzgPropeller);

		m_zpPropeller1.AddPlacement(&m_zpPropellerMiddle);
		m_zpPropeller2.AddPlacement(&m_zpPropellerMiddle);
		AddPlacement(&m_zpPropeller1);
		AddPlacement(&m_zpPropeller2);
		m_zmPropellerMaterial.Copy(m_zmPlaneMaterial);

		m_zpPropeller1.TranslateX(3.3);
		m_zpPropeller1.TranslateYDelta(0.5);
		m_zpPropeller2.TranslateX(-3.3);
		m_zpPropeller2.TranslateYDelta(0.5);

		break;
	}
	}
	AddGeo(m_pzgPlane);
	AddPlacement(&m_zpTriebwerkL);
	AddPlacement(&m_zpTriebwerkR);
	AddPlacement(&m_zpTriebwerkStealthL);


	m_pzgPlane->SetMaterial(&m_zmPlaneMaterial);
	if (m_pzgPropeller)
		m_pzgPropeller->SetMaterial(&m_zmPropellerMaterial);
}

void CAirplaneModel::Tick(float fTime, float fTimeDelta)
{
	m_zpPropellerMiddle.RotateZDelta(fTime * 100);
}
