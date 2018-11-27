#pragma once

namespace rczEngine
{
	class TextTool
	{
	public:
		static String FloatTo(float var)
		{
			ANSICHAR text[30];

			sprintf_s(text, sizeof(ANSICHAR)*30, "%f\n", var);
			return text;
		};

		static String IntTo(int32 var)
		{
			ANSICHAR text[30];
			sprintf_s(text, sizeof(ANSICHAR)*30, "%d\n", var);
			return text;
		};

		static String TextFloatTo(char* text, float var)
		{
			ANSICHAR textf[30];
			sprintf_s(textf, sizeof(ANSICHAR)*30, "%s%f\n",text, var);
			return textf;
		};

		static String TextIntTo(char* text, int32 var)
		{
			ANSICHAR textf[30];
			sprintf_s(textf, sizeof(ANSICHAR)*30, "%s%d\n",text, var);
			return textf;
		};

		static String UniToAnsi(UNICHAR* text)
		{
			size_t origsize = wcslen(text) + 1;
			size_t convertedChars = 0;

			const size_t newsize = origsize * 2;
		
			char *nstring = new char[newsize];

			wcstombs_s(&convertedChars, nstring, newsize, text, _TRUNCATE);

			return nstring;
		};

		static String UniToAnsi(const UNICHAR* text)
		{
			size_t origsize = wcslen(text) + 1;
			size_t convertedChars = 0;

			const size_t newsize = origsize * 2;

			char *nstring = new char[newsize];

			wcstombs_s(&convertedChars, nstring, newsize, text, _TRUNCATE);

			return nstring;
		};

		////UNICHAR////

		static StringW WFloatTo(float var)
		{
			UNICHAR text[70];
			swprintf_s(text, (30 * sizeof(UNICHAR)), L"%f\n", var);
			return text;
		};

		static StringW WIntTo(int32 var)
		{
			UNICHAR text[70];
			swprintf_s(text, (30 * sizeof(UNICHAR)), L"%d\n", var);
			return text;
		};

		static StringW WTextFloatTo(UNICHAR* text, float var)
		{
			UNICHAR textf[70];
			swprintf_s(textf, (30 * sizeof(UNICHAR)), L"%s%f\n", text, var);
			return textf;
		};

		static StringW WTextIntTo(UNICHAR* text, int32 var)
		{
			UNICHAR textf[70];
			swprintf_s(textf, (30 * sizeof(UNICHAR)), L"%s%d\n", text, var);
			return textf;
		};

		static StringW AnsiToUni(ANSICHAR* text)
		{
			size_t newsize = strlen(text) + 1;

			wchar_t * wcstring = new wchar_t[newsize];

			size_t convertedChars = 0;
			mbstowcs_s(&convertedChars, wcstring, newsize, text, _TRUNCATE);
			
			return wcstring;
		};

		static StringW AnsiToUni(const ANSICHAR* text)
		{
			size_t newsize = strlen(text) + 1;

			wchar_t * wcstring = new wchar_t[newsize];

			size_t convertedChars = 0;
			mbstowcs_s(&convertedChars, wcstring, newsize, text, _TRUNCATE);

			return wcstring;
		};
	};
};