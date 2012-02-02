//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef OPTION_STATE_HPP
#define OPTION_STATE_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

class OptionState : public AppState
{
public:
	OptionState();

	DECLARE_APPSTATE_CLASS(OptionState)

	void enter();
	void createScene();
	void exit();

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	void buttonHit(OgreBites::Button* button);
	void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);

	void update(double timeSinceLastFrame);

private:
	bool m_bQuit;
	bool m_bQuestionActive;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||