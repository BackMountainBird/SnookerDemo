//|||||||||||||||||||||||||||||||||||||||||||||||

#include "OptionState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

OptionState::OptionState()
{
	m_bQuit             = false;
	m_bQuestionActive   = false;
	m_FrameEvent        = Ogre::FrameEvent();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OptionState::enter()
{
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering OptionState...");

	m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "OptionSceneMgr");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

	m_pCamera = m_pSceneMgr->createCamera("OptionCam");
	m_pCamera->setPosition(Vector3(0, 25, -50));
	m_pCamera->lookAt(Vector3(0, 0, 0));
	m_pCamera->setNearClipDistance(1);

	m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
		Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));

	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

	OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
	OgreFramework::getSingletonPtr()->m_pTrayMgr->showCursor();
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "BackToMenuBtn", "Return to Menu", 250);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_TOP, "PauseLbl", "Options", 250);

	m_bQuit = false;

	createScene();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OptionState::createScene()
{
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OptionState::exit()
{
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving OptionState...");

	m_pSceneMgr->destroyCamera(m_pCamera);
	if(m_pSceneMgr)
		OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);

	OgreFramework::getSingletonPtr()->m_pTrayMgr->clearAllTrays();
	OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
	OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(0);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OptionState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE) && !m_bQuestionActive)
	{
		m_bQuit = true;
		return true;
	}

	OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OptionState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);

	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OptionState::mouseMoved(const OIS::MouseEvent &evt)
{
	if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OptionState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OptionState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OptionState::update(double timeSinceLastFrame)
{
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);

	if(m_bQuit == true)
	{
		popAppState();
		//pushAppState(findByName("MenuState"));
		return;
	}
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OptionState::buttonHit(OgreBites::Button *button)
{
	if(button->getName() == "BackToMenuBtn")
		m_bQuit=true;
		//popAppState();
		//m_bQuit=true;
		//popAllAndPushAppState(findByName("MenuState"));
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OptionState::yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit)
{
	if(yesHit == true)
		shutdown();
	else
		OgreFramework::getSingletonPtr()->m_pTrayMgr->closeDialog();

	m_bQuestionActive = false;
}

//|||||||||||||||||||||||||||||||||||||||||||||||