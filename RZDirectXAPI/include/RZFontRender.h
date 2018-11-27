#pragma once
#include <RZDirectXPCH.h>

namespace rczEngine
{
	namespace Gfx
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
			IFW1Factory *m_pFW1Factory;
			IFW1FontWrapper *m_pFontWrapper;
			GfxCore* m_gfx;
		};
	}
};