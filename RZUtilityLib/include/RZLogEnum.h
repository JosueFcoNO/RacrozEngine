#pragma once

namespace rczEngine
{
	enum class eLogMsgType
	{
		Message,
		Warning,
		Error,
		CriticalError
	};

	enum class eLogType
	{
		Common,
		GFX,
		Resource,
		Engine,
		Audio,
		Input,
		Compute,
		Memory
	};
};