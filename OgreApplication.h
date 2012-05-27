/**
 * Class OgreApplication - Header File
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

#ifndef __OgreApplication_h__
#define __OgreApplication_h__

#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include <OgreCEGUIRenderer.h>

using namespace Ogre;

class OgreApplication: public OIS::KeyListener, public OIS::MouseListener, public FrameListener {

public:
	OgreApplication(void);
	virtual ~OgreApplication(void);
	void updateStats(void);
	virtual void go(void);

protected:
	virtual bool initialise();
	virtual bool initOgreCore();
	virtual void createSceneManager();
	virtual void createCamera();
	virtual void createViewports();
	virtual void createResourceListener();
	virtual void createFrameListener();
	virtual void SetupInputSystem();

	virtual void addResourceLocations();
	virtual void initResources();

	virtual void createScene() = 0; // I am pure virtual, override me!
	virtual void destroyScene();

	// FrameListener overrides
	virtual bool frameStarted(const FrameEvent& evt);
	virtual bool frameEnded(const FrameEvent& evt);

	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);

	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

protected:
	Root *mRoot;
	SceneManager* mSceneManager;
	RenderWindow* mWindow;
	Camera* mCamera;

	OIS::Keyboard* mKeyboard;
	OIS::Mouse* mMouse;
	OIS::InputManager *mInputManager;

	Radian mRotX, mRotY;

	FrameListener *mListener;

	float mMoveScale;

	bool mContinue;
	bool mMouseDown;

	Real mMoveSpeed;
	Real mToggle;
	Real mRotate;
	Degree mRotateSpeed;
	Degree mRotScale;

	Overlay* m_pDebugOverlay;
	Overlay* m_pInfoOverlay;

}; // end of class OgreApplication

#endif // __OgreApplication_h__
