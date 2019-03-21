#pragma once

namespace rczEngine
{
	class TextTool
	{
	public:

		////CHAR////

		static String UniToAnsi(const UNICHAR* text)
		{
			const size_t origsize = wcslen(text) + 1;
			const size_t newsize = origsize * 2;
			size_t convertedChars = 0;

			auto nstring = Vector<char>(newsize);

			wcstombs_s(&convertedChars, nstring.data(), newsize, text, _TRUNCATE);

			return String(&nstring[0]);
		};

		////UNICHAR////
		
		static StringW AnsiToUni(const ANSICHAR* text)
		{
			const size_t newsize = strlen(text) + 1;
			size_t convertedChars = 0;

			auto wcstring = Vector<wchar_t>(newsize);

			mbstowcs_s(&convertedChars, wcstring.data(), newsize, text, _TRUNCATE);

			return StringW(wcstring.data());
		};
	};
};