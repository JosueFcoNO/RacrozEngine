#pragma once

namespace rczEngine
{
	class RZ_EXP Color : public Vector4
	{
	public:
		Color() noexcept : Vector4() {}
		Color(float x, float y, float z, float w = 1.0f) noexcept : Vector4(x, y, z, w) {}

		void Set(float R, float G, float B, float a = 1.0f) noexcept;
		FORCEINLINE void Set(const Color& color) noexcept { *this = color; };
		FORCEINLINE void Set(const Vector3& color) noexcept { *this = Vector4(color.m_x, color.m_y, color.m_z, 1.0f); };
		FORCEINLINE void Set(const Vector4& color) noexcept { *this = color; };

		FORCEINLINE const Color& operator=(const Vector4& rv) noexcept { m_x = rv.m_x;  m_y = rv.m_y;  m_z = rv.m_z;  m_w = rv.m_w; return *this; };
		FORCEINLINE const Color& operator=(const Vector3& rv) noexcept { m_x = rv.m_x;  m_y = rv.m_y;  m_z = rv.m_z;  m_w = 1.0f; return *this; };

		void AddColor(const Color& c) noexcept;
		void RemoveColor(const Color& c) noexcept;

		FORCEINLINE Color GetLineal()
		{
			Color newColor;

#pragma omp for
			for (int i = 0; i < 4; ++i)
			{
				m_elements[i] = Math::Pow(m_elements[i], 2.2f);
			};

			return newColor;
		};
		FORCEINLINE Color GetSRGB() 
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
