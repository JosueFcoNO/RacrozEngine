#pragma once

namespace rczEngine
{
	class TextTool
	{
	public:

		////CHAR////

		static String UniToAnsi(const UNICHAR* text)
		{
			const StringW str(text);

			return String(str.begin(), str.end());
		};

		static String UniToAnsi(const StringW& text)
		{
			return String(text.begin(), text.end());
		};

		////UNICHAR////
		
		static StringW AnsiToUni(const ANSICHAR* text)
		{
			const String str(text);

			return StringW(str.begin(), str.end());
		};

		static StringW AnsiToUni(const String& text)
		{
			return StringW(text.begin(), text.end());
		};
	};
};