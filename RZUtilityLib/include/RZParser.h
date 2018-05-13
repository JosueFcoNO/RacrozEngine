#pragma once

namespace rczEngine
{
	///A String Parser. Can use UNICHAR and ANSICHAR
	class RZ_UTILITY_EXPORT Parser
	{
	public:	
		template <typename Char>
		///Parses the stringToParse to a vector of strings that it 'cuts' from the original string.
		static Vector<BasicString<Char>> ParseToStrings(BasicString<Char> stringToParse, BasicString<Char> Separator, uint32 stringPos)
		{
			///Create the Vector to hold the strings
			Vector<BasicString<Char>> TempVectorString;
			///Create Current Index and Last Index Variables
			BasicString<Char>::size_type TempStringIndex = stringPos, LastIndex = stringPos;

			///While it keeps finding a separator
			while (1)
			{
				///Set the LastIndex to the new Index
				LastIndex = TempStringIndex;
				///Find on the string to Parse the next Separator
				TempStringIndex = stringToParse.find(Separator, TempStringIndex);
					
				///If there is one, cut from the last index to the new index.
					TempVectorString.push_back(stringToParse.substr(LastIndex, stringToParse.find(Separator, LastIndex)-LastIndex));
					BasicString<Char> T = TempVectorString[TempVectorString.size()-1];

					///While Repeat
					if (TempStringIndex == String::npos)
					{
						break;
					}

					TempStringIndex += Separator.size();
					/*TempStringIndex += sizeof(T.c_str());*/
				

			}

			return TempVectorString;
		}

		template <typename Char>
		///Parses the stringToParse from strinPos to the first Separator it finds and it returns the first separator's position. 
		FORCEINLINE static int ParseToStringPosition(BasicString<Char> stringToParse, BasicString<Char> Separator, uint32 stringPos)
		{
			///Return stringToParse.find(Separator, TempStringIndex)
			return stringToParse.find(Separator, TempStringIndex);
		}
	};
	
}