#ifndef __GAME__PATHBUILDER__H__
#define __GAME__PATHBUILDER__H__

#include "Path.h"
#include "PathSegment.h"

#include <irrlicht.h>

namespace Game
{
	/**
	 * @brief	The PathBuilder handles the creation of a Path and PathRoute(s)
	 *
	 *			
	 * @author	Michel van Os
	 * @author	Menno Postma
	 */
	class PathBuilder
	{
	public:
		/**
		 * @brief	Build a path out of given pairs of PathPoints 
		 *
		 *			Builds the path out of given pairs of pathpoints and generates all possible routes
		 * @param	p_points1 is a list with the first points of a pair
		 * @param	p_points2 is a list with the second points of a pair
		 * @param	p_pointsCount is the amount of points
		 * @param   P_pointBegin is a vector3df of the point where the path should begin
		 * @param   P_pointBegin is a vector3df of the point where the path should end
		 */
		Path* BuildPath(
				irr::core::vector3df* p_points1,
				irr::core::vector3df* p_points2,
				int p_pointsCount,
				float p_range,
				irr::core::vector3df p_pointBegin,
				irr::core::vector3df p_pointEnd);

	private:
		/**
		 * @brief	Returns true if the direction from point1 to point2 is allowed
		 * @param	p_path is a Path
		 * @param	p_pathpoint1 is the direction from
		 * @param   p_pathpoint2 is the direction to
		 * @author	Michel van Os
		 * @author	Menno Postma
		 */
		bool IsPathDirectionAllowed(Path* p_path, PathPoint* p_pathPoint1, PathPoint* p_pathPoint2);
		
		/**
		 * @brief	Saves the directions used in a route into the allowed list
		 * @param	p_path is a Path
		 * @param	p_pathRoute is a PathRoute
		 * @author	Michel van Os
		 * @author	Menno Postma
		 * @todo	Currently does not avoid duplicates
		 */
		void AddAllowedPathDirectionsFromRoute(Path* p_path, std::list<PathPoint*>* p_pathRoute);

		/**
		 * @brief	Recursively build all routes possible in a path
		 * @author	Menno Postma
		 * @author	Michel van Os
		 */
		void BuildRoute(Path* path, std::list<PathPoint*>*& p_pathRoute, PathPoint* p_pathPointCurrent);

		/**
		 * @brief	Creates the connection between a pair of points and puts them in a list
		 * @param	p_points1 is the first point of the pair
		 * @param	p_points2 is the second point of the pair
		 * @param	p_pointsCount is the amount of points
		 * @author	Michel van Os
		 * @author	Menno Postma
		 */
		std::list<PathPoint*>* CreatePathPoints(
				irr::core::vector3df* p_points1,
				irr::core::vector3df* p_points2,
				int p_pointsCount);

		/**
		 * @brief	Joins the PathPoint(s) of different PathPoint pairs
		 * @param	p_path is a Path
		 * @param	p_range is the range in wich the points will be joined
		 * @author	Michel van Os
		 * @author	Menno Postma
		 */
		void PathJoinPathPoints(Path* p_path, float p_range);

		/**
		 * @brief	Determines the begin and end PathPoint of a path
		 * @param	p_path is a Path
		 * @param	p_pointBegin is a vector3df of point where the path should begin
		 * @param	p_pointEnd is a vector3df of point where the path should end
		 * @author	Michel van Os
		 * @author	Menno Postma
		 */
		void PathDetermineBeginAndEndPathPoints(
				Path* p_path,
				irr::core::vector3df p_pointBegin,
				irr::core::vector3df p_pointEnd,
				float p_range);
	};
}

#endif