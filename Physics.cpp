#include "Physics.hpp"
namespace Physics{
	Ogre::Vector3 projectTo(Ogre::Vector3 src,Ogre::Vector3 dest)
	{
		return src.dotProduct(dest)*dest/dest.squaredLength();
	}

	void preciseMove(PoolBall *poolBall,Ogre::Real interval)
	{
		Ogre::SceneNode *sceneNode=poolBall->getEntity()->getParentSceneNode();
		Ogre::Vector3 oldVelocity=poolBall->getVelocity();
		Ogre::Vector3 frictionDirection=oldVelocity;
		frictionDirection.normalise();
		Ogre::Vector3 newVelocity=oldVelocity-frictionDirection*poolBall->getFricCoef()*interval*GRAVITY_ACCELERATION;
		Ogre::Vector3 transVec=(oldVelocity+newVelocity)*interval/2;
		sceneNode->translate(transVec);
		sceneNode->pitch(Ogre::Radian(transVec.y/poolBall->getRadius()));
		sceneNode->yaw(Ogre::Radian(transVec.x/poolBall->getRadius()));
		poolBall->setVelocity(newVelocity);
//#ifdef _DEBUG
//		if((abs(sceneNode->getPosition().x)>1000)||(abs(sceneNode->getPosition().y)>2000))
//		{
//			setStop(poolBall);
//		}
//#endif // _DEBUG
	}

	void fuzzyMove(PoolBall* poolBall,Ogre::Real interval)
	{
		Ogre::SceneNode *sceneNode=poolBall->getEntity()->getParentSceneNode();
		sceneNode->setPosition(sceneNode->getPosition()+poolBall->getVelocity()*interval);
	}

	void setStop(PoolBall *poolBall)
	{
		poolBall->setVelocity(Ogre::Vector3(0,0,0));
	}

	bool isStop(PoolBall *poolBall)
	{
		return poolBall->getVelocity().squaredLength()<=VELOCITY_THRESHOLD*VELOCITY_THRESHOLD;
	}

	bool isCollide(PoolBall * lhs,PoolBall* rhs,Ogre::Real interval)
	{
		Ogre::Real distance=
			(lhs->getPosition()+lhs->getVelocity()*interval).distance(
			(rhs->getPosition()+rhs->getVelocity()*interval));
		if (!((isStop(lhs))&&isStop(rhs)))
			if (distance<=(lhs->getRadius()+rhs->getRadius()))
				return true;
		return false;
	}

	void collideTwinBall(PoolBall *lhs,PoolBall*rhs,Ogre::Real restCoef)
	{
		Ogre::Vector3 deltaVelo=lhs->getVelocity()-rhs->getVelocity();
		Ogre::Vector3 deltaPos=lhs->getPosition()-rhs->getPosition();
		Ogre::Real	l_m=lhs->getMass();
		Ogre::Real	r_m=rhs->getMass();
		lhs->changeVelocity(-projectTo(deltaVelo,deltaPos));
		rhs->changeVelocity(l_m*projectTo(deltaVelo,deltaPos)/r_m);
	}

	bool collideBoundary(PoolBall *poolBall,int collideWallTag){
		if (collideWallTag==1)
		{
			poolBall->setVelocity(poolBall->getVelocity().reflect(Ogre::Vector3(1,0,0)));
			return true;
		}else if(collideWallTag==2){
			poolBall->setVelocity(poolBall->getVelocity().reflect(Ogre::Vector3(0,1,0)));
			return true;
		}else if(collideWallTag==3){
			poolBall->setVelocity(poolBall->getVelocity().reflect(Ogre::Vector3(1,0,0)));
			poolBall->setVelocity(poolBall->getVelocity().reflect(Ogre::Vector3(0,1,0)));
			return true;
		}
		return false;
	}

	Ogre::Real timeBeforeCollide(PoolBall *lhs,PoolBall *rhs)
	{
		Ogre::Vector3 deltaPos=lhs->getPosition()-rhs->getPosition();
		Ogre::Vector3 deltaVelo=lhs->getVelocity()-rhs->getVelocity();
		Ogre::Real lresult=(-deltaPos.dotProduct(deltaVelo)
			-Ogre::Math::Sqrt(deltaVelo.squaredLength()*Ogre::Math::Sqr(lhs->getRadius()+rhs->getRadius())
			-deltaPos.crossProduct(deltaVelo).squaredLength()))/deltaVelo.squaredLength();
		Ogre::Real rresult=(-deltaPos.dotProduct(deltaVelo)
			+Ogre::Math::Sqrt(deltaVelo.squaredLength()*Ogre::Math::Sqr(lhs->getRadius()+rhs->getRadius())
			-deltaPos.crossProduct(deltaVelo).squaredLength()))/deltaVelo.squaredLength();
		return lresult;
	}
}