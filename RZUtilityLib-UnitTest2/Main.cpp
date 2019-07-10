#include <cmath>
#include <cassert>
#include <iostream>
#include <memory>
#include <RZUtilityPCH.h>


bool Equals(float V1, float V2) 
{
	return V1 >= V2 - 0.01 && V1 <= V2 + 0.01;
}

///Unit Test - Math Library
void MathUnitTest()
{
	using namespace rczEngine;

	///Factorial Assert
	assert(Math::Factorial(5) == 120);

	///Truncate Assert
	assert(Math::Truncate(5.6f) == 5);

	///TruncateF Assert
	assert(Math::TruncateF(5.6f) == 5);

	///Ceil Assert
	assert(Math::Ceil(6.8f) == 7);

	///Round Assert
	assert(Math::Round(5.8f) == 6);

	///MultX2 Assert
	assert(Math::MulX2(4) == 8);

	///Clamp Assert
	assert(Math::Clamp(17, 10, 15) == 15);

	///Clamp01 Assert
	assert(Math::Clamp01(1.7f) == 1);

	///DivX2 Asserts
	assert(Math::DivX2(8) == 4);

	///Floor Assert
	assert(Math::Floor(9.9f) == 9);

	///Max Assert
	assert(Math::Max(5, 10) == 10);

	///Max3 Assert
	assert(Math::Max3(5, 10, 19) == 19);

	///Min Assert
	assert(Math::Min(5, 10) == 5);

	///Min3 Assert
	assert(Math::Min3(3, 5, 10) == 3);

	///Square Assert
	assert(Math::Square(5) == 25);

	///Sqrt Assert
	assert(Math::Sqrt(25) == 5);

	///Pow Assert
	assert(Math::Pow(5, 3) == 125);

	///RadiansToDegrees Assert
	assert(Math::RadiansToDegrees(PI) == 180.0f);

	///DegreesToRad Assert
	assert(Math::DegreesToRad(180.0f) == PI);

	///UnwindDegree Assert
	assert(Math::UnwindDegree(400.0f) == 40.0f);

	///UnwindRadian Assert
	assert(Math::UnwindRadian(PI * 4) == 0.0f);

	///Exp Assert
	assert(Math::Exp(5.0f));

	///Sin
	assert(Math::Sin(PI) == sinf(PI));


	///Fast Sin0
	assert(Math::FastSin0(PI) - .5 <= sinf(PI) && Math::FastSin0(PI) + .5 >= sinf(PI));

	///Fast Sin1
	assert(Math::FastSin1(PI) - .5 <= sinf(PI) && Math::FastSin1(PI) + .5 >= sinf(PI));

	///Cos
	assert(Math::Cos(PI) == cosf(PI));

	///Fast Cos0
	assert(Math::FastCos0(PI) - .5 <= cosf(PI) && Math::FastCos0(PI) + .5 >= cosf(PI));

	///Fast Cos1
	assert(Math::FastCos1(PI) - .5 <= cosf(PI) && Math::FastCos1(PI) + .5 >= cosf(PI));

	///Tan
	assert(Math::Tan(PI) == tanf(PI));

	///Fast Tan0
	assert(Math::FastTan0(PI) - .5 <= tanf(PI) && Math::FastTan0(PI) + .5 >= tanf(PI));

	///Fast Tan1
	assert(Math::FastTan1(PI) - .5 <= tanf(PI) && Math::FastTan1(PI) + .5 >= tanf(PI));

	///aSin
	assert(Math::aSin(1) == asinf(1));

	///Fast aSin0
	assert(Math::FastASin0(1) - .5 <= asinf(1) && Math::FastASin0(1) + .5 >= asinf(1));

	///Fast aSin1
	assert(Math::FastASin1(1) - .5 <= asinf(1) && Math::FastASin1(1) + .5 >= asinf(1));

	///aCos
	assert(Math::aCos(1) == acosf(1));

	///Fast aCos
	assert((Math::FastACos0(1) - .5 <= acosf(1) && Math::FastACos0(1) + .5 >= acosf(1)));

	///aTan
	assert(Math::aTan(1) == atanf(1));

	///Fast aTan0
	assert(Math::FastATan0(1) - .5 <= atanf(1) && Math::FastATan0(1) + .5 >= atanf(1));

	///Fast aTan1
	assert(Math::FastATan1(1) - .5 <= atanf(1) && Math::FastATan1(1) + .5 >= atanf(1));

	///aTan2
	assert(Math::aTan2(1,.5) == atan2f(1,.5));
}

void VectorUnitTest()
{
	using namespace rczEngine;

	///Vector2 Assert
	Vector2 v(5,5);

	///Assert Scale
	v.Scale(2);
	assert(v.m_x == 10 && v.m_y==10);

	///Assert Set
	v.Set(2, 2);
	assert(v.m_x == 2 && v.m_y == 2);

	///Assert Magnitude
	assert(Math::Ceil(v.Magnitude()) == Math::Ceil(2.82f));

	///Assert Normalize
	v.Normalize();
	assert(v.Magnitude()-.1f <= 1 && v.Magnitude() + .1f >= 1);

	///////////////////////////////////////////////////////
	///Vector 3 Asserts
	//////////////////////////////////////////////////////

	Vector3 v3(1,0,0);
	Vector3 v4(5,0,0);

	///Assert Magnitude
	assert(v3.Magnitude() == 1);
	
	///Assert Distance
	assert(v3.Distance(v4,v3) == 4);

	///Assert Normalize
	Vector3 temp = v4.GetNormalized();
	assert(temp.m_x == 1);
	
	///Assert Dot
	assert((temp | v3) == 1);

	///Assert Scale
	v3.Scale(5);
	assert(v3.m_x == 5);


	///Assert multiply
	v3 *= v3;
	assert(v3.m_x == 25);

	///Assert Division
	v3 /= 5;
	assert(v3.m_x == 5);

	///Assert Add
	v3 += v4;
	assert(v3.m_x == 10);

	///Assert Substract
	v3 -= v4;
	assert(v3.m_x == 5);


};

void MatrixUnitText()
{
	/////////////////////////////////////////////////
	//////////////////Matrix 3D
	////////////////////////////////////////////////

	rczEngine::Matrix3 Temp(rczEngine::Vector3(1, 2, 3), rczEngine::Vector3(0, 4, 5), rczEngine::Vector3(1, 0, 6));
	rczEngine::Matrix3 Temp2 = Temp;
	rczEngine::Matrix3 Temp3 = Temp;

	assert(Temp.Determinant() == 22);

	///Returns the cofactor of the nRow and nCol from this Matrix
	assert(Temp.CoFactor2x2(0, 0) == 24);

	///Transposes this Matrix
	Temp.Transpose();
	assert(Temp.m_matrix[0][0] == 1);
	assert(Temp.m_matrix[0][1] == 0);
	assert(Temp.m_matrix[0][2] == 1);
	assert(Temp.m_matrix[1][0] == 2);
	assert(Temp.m_matrix[1][1] == 4);
	assert(Temp.m_matrix[1][2] == 0);
	assert(Temp.m_matrix[2][0] == 3);
	assert(Temp.m_matrix[2][1] == 5);
	assert(Temp.m_matrix[2][2] == 6);

	///Returns the Adjoint of this Matrix
	Temp2 = Temp2.GetAdjoint();
	assert(Temp2.m_matrix[0][0] == 24);
	assert(Temp2.m_matrix[0][1] == -12);
	assert(Temp2.m_matrix[0][2] == -2);
	assert(Temp2.m_matrix[1][0] == 5);
	assert(Temp2.m_matrix[1][1] == 3);
	assert(Temp2.m_matrix[1][2] == -5);
	assert(Temp2.m_matrix[2][0] == -4);
	assert(Temp2.m_matrix[2][1] == 2);
	assert(Temp2.m_matrix[2][2] == 4);



	///Returns the inverse of this Matrix
	Temp3 = Temp3.GetInverse();
	assert(Equals(Temp3.m_matrix[0][0], (float)12 / 11));
	assert(Equals(Temp3.m_matrix[0][1], (float)-6 / 11));
	assert(Equals(Temp3.m_matrix[0][2], (float)-1 / 11));
	assert(Equals(Temp3.m_matrix[1][0], (float)5 / 22));
	assert(Equals(Temp3.m_matrix[1][1], (float)3 / 22));
	assert(Equals(Temp3.m_matrix[1][2], (float)-5 / 22));
	assert(Equals(Temp3.m_matrix[2][0], (float)-2 / 11));
	assert(Equals(Temp3.m_matrix[2][1], (float)1 / 11));
	assert(Equals(Temp3.m_matrix[2][2], (float)2 / 11));

	///Makes it an Identity Matrix
	Temp.Identity();
	assert(Temp.m_matrix[0][0] == 1);
	assert(Temp.m_matrix[0][1] == 0);
	assert(Temp.m_matrix[0][2] == 0);
	assert(Temp.m_matrix[1][0] == 0);
	assert(Temp.m_matrix[1][1] == 1);
	assert(Temp.m_matrix[1][2] == 0);
	assert(Temp.m_matrix[2][0] == 0);
	assert(Temp.m_matrix[2][1] == 0);
	assert(Temp.m_matrix[2][2] == 1);

	///Multiplies the matrix and scalar. Constant.
	Temp3 = Temp * 5;
	assert(Temp3.m_matrix[0][0] == 5);
	assert(Temp3.m_matrix[0][1] == 0);
	assert(Temp3.m_matrix[0][2] == 0);
	assert(Temp3.m_matrix[1][0] == 0);
	assert(Temp3.m_matrix[1][1] == 5);
	assert(Temp3.m_matrix[1][2] == 0);
	assert(Temp3.m_matrix[2][0] == 0);
	assert(Temp3.m_matrix[2][1] == 0);
	assert(Temp3.m_matrix[2][2] == 5);

	///Multiplies the matrix and another 3x3 Matrix. Constant.
	Temp2 = Temp3*Temp2;
	assert(Temp2.m_matrix[0][0] == 24 * 5);
	assert(Temp2.m_matrix[0][1] == -12 * 5);
	assert(Temp2.m_matrix[0][2] == -2 * 5);
	assert(Temp2.m_matrix[1][0] == 5 * 5);
	assert(Temp2.m_matrix[1][1] == 3 * 5);
	assert(Temp2.m_matrix[1][2] == -5 * 5);
	assert(Temp2.m_matrix[2][0] == -4 * 5);
	assert(Temp2.m_matrix[2][1] == 2 * 5);
	assert(Temp2.m_matrix[2][2] == 4 * 5);

	rczEngine::Vector3 TempVector(1, 1, 1);
	///Multiplies the matrix and a Vector3: Vector * Matrix so it returns a Vector3 . Constant.
	TempVector = Temp3*TempVector;
	assert(TempVector.m_x == 5);
	assert(TempVector.m_y == 5);
	assert(TempVector.m_z == 5);

	float f = float(1) / float(5);
	///Multiplies the Matrix and a Scalar.
	Temp3 *= f;
	assert(Temp3.m_matrix[0][0] == 1);
	assert(Temp3.m_matrix[0][1] == 0);
	assert(Temp3.m_matrix[0][2] == 0);
	assert(Temp3.m_matrix[1][0] == 0);
	assert(Temp3.m_matrix[1][1] == 1);
	assert(Temp3.m_matrix[1][2] == 0);
	assert(Temp3.m_matrix[2][0] == 0);
	assert(Temp3.m_matrix[2][1] == 0);
	assert(Temp3.m_matrix[2][2] == 1);

	///Multiplies the Matrix and another Matrix.
	Temp3 *= Temp2;
	assert(Temp2.m_matrix[0][0] == 24 * 5);
	assert(Temp2.m_matrix[0][1] == -12 * 5);
	assert(Temp2.m_matrix[0][2] == -2 * 5);
	assert(Temp2.m_matrix[1][0] == 5 * 5);
	assert(Temp2.m_matrix[1][1] == 3 * 5);
	assert(Temp2.m_matrix[1][2] == -5 * 5);
	assert(Temp2.m_matrix[2][0] == -4 * 5);
	assert(Temp2.m_matrix[2][1] == 2 * 5);
	assert(Temp2.m_matrix[2][2] == 4 * 5);

	/////////////////////////////////////////////////
	//////////////////Matrix 4D
	////////////////////////////////////////////////

	rczEngine::Matrix4 Temp4(rczEngine::Vector4(4, 5, 3, 2), 
							 rczEngine::Vector4(5, 8, 6, 4), 
							 rczEngine::Vector4(-4, 3, 6, 4), 
							 rczEngine::Vector4(3, 3, 3, 4));

	rczEngine::Matrix4 Temp42 = Temp4;
	rczEngine::Matrix4 Temp43 = Temp4;
	rczEngine::Matrix4 Adj = Temp4;

	assert(Temp4.Determinant() == -18);

	///Transposes this Matrix
	Temp42.Transpose();
	assert(Temp42.m_matrix[0][0] == 4);
	assert(Temp42.m_matrix[0][1] == 5);
	assert(Temp42.m_matrix[0][2] == -4);
	assert(Temp42.m_matrix[0][3] == 3);
	assert(Temp42.m_matrix[1][0] == 5);
	assert(Temp42.m_matrix[1][1] == 8);
	assert(Temp42.m_matrix[1][2] == 3);
	assert(Temp42.m_matrix[1][3] == 3);
	assert(Temp42.m_matrix[2][0] == 3);
	assert(Temp42.m_matrix[2][1] == 6);
	assert(Temp42.m_matrix[2][2] == 6);
	assert(Temp42.m_matrix[2][3] == 3);
	assert(Temp42.m_matrix[3][0] == 2);
	assert(Temp42.m_matrix[3][1] == 4);
	assert(Temp42.m_matrix[3][2] == 4);
	assert(Temp42.m_matrix[3][3] == 4);

	///Returns the inverse of this Matrix
	Temp43 = Temp43.GetInverse();
	assert(Equals(Temp43.m_matrix[0][0], float(-60) / float(18)  ));
	assert(Equals(Temp43.m_matrix[1][1], float(-72) / float(18)  ));
	assert(Equals(Temp43.m_matrix[2][2], float(-22) / float(18)  ));
	assert(Equals(Temp43.m_matrix[3][3], float(9) / float(18)  ));

	Adj = Adj.GetAdjoint();

	///Makes it an Identity Matrix
	Temp4.Identity();
	assert(Temp4.m_matrix[0][0] == 1);
	assert(Temp4.m_matrix[0][1] == 0);
	assert(Temp4.m_matrix[0][2] == 0);
	assert(Temp4.m_matrix[1][0] == 0);
	assert(Temp4.m_matrix[1][1] == 1);
	assert(Temp4.m_matrix[1][2] == 0);
	assert(Temp4.m_matrix[2][0] == 0);
	assert(Temp4.m_matrix[2][1] == 0);
	assert(Temp4.m_matrix[2][2] == 1);

	///Multiplies the matrix and scalar. Constant.
	Temp43 = Temp4 * 5;
	assert(Temp43.m_matrix[0][0] == 5);
	assert(Temp43.m_matrix[0][1] == 0);
	assert(Temp43.m_matrix[0][2] == 0);
	assert(Temp43.m_matrix[1][0] == 0);
	assert(Temp43.m_matrix[1][1] == 5);
	assert(Temp43.m_matrix[1][2] == 0);
	assert(Temp43.m_matrix[2][0] == 0);
	assert(Temp43.m_matrix[2][1] == 0);
	assert(Temp43.m_matrix[2][2] == 5);

	///Multiplies the matrix and another 3x3 Matrix. Constant.
	Temp43.Identity();
	Temp42.Transpose();
	Temp42 = Temp43*Temp42;
	assert(Temp42.m_matrix[0][0] == 4);
	assert(Temp42.m_matrix[1][1] == 8);
	assert(Temp42.m_matrix[2][2] == 6);
	assert(Temp42.m_matrix[3][3] == 4);


	rczEngine::Vector4 Temp4Vector(5, 5, 5, 1);
	///Multiplies the matrix and a Vector3: Vector * Matrix so it returns a Vector3 . Constant.
	Temp4Vector = rczEngine::Matrix4::Translate3D(20, 0, 20) *Temp4Vector;
	assert(Temp4Vector.m_x == 25);
	assert(Temp4Vector.m_y == 5);
	assert(Temp4Vector.m_z == 25);
	assert(Temp4Vector.m_w == 1);

	rczEngine::Vector3 Temp3Vector(5, 5, 5);
	///Multiplies the matrix and a Vector3: Vector * Matrix so it returns a Vector3 . Constant.
	Temp3Vector = rczEngine::Matrix4::Translate3D(20, 0, 20) *Temp3Vector;
	assert(Temp3Vector.m_x == 25);
	assert(Temp3Vector.m_y == 5);
	assert(Temp3Vector.m_z == 25);


	f = float(1) / float(5);
	///Multiplies the Matrix and a Scalar.
	Temp43 *= 5;
	assert(Temp43.m_matrix[0][0] == 5);
	assert(Temp43.m_matrix[1][1] == 5);
	assert(Temp43.m_matrix[2][2] == 5);
	assert(Temp43.m_matrix[3][3] == 5);


	///Multiplies the Matrix and another Matrix.
	Temp42.Identity();
	Temp43 *= Temp42;
	assert(Temp42.m_matrix[0][0] == 1);
	assert(Temp42.m_matrix[1][1] == 1);
	assert(Temp42.m_matrix[2][2] == 1);
	assert(Temp42.m_matrix[3][3] == 1);

}			   

int main(void)
{

	using namespace rczEngine;

	Profiler::Start();
	Logger::Start();

	Profiler::Pointer()->StartProfiler();
	Profiler::Pointer()->NewFrameStart();
	double Pid = (4.0f * atanf(1.0f));
	float Pi = (4.0f * atanf(1.0f));

	//MathUnitTest();
	//VectorUnitTest();
	//MatrixUnitText();

	Vector3 v;

	v.Set(0,0,0);
	std::cout << Vector3::Hash(v) << std::endl;

	v.Set(1.0f, 1.0f, 1.0f);
	std::cout << Vector3::Hash(v) << std::endl;

	v.Set(-1.0f, -1.0f, -1.0f);
	std::cout << Vector3::Hash(v) << std::endl;

	std::cin;

	Profiler::Pointer()->AddTime("Utilities", PROFILE_EVENTS::PROF_GAME);

	Profiler::ShutDown();
	Logger::ShutDown();
}