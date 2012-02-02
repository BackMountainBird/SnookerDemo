#include "Snooker.hpp"
#include <boost/lexical_cast.hpp>

Snooker::Snooker(){
	TIMESLICE_THRESHOLD=Ogre::Real(0.000001);
	UPDATE_STEP=1;
	STATS_MASS=154.5;
	STATS_RESTITUTION_COEFFICIENT=Ogre::Real(0.9);
	STATS_FRICTION_COEFFICIENT=Ogre::Real(0.1);
	STATS_BALL_RADIUS=26.25;
	STATS_BALL_NUMBER=22;						//球数
	STATS_TABLE_INNER_WIDTH=2035;				//球桌内部宽度
	STATS_TABLE_OUTTER_WIDTH=2235;				//球桌外部宽度
	STATS_TABLE_INNER_LENGTH=3820;				//球桌内部长度
	STATS_TABLE_OUTTER_LENGTH=4020;				//球桌外部长度
	STATS_STICK_LENGTH=1450;					//球杆长度

	previous_BallStates=new BallState[STATS_BALL_NUMBER];
	current_BallStates =new BallState[STATS_BALL_NUMBER];
	int i=STATS_BALL_NUMBER;
	while (i--)
	{
		previous_BallStates[i]=BALLSTATE_ONTABLE;
		current_BallStates[i]=BALLSTATE_ONTABLE;
	}
	//poolBalls.insert(new PoolBall(STATS_BALL_RADIUS,  ))
}

void Snooker::beginGame(Ogre::SceneManager* m_pSceneMgr)
{
	CueGameCore::beginGame(m_pSceneMgr);
	poolBalls.push_back((new PoolBall(STATS_MASS,STATS_FRICTION_COEFFICIENT,STATS_BALL_RADIUS,
		m_pSceneMgr->getEntity("Entity_White")))->setMaterialName("Snooker_Ball/white"));
	std::string str;
	for (int i=1;i<=15;i++)
	{
		str="Entity_Red"+boost::lexical_cast<std::string>(i);
		poolBalls.push_back((new PoolBall(STATS_MASS,STATS_FRICTION_COEFFICIENT,STATS_BALL_RADIUS,
			m_pSceneMgr->getEntity(str)))->setMaterialName("Snooker_Ball/red"));
	}
	poolBalls.push_back((new PoolBall(STATS_MASS,STATS_FRICTION_COEFFICIENT,STATS_BALL_RADIUS,
		m_pSceneMgr->getEntity("Entity_Yellow")))->setMaterialName("Snooker_Ball/yellow"));
	poolBalls.push_back((new PoolBall(STATS_MASS,STATS_FRICTION_COEFFICIENT,STATS_BALL_RADIUS,
		m_pSceneMgr->getEntity("Entity_Brown")))->setMaterialName("Snooker_Ball/brown"));
	poolBalls.push_back((new PoolBall(STATS_MASS,STATS_FRICTION_COEFFICIENT,STATS_BALL_RADIUS,
		m_pSceneMgr->getEntity("Entity_Green")))->setMaterialName("Snooker_Ball/green"));
	poolBalls.push_back((new PoolBall(STATS_MASS,STATS_FRICTION_COEFFICIENT,STATS_BALL_RADIUS,
		m_pSceneMgr->getEntity("Entity_Blue")))->setMaterialName("Snooker_Ball/blue"));
	poolBalls.push_back((new PoolBall(STATS_MASS,STATS_FRICTION_COEFFICIENT,STATS_BALL_RADIUS,
		m_pSceneMgr->getEntity("Entity_Purple")))->setMaterialName("Snooker_Ball/purple"));
	poolBalls.push_back((new PoolBall(STATS_MASS,STATS_FRICTION_COEFFICIENT,STATS_BALL_RADIUS,
		m_pSceneMgr->getEntity("Entity_Black")))->setMaterialName("Snooker_Ball/black"));
	initialiseAudio(m_pSceneMgr);
	for (std::vector<PoolBall*>::iterator l_it=poolBalls.begin();l_it<poolBalls.end();l_it++)
	{
		for (std::vector<PoolBall*>::iterator r_it=poolBalls.begin();r_it<poolBalls.end();r_it++)
		{
			if((*l_it)==(*r_it)) continue;
			if ((*l_it)->getPosition().squaredDistance((*r_it)->getPosition())<=Ogre::Math::Sqr((*l_it)->getRadius()+(*r_it)->getRadius()))
			{
				(*l_it)->getEntity()->getParentSceneNode()->showBoundingBox(true);
				(*r_it)->getEntity()->getParentSceneNode()->showBoundingBox(true);
			}
		}
	}
}

bool Snooker::compareAndJudge()
{
	return true;
}

void Snooker::nextRound()
{

}

void Snooker::update(Ogre::Real interval)
{
	CueGameCore::update(interval);
}

void Snooker::keyPress(const OIS::KeyEvent &keyEventRef,OIS::Keyboard* m_pKeyboard)
{
	CueGameCore::keyPress(keyEventRef,m_pKeyboard);

//#ifdef _DEBUG||NDEBUG
	if(m_pKeyboard->isKeyDown(OIS::KC_1)){
		poolBalls[YELLOW]->setVelocity(Ogre::Vector3(500,-4000,0));
		#ifdef _DEBUG||NDEBUG
			gameLog->logMessage("YELLOW Ball Launch!");
		#endif
	}
	if(m_pKeyboard->isKeyDown(OIS::KC_2)){
		poolBalls[GREEN]->setVelocity(Ogre::Vector3(-500,-4000,0));
#ifdef _DEBUG||NDEBUG
		gameLog->logMessage("Green Ball Launch!");
#endif
	}
	if(m_pKeyboard->isKeyDown(OIS::KC_3)){
		poolBalls[BROWN]->setVelocity(Ogre::Vector3(0,-8000,0));
		#ifdef _DEBUG||NDEBUG
				gameLog->logMessage("BROWN Ball Launch!");
		#endif
	}
	if(m_pKeyboard->isKeyDown(OIS::KC_4)){
		poolBalls[BLUE]->setVelocity(Ogre::Vector3(0,-5000,0));
		#ifdef _DEBUG||NDEBUG
				gameLog->logMessage("Blue Ball Launch!");
		#endif
	}
	if(m_pKeyboard->isKeyDown(OIS::KC_5)){
		poolBalls[PURPLE]->setVelocity(Ogre::Vector3(3000,-4000,0));
		#ifdef _DEBUG||NDEBUG
				gameLog->logMessage("Purple Ball Launch!");
		#endif
	}
	if(m_pKeyboard->isKeyDown(OIS::KC_6)){
		poolBalls[BLACK]->setVelocity(Ogre::Vector3(-3000,4000,0));
		#ifdef _DEBUG||NDEBUG
				gameLog->logMessage("Black Ball Launch!");
		#endif
	}
//#endif // _DEBUG
};

void Snooker::initialiseAudio(Ogre::SceneManager* sceneManager)
{
	CueGameCore::initialiseAudio(sceneManager);
}