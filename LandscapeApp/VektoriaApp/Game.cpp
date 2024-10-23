#include "Game.h"

CGame::CGame(void)
{
}

CGame::~CGame(void)
{
}

void CGame::Init(HWND hwnd, void(*procOS)(HWND hwnd, unsigned int uWndFlags), CSplash * psplash)
{
	//---------------------------------
	// Initialisiere die Knotenobjekte:
	//---------------------------------

	// Zuallererst muss die Root mit dem Splash-Screen initialisiert werden, 
	// damit die Engine freigeschaltet wird:
	m_zr.Init(psplash);

	// Hier kommen alle weiteren Initialisierungen hinein: 
	m_zr.Init(psplash);
	m_zf.Init(hwnd, procOS);
	m_zv.InitFull(&m_zc);
	m_zr.AddFrame(&m_zf);
	m_zf.AddViewport(&m_zv);
	m_zf.AddDeviceKeyboard(&m_zdk);
	m_zr.AddScene(&m_zs);



	// Erzeuge einen Himmel mit Sonne, Mond und Sternen:
	m_zs.SetSkyOn(&m_zpCamera);
	m_zs.SetSkyFlowOn(0);

	// Schalte den Haze-Postprocessing-Filter fürs Wasser an:
	m_zv.SetHazeOn();

	// Erzeuge einen Cut, der alles unter N.N. wegschneidet:
	m_cutUnderSea.SetHeightLimits(-F_MAX, -100.0f);

	// Die Vertices der den Rand überlappenden Polygone werden
	// auf N.N. angehoben:
	m_cutUnderSea.SetFlattenLowerOn();

	// Und in der XZ-Ebene mittels Bézier „glattgezurrt“:
	m_cutUnderSea.SetFlattenSmartOn();

	// Füge den Cut dem Terrain hinzu:
	m_zgTerrain.AddCut(&m_cutUnderSea);


	// Lade die Texturen für Land:
	m_zmEarth.LoadPreset("RockMossy");

	// --------------------
	// Erzeuge des Wassers:
	// --------------------

	// Lade die vorgefertigte Wassertextur aus der Lib:
	m_zmWater.LoadPreset("Water");

	// Die animierte Wassertextur hat 8*8 Bilder:
	m_zmWater.SetAni(8, 8, 7);

	// Erh�he den Blauwert (Bilde die Blaustreuung des Wassers nach):
	m_zmWater.Translate(CColor(0.0f, 0.2f, 0.3f));

	// Die Transparenz des Wassers ist 90%:
	m_zmWater.SetTransparency(0.7f);

	// Steuere den Haze-Filter, damit das Wasser "schwappt": 
	m_zmWater.MakeTextureHaze("textures\\waterHaze.jpg");
	m_zmWater.SetPostprocessingOn();
	m_zmWater.SetHazeOn();
	m_zmWater.SetHazeStrength(1.8);

	// Erzeuge die Wasseroberfläche:
	m_zgWater.Init(
		70000, 70000, // Das Meer ist groß (70 *70 qkm)
		&m_zmWater, // Ein bewegtes Wassermaterial mit Wellen
		10, 10, // Brauchen nur wenige Polygone (hier 100)
		0.0f, 0.0f, // Eine UV-Verschiebung brauchen wir nicht
		500.0f, // Aber die Textur soll sich 500 Mal …
		500.0f); // …in jeder Richtung wiederholen


	// -----------------------
	// Erzeuge die Landschaft:
	// -----------------------
	m_pperlin = new CPerlin(42, 3.0f, 15, 0.5f, 8.0f, 1.50f, 1.5f, ePerlinInterpol_Standard, false);

	m_pblob = new CBlob(0.5f, 0.5f, 0.6f, 0.6f, -0.1f, eBlobShapeGround_Rect, eBlobShapeSide_All, NULL);
	m_pblob2 = new CBlob(0.5f, 0.5f, 0.5f, 0.5f, 500, eBlobShapeGround_Radial, eBlobShapeSide_Hill, m_pperlin);
	m_pblob3 = new CBlob(0.2f, 0.2f, 0.2f, 0.2f, 500, eBlobShapeGround_Donut, eBlobShapeSide_Hill, m_pperlin);

	m_zgTerrainOriginal.AddBlob(m_pblob);
	m_zgTerrainOriginal.AddBlob(m_pblob2);
	m_zgTerrainOriginal.AddBlob(m_pblob3);

	m_zgTerrainOriginal.CreateField(
		2500, 2500, // Terraingröße
		1000, 1000, // Vertices
		0.0f, 0.0f, // UV Textur
		1.0f, 1.0f); // UV Textur

	// Erzeuge mit dem Terraindaten eine Terraingeometrie:
	m_zgTerrain.InitFromOther(m_zgTerrainOriginal, &m_zmEarth);

	// Das Blaupausen-Terrain soll nicht gerendert werden:
	m_zgTerrainOriginal.SetDrawingOff();

	// Hänge die Geometrien an ein geeignetes Placement:
	m_zs.AddPlacement(&m_zpLandscape);
	m_zpLandscape.AddGeo(&m_zgTerrain);
	m_zpLandscape.AddGeo(&m_zgWater);

	// Füge das Terrain dem Kollisionscontainer hinzu:
	m_zgsTerrain.Add(&m_zgTerrain);

	// -----------------------
	// Kamera:
	// -----------------------

	// Initialisiere die Kamera mit Outdoor-BVH-Schattenfrustumcasting (OBVHSFC) zur Beschleunigung:
	m_zs.SetFrustumCullingOn();
	m_zc.Init(HALFPI, // 45° Kameraöffnungswinkel
		0.3, 170000.0f, // 30cm bis 170 km Sicht
		true, // BVH-Schattenfrustumcasting an!
		m_zs.GetSkyLightPlacement()); // Info für das OBVHSFC

	// Initialisiere die Kamera:
	m_zs.AddPlacement(&m_zpCamera);
	m_zpCamera.AddCamera(&m_zc);

	// Die Kamera soll sich bewegen:
	m_zpCamera.SetTranslationSensitivity(500);
	m_zpCamera.SetRotationSensitivity(1.0f);

	// Stelle die Kamera an einen geeigneten Anfangsort:
	m_zpCamera.Translate(0.0f, 1000.0f, 1000.0f);


	// Turret importieren
	//m_zgTurret = m_fileWavefront.LoadGeoTriangleTable("C:\\Users\\y_sch\\Desktop\\Hochschule\\3 Semester\\Vektoria\\VektoriaV23.20\\LandscapeApp\\wavefront\\turret.obj");
	m_zgTurret = m_fileWavefront.LoadGeoTriangleTable("C:\\Users\\y_sch\\Desktop\\Hochschule\\3 Semester\\Vektoria\\VektoriaV23.20\\LandscapeApp\\Models\\Star Wars emperor Turret.obj");

	m_zs.AddPlacement(&m_zpTurret);
	m_zpTurret.AddGeo(m_zgTurret);

	m_zpTurret.ScaleDelta(50.0f);
	/*m_zpTurret.GetPos().GetX;*/
	float fH = m_zgTerrain.GetHeight(0.0f, 0.0f);

	m_zpTurret.TranslateYDelta(fH);
	m_zpTurret.RotateYDelta(-HALFPI);


	/*

	m_zgTerrain.SwitchCollisionGroundOn();

	CHVector rayStart = m_zpTurret.GetPos();
	CRay r(rayStart, CHVector(0.0f, -1.0f, 0.0f));
	CHitPoint hitpoint;



	m_zpTurret.IntersectsGeosGround(r, hitpoint, 1000000000, true);
	m_zgTerrain.Intersects(r, hitpoint, 1000000000, true);
	m_zgTerrainOriginal.Intersects(r, hitpoint, 1000000000 , true);
	*/
	//m_zpTurret.TranslateYDelta(-hitpoint.m_fDist);
	


	m_zgSphere.Init(25.0f, NULL, 100, 100);
	m_zs.AddPlacement(&m_zpSphere);
	m_zpSphere.AddGeo(&m_zgSphere);
	m_zpSphere.TranslateY(800.0f);
}

void CGame::Tick(float fTime, float fTimeDelta)
{
	//-------------------------------------------
	// Veränderungen während der Simulationszeit:
	//-------------------------------------------

	// Hier kommen die Veränderungen pro Renderschritt hinein: 
	
	m_zdk.PlaceWASD(m_zpCamera, fTimeDelta, true);

	// Traversiere am Schluss den Szenengraf und rendere:
	m_zr.Tick(fTimeDelta);
}

void CGame::Fini()
{
	//-------------------------------
	// Finalisiere die Knotenobjekte:
	//-------------------------------

	// Hier die Finalisierung Deiner Vektoria-Objekte einfügen:

	// Auch die Root sollte finalisiert werden:   
	m_zr.Fini();
}

void CGame::WindowReSize(int iNewWidth, int iNewHeight)
{
	// Windows ReSize wird immer automatisch aufgerufen, wenn die Fenstergröße verändert wurde.
	// Hier kannst Du dann die Auflösung des Viewports neu einstellen:
	// z.B. mittels: m_zf.ReSize(iNewWidth, iNewHeight);
}

float CGame::GetTimeDeltaMin()
{
	return m_zr.GetTimeDeltaMin();
}

float CGame::GetVersion()
{
	return m_zr.GetVersion();
}


