#include <cmath>
#include <cassert>
#include "../RZCore/include/RZCorePCH.h"
#include <Windows.h>

///Unit Test - Math Library
void ParserUnitTest()
{
	using namespace rczEngine;

	rczEngine::Parser Parse;
	BasicString<UNICHAR> CadenaOriginal;
	CadenaOriginal.append(L"Hola,Como,,Estas,Yo,Bien,añeñe,ya,tengo,,el,parser");
	Vector<BasicString<UNICHAR>> P = Parse.ParseToStrings<UNICHAR>(CadenaOriginal, BasicString<UNICHAR>(L",,"), 0);

	for (int i = 0; i < P.size(); i++)
	{
		std::wcout << P[i] << std::endl;
	}
}

void ModuleUnitTest()
{
	using namespace rczEngine;

}

int main(void)
{
	ParserUnitTest();
	ModuleUnitTest();

	std::cin.get();
}