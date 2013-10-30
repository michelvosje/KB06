#ifndef __GAME__SCOREMANAGER__H__
#define __GAME__SCOREMANAGER__H__

namespace Game
{
	class ScoreManager
	/**
	 * @todo
	 *			
	 * @author	Alex Hodes
	 * @todo comments
	 */
	{
	public:
		/**
		* @brief	TODO
		*/	
		ScoreManager();

		/**
		* @brief	TODO
		*/	
		int GetPlayerScore(int p_playerNumber);

	   /**
		* @todo
		*/
		bool CanBuyPencil(int p_playerNumber);

	   /**
		* @todo
		*/
		bool CanCreateTower(int p_playerNumber);

	   /**
		* @todo
		*/
		bool CanUpgradeTowerSpeed(int p_playerNumber);

	   /**
		* @todo
		*/
		bool CanUpgradeTowerRange(int p_playerNumber);

	   /**
		* @todo
		*/
		bool CanUpgradeTowerDamage(int p_playerNumber);

		/**
		 * @brief	TODO
		 */	
		void SetPlayerScore(int p_playerNumber, int p_score);
		/**
		 * @brief	TODO
		 */
		void ResetPlayerScore(int p_playerNumber);

		/**
		 * @brief	TODO
		 */
		void CastleReached(int p_playerNumber);

		/**
		 * @brief	TODO
		 */
		void PencilBought(int p_playerNumber);

		/**
		 * @brief	TODO
		 */
		void CreatureKilled(int p_playerNumber);

	   /**
		* @todo
		*/
		void TowerCreated(int p_playerNumber);

	   /**
		* @todo
		*/
		void TowerDestroyed(int p_playerNumber);

		/**
		 * @brief	TODO
		 */
		void TowerIncreasedSpeed(int p_playerNumber);

		/**
		 * @brief	TODO
		 */
		void TowerIncreasedRange(int p_playerNumber);

		/**
		 * @brief	TODO
		 */
		void TowerIncreasedDamage(int p_playerNumber);

	private:
		int m_playerPoints[2];
		int m_playerPointsDefault[2];
		
		int m_pencilsOwned;

		int m_pointsCastleReached;
		int m_pointsPencilBought;
		int m_pointsCreatureKilled;
		int m_pointsTowerCreated;
		int m_pointsTowerDestroyed;
		int m_pointsTowerIncreasedSpeed;
		int m_pointsTowerIncreasedRange;
		int m_pointsTowerIncreasedDamage;
	};
}

#endif