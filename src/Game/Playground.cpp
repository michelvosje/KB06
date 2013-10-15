#include "Game/Playground.h"

namespace Game
{
	Playground::Playground(irr::scene::ISceneManager* p_sceneManager)
	{
		m_sceneManager = p_sceneManager;
		m_pathBuilder = new PathBuilder();
		m_path = NULL;
		m_selector = NULL;

		m_gameStatus = GameStatus::BUILD_PHASE;
		m_playerHealth = 100;
		m_playerResources = 1000;
		Initialize(p_sceneManager);
	}

	Playground::~Playground()
	{
		delete m_pathBuilder;

		m_pathBuilder = NULL;
	}

	void Playground::Initialize(irr::scene::ISceneManager* p_sceneManager)
	{
		m_castle = new Castle(p_sceneManager, this, irr::core::vector3df(0, 0, -1200));
		m_stargate = new Stargate(p_sceneManager, this, irr::core::vector3df(0, 0, 1200));

		float range = 10.0f;
		int amount = 8;
		irr::core::vector3df* points1 = new irr::core::vector3df[amount];
		irr::core::vector3df* points2 = new irr::core::vector3df[amount];
		/*
		points1[0].set(50, 0, 0	);	points2[0].set(50, 0, 50);	//1
		points1[1].set(48, 0, 52);	points2[1].set(25, 0, 75);	//2
		points1[2].set(52, 0, 52);	points2[2].set(75, 0, 75);	//3
		points1[3].set(25, 0, 77);	points2[3].set(48, 0, 100);	//4
		points1[4].set(75, 0, 77);	points2[4].set(52, 0, 100);	//5
		points1[5].set(50, 0, 102);	points2[5].set(50, 0, 150);	//6
		points1[6].set(27, 0, 76);	points2[6].set(71, 0, 74);	//7
		*/

		points1[0].set(500, 0, 0);		points2[0].set(250, 0, 500);	//2
		points1[1].set(500, 0, 0);		points2[1].set(750, 0, 500);	//3
		points1[2].set(250, 0, 500);	points2[2].set(750, 0, 500);	//4
		points1[3].set(250, 0, 500);	points2[3].set(250, 0, 1000);	//5
		points1[4].set(750, 0, 500);	points2[4].set(750, 0, 1000);	//6
		points1[5].set(250, 0, 1000);	points2[5].set(750, 0, 1000);	//7
		points1[6].set(250, 0, 1000);	 points2[6].set(500, 0, 1500); //8
		points1[7].set(750, 0, 1000);	points2[7].set(500, 0, 1500); //9

		irr::core::vector3df startPoint(500.0f, 0.0f, 0.0f);
		irr::core::vector3df endPoint(500.0f, 0.0f, 1500.0f);

		SetupPath(points1, points2, amount, range, startPoint, endPoint);

		m_pathNumber = m_path->m_routes.begin();
		//Setup Waves
		m_waveNumber = 0;
		for(int i = 0; i<3; ++i)
		{
			waves.push_back(new Game::Wave(p_sceneManager, this));
		}

		Terrain* terrain = new Terrain();
		m_selector = terrain->GenerateTerrain(p_sceneManager, 10.0);
	}

	bool Playground::SetupPath(
			irr::core::vector3df* p_points1,
			irr::core::vector3df* p_points2,
			int p_amount,
			float p_range,
			irr::core::vector3df p_pointBegin,
			irr::core::vector3df p_pointEnd)
	{
		m_path = m_pathBuilder->BuildPath(p_points1, p_points2, p_amount, p_range, p_pointBegin, p_pointEnd);	
		return true;
	}

	void Playground::Update(float p_deltaTime)
	{
		//p_deltaTime *= 4;

		if (m_playerHealth <= 0)
		{
			m_gameStatus = GameStatus::GAME_OVER;
		}
		if (waves.size() == 0)
		{
			m_gameStatus = GameStatus::VICTORY;
		}

		std::list<Creature*>::iterator itCreature = m_creatures.begin();
		std::list<Creature*>::iterator itCreatureEnd = m_creatures.end();
		Creature* creature;

		//Update Creatures
		while (itCreature != itCreatureEnd)
		{
			creature = (*itCreature);
			++itCreature;

			creature->FollowPath(p_deltaTime);
		}

		//Update Towers
		std::list<Tower*>::iterator itTower = m_towers.begin();
		std::list<Tower*>::iterator itTowerEnd = m_towers.end();
		Tower* tower;

		while (itTower != itTowerEnd)
		{
			tower = (*itTower);
			++itTower;

			tower->ShootAtNearestCreature(m_creatures);
		}

		//Update Projectiles
		std::list<Projectile*>::iterator itProjectile = m_projectiles.begin();
		std::list<Projectile*>::iterator itProjectileEnd = m_projectiles.end();
		Projectile* projectile;

		while (itProjectile != itProjectileEnd)
		{
			projectile = (*itProjectile);
			++itProjectile;

			projectile->MoveTowardsTarget(p_deltaTime);
		}

		if (waves.size() != 0)
		{
			if (waves[0]->CheckWaveStatus(m_creatures))
			{
				waves[0]->SpawnCreature(m_creatures, *m_pathNumber,m_selector);
				if (*m_pathNumber == m_path->m_routes.back())
				{
					m_pathNumber = m_path->m_routes.begin();
				}
				else
				{
					std::advance(m_pathNumber,1);
				}
			}
			else
			{
				std::cout << m_waveNumber;
				m_gameStatus = GameStatus::BUILD_PHASE;
				waves.erase(waves.begin());
				m_playerResources += 1000;
			}
		}
	}

	void Playground::Render()
	{
		irr::video::IVideoDriver* videoDriver = m_sceneManager->getVideoDriver();

		std::list<PathPoint*>::iterator front = m_path->m_pathPoints->begin();
		std::list<PathPoint*>::iterator last = m_path->m_pathPoints->end();
		std::list<PathPoint*>::iterator it;
		PathPoint* pathPoint;

		for (it = front; it != last; ++it)
		{
			pathPoint = (*it);

			std::list<PathPoint*>::iterator front2 = pathPoint->m_pointsConnected.begin();
			std::list<PathPoint*>::iterator last2 = pathPoint->m_pointsConnected.end();
			std::list<PathPoint*>::iterator it2;

			for (it2 = front2; it2 != last2; ++it2)
			{
				PathPoint* pathPoint2 = (*it2);

				irr::core::vector3df start = pathPoint->m_point;
				irr::core::vector3df end = pathPoint2->m_point;
				irr::video::SColor color(255, 255, 0, 0);

				videoDriver->draw3DLine(start, end, color);
			}
		}		
	}

	void Playground::SpawnTower(irr::core::vector2d<irr::s32> p_position)
	{	
		if ((m_playerResources - 500 ) >= 0)
		{
			irr::scene::ISceneNode* sceneNodeOut;
			sceneNodeOut = m_sceneManager->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(p_position);

			std::list<Tower*>::iterator itTower;
			std::list<Tower*>::iterator itTowerEnd = m_towers.end();
			bool towerB = false;
			itTower = m_towers.begin();
			while (itTower != itTowerEnd)
			{		
				if (sceneNodeOut  == (*itTower)->GetSceneNode())
				{
					towerB = true;				
				} 
				++itTower;
			}

			if (!towerB)
			{
				m_playerResources -= 0;
				irr::core::line3d<irr::f32> line = m_sceneManager->getSceneCollisionManager()->getRayFromScreenCoordinates(p_position,m_sceneManager->getActiveCamera());
				irr::core::vector3df towerPosition;
				irr::core::triangle3df triangle;
				irr::scene::ISceneNode* sceneNodeOut;
				m_sceneManager->getSceneCollisionManager()->getCollisionPoint(line,m_selector, towerPosition,triangle, sceneNodeOut);
				m_towers.push_back(new Tower(m_sceneManager, this, towerPosition));
			}
		}	
	}

	void Playground::SellTower(irr::core::vector2d<irr::s32> p_position)
	{
		irr::scene::ISceneNode* sceneNodeOut;
		sceneNodeOut = m_sceneManager->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(p_position);

		std::list<Tower*>::iterator itTower;
		std::list<Tower*>::iterator itTowerEnd = m_towers.end();
		Tower* tower;

		itTower = m_towers.begin();

		while (itTower != itTowerEnd)
		{		
			if (sceneNodeOut  == (*itTower)->GetSceneNode())
			{
				tower = (*itTower);

				++itTower;
				sceneNodeOut->remove();
				delete tower;
				m_towers.remove(tower);
				m_playerResources += 250;

				return;
			}
			else
			{
				++itTower;
			}
		}
	}

	void Playground::StartNextWave()
	{
		if (m_gameStatus == GameStatus::BUILD_PHASE)
		{
			if (waves.size() != 0)
			{		
				Game::Wave* wave = waves[0];
				if (wave)
				{
					wave->SpawnWave(m_path->m_pointBegin->m_point);
					m_gameStatus = GameStatus::WAVE_SPAWNED;				
					++m_waveNumber;
				}
			}
		}
	}

	int Playground::GetWaveNumber()
	{
		return m_waveNumber;
	}

	int Playground::GetAmountOfCreatures()
	{
		return m_creatures.size();

	}

	int Playground::GetGameStatus()
	{
		return m_gameStatus;
	}

	int Playground::GetPlayerHealth()
	{
		return m_playerHealth;
	}

	int Playground::GetPlayerResources()
	{
		return m_playerResources;
	}

	void Playground::ProjectileCreated(Projectile* p_projectile)
	{
		if (p_projectile != NULL)
		{
			m_projectiles.push_back(p_projectile);
		}
	}

	void Playground::ProjectileDestroyed(Projectile* p_projectile)
	{
		if (p_projectile != NULL)
		{
			m_projectiles.remove(p_projectile);
			delete p_projectile;
		}
	}

	void Playground::CreatureHit(Creature* p_creature, Projectile* p_projectile)
	{
		if (p_creature != NULL && p_projectile != NULL)
		{

		}
	}

	void Playground::CreatureCreated(Creature* p_creature)
	{
		if (p_creature != NULL)
		{
			m_creatures.push_back(p_creature);
		}
	}

	void Playground::CreatureDestroyed(Creature* p_creature)
	{
		if (p_creature != NULL)
		{
			m_creatures.remove(p_creature);
			delete p_creature;
		}
	}

	void Playground::CreatureRouteEndReached(Creature* p_creature)
	{
		if (p_creature != NULL)
		{
			m_creatures.remove(p_creature);
			delete p_creature;

			m_playerHealth -= 1;
		}
	}
}