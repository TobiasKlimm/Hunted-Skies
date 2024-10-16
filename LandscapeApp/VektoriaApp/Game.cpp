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
	// Das kennen wir ja schon:
	m_zr.Init(psplash);
	m_zf.Init(hwnd, procOS);
	m_zv.InitFull(&m_zc);
	m_zr.AddFrame(&m_zf);
	m_zf.AddViewport(&m_zv);
	m_zf.AddDeviceKeyboard(&m_zdk);
	m_zr.AddScene(&m_zs);

	// Schalte den Haze-Postprocessing-Filter fürs Wasser an:
	m_zv.SetHazeOn();


	// Erzeuge einen Himmel mit Sonne, Mond und Sternen:
	m_zs.SetSkyOn(&m_zpCamera);
	m_zs.SetSkyFlowOn(0);

	//// Beschränke die Bewegungsfreiheit der Kamera:
	//m_zpCamera.SetMoveRange(CAABB(
	//	CHVector(-50000.0f, 4.0f, -50000.0f, 1.0f),
	//	CHVector(+50000.0f, 20000.0f, +50000.0f, 1.0f)));

	// Erzeuge einen Cut, der alles unter N.N. wegschneidet:
	m_cutUnderSea.SetHeightLimits(-F_MAX, 0.0f);

	// Die Vertices der den Rand überlappenden Polygone werden
	// auf N.N. angehoben:
	m_cutUnderSea.SetFlattenLowerOn();

	// Und in der XZ-Ebene mittels Bézier „glattgezurrt“:
	m_cutUnderSea.SetFlattenSmartOn();

	// Füge den Cut dem Terrain hinzu:
	m_zgTerrain.AddCut(&m_cutUnderSea);

	// Erzeuge Perlin-Noise:
	m_pperlin = new CPerlin(4, 3.0f, 12, 0.5f, 5.0f, 0.0f,
		0.0f, ePerlinInterpol_Standard, false);

	// Lade die Texturen für Land :
	m_zmEarth.LoadPreset("RockMossy");

	// ------------------------------------
	// Erzeuge die Materialien des Wassers:
	// ------------------------------------

	// Lade die vorgefertigte Wassertextur aus der Lib:
	m_zmWater.LoadPreset("Water");

	// Die animierte Wassertextur hat 8*8 Bilder:
	m_zmWater.SetAni(8, 8, 7);

	// Erh�he den Blauwert (Bilde die Blaustreuung des Wassers nach):
	m_zmWater.Translate(CColor(0.0f, 0.2f, 0.3f));

	// Die Transparenz des Wassers ist 90%:
	m_zmWater.SetTransparency(0.9f);

	// Steuere den Haze-Filter, damit das Wasser "schwappt": 
	m_zmWater.MakeTextureHaze("textures\\waterHaze.jpg");
	m_zmWater.SetPostprocessingOn();
	m_zmWater.SetHazeOn();
	m_zmWater.SetHazeStrength(1.8);

	// Mische den Perlin-Noise mit einem radialen Blob:
	m_pblob = new CBlob(0.3f, 0.8f, 0.5f, 0.5f, 5000,
		eBlobShapeGround_Radial, eBlobShapeSide_Hill,
		m_pperlin);	
	
	m_pblob2 = new CBlob(0.8f, 0.5f, 0.5f, 0.5f, 5000,
		eBlobShapeGround_Radial, eBlobShapeSide_Hill,
		m_pperlin);	
	
	m_pblob3 = new CBlob(0.4f, 0.2f, 0.5f, 0.5f, 7000,
		eBlobShapeGround_Radial, eBlobShapeSide_Hill,
		m_pperlin);

	m_pblob4 = new CBlob(0.6f, 0.5f, 0.5f, 0.5f, 3000,
		eBlobShapeGround_Radial, eBlobShapeSide_Hill,
		m_pperlin);

	m_pblob5 = new CBlob(0.8f, 0.9f, 0.5f, 0.5f, 2000,
		eBlobShapeGround_Radial, eBlobShapeSide_Hill,
		m_pperlin);

	// Füge den Blob einem Blaupausen-Terrain hinzu:
	m_zgTerrainOri.AddBlob(m_pblob);
	m_zgTerrainOri.AddBlob(m_pblob2);
	m_zgTerrainOri.AddBlob(m_pblob3);
	m_zgTerrainOri.AddBlob(m_pblob4);
	m_zgTerrainOri.AddBlob(m_pblob5);

	// Erzeuge die Vertex-Daten des Blaupausen-Terrains:
	m_zgTerrainOri.CreateField(
		50000, 50000, // Terraingröße in KM
		1000, 1000, // Vertices
		0.0f, 0.0f, // Die UV-Textur beginnt bei (0,0) …
		1.0f, 1.0f);// … und geht bis (1,1)

	// Erzeuge mit dem Terraindaten eine Terraingeometrie:
	m_zgTerrain.InitFromOther(m_zgTerrainOri, &m_zmEarth);

	// Erzeuge die Wasseroberfläche:
	m_zgWater.Init(
		70000, 70000, // Das Meer ist groß (70 *70 qkm)
		&m_zmWater, // Ein bewegtes Wassermaterial mit Wellen
		10, 10, // Brauchen nur wenige Polygone (hier 100)
		0.0f, 0.0f, // Eine UV-Verschiebung brauchen wir nicht
		500.0f, // Aber die Textur soll sich 500 Mal …
		500.0f); // …in jeder Richtung wiederholen

	// Hänge die Geometrien an ein geeignetes Placement:
	m_zs.AddPlacement(&m_zpLandscape);
	m_zpLandscape.AddGeo(&m_zgTerrain);
	//m_zpLandscape.AddGeo(&m_zgWater);

	// Füge das Terrain dem Kollisionscontainer hinzu:
	m_zgsTerrain.Add(&m_zgTerrain);

	// Initialisiere die Kamera mit Outdoor-BVH-
    // Schattenfrustumcasting (OBVHSFC) zur Beschleunigung:
	m_zs.SetFrustumCullingOn();
	m_zc.Init(HALFPI, // 45° Kameraöffnungswinkel
		0.3, 170000.0f, // 30cm bis 170 km Sicht
		true, // BVH-Schattenfrustumcasting an!
		m_zs.GetSkyLightPlacement()); // Info für das OBVHSFC

	// Initialisiere die Kamera:
	m_zs.AddPlacement(&m_zpCamera);
	m_zpCamera.AddCamera(&m_zc);

	// Die Kamera soll sich bewegen:
	m_zpCamera.SetTranslationSensitivity(5000);
	m_zpCamera.SetRotationSensitivity(1.0f);

	// Stelle die Kamera an einen geeigneten Anfangsort:
	m_zpCamera.Translate(0.0f, 1000.0f, 1000.0f);


}

void CGame::Tick(float fTime, float fTimeDelta)
{
	//-------------------------------------------
	// Veränderungen während der Simulationszeit:
	//-------------------------------------------

	// Hier kommen die Veränderungen pro Renderschritt hinein: 
	
//	// Die WASD-Steuerung in der Tick-Methode geht über
//// 3 Kollisionscontainer: 1. m_zgsCollision = Frontal-
//// kollisionen, 2. m_zgHeight = Spezialobjeke zur
//// Beeinflussung der Bodenhöhe, 3. m_zgsTerrain =
//// Fraktales Terrain
//	CHitPoint hitpointGround;
//	CHitPoint hitpointCollision;
//	m_zdk.PlaceWASDTerrain(
//		m_zpCamera,
//		m_zgsCollision,
//		m_zgsHeight,
//		m_zgsTerrain,
//		4.0f,
//		3000.0f,
//		hitpointCollision,
//		hitpointGround,
//		fTimeDelta);

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


