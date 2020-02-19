#pragma once

namespace rczEngine
{
	class RZ_EXP ScenesWindow : public BaseWindow
	{
	public:

	protected:
		virtual void Render() override;

		void RenderChildActors(StrGameObjectPtr obj);

		StrGameObjectPtr m_ActiveGameObject;

	private:
		Vector<String> m_SceneGameObjects;
		String m_SceneFileName = "Default Scene";

	};
}