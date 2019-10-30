#pragma once

namespace rczEngine
{
	class RZ_EXP Color : public Vector4
	{
	public:
		Color() noexcept : Vector4() {}
		Color(float x, float y, float z, float w = 1.0f) noexcept : Vector4(x, y, z, w) {}

		void SetColor(float R, float G, float B, float a = 1.0f) noexcept;
		FORCEINLINE void SetColor(const Color& color) noexcept { *this = color; };

		FORCEINLINE void AddColor(const Color& c) noexcept;
		FORCEINLINE void RemoveColor(const Color& c) noexcept;

		///Copies the color and converts into Lineal.
		FORCEINLINE Color GetLineal() const noexcept;

		///Copies and converts into sRGB.
		FORCEINLINE Color GetSRGB() const noexcept;
	};
};
