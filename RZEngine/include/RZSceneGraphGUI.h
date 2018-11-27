#pragma once

namespace rczEngine
{
	class RZ_EXP GUISceneGraph
	{
	public:
		void InitSceneGraphGUI();
		void RenderSceneGraphGUI();
		void RenderChildren(StrGameObjectPtr obj);

		StrGameObjectPtr m_ActiveGameObject;

	private:
		Vector<String> m_SceneGameObjects;
		String m_SceneFileName;
	};
};
