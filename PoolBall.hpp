#ifndef _POOL_BALL_HPP
#define _POOL_BALL_HPP

#include <Ogre.h>
#include <OgreOggISound.h>
class PoolBall{
private:
	Ogre::Real timeSlice;
	Ogre::Real mass;
	Ogre::Real frictionCoefficient;
	Ogre::Real radius;
	Ogre::Vector3 velocity;
	Ogre::Entity *entity;
public:
	OgreOggSound::OgreOggISound* hitSound;

	PoolBall(Ogre::Real mass,Ogre::Real frictionCoefficient,Ogre::Real radius,Ogre::Entity *entity);
	~PoolBall();
	PoolBall *setTimeSlice(const Ogre::Real timeSlice);
	PoolBall *setMaterialName(const std::string materialName);
	PoolBall *setVelocity(Ogre::Vector3 velocity);
	PoolBall *changeVelocity(Ogre::Vector3 diffVector);
	Ogre::Real getTimeSlice(void);
	Ogre::Real getMass(void);
	Ogre::Real getFricCoef(void);
	Ogre::Real getRadius(void);
	Ogre::Vector3 getVelocity(void);
	Ogre::Vector3 getPosition(void);
	Ogre::Entity *getEntity(void);
};
#endif