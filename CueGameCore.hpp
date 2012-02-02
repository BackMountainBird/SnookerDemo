#ifndef CUE_GAME_CORE_HPP
#define CUE_GAME_CORE_HPP

#include "PoolBall.hpp"
#include "DotSceneLoader.hpp"
#include "Physics.hpp"

#include <OgreOggISound.h>
#include <OIS.h>
#include <vector>
#include <stack>
//////////////////////////////////////////////////////////////////////////

enum QueryFlags
{
	MASK_POOL_SURFACE	= 1<<0,
	MASK_POOL_CUSHION = 1<<1,
	MASK_POOL_STICK	= 1<<2,
	MASK_POOL_BALLS	= 1<<3
};

//////////////////////////////////////////////////////////////////////////

enum BallState{
	BALLSTATE_UNKOWN=-1,
	BALLSTATE_FALLEN=0,
	BALLSTATE_ONTABLE=1,
	BALLSTATE_FALLENDRETURN=2
};

//////////////////////////////////////////////////////////////////////////

class CueGameCore {
public:
	Ogre::Real TIMESLICE_THRESHOLD;
	int UPDATE_STEP;
	Ogre::Real STATS_MASS;						//�������
	Ogre::Real STATS_RESTITUTION_COEFFICIENT;	//��ײ�ָ�ϵ��
	Ogre::Real STATS_FRICTION_COEFFICIENT;		//��İ뾶
	Ogre::Real STATS_BALL_RADIUS;				//��İ뾶
	int STATS_BALL_NUMBER;						//����
	int STATS_TABLE_INNER_WIDTH;				//�����ڲ����
	int STATS_TABLE_OUTTER_WIDTH;				//�����ⲿ���
	int STATS_TABLE_INNER_LENGTH;				//�����ڲ�����
	int STATS_TABLE_OUTTER_LENGTH;				//�����ⲿ����
	int STATS_STICK_LENGTH;					//��˳���

	CueGameCore();
	~CueGameCore();
	virtual void beginGame(Ogre::SceneManager *);
	virtual bool compareAndJudge(void)=0;
	virtual void nextRound(void);
	virtual void update(Ogre::Real interval);
	virtual void keyPress(const OIS::KeyEvent &keyEventRef,OIS::Keyboard* m_pKeyboard);
	PoolBall* mostPossiblCollideBall(PoolBall* mainBall,Ogre::Real& time);
	int boundaryToCollide(PoolBall * poolBall,Ogre::Real &time);
	void adjustBall(PoolBall * poolBall);
protected:
	OgreOggSound::OgreOggISound* m_pSound;
	Ogre::Log* gameLog;
	BallState* previous_BallStates;
	BallState* current_BallStates;
	std::stack<PoolBall *>movingBalls;
	std::vector<PoolBall *> poolBalls;
	virtual void initialiseAudio(Ogre::SceneManager* sceneManager);
	void playSound3D(const std::string soundName,Ogre::SceneNode* sceneNode,float maxDistance=5000,float referenceDistance=2000);
};

#endif