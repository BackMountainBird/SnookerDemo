#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "Ogre.h"
#include "PoolBall.hpp"

namespace Physics
{
	const Ogre::Real VELOCITY_THRESHOLD=Ogre::Real(10);
	const Ogre::Real GRAVITY_ACCELERATION=Ogre::Real(9800);
	void preciseMove(PoolBall *poolBall,Ogre::Real interval);
	void fuzzyMove(PoolBall *poolBall,Ogre::Real);
	void setStop(PoolBall *poolBall);
	bool isStop(PoolBall *poolBall);
	bool isCollide(PoolBall * lhs,PoolBall* rhs,Ogre::Real interval);
	void collideTwinBall(PoolBall *lhs,PoolBall*rhs,Ogre::Real);
	bool collideBoundary(PoolBall *poolBall,int collideWallTag);
	Ogre::Real timeBeforeCollide(PoolBall *lhs,PoolBall *rhs);
	Ogre::Vector3 projectTo(Ogre::Vector3 src,Ogre::Vector3 dest);
}

#endif