#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT Color
	{
	public:
		Color(uint8 r, uint8 g, uint8 b, uint8 a);
		Color();

		void Set(uint8 r, uint8 g, uint8 b, uint8 a);
		void Add(Color c);
		void Remove(Color c);

	private:
		uint8 R, G, B, A;
	};
};
