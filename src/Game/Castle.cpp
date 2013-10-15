#include "Game/Castle.h"

namespace Game
{
	Castle::Castle(irr::scene::ISceneManager* p_sceneManager,
			irr::core::vector3df& p_position)
	{
		m_animatedMesh = p_sceneManager->getMesh("resources/models/castle/castleofpeaches1.2.x");
	
		irr::scene::IAnimatedMeshSceneNode* animatedMeshSceneNode = p_sceneManager->addAnimatedMeshSceneNode(m_animatedMesh);
		m_meshSceneNode = animatedMeshSceneNode;
		m_meshSceneNode->setPosition(p_position);

		/*	
		int jointCount = animatedMeshSceneNode->getJointCount();

		for (int i = 0; i < jointCount; i++)
		{
			irr::scene::IBoneSceneNode* bone = animatedMeshSceneNode->getJointNode(i);
		}
		*/

		SetMaterialFlags();
	}
}