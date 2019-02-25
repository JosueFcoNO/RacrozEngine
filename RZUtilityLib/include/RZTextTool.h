#pragma once

namespace rczEngine
{
	class TextTool
	{
	public:

		static String UniToAnsi(const UNICHAR* text)
		{
			const size_t origsize = wcslen(text) + 1;
			size_t convertedChars = 0;

			const size_t newsize = origsize * 2;

			char *nstring = new char[newsize];

			wcstombs_s(&convertedChars, nstring, newsize, text, _TRUNCATE);

			const String strReturn = nstring;
			delete[] nstring;

			return strReturn;
		};

		////UNICHAR////
		
		static StringW AnsiToUni(const ANSICHAR* text)
		{
			const size_t newsize = strlen(text) + 1;

			wchar_t * wcstring = new wchar_t[newsize];

			size_t convertedChars = 0;
			mbstowcs_s(&convertedChars, wcstring, newsize, text, _TRUNCATE);

			const StringW strReturn = wcstring;
			delete[] wcstring;

			return strReturn;
		};
	};
};