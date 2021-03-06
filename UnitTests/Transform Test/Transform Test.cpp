// Transform Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Source/Engine/Matrix4.h"
#include <assert.h>

using namespace Math;

Matrix4 GenerateMatrix(float value) {

	float m[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = value;
		}
	}

	return Matrix4(m);
}

Matrix4 GenerateRotatable() {

	float m[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = 0;
		}
	}
	m[0][0] = 1;

	return Matrix4(m);
}

Matrix4 GenerateInvertible() {

	Matrix4 result;

	result.matrix[0][0] = 4;
	result.matrix[0][1] = 0;
	result.matrix[0][2] = 0;
	result.matrix[0][3] = 0;

	result.matrix[1][0] = 0;
	result.matrix[1][1] = 0;
	result.matrix[1][2] = 2;
	result.matrix[1][3] = 0;

	result.matrix[2][0] = 0;
	result.matrix[2][1] = 1;
	result.matrix[2][2] = 2;
	result.matrix[2][3] = 0;

	result.matrix[3][0] = 1;
	result.matrix[3][1] = 0;
	result.matrix[3][2] = 0;
	result.matrix[3][3] = 1;

	return result;
}

int main()
{
	
	Matrix4 A = GenerateMatrix(1);
	Matrix4 B = GenerateMatrix(2);
	Matrix4 C = GenerateMatrix(16);
	Matrix4 D = GenerateMatrix(4);
	Matrix4 invertible = GenerateInvertible();

	//test muliplying by scalar 
	assert((A * 2)== B);

	assert(C == (B*B));
	
	//make sure inverse is equal to identity matrix
	assert(Invert_4x4(invertible)*invertible == IdentityMatrix());

	system("pause");
}

