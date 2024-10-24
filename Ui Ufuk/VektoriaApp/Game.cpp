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
	m_zf.Init(hwnd, procOS);
	m_zv.InitFull(&m_zc);
	m_zc.Init(QUARTERPI);
	m_zlp.Init(CHVector(-0.8f, 0.02f, 0.2f), CColor(1.0f, 1.0f, 1.0f));
	m_zr.AddFrame(&m_zf);
	m_zf.AddViewport(&m_zv);
	m_zs.AddLightParallel(&m_zlp);
	m_zr.AddScene(&m_zs);
	m_zs.AddPlacement(&m_zpCamera);
	m_zpCamera.AddCamera(&m_zc);
	
	m_zf.AddDeviceKeyboard(&m_zdk);
	m_zpCamera.SetTranslationSensitivity(5.0f);
	m_zpCamera.SetRotationSensitivity(0.8f);
	
	//Overlay

	m_zwf2.LoadPreset("RodWhite");
	m_zw2.Init(C2dRect(0.04f, 0.04f, 0.0f, 0.0f), 5, &m_zwf2);
	m_zwf2.SetTransparency(0.0f);
	m_zw2.PrintF("Ammo:");
	m_zv.AddWriting(&m_zw2);
	m_zwf2.SetTransparencyKind(eTransparencyKind_BinaryByChromaKey);
	

	m_zwf.LoadPreset("RodWhite");
	m_zw.Init(C2dRect(0.04f, 0.04f, 0.0f, 0.05f), 5, &m_zwf);
	m_zw.PrintF("Fuel:");
	m_zv.AddWriting(&m_zw);
	m_zwf.SetTransparencyKind(eTransparencyKind_BinaryByChromaKey);
	

	m_zwf3.LoadPreset("RodWhite");
	m_zw3.Init(C2dRect(0.04f, 0.04f, 0.0f, 0.1f), 6, &m_zwf3);
	m_zwf3.SetTransparency(0.0f);
	m_zw3.PrintF("Speed:");
	m_zv.AddWriting(&m_zw3);
	m_zwf3.SetTransparencyKind(eTransparencyKind_BinaryByChromaKey);


	m_zpCamera.TranslateZ(8.0f);

	//Object rein

	m_mpgTest = m_fileWavefront.LoadGeoTriangleTable("C:\\Users\\Fozer\\OneDrive\\Desktop\\Programme\\Vektoria\\Vektoria\\VektoriaV23.19\\App - Kopie (2)\\modells\\test.obj");

	m_zs.AddPlacement(&m_zpTest);
	m_zpTest.AddGeo(m_mpgTest);
	m_zpTest.TranslateY(50);
	m_zpTest.TranslateZ(-10.0f);
	m_zpTest.Scale(15);

	//Hier ist der Teil für das Terrain

	//Haze_Postprocessing Filter für Wasser
	m_zv.SetHazeOn();

	//Erzeugt den Himmel
	m_zs.SetSkyOn(&m_zpCamera);
	m_zs.SetSkyFlowOn(0);
	for (int i = 0; i < 3; i++)
		m_zs.m_psceneweather->m_azmSky[i].SetPostprocessingOn();

	//Beschränkung der Kamera
	m_zpCamera.SetMoveRange(CAABB(
		CHVector(-50000.0f, 4.0f, -50000.0f, 1.0f),
		CHVector(+50000.0f, 20000.0f, +50000.0f, 1.0f)));

	//Erzeugt einen Cut, der alles unter N.N wegschneidet
	m_cutUnderSea.SetHeightLimits(-F_MAX, 0.0f);

	//Die Vertices der den Rand überlappenden Polygone werden auf N.N angehoben
	m_cutUnderSea.SetFlattenLowerOn();

	//XZ-Ebene mittel Bezier Curves
	m_cutUnderSea.SetFlattenSmartOn();

	//Füge den Cut dem Terrain hizu:
	/*m_cutUnderSea.SetFlattenSmartOn();*/

	//Perlin-noise wird hier erzeugt
	m_pperlin = new CPerlin(42, 3.0f, 12, 0.5f, 5.0f, 0.0f, 0.0f, ePerlinInterpol_Standard, false);

	//Texturen für Wassen und land werden hier geladen:
	m_zmWater.LoadPreset("WaterHaze");
	m_zmEarth.LoadPreset("RockMossy");

	//Mische den Perlin-Noise mit einem radialen Blob
	m_mpblob = new CBlob(0.5f, 0.5f, 0.5f, 0.5f, 300, eBlobShapeGround_Radial, eBlobShapeSide_Hill, m_pperlin);
	m_mpblob2 = new CBlob(0.8f, 0.8f, 0.2f, 0.2f, 400, eBlobShapeGround_Radial, eBlobShapeSide_Hill, nullptr);
	m_mpblob3 = new CBlob(0.2f, 0.2f, 0.2f, 0.2f, 200, eBlobShapeGround_Radial, eBlobShapeSide_Hill, m_pperlin);
	m_mpblob4 = new CBlob(0.5f, 0.5f, 0.5f, 0.5f, -0.1, eBlobShapeGround_Rect, eBlobShapeSide_All, nullptr);


	//Füge den Blob einem Blaupausen-Terrain hinzu:
	m_zgTerrrainOri.AddBlob(m_mpblob);
	m_zgTerrrainOri.AddBlob(m_mpblob2);
	m_zgTerrrainOri.AddBlob(m_mpblob3);
	m_zgTerrrainOri.AddBlob(m_mpblob4);


	m_zgTerrrain.AddCut(&m_cutUnderSea);

	//Erzeuge die Vertex-Daten des Blueprint Terrains
	m_zgTerrrainOri.CreateField(
		2000, 2000, // Ein Quadratkilometer Terrain
		2000, 2000, // 2000 mal 2000 Vertices0000
		0.0f, 0.0f, // Die UV-Textur beginnt bei (0,0) …
		1.0f, 1.0f);// … und geht bis (1,1)

	// Erzeuge mit dem Terraindaten eine Terraingeometrie: 
	m_zgTerrrain.InitFromOther(m_zgTerrrainOri, &m_zmEarth);

	// Erzeuge die Wasseroberfläche:
	m_zgWater.Init(
		70000, 70000, // Das Meer ist groß (70 *70 qkm)
		&m_zmWater,
		// Ein bewegtes Wassermaterial mit Wellen
		10, 10,
		0.0f, 0.0f,
		500.0f,
		500.0f);
	// Brauchen nur wenige Polygone (hier 100)
	// Eine UV-Verschiebung brauchen wir nicht
	// Aber die Textur soll sich 500 Mal …  
   // …in jeder Richtung wiederholen
  
	 // Hänge die Geometrien an ein geeignetes Placement:
	m_zs.AddPlacement(&m_zpLandscape);
	m_zpLandscape.AddGeo(&m_zgTerrrain);
	m_zpLandscape.AddGeo(&m_zgWater);
	// Füge das Terrain dem Kollisionscontainer hinzu:
	m_zgsTerrain.Add(&m_zgTerrrain);

	//Initialisiere die Kamera mit Outdoor-BVH-Schattenfrusumcasting (OBVHSFC) zur Beschleunigung:
	m_zs.SetFrustumCullingOn();
	m_zc.Init(HALFPI,			//45° Gradwinkel
		0.3, 170000.0f,			//30cm bis 170km Sicht
		true,					//BVH_Schattenfrustumcasting an!
		m_zs.GetSkyLightPlacement());	//Info für das OBVHSFC

	// Initialisiere die Kamera:
	/*m_zs.AddPlacement(&m_zpCamera);
	m_zpCamera.AddCamera(&m_zc);*/

	// Die Kamera soll sich 30 m/s bewegen:
	m_zpCamera.SetTranslationSensitivity(300);
	// Stelle die Kamera an einen geeigneten Anfangsort: 
	m_zpCamera.Translate(0.0f, 20.0f, 1000.0f);

	// Hier kommen alle weiteren Initialisierungen hinein: 

}

void CGame::Tick(float fTime, float fTimeDelta)
{
	//-------------------------------------------
	// Veränderungen während der Simulationszeit:
	//-------------------------------------------

	// Hier kommen die Veränderungen pro Renderschritt hinein: 



	// Traversiere am Schluss den Szenengraf und rendere:
	m_zr.Tick(fTimeDelta);

	m_zdk.PlaceWASD(m_zpCamera, fTimeDelta);

	
}

void CGame::Fini()
{
	//-------------------------------
	// Finalisiere die Knotenobjekte:
	//-------------------------------

	// Hier die Finalisierung Deiner Vektoria-Objekte einfügen:

	// Auch die Root sollte finalisiert werden:   
	m_zr.Fini();

	delete m_pperlin;
	delete m_mpblob;
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


