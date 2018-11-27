#pragma once
#include <RZVulkanPCH.h>

namespace rczEngine
{
	class RZ_EXP FontRender
	{
	public:
		~FontRender()
		{
			DestroyFont();
		}

		void InitFont(UNICHAR* fontName, GfxCore* gfx);
		void DestroyFont();
		void DrawString(GfxCore* gfx, UNICHAR* String, int textSize, Vector2 pos);


	private:
		//TODO:
		///Font wrapper.
		GfxCore* m_gfx;
	};
};