#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT GUISceneGraph
	{
	public:
		void InitSceneGraphGUI();
		void RenderSceneGraphGUI();
		void RenderChildren(StrGameObjectPtr obj);

		StrGameObjectPtr m_ActiveGameObject;

	private:
		Vector<String> m_SceneGameObjects;
	};
};
