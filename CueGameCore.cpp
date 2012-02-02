#include "AdvancedOgreFramework.hpp"
#include "CueGameCore.hpp"
#include "Physics.hpp"

#include <time.h>
#include "boost/lexical_cast.hpp"
/************************************************************************/
/*Do Some Initialising work here									*/
/************************************************************************/
using namespace Ogre;
CueGameCore::CueGameCore()
{
	time_t now;
	time(&now);
	tm *temptm=gmtime(&now);
	char           today[32]     = " ";
	char           second[32]     = " "; 
	sprintf(today, "%d-%02d-%02d-",temptm-> tm_year+1900,temptm-> tm_mon+1,temptm-> tm_mday);    
	sprintf(second, "%02d:%02d:%02d ",temptm-> tm_hour,temptm-> tm_min,temptm-> tm_sec);
	std::string str(today);
	str=str.append(second);
	gameLog=LogManager::getSingleton().createLog("gameLog.log");
	gameLog->logMessage("Game Log initialize complete");
}

CueGameCore::~CueGameCore()
{
	if (gameLog)	{delete gameLog;}
	if (previous_BallStates)	{delete previous_BallStates;}
	if (current_BallStates)	{delete current_BallStates;}
	for   (std::vector<PoolBall*>::iterator it=poolBalls.begin();it!=poolBalls.end();++it)
	{  
		delete   *it;
	}
}
void CueGameCore::beginGame(SceneManager * m_pSceneMgr)
{
	m_pSceneMgr->createLight("Light")->setPosition(0,0,2000);
	m_pSound=0;
	Plane poolPlane(Vector3::UNIT_Z,0);
	MeshPtr pGround = MeshManager::getSingleton().createPlane(
		"meshPoolPlane",
		"General",
		poolPlane,
		this->STATS_TABLE_INNER_WIDTH,
		this->STATS_TABLE_INNER_LENGTH,
		1,
		1,
		true,
		1,
		1.0F,
		1.0F,
		Vector3::UNIT_Y
		);
	m_pSceneMgr->getRootSceneNode()->createChildSceneNode("SceneNode_Plane")->attachObject(
		m_pSceneMgr->createEntity("Entity_Table","meshPoolPlane","General")
		);
	m_pSceneMgr->getEntity("Entity_Table")->setMaterialName("Snooker_Table/surface");
	m_pSceneMgr->getEntity("Entity_Table")->setQueryFlags(MASK_POOL_SURFACE);
	m_pSceneMgr->getSceneNode("SceneNode_Plane")->createChildSceneNode("SceneNode_BallRoot");
	DotSceneLoader* pDotSceneLoader = new DotSceneLoader();
	pDotSceneLoader->parseDotScene("SnookerInit.xml", "General", m_pSceneMgr, m_pSceneMgr->getSceneNode("SceneNode_BallRoot"));
	delete pDotSceneLoader;
}


void CueGameCore::nextRound()
{

}

PoolBall* CueGameCore::mostPossiblCollideBall(PoolBall* mainBall,Ogre::Real& time)
{
	PoolBall* result=NULL;
	Ogre::Real TIME=mainBall->getTimeSlice();
	for(std::vector<PoolBall*>::iterator it=poolBalls.begin();it!=poolBalls.end();it++)
	{
		if((*it)==mainBall) continue;
		time=Physics::timeBeforeCollide(mainBall,(*it));
		if(time<0) continue;
		if(time<TIME)
		{
			TIME=time;
			result=(*it);
		}
	}
	time=TIME;
	return result;
}

int CueGameCore::boundaryToCollide(PoolBall * poolBall,Ogre::Real &time){
	Ogre::Real TIME=poolBall->getTimeSlice();
	Ogre::Real timeX=0,timeY=0;

	if(poolBall->getVelocity().x>0)
		timeX=(this->STATS_TABLE_INNER_WIDTH-2*poolBall->getPosition().x)/(2*poolBall->getVelocity().x);
	else if(poolBall->getVelocity().x<0)
		timeX=(-this->STATS_TABLE_INNER_WIDTH-2*poolBall->getPosition().x)/(2*poolBall->getVelocity().x);
	else return 0;

	if(poolBall->getVelocity().y>0)
		timeY=(this->STATS_TABLE_INNER_LENGTH-2*poolBall->getPosition().y)/(2*poolBall->getVelocity().y);
	else if(poolBall->getVelocity().y<0)
		timeY=(-this->STATS_TABLE_INNER_LENGTH-2*poolBall->getPosition().y)/(2*poolBall->getVelocity().y);
	else return 0;
#ifdef _DEBUG||NDEBUG
	gameLog->logMessage("TIME:"+boost::lexical_cast<std::string>(TIME)+"timeX:"+boost::lexical_cast<std::string>(timeX)+"timeY:"+boost::lexical_cast<std::string>(timeY));
	gameLog->logMessage("Pos:"+Ogre::StringConverter::toString(poolBall->getPosition())+"velo:"+Ogre::StringConverter::toString(poolBall->getVelocity()));
#endif
	if(((timeX>TIME)&&(timeY>TIME))||((timeX<0)&&(timeY<0)))
		return 0;
	if((timeX<timeY)&&(timeX>=0)&&(timeX<=TIME)){
		time=timeX;
		return 1;
	}else if((timeX>timeY)&&(timeY>=0)&&(timeY<=TIME)){
		time=timeY;
		return 2;
	}else if((timeX<=TIME)&&(timeY<=TIME)){
		time=timeX;
		return 1&2;
	}
	return 0;
}

void CueGameCore::adjustBall(PoolBall * poolBall)
{
	if(poolBall->getPosition().x>this->STATS_TABLE_INNER_WIDTH/2){
		Physics::collideBoundary(poolBall,1);
		poolBall->getEntity()->getParentSceneNode()->setPosition(
			Ogre::Vector3(STATS_TABLE_INNER_WIDTH/2,
			poolBall->getPosition().y,
			poolBall->getPosition().z)
		);
	poolBall->hitSound->play(true);
	}
	else if(poolBall->getPosition().x<-this->STATS_TABLE_INNER_WIDTH/2){
		Physics::collideBoundary(poolBall,1);
		poolBall->getEntity()->getParentSceneNode()->setPosition(
			Ogre::Vector3(-STATS_TABLE_INNER_WIDTH/2,
			poolBall->getPosition().y,
			poolBall->getPosition().z)
			);
	poolBall->hitSound->play(true);
	}
	if(poolBall->getPosition().y>this->STATS_TABLE_INNER_LENGTH/2){
		Physics::collideBoundary(poolBall,2);
		poolBall->getEntity()->getParentSceneNode()->setPosition(
		Ogre::Vector3(poolBall->getPosition().x,
		STATS_TABLE_INNER_LENGTH/2,
		poolBall->getPosition().z)
		);
	poolBall->hitSound->play(true);
	}
	else if(poolBall->getPosition().y<-this->STATS_TABLE_INNER_LENGTH/2){
		Physics::collideBoundary(poolBall,2);
		poolBall->getEntity()->getParentSceneNode()->setPosition(
		Ogre::Vector3(poolBall->getPosition().x,
		-STATS_TABLE_INNER_LENGTH/2,
		poolBall->getPosition().z)
		);
	poolBall->hitSound->play(true);
	}
}

void CueGameCore::update(Ogre::Real interval)
{
	gameLog->logMessage(Ogre::StringConverter::toString(interval));
	for(std::vector<PoolBall*>::iterator l_it=poolBalls.begin();l_it!=poolBalls.end();++l_it)
	{
		(*l_it)->setTimeSlice(interval/*+(*l_it)->getTimeSlice()*/);
		if(!Physics::isStop(*l_it))
			movingBalls.push(*l_it);
#ifdef _DEBUG
		gameLog->logMessage((*l_it)->getEntity()->getName()+" "+Ogre::StringConverter::toString((*l_it)->getTimeSlice()));
#endif
	}
	PoolBall* currentBall,*targetBall;
	int boundary=0;
	while(!movingBalls.empty())
	{
		currentBall=movingBalls.top();
		movingBalls.pop();
		Ogre::Real time;
		adjustBall(currentBall);
		targetBall=mostPossiblCollideBall(currentBall,time);
#ifdef _DEBUG
		if(targetBall)
			gameLog->logMessage("Most possible ball to collide:"+targetBall->getEntity()->getName());
#endif
		if (targetBall)
		{
#ifdef _DEBUG
				gameLog->logMessage(currentBall->getEntity()->getName()+
					" collides with "+
					targetBall->getEntity()->getName());
#endif
				Physics::fuzzyMove(currentBall,time);
				Physics::fuzzyMove(targetBall,time);
				currentBall->setTimeSlice(currentBall->getTimeSlice()-time);
				targetBall->setTimeSlice(targetBall->getTimeSlice()-time);
				Physics::collideTwinBall(currentBall,targetBall,STATS_RESTITUTION_COEFFICIENT);
				//playSound3D("Game_Hit",currentBall->getEntity()->getParentSceneNode());
				currentBall->hitSound->play(true);
		}else if(boundary=boundaryToCollide(currentBall,time)){
			Physics::fuzzyMove(currentBall,time);
			currentBall->setTimeSlice(currentBall->getTimeSlice()-time);
			Physics::collideBoundary(currentBall,boundary);
			currentBall->hitSound->play(true);
		}else{
#ifdef _DEBUG
			gameLog->logMessage(currentBall->getEntity()->getName()+
				" precise move."
				);
#endif
			Physics::preciseMove(currentBall,currentBall->getTimeSlice());
			currentBall->setTimeSlice(0);
		}
		if(Physics::isStop(currentBall))
			Physics::setStop(currentBall);
		else if(currentBall->getTimeSlice()>TIMESLICE_THRESHOLD)
			movingBalls.push(currentBall);
		if(targetBall)
		{
			if(Physics::isStop(targetBall))
				Physics::setStop(targetBall);
			else if(targetBall->getTimeSlice()>TIMESLICE_THRESHOLD)
				movingBalls.push(targetBall);
		}
	}
}

void CueGameCore::keyPress(const OIS::KeyEvent &keyEventRef,OIS::Keyboard* m_pKeyboard)
{

}

void CueGameCore::initialiseAudio(Ogre::SceneManager* sceneManager)
{
	for (std::vector<PoolBall*>::iterator it=poolBalls.begin();it!=poolBalls.end();++it)
	{
		(*it)->hitSound=OgreFramework::getSingletonPtr()->m_pSoundManager
			->createSound((*it)->getEntity()->getName()+"_Hit","hit.wav",false,false,false,sceneManager,true);
		(*it)->hitSound->disable3D(false);
		(*it)->hitSound->setMaxDistance(5000);
		(*it)->hitSound->setReferenceDistance(2000);
		(*it)->getEntity()->getParentSceneNode()->attachObject((*it)->hitSound);
	}
}

void CueGameCore::playSound3D(const std::string soundName,Ogre::SceneNode* sceneNode,float maxDistance,float referenceDistance)
{
	if (m_pSound = OgreFramework::getSingletonPtr()->m_pSoundManager->getSound(soundName))
	{
		if(sceneNode)
		{
			m_pSound->disable3D(false);
			m_pSound->setMaxDistance(maxDistance);
			m_pSound->setReferenceDistance(referenceDistance);
			sceneNode->attachObject(m_pSound);
			m_pSound->play();
			sceneNode->detachObject(m_pSound);
		}
	}
}