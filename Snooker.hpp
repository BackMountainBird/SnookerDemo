
#ifndef SNOOKER_HPP
#define SNOOKER_HPP

#include "CueGameCore.hpp"
#include "SnookerInit.hpp"

class Snooker : public CueGameCore
{
public:
	Snooker();
	void beginGame(Ogre::SceneManager* m_pManagerMgr);
	void nextRound(void);
	bool compareAndJudge(void);
	void update(Ogre::Real interval);
	void keyPress(const OIS::KeyEvent &keyEventRef,OIS::Keyboard* m_pKeyboard);
private:
	void initialiseAudio(Ogre::SceneManager* sceneManager);
};
#endif