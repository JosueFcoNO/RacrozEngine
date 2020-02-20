#pragma once

namespace rczEngine
{
	class RZ_EXP BaseWindow
	{
	public:
		virtual void InitWindow(const String && name, int width = 0, int height = 0);
		virtual void UpdateWindow(float deltaTime);
		void RenderWindow();

		bool IsOpen() {
			return m_Open;
		};

	protected:
		virtual void ConfigRender();
		void BeginRender();
		virtual void Render() = 0;
		void EndRender();

		String m_Name;

		bool m_Open = true;
		bool m_FirstOpen = true;

		int m_Width;
		int m_Height;
	};
};