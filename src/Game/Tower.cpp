#include "Game/Tower.h"

using namespace Game;

Tower::Tower(irr::scene::ISceneManager* p_sceneManager,
			 irr::core::vector3df p_position)
{
	m_animatedMesh = p_sceneManager->getMesh("resources/models/tower/tower.3ds");

	m_meshSceneNode = p_sceneManager->addAnimatedMeshSceneNode(m_animatedMesh);
	m_meshSceneNode->setPosition(p_position);

	SetMaterialFlags();
}

void Tower::updatePosition()
{
	irr::core::vector3df v = GetPosition();
	
	if (v.X < 250)
	{
		v += irr::core::vector3df(0.04f, 0.0f, 0.0f);

		SetPosition(v);
	}
}


Creature* Tower::SearchNearestCreature(std::list<Creature*>* p_creatureList)
{
	float distanceNearest = 0;
	Creature* objectNearest = NULL;

	for (std::list<Creature*>::const_iterator iterator = p_creatureList->begin(), end = p_creatureList->end(); iterator != end; ++iterator)
	{
		float x = GetPosition().X - (*iterator)->GetPosition().X;
		float y = GetPosition().Y - (*iterator)->GetPosition().Y;

		float result = std::sqrt(std::pow(x, 2.0f) + std::pow(y, 2.0f));

		if (distanceNearest == 0)
		{
			distanceNearest = result;
		}
		else if (result < distanceNearest)
		{
			distanceNearest = result;
			objectNearest = (*iterator);
		}
	}

	m_target = objectNearest;
	
	return objectNearest;
}

Projectile* Tower::ShootAtTarget(irr::scene::ISceneManager* p_sceneManager)
{
	Projectile* projectile = new Projectile(p_sceneManager, GetPosition());

	projectile->SetPosition(this->GetPosition());
	projectile->setFrom(this);
	projectile->setTo(m_target);

	return projectile;
}

double Tower::getHealthPoints()
{
	return m_healthPoints;
}

double Tower::getShootingSpeed()
{
	return m_shootingSpeed;
}

double Tower::getRange()
{
	return m_range;
}

void Tower::setHealthPoints(double p_healthPoints)
{
	m_healthPoints = p_healthPoints;
}

void Tower::setShootingSpeed(double p_shootingSpeed)
{
	m_shootingSpeed = p_shootingSpeed;
}

void Tower::setRange(double p_range)
{
	m_range = p_range;
}