#pragma once

namespace rczEngine
{
	class RZ_EXP BaseWindow
	{
	public:
		virtual void InitWindow(const String && name);
		virtual void UpdateWindow(float deltaTime);
		void RenderWindow();

	protected:
		virtual void ConfigRender();
		void BeginRender();
		virtual void Render() = 0;
		void EndRender();

		String m_Name;

		int m_Width;
		int m_Height;
	};
};