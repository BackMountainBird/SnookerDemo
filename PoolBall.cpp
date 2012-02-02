#include "PoolBall.hpp"

PoolBall::~PoolBall()
{
	entity->getParentSceneNode()->detachObject(entity);
	delete entity;
}

PoolBall::PoolBall(Ogre::Real mass,Ogre::Real frictionCoefficient,Ogre::Real radius,Ogre::Entity * entity)
{
	this->timeSlice=0;
	this->mass=mass;
	this->frictionCoefficient=frictionCoefficient;
	this->radius=radius;
	this->entity=entity;
	this->velocity=Ogre::Vector3(0,0,0);
}

PoolBall* PoolBall::setMaterialName(const std::string materialName)
{
	entity->setMaterialName(materialName);
	return this;
}

PoolBall *PoolBall::setVelocity(Ogre::Vector3 velocity)
{
	this->velocity=velocity;
	return this;
}

PoolBall *PoolBall::changeVelocity(Ogre::Vector3 diffVector)
{
	this->velocity+=diffVector;
	return this;
}

Ogre::Real PoolBall::getMass(void)
{
	return this->mass;
}

Ogre::Real PoolBall::getFricCoef(void)
{
	return this->frictionCoefficient;
}

Ogre::Real PoolBall::getRadius(void)
{
	return this->radius;
}

Ogre::Vector3 PoolBall::getVelocity(void)
{
	return this->velocity;
}

Ogre::Vector3 PoolBall::getPosition(void)
{
	return getEntity()->getParentSceneNode()->getPosition();
}
Ogre::Entity *PoolBall::getEntity(void)
{
	return this->entity;
}

PoolBall *PoolBall::setTimeSlice(const Ogre::Real timeSlice)
{
	if(timeSlice>=0)
		this->timeSlice=timeSlice;
	else
		this->timeSlice=0;
	return this;
}

Ogre::Real PoolBall::getTimeSlice(void)
{
	return timeSlice;
}