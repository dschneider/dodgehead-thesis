/**
 * Class OgreApplication - Implementation
 *
 * Abstract Class using virtual functions for framework functionality
 *
 * It provides basic functionality necessary for an ogre application
 * to run
 * It contains the initialization of the camera, looks for the
 * resource locations, initializes the ogre core, creates the
 * scene manager, the viewports, sets the input system etc.
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#include "OgreApplication.h"

#include <iostream>

using namespace std;


/*
 * Constructor
 *
 * Initializes all the necessary attributes
 *
 */
OgreApplication::OgreApplication() : mRoot(0) {

	mContinue = true;
	mToggle = 0.0;
	mRotate = 0.13;
	m_pDebugOverlay = 0;
	m_pInfoOverlay = 0;

}


/*
 * Destructor
 *
 * Frees memory by destroying objects
 *
 */
OgreApplication::~OgreApplication() {

	//OIS
	mInputManager->destroyInputObject(mMouse);
	mMouse = 0;
	mInputManager->destroyInputObject(mKeyboard);
	mKeyboard = 0;
	OIS::InputManager::destroyInputSystem(mInputManager);
	mInputManager = 0;

	// Ogre
	delete mRoot;

}

/*
 * Start the application
 *
 */
void OgreApplication::go() {

	if (!initialise())
		return;
}

/*
 * Creates a root object, calls all necessary methods and
 * starts rendering
 *
 */
bool OgreApplication::initialise() {

	mRoot = new Root();

	mRoot->showConfigDialog();

	// Add resource locations
	addResourceLocations();

	// If we cannot initialise Ogre, just abandon the whole deal
	if (!initOgreCore()) return false;

	createSceneManager();
	createCamera();
	createViewports();

	// Set default mipmap level (NB some APIs ignore this)
	TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Create any resource listeners (for loading screens)
	createResourceListener();

	// Initialize resources
	initResources();

	// Debug Overlay
	m_pDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
	//m_pDebugOverlay->show();

	// Create the scene
	createScene();

	// Set the input system
	SetupInputSystem();

	// Create the frame listener
	createFrameListener();

	// Start rendering
	mRoot->startRendering();

	return true;

};

/*
 * Adds the resource locations according to the
 * resources.cfg file
 *
 */
void OgreApplication::addResourceLocations() {
	// Load resource paths from config file
	ConfigFile cf;
	cf.load("resources.cfg");

	// Go through all sections & settings in the file
	ConfigFile::SectionIterator seci = cf.getSectionIterator();

	String secName, typeName, archName;
	while (seci.hasMoreElements()) {

		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;

		for (i = settings->begin(); i != settings->end(); ++i) {

			typeName = i->first;
			archName = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);

		}

	}
}

/*
 * Initializes the ogre core and setup a render window
 *
 */
bool OgreApplication::initOgreCore() {

	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	if (mRoot->restoreConfig() || mRoot->showConfigDialog()) {
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		mWindow = mRoot->initialise(true);

		return true;
	} else {
		return false;
	}

}

/*
 * Creates an exterior scene manager
 *
 */
void OgreApplication::createSceneManager() {

	// Create the SceneManager
	mSceneManager = mRoot->createSceneManager(ST_EXTERIOR_REAL_FAR);

}

/*
 * Creates the camera
 *
 */
void OgreApplication::createCamera() {
}

/*
 * Creates the viewports and adds a camera
 *
 */
void OgreApplication::createViewports() {

	// Create one viewport, entire window

	// Viewport* vp = m_root->getAutoCreatedWindow()->getViewport(0);
	Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(ColourValue(0, 0, 0));

	// Create one viewport, left bottom corner
	/*Viewport* vp2 = mWindow->addViewport(mCamera, 1, 0, 0.8, 0.2, 0.2);
	vp2->setBackgroundColour(ColourValue(1,1,1));*/

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

}

/*
 * Create a resource listener
 *
 */
void OgreApplication::createResourceListener() {

}

/*
 * Initializes the resources
 *
 */
void OgreApplication::initResources() {

	// Initialize the resources, parse scripts
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

}

/*
 * Destroys the scene
 *
 */
void OgreApplication::destroyScene() {
}

/*
 * Set the input system
 *
 */
void OgreApplication::SetupInputSystem() {

	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	OIS::ParamList pl;
	RenderWindow *win = mRoot->getAutoCreatedWindow();

	win->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	mInputManager = OIS::InputManager::createInputSystem(pl);

	try {

		mKeyboard = static_cast<OIS::Keyboard*> (mInputManager->createInputObject(OIS::OISKeyboard, true));
		mMouse = static_cast<OIS::Mouse*> (mInputManager->createInputObject(OIS::OISMouse, true));
		//mJoy = static_cast<OIS::JoyStick*>(mInputManager->createInputObject(OIS::OISJoyStick, false));

		mKeyboard->setEventCallback(this);
		mMouse->setEventCallback(this);

	} catch (const OIS::Exception &e) {

		throw Exception(42, e.eText, "Application::setupInputSystem");

	}

}

/*
 * Updates the overlay stats like frame rate points etc.
 *
 */
void OgreApplication::updateStats() {

		static String currFps = "Current FPS: ";
		static String avgFps = "Average FPS: ";
		static String bestFps = "Best FPS: ";
		static String worstFps = "Worst FPS: ";
		static String tris = "Triangle Count: ";
		static String batches = "Batch Count: ";

		OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
		OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
		OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
		OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

		const RenderTarget::FrameStats& stats = mWindow->getStatistics();
		guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
		guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
		guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS) + " " + StringConverter::toString(stats.bestFrameTime) + " ms");
		guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS) + " " + StringConverter::toString(stats.worstFrameTime) + " ms");

		OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
		guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

		OverlayElement* guiBatches = OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
		guiBatches->setCaption(batches + StringConverter::toString(stats.batchCount));
		OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
		guiDbg->setCaption("");

}

/*
 * After each frame has started execute the commands
 * within the function
 *
 * @param evt	frame event
 *
 */
bool OgreApplication::frameStarted(const FrameEvent& evt) {

	if (mKeyboard)
		mKeyboard->capture();

	if (mMouse)
		mMouse->capture();

	OgreApplication::updateStats();

	if (mWindow->isClosed()) return false;

	return true;

}

/*
 * Checks for pressed keys
 *
 * @param arg	key event
 *
 */
bool OgreApplication::keyPressed(const OIS::KeyEvent &arg) {

    CEGUI::System::getSingleton().injectKeyDown(arg.key);
    CEGUI::System::getSingleton().injectChar(arg.text);

	return true;

}

/*
 * Checks for released keys
 *
 * @param arg	key event
 *
 */
bool OgreApplication::keyReleased(const OIS::KeyEvent &arg) {

	CEGUI::System::getSingleton().injectKeyUp(arg.key);

	return true;

}

/*
 * What it does
 *
 * @param evt	frame event
 *
 */
bool OgreApplication::frameEnded(const FrameEvent& evt) {

	return true;

}

/*
 * Creates a frame listener and adds it to
 * the root object
 *
 */
void OgreApplication::createFrameListener() {

	mListener = new FrameListener();
	mRoot->addFrameListener(this);

}

/*
 * Checks if the mouse was moved
 *
 * @param arg	mouse event
 *
 */
bool OgreApplication::mouseMoved(const OIS::MouseEvent &arg) {

	//mCamera->yaw(Degree(arg.state.X.rel * -0.1));
	//mCamera->pitch(Degree(arg.state.Y.rel * -0.1));

	CEGUI::System::getSingleton().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);

	return true;

}

/*
 * Converts CEGUI mouse clicks to OIS format
 * CEGUI uses another mouse coding than OIS
 *
 * @param buttonID	the mouse button id
 *
 */
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;

    case OIS::MB_Right:
        return CEGUI::RightButton;

    case OIS::MB_Middle:
        return CEGUI::MiddleButton;

    default:
        return CEGUI::LeftButton;
    }
}

/*
 * Checks if a mouse button was pressed
 *
 * @param arg	mouse event
 * @param id	ID of the mouse button
 *
 */
bool OgreApplication::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {

	CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
	return true;

}

/*
 * Checks if a mouse button was released
 *
 * @param arg	mouse event
 * @param id	ID of the mouse button
 *
 */
bool OgreApplication::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {

	CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
	return true;

}
