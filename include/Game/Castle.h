#ifndef __GAME__CASTLE__H__
#define __GAME__CASTLE__H__

#include "Entity.h"

namespace Game
{
	class Castle : public Entity
	{
	public:
		Castle(irr::scene::ISceneManager*, irr::core::vector3df&);
		
		virtual void Update(float);
	};
}

#endif