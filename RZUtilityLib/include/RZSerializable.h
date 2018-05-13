#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT Serializable
	{
	public:
		virtual void Serialize() = 0;
		virtual void DeSerialize() = 0;

	private:

	};
}