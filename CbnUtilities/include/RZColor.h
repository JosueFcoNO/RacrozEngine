#pragma once

namespace rczEngine
{
	class RZ_EXP Color : public Vector4
	{
	public:
		Color() noexcept : Vector4() {}
		Color(float x, float y, float z, float w = 1.0f) noexcept : Vector4(x, y, z, w) {}

		FORCEINLINE void SetColor(const Color& color) noexcept { *this = color; };

		FORCEINLINE void AddColor(const Color& c) noexcept;
		FORCEINLINE void RemoveColor(const Color& c) noexcept;

		FORCEINLINE Color GetLineal() const noexcept
		{
			Color newColor;

#pragma omp for
			for (int i = 0; i < 4; ++i)
			{
				newColor.m_elements[i] = Math::Pow(newColor.m_elements[i], 2.2f);
			};

			return newColor;
		};
		FORCEINLINE Color GetSRGB() const noexcept
		{
			Color newColor;

#pragma omp for
			for (int i = 0; i < 4; ++i)
			{
				newColor.m_elements[i] = Math::Pow(m_elements[i], 1.0f / 2.2f);
			};

			return newColor;
		};
	};
};
