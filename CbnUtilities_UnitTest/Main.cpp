#include <cmath>
#include <cassert>
#include <iostream>
#include <memory>

#include <CbnUtilitiesPCH.h>


bool Equals(float V1, float V2)
{
	return V1 >= V2 - 0.01 && V1 <= V2 + 0.01;
}

enum Colors
{
	White = 15,
	Red = 12,
	Green = 10,
	Cyan = 11
};

void PrintMessageColor(const std::string& str, Colors color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, color);
	std::cout << str;

	SetConsoleTextAttribute(hConsole, Colors::White);
};

void CarbonAssert(bool CarbonAssert, const std::string& strMessage)
{
	static constexpr bool BreakOnFail = true;

	if (CarbonAssert)
	{
		PrintMessageColor("Pass |", Colors::Green);
	}
	else
	{
		PrintMessageColor("Fail |", Colors::Red);

		if (BreakOnFail) __debugbreak();
	}

	PrintMessageColor(strMessage, Colors::White);

	std::cout << std::endl;

};

///Unit Test - Math Library
void MathUnitTest()
{
	using namespace rczEngine;

	PrintMessageColor("=== Math Unit Test ===", Colors::Cyan);
	std::cout << std::endl;

	///Factorial Assert
	CarbonAssert(Math::Factorial(5) == 120, "Factorial");

	///Truncate Assert
	CarbonAssert(Math::Truncate(5.6f) == 5, "Truncate");

	///TruncateF Assert
	CarbonAssert(Math::TruncateF(5.6f) == 5, "TruncateF");

	///Ceil Assert
	CarbonAssert(Math::Ceil(6.8f) == 7, "Ceil");

	///Round Assert
	CarbonAssert(Math::Round(5.8f) == 6, "Round");

	///MultX2 Assert
	CarbonAssert(Math::MulX2(4) == 8, "MulX2");

	///Clamp Assert
	CarbonAssert(Math::Clamp(17, 10, 15) == 15, "Clamp");

	///Clamp01 Assert
	CarbonAssert(Math::Clamp01(1.7f) == 1, "Clamp 0-1");

	///DivX2 Asserts
	CarbonAssert(Math::DivX2(8) == 4, "DivX2");

	///Floor Assert
	CarbonAssert(Math::Floor(9.9f) == 9, "Floor");

	///Max Assert
	CarbonAssert(Math::Max(5, 10) == 10, "Max");

	///Max3 Assert
	CarbonAssert(Math::Max3(5, 10, 19) == 19, "Max3");

	///Min Assert
	CarbonAssert(Math::Min(5, 10) == 5, "Min");

	///Min3 Assert
	CarbonAssert(Math::Min3(3, 5, 10) == 3, "Min3");

	///Square Assert
	CarbonAssert(Math::Square(5) == 25, "Square");

	///Sqrt Assert
	CarbonAssert(Math::Sqrt(25) == 5, "Squared Root");

	///Pow Assert
	CarbonAssert(Math::Pow(5, 3) == 125, "Pow");

	///RadiansToDegrees Assert
	CarbonAssert(Math::RadiansToDegrees(PI) == 180.0f, "Radians to Degrees");

	///DegreesToRad Assert
	CarbonAssert(Math::DegreesToRad(180.0f) == PI, "Degrees to Radian");

	///UnwindDegree Assert
	CarbonAssert(Math::UnwindDegree(400.0f) == 40.0f, "Unwind Degree");

	///UnwindRadian Assert
	CarbonAssert(Math::UnwindRadian(PI * 4) == 0.0f, "Unwind Radian");

	///Exp Assert
	CarbonAssert(Math::Exp(5.0f), "Exp");

	///Sin
	CarbonAssert(Math::Sin(PI) == sinf(PI), "Sin");


	///Fast Sin0
	CarbonAssert(TrigFast::FastSin0(PI) - .5 <= sinf(PI) && TrigFast::FastSin0(PI) + .5 >= sinf(PI), "Fast Sin 0");

	///Fast Sin1
	CarbonAssert(TrigFast::FastSin1(PI) - .5 <= sinf(PI) && TrigFast::FastSin1(PI) + .5 >= sinf(PI), "Fast Sin 1");

	///Cos
	CarbonAssert(Math::Cos(PI) == cosf(PI), "Cos");

	///Fast Cos0
	CarbonAssert(TrigFast::FastCos0(PI) - .5 <= cosf(PI) && TrigFast::FastCos0(PI) + .5 >= cosf(PI), "Fast Cos 0");

	///Fast Cos1
	CarbonAssert(TrigFast::FastCos1(PI) - .5 <= cosf(PI) && TrigFast::FastCos1(PI) + .5 >= cosf(PI), "Fast Cos 1");

	///Tan
	CarbonAssert(Math::Tan(PI) == tanf(PI), "Tan");

	///Fast Tan0
	CarbonAssert(TrigFast::FastTan0(PI) - .5 <= tanf(PI) && TrigFast::FastTan0(PI) + .5 >= tanf(PI), "Fast Tan 0");

	///Fast Tan1
	CarbonAssert(TrigFast::FastTan1(PI) - .5 <= tanf(PI) && TrigFast::FastTan1(PI) + .5 >= tanf(PI), "Fast Tan 1");

	///aSin
	CarbonAssert(Math::aSin(1) == asinf(1), "aSin");

	///Fast aSin0
	CarbonAssert(TrigFast::FastASin0(1) - .5 <= asinf(1) && TrigFast::FastASin0(1) + .5 >= asinf(1), "Fast aSin 0");

	///Fast aSin1
	CarbonAssert(TrigFast::FastASin1(1) - .5 <= asinf(1) && TrigFast::FastASin1(1) + .5 >= asinf(1), "Fast aSin 1");

	///aCos
	CarbonAssert(Math::aCos(1) == acosf(1), "aCos");

	///Fast aCos
	CarbonAssert((TrigFast::FastACos0(1) - .5 <= acosf(1) && TrigFast::FastACos0(1) + .5 >= acosf(1)), "Fast aCos 0");

	///aTan
	CarbonAssert(Math::aTan(1) == atanf(1), "aTan");

	///Fast aTan0
	CarbonAssert(TrigFast::FastATan0(1) - .5 <= atanf(1) && TrigFast::FastATan0(1) + .5 >= atanf(1), "Fast ATan 0");

	///Fast aTan1
	CarbonAssert(TrigFast::FastATan1(1) - .5 <= atanf(1) && TrigFast::FastATan1(1) + .5 >= atanf(1), "Fast ATan 1");

	///aTan2
	CarbonAssert(Math::aTan2(1, .5) == atan2f(1, .5), "aTan2");
}

void VectorUnitTest()
{
	using namespace rczEngine;

	PrintMessageColor("=== Vector Unit Test ===", Colors::Cyan);
	std::cout << std::endl;

	///Vector2 Assert
	Vector2 v(5, 5);

	///Assert Scale
	v.Scale(2);
	CarbonAssert(v.m_x == 10 && v.m_y == 10, "Vector2 Scale");

	///Assert Set
	v.Set(2, 2);
	CarbonAssert(v.m_x == 2 && v.m_y == 2, "Vector2 Set");

	///Assert Magnitude
	CarbonAssert(Equals(v.Magnitude(), 2.82f), "Vector2 Magnitude");

	///Assert Normalize
	v.Normalize();
	CarbonAssert(v.Magnitude() - .1f <= 1 && v.Magnitude() + .1f >= 1, "Vector2 Normalize");

	///////////////////////////////////////////////////////
	///Vector 3 Asserts
	//////////////////////////////////////////////////////

	Vector3 Vector3Alpha(1, 0, 0);
	Vector3 Vector3Beta(5, 0, 0);

	///Assert Magnitude
	CarbonAssert(Vector3Alpha.Magnitude() == 1, "Vector3 Magnitude");

	///Assert Distance
	CarbonAssert(Vector3Alpha.Distance(Vector3Beta, Vector3Alpha) == 4, "Vector3 Distance");

	///Assert Normalize
	Vector3 temp = Vector3Beta.GetNormalized();
	CarbonAssert(temp.m_x == 1, "Vector3 Normalize");

	///Assert Dot
	CarbonAssert((temp | Vector3Alpha) == 1, "Vector3 Dot");

	///Assert Scale
	Vector3Alpha.Scale(5);
	CarbonAssert(Vector3Alpha.m_x == 5, "Vector3 Scale");


	///Assert multiply
	Vector3Alpha *= Vector3Alpha;
	CarbonAssert(Vector3Alpha.m_x == 25, "Vector3 by Vector3 *=");

	///Assert Division
	Vector3Alpha /= 5;
	CarbonAssert(Vector3Alpha.m_x == 5, "Vector3 divided Vector3 /=");

	///Assert Add
	Vector3Alpha += Vector3Beta;
	CarbonAssert(Vector3Alpha.m_x == 10, "Vector3 Add Vector3 +=");

	///Assert Substract
	Vector3Alpha -= Vector3Beta;
	CarbonAssert(Vector3Alpha.m_x == 5, "Vector3 Substract Vector3 -=");

	///Vector 3 Hashing

	Vector3 Vector3Omega;

	Vector3Omega.Set(0, 0, 0);
	auto hash1 = Vector3::Hash(Vector3Omega);

	Vector3Omega.Set(1.0f, 1.0f, 1.0f);
	auto hash2 = Vector3::Hash(Vector3Omega);

	Vector3Omega.Set(-1.0f, -1.0f, -1.0f);
	auto hash3 = Vector3::Hash(Vector3Omega);

	Vector3Omega.Set(1.0f, 1.0f, 1.0005f);
	auto hash4 = Vector3::Hash(Vector3Omega);

	CarbonAssert((hash1 != hash2) && (hash1 != hash3) && (hash1 != hash4) && (hash2 != hash3) && (hash2 != hash4) && (hash3 != hash4), "Vector3 Hash");

};

void MatrixUnitText()
{
	PrintMessageColor("=== Matrix Unit Test ===", Colors::Cyan);
	std::cout << std::endl;

	/////////////////////////////////////////////////
	//////////////////Matrix 3D
	////////////////////////////////////////////////
	using namespace rczEngine;

	auto test = rczEngine::Matrix4::LookAtMatrix(
		Vector3(230, 175, -40),
		Vector3(0, 1, 0),
		Vector3(229, 175, -40)
		);

	rczEngine::Matrix3 Matrix3Alpha(rczEngine::Vector3(1, 2, 3), rczEngine::Vector3(0, 4, 5), rczEngine::Vector3(1, 0, 6));
	rczEngine::Matrix3 Matrix3Beta = Matrix3Alpha;
	rczEngine::Matrix3 Matrix3Gamma = Matrix3Alpha;

	CarbonAssert(Matrix3Alpha.Determinant() == 22, "Matrix3 Determinant");

	///Returns the cofactor of the nRow and nCol from this Matrix
	CarbonAssert(Matrix3Alpha.CoFactor2x2(0, 0) == 24, "Matrix3 CoFactor 2x2");

	///Transposes this Matrix
	Matrix3Alpha.Transpose();
	CarbonAssert(Matrix3Alpha.m_matrix[0][0] == 1 ||
	Matrix3Alpha.m_matrix[0][1] == 0 ||
	Matrix3Alpha.m_matrix[0][2] == 1 ||
	Matrix3Alpha.m_matrix[1][0] == 2 ||
	Matrix3Alpha.m_matrix[1][1] == 4 ||
	Matrix3Alpha.m_matrix[1][2] == 0 ||
	Matrix3Alpha.m_matrix[2][0] == 3 ||
	Matrix3Alpha.m_matrix[2][1] == 5 ||
	Matrix3Alpha.m_matrix[2][2] == 6, "Matrix3 Transpose");

	///Returns the Adjoint of this Matrix
	Matrix3Beta = Matrix3Beta.GetAdjoint();
	CarbonAssert(Matrix3Beta.m_matrix[0][0] == 24 ||
	Matrix3Beta.m_matrix[0][1] == -12 ||
	Matrix3Beta.m_matrix[0][2] == -2 ||
	Matrix3Beta.m_matrix[1][0] == 5 ||
	Matrix3Beta.m_matrix[1][1] == 3 ||
	Matrix3Beta.m_matrix[1][2] == -5 ||
	Matrix3Beta.m_matrix[2][0] == -4 ||
	Matrix3Beta.m_matrix[2][1] == 2 ||
	Matrix3Beta.m_matrix[2][2] == 4, "Matrix3 Adjoint");

	///Returns the inverse of this Matrix
	Matrix3Gamma = Matrix3Gamma.GetInverse();
	CarbonAssert(Equals(Matrix3Gamma.m_matrix[0][0], (float)12 / 11) ||
	Equals(Matrix3Gamma.m_matrix[0][1], (float)-6 / 11) ||
	Equals(Matrix3Gamma.m_matrix[0][2], (float)-1 / 11) ||
	Equals(Matrix3Gamma.m_matrix[1][0], (float)5 / 22) ||
	Equals(Matrix3Gamma.m_matrix[1][1], (float)3 / 22) ||
	Equals(Matrix3Gamma.m_matrix[1][2], (float)-5 / 22) ||
	Equals(Matrix3Gamma.m_matrix[2][0], (float)-2 / 11) ||
	Equals(Matrix3Gamma.m_matrix[2][1], (float)1 / 11) ||
	Equals(Matrix3Gamma.m_matrix[2][2], (float)2 / 11), "Matrix3 Inverse");

	///Makes it an Identity Matrix
	Matrix3Alpha.Identity();
	CarbonAssert(Matrix3Alpha.m_matrix[0][0] == 1 ||
	Matrix3Alpha.m_matrix[0][1] == 0 ||
	Matrix3Alpha.m_matrix[0][2] == 0 ||
	Matrix3Alpha.m_matrix[1][0] == 0 ||
	Matrix3Alpha.m_matrix[1][1] == 1 ||
	Matrix3Alpha.m_matrix[1][2] == 0 ||
	Matrix3Alpha.m_matrix[2][0] == 0 ||
	Matrix3Alpha.m_matrix[2][1] == 0 ||
	Matrix3Alpha.m_matrix[2][2] == 1, "Matrix3 Identity");

	///Multiplies the matrix and scalar. Constant.
	Matrix3Gamma = Matrix3Alpha * 5;
	CarbonAssert(Matrix3Gamma.m_matrix[0][0] == 5 ||
	Matrix3Gamma.m_matrix[0][1] == 0 ||
	Matrix3Gamma.m_matrix[0][2] == 0 ||
	Matrix3Gamma.m_matrix[1][0] == 0 ||
	Matrix3Gamma.m_matrix[1][1] == 5 ||
	Matrix3Gamma.m_matrix[1][2] == 0 ||
	Matrix3Gamma.m_matrix[2][0] == 0 ||
	Matrix3Gamma.m_matrix[2][1] == 0 ||
	Matrix3Gamma.m_matrix[2][2] == 5, "Matrix3 by Scalar *");

	///Multiplies the matrix and another 3x3 Matrix. Constant.
	Matrix3Beta = Matrix3Gamma * Matrix3Beta;
	CarbonAssert(Matrix3Beta.m_matrix[0][0] == 24 * 5 ||
	Matrix3Beta.m_matrix[0][1] == -12 * 5 ||
	Matrix3Beta.m_matrix[0][2] == -2 * 5 ||
	Matrix3Beta.m_matrix[1][0] == 5 * 5 ||
	Matrix3Beta.m_matrix[1][1] == 3 * 5 ||
	Matrix3Beta.m_matrix[1][2] == -5 * 5 ||
	Matrix3Beta.m_matrix[2][0] == -4 * 5 ||
	Matrix3Beta.m_matrix[2][1] == 2 * 5 ||
	Matrix3Beta.m_matrix[2][2] == 4 * 5, "Matrix3 by Matrix3 *");

	rczEngine::Vector3 TempVector(1, 1, 1);
	///Multiplies the matrix and a Vector3: Vector * Matrix so it returns a Vector3 . Constant.
	TempVector = Matrix3Gamma * TempVector;
	CarbonAssert(TempVector.m_x == 5 ||
	TempVector.m_y == 5 ||
	TempVector.m_z == 5, "Matrix3 by Vector3");

	float f = float(1) / float(5);
	///Multiplies the Matrix and a Scalar.
	Matrix3Gamma *= f;
	CarbonAssert(Matrix3Gamma.m_matrix[0][0] == 1 ||
	Matrix3Gamma.m_matrix[0][1] == 0 ||
	Matrix3Gamma.m_matrix[0][2] == 0 ||
	Matrix3Gamma.m_matrix[1][0] == 0 ||
	Matrix3Gamma.m_matrix[1][1] == 1 ||
	Matrix3Gamma.m_matrix[1][2] == 0 ||
	Matrix3Gamma.m_matrix[2][0] == 0 ||
	Matrix3Gamma.m_matrix[2][1] == 0 ||
	Matrix3Gamma.m_matrix[2][2] == 1, "Matrix3 by Scalar *=");

	///Multiplies the Matrix and another Matrix.
	Matrix3Gamma *= Matrix3Beta;
	CarbonAssert(Matrix3Beta.m_matrix[0][0] == 24 * 5 ||
	Matrix3Beta.m_matrix[0][1] == -12 * 5 ||
	Matrix3Beta.m_matrix[0][2] == -2 * 5 ||
	Matrix3Beta.m_matrix[1][0] == 5 * 5 ||
	Matrix3Beta.m_matrix[1][1] == 3 * 5 ||
	Matrix3Beta.m_matrix[1][2] == -5 * 5 ||
	Matrix3Beta.m_matrix[2][0] == -4 * 5 ||
	Matrix3Beta.m_matrix[2][1] == 2 * 5 ||
	Matrix3Beta.m_matrix[2][2] == 4 * 5, "Matrix3 by Matrix3 *=");

	/////////////////////////////////////////////////
	//////////////////Matrix 4D
	////////////////////////////////////////////////

	rczEngine::Matrix4 Matrix4Alpha(rczEngine::Vector4(4, 5, 3, 2),
		rczEngine::Vector4(5, 8, 6, 4),
		rczEngine::Vector4(-4, 3, 6, 4),
		rczEngine::Vector4(3, 3, 3, 4));

	rczEngine::Matrix4 Matrix4Beta = Matrix4Alpha;
	rczEngine::Matrix4 Matrix4Gamma = Matrix4Alpha;
	rczEngine::Matrix4 Matrix4Adjoint = Matrix4Alpha;

	CarbonAssert(Matrix4Alpha.Determinant() == -18, "Matrix4 Determinant");

	///Transposes this Matrix
	Matrix4Beta.Transpose();
	CarbonAssert(Matrix4Beta.m_matrix[0][0] == 4 ||
		Matrix4Beta.m_matrix[0][1] == 5 ||
		Matrix4Beta.m_matrix[0][2] == -4 ||
		Matrix4Beta.m_matrix[0][3] == 3 ||
		Matrix4Beta.m_matrix[1][0] == 5 ||
		Matrix4Beta.m_matrix[1][1] == 8 ||
		Matrix4Beta.m_matrix[1][2] == 3 ||
		Matrix4Beta.m_matrix[1][3] == 3 ||
		Matrix4Beta.m_matrix[2][0] == 3 ||
		Matrix4Beta.m_matrix[2][1] == 6 ||
		Matrix4Beta.m_matrix[2][2] == 6 ||
		Matrix4Beta.m_matrix[2][3] == 3 ||
		Matrix4Beta.m_matrix[3][0] == 2 ||
		Matrix4Beta.m_matrix[3][1] == 4 ||
		Matrix4Beta.m_matrix[3][2] == 4 ||
		Matrix4Beta.m_matrix[3][3] == 4, "Matrix Transpose");

	///Returns the inverse of this Matrix
	Matrix4Gamma = Matrix4Gamma.GetInverse();
	CarbonAssert(Equals(Matrix4Gamma.m_matrix[0][0], float(-60) / float(18)) ||
	Equals(Matrix4Gamma.m_matrix[1][1], float(-72) / float(18)) ||
	Equals(Matrix4Gamma.m_matrix[2][2], float(-22) / float(18)) ||
	Equals(Matrix4Gamma.m_matrix[3][3], float(9) / float(18)), "Matrix4 Inverse");

	Matrix4Adjoint = Matrix4Adjoint.GetAdjoint();

	///Makes it an Identity Matrix
	Matrix4Alpha.Identity();
	CarbonAssert(Matrix4Alpha.m_matrix[0][0] == 1 ||
	Matrix4Alpha.m_matrix[0][1] == 0 ||
	Matrix4Alpha.m_matrix[0][2] == 0 ||
	Matrix4Alpha.m_matrix[1][0] == 0 ||
	Matrix4Alpha.m_matrix[1][1] == 1 ||
	Matrix4Alpha.m_matrix[1][2] == 0 ||
	Matrix4Alpha.m_matrix[2][0] == 0 ||
	Matrix4Alpha.m_matrix[2][1] == 0 ||
	Matrix4Alpha.m_matrix[2][2] == 1, "Matrix4 Identity");

	///Multiplies the matrix and scalar. Constant.
	Matrix4Gamma = Matrix4Alpha * 5;
	CarbonAssert(Matrix4Gamma.m_matrix[0][0] == 5 ||
	Matrix4Gamma.m_matrix[0][1] == 0 ||
	Matrix4Gamma.m_matrix[0][2] == 0 ||
	Matrix4Gamma.m_matrix[1][0] == 0 ||
	Matrix4Gamma.m_matrix[1][1] == 5 ||
	Matrix4Gamma.m_matrix[1][2] == 0 ||
	Matrix4Gamma.m_matrix[2][0] == 0 ||
	Matrix4Gamma.m_matrix[2][1] == 0 ||
	Matrix4Gamma.m_matrix[2][2] == 5, "Matrix4 By Scalar");

	///Multiplies the matrix and another 3x3 Matrix. Constant.
	Matrix4Gamma.Identity();
	Matrix4Beta.Transpose();
	Matrix4Beta = Matrix4Gamma * Matrix4Beta;
	CarbonAssert(Matrix4Beta.m_matrix[0][0] == 4 ||
	Matrix4Beta.m_matrix[1][1] == 8 ||
	Matrix4Beta.m_matrix[2][2] == 6 ||
	Matrix4Beta.m_matrix[3][3] == 4, "Matrix4 by Matrix3");


	rczEngine::Vector4 Temp4Vector(5, 5, 5, 1);
	///Multiplies the matrix and a Vector3: Vector * Matrix so it returns a Vector3 . Constant.
	Temp4Vector = rczEngine::Matrix4::Translate3D(20, 0, 20) *Temp4Vector;
	CarbonAssert(Temp4Vector.m_x == 25 ||
		Temp4Vector.m_y == 5 ||
		Temp4Vector.m_z == 25 ||
		Temp4Vector.m_w == 1, "Matrix4 by Vector4");

	rczEngine::Vector3 Temp3Vector(5, 5, 5);
	///Multiplies the matrix and a Vector3: Vector * Matrix so it returns a Vector3 . Constant.
	Temp3Vector = rczEngine::Matrix4::Translate3D(20, 0, 20) * Temp3Vector;
	CarbonAssert(Temp3Vector.m_x == 25 ||
	Temp3Vector.m_y == 5 ||
	Temp3Vector.m_z == 25, "Matrix4 by Vector3");


	f = float(1) / float(5);
	///Multiplies the Matrix and a Scalar.
	Matrix4Gamma *= 5;
	CarbonAssert(Matrix4Gamma.m_matrix[0][0] == 5 ||
	Matrix4Gamma.m_matrix[1][1] == 5 ||
	Matrix4Gamma.m_matrix[2][2] == 5 ||
	Matrix4Gamma.m_matrix[3][3] == 5, "Matrix4 By Scalar *=");


	///Multiplies the Matrix and another Matrix.
	Matrix4Beta.Identity();
	Matrix4Gamma *= Matrix4Beta;
	CarbonAssert(Matrix4Beta.m_matrix[0][0] == 1 ||
	Matrix4Beta.m_matrix[1][1] == 1 ||
	Matrix4Beta.m_matrix[2][2] == 1 ||
	Matrix4Beta.m_matrix[3][3] == 1, "Matrix4 by Matrix4");

}

void QuaternionUnitTest()
{
	using namespace rczEngine;

	PrintMessageColor("=== Quaternion Unit Test ===", Colors::Cyan);
	std::cout << std::endl;

	Quaternion QuaternionAlpha = Matrix3::Rotate3D(0, 0, -90).GetAsQuaternion();
	QuaternionAlpha.Normalize();

	Vector3 VectorAlpha = {0.0f, 1.0f, 0.0f};

	auto rotatedVector = QuaternionAlpha.RotateVector(VectorAlpha, 1);

	const Quaternion q(1, .6, 0.27, .25);
	const Quaternion p(.12, 1, .88, .76);
	auto quat = q * p;

	CarbonAssert(
		Math::Equals(quat.m_w, -0.7676, .001f) &&
		Math::Equals(quat.m_v.m_x, 1.048, .001f) &&
		Math::Equals(quat.m_v.m_y, -0.1416, .001f) &&
		Math::Equals(quat.m_v.m_z, 1.3532, .001f),
		"Quaternion Multiplication"
	);

}

int main(void)
{
	using namespace rczEngine;

	PrintMessageColor("========== ", Colors::Green);
	PrintMessageColor("Carbon Utilities Unit Test", Colors::White);
	PrintMessageColor(" ==========", Colors::Green);

	std::cout << std::endl;

	Profiler::Start();
	Logger::Start();

	Profiler::Pointer()->StartProfiler();
	Profiler::Pointer()->NewFrameStart();

	MathUnitTest();
	VectorUnitTest();
	MatrixUnitText();
	QuaternionUnitTest();

	rczEngine::FileManager file;

	file.OpenBinaryFile("Perro", FileStream::binary | FileStream::out);
	file << 245;
	file.CloseFile();

	int i245;

	file.OpenBinaryFile("Perro", FileStream::binary | FileStream::in);
	file >> i245;
	file.CloseFile();

	StringW perro = L"Nuñez";
	String perro2 = "Josue";

	StringW result = TextTool::AnsiToUni(perro2);
	String result2 = TextTool::UniToAnsi(perro);

	std::cin.get();

	Profiler::Pointer()->AddTime("Utilities");

	Profiler::ShutDown();
	Logger::ShutDown();
}