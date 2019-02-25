#pragma once

namespace rczEngine
{
	class RZ_EXP Color
	{
	public:
		Color() noexcept : R(uint8{ 0 }), G(uint8{ 0 }), B(uint8{ 0 }), A(uint8{ 0 }) {};
		Color(uint8 r, uint8 g, uint8 b, uint8 a) : R(r), G(g), B(b), A(a) {}

		void Set(uint8 r, uint8 g, uint8 b, uint8 a) noexcept;
		void Add(Color c) noexcept;
		void Remove(Color c) noexcept;
		
	private:
		uint8 R, G, B, A;
	};
};
