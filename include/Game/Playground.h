#ifndef __GAME__PLAYGROUND__H__
#define __GAME__PLAYGROUND__H__

#include "GameListener.h"
#include "PlaygroundListener.h"
#include "Path.h" 
#include "PathBuilder.h"
#include "Tower.h"
#include "Creature.h"
#include "Projectile.h"
#include "PathFollower.h"
#include "Creature.h"
#include "Wave.h"
#include "Castle.h"
#include "Stargate.h"
#include "Terrain.h"
#include "Marker.h"
#include "Utility/Logger.h"

#include <vector>
#include <irrlicht.h>
#include <math.h>

namespace Game
{
	/**
	* @brief	Playground is an area the game is played on
	*
	*			The playground consist of Tower(s)  Monster(s)  and a Path
	* @author	Michel van Os
	* @author	Menno Postma
	* @author	Thomas Gravekamp
	* @todo		comments
	*/
	class Playground : public PlaygroundListener
	{
	public:
	   /**
		* @todo
		*/
		Playground(GameListener* p_gameListener, irr::scene::ISceneManager* p_sceneManager);

	   /**
		* @brief destructs the playground
		*/
		~Playground();

	   /**
		* @brief	Initializes the playground
		*
		*			1. Generate the creature waves. 
		*			2. Generate Path
		*			3. Creates terrain
		*			4. Creates PathMarker
		* @param	p_sceneManager is the sceneManager
		*/
		void Initialize(irr::scene::ISceneManager* p_sceneManager);

	   /**
		* @brief	Updates all game objects on the playground
		*
		*			This is called every game-loop. This will update the state, position and rotation of various objects on the playground
		* @param	p_deltaTime is the deltaTimer
		*/
		void Update(float p_deltaTime);

	   /**
		* @brief	Scales the te terrain, stargate and castle to match the gamelength
		* @param	p_gameLength is the length of the longest edge of the table
		*/
		void UpdateGameScale(float p_gameLength);

	   /**
		* @todo		Probable won't be used in the near future
		*/
		void Render();

		/**
		 * @todo	SetupPath(,,,,,) moet vervangen worden door deze
		 * @brief	Used to build the path
		 * @param	p_points1 is the first point of a line
		 * @param	P_points2 is the second point of a line
		 * @param	p_amount is the amount of lines
		 * @author	Michel van Os
		 */
		bool SetupPath(
				irr::core::vector3df* p_points1,
				irr::core::vector3df* p_points2,
				int p_amount);

		bool SetupPath(
				irr::core::vector3df* p_points1,
				irr::core::vector3df* p_points2,
				int p_amount,
				float p_range,
				irr::core::vector3df p_pointBegin,
				irr::core::vector3df p_pointEnd
			);

		/**
		 * @brief	Create a Tower at the given 2d screen coordinates.
		 *
		 *			A Tower can only be created if it doesn't collide with
		 *			Towers, Creatures or the Path.
		 * @author	Michel van Os.
		 * @param	p_positon The screen coordinates.
		 * @return	Returns True if a Tower has been created at the screen coordinates.
		 * @todo	Make sure it can't be created at the position of other entities.
		 */
		bool CreateTower(irr::core::vector2di p_position);

		/**
		 * @brief	Destroy a Tower at the given 2d screen coordinates.
		 *
		 * @author	Michel van Os.
		 * @param	p_positon The screen coordinates.
		 * @return	Returns True if a Tower has been destroyed at the screen coordinates.
		 */
		bool DestroyTower(irr::core::vector2di p_position);

		/**
		 * @brief	Increase the fire rate of the Tower at the given 2d screen coordinates.
		 *
		 * @author	Michel van Os.
		 * @param	p_positon The screen coordinates.
		 * @return	Returns True if a Tower has been found and upgraded at the screen coordinates.
		 */
		bool UpgradeTowerSpeed(irr::core::vector2di p_position);

		/**
		 * @brief	Increase the fire range of the Tower at the given 2d screen coordinates.
		 
		 * @author	Michel van Os.
		 * @param	p_positon The screen coordinates.
		 * @return	Returns True if a Tower has been found and upgraded at the screen coordinates.
		 */
		bool UpgradeTowerRange(irr::core::vector2di p_position);

		/**
		 * @brief	Increase the fire damage of the Tower at the given 2d screen coordinates.
		 *
		 * @author	Michel van Os.
		 * @param	p_positon The screen coordinates.
		 * @return	Returns True if a Tower has been found and upgraded at the screen coordinates.
		 */
		bool UpgradeTowerDamage(irr::core::vector2di p_position);

		/**
		 * @brief	Starts the next Wave.
		 *
		 *			If the Wave has been used in the previous round
		 *			the amount of spawned Creatures will be reset automatically.
		 * @author	Michel van Os.
		 */
		void StartNextWave();

		/**
		 * @brief	Returns whether all Waves are finished.
		 *
		 *			All Creatures for all Waves should have been spawned
		 *			and all Creatures should been killed or reached the castle.
		 * @author	Michel van Os.
		 * @return	Returns true if all Waves are finished.
		 * @todo	Check if the codition works.
		 */
		bool AreAllWavesFinished();

		/**
		 * @brief	Returns the current Wave number.
		 *
		 * @author	Michel van Os.
		 * @return	The current Wave number.
		 */
		int GetWaveNumber();

		/**
		 * @brief	Returns the amount of Creatures currently alive.
		 *
		 * @author	Michel van Os.
		 * @return	The amount of Creatures currently alive.
		 */
		int GetAmountOfCreatures();

	   /**
		* @brief	returns the amount of creatures spawned by the current wave
		*/
		int GetCreaturesSpawned();

	   /**
		* @brief	returns the size of the wave
		*/
		int GetWaveSize();

	   /**
		* @brief	returns the heigth of the playground
		*/
		float GetPlaygroundHeight();

		//------PlaygroundListenerEvents------

	   /**
		* @brief	adds the new projectile to the list
		*/
		void OnProjectileCreated(Projectile* p_projectile);

	   /**
		* @brief	removes the projectile
		*/
		void OnProjectileDestroyed(Projectile* p_projectile);

	   /**
		* @brief	adds the creature to the list
		*/
		void OnCreatureCreated(Creature* p_creature);

	   /**
		* @brief	removes the creature
		*/
		void OnCreatureDestroyed(Creature* p_creature);

	   /**
		* @brief	decreases creatures health based on the projectile
		*/
		void OnCreatureHit(Creature* p_creature, Projectile* p_projectile);

	   /**
		* @brief	removes the creature and change score
		*/
		void OnCreatureRouteEndReached(Creature* p_creature);

	private:
		GameListener* m_gameListener;
		irr::scene::ISceneManager* m_sceneManager;
		
		PathBuilder* m_pathBuilder;
		Path* m_path;
		float m_pointRange;
		irr::core::vector3df m_pointBegin;
		irr::core::vector3df m_pointEnd;

		std::list<Creature*> m_creatures;
		std::list<Tower*> m_towers;
		std::list<Projectile*> m_projectiles;
		std::list<PathFollower*> m_pathFollowers;
		std::list<PathRoute*>::iterator m_pathRouteSelected;
		std::list<Marker*> m_markers;

		std::vector<Wave*> m_waves;
		int m_waveNumber;

		Terrain* m_terrain;
		irr::scene::ITriangleSelector* m_selector;
		irr::core::dimension2d<float> m_gameDimensions;

		Castle* m_castle;
		Stargate* m_stargate;

	   /**
		* @brief calculates the speedscale
		*/
		float CalculateSpeedScale();

		/**
		 * @brief	Generates Waves with the amount of Creatures to spawn as 
		 *			described in the Game Design Document.
		 *
		 * @author	Michel van Os.
		 */
		void GenerateWaves();

		/**
		 * @brief	Returns the Tower at the given 2d screen coordinates.
		 * @author	Michel van Os.
		 * @param	p_position The 2d coordinates on the screen.
		 * @return	The Tower at the given 2d screen coordinates.
		 *			It return NULL when there's no Tower at the 2d screen coordinates.
		 * @todo	Doesn't work properly? It only finds the Tower when you click at it's center?
		 */
		Tower* GetTowerAtPosition(irr::core::vector2di p_position);

		/**
		 * @brief	Creates Markers at all PathPoints in the Path.
		 *
		 *			For every PathPoint in m_path a Marker will be created.
		 *			The position of the Marker will be the position of the
		 *			PathPoint. But the Y-coordinate will be 100 units above
		 *			the Terrain m_terrain Y-coordinate.
		 * @author	Michel van Os.
		 */
		void CreatePathPointMarkers();

		/**
		 * @brief	Connects the Path with the Startgate and the Castle.
		 *
		 *			Updates the Path with a new begin PathPoint and
		 *			a new end PathPoint. It also creates the new
		 *			PathSegments and updates all PathRoutes.
		 * @author	Michel van Os.
		 */
		void ConnectPathToStargateAndCastle();
	};
}

#endif