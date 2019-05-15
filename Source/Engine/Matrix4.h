#pragma once
#include <stdio.h>
#include <smmintrin.h>
#include <math.h>
#include <utility>
#include <glm/glm.hpp>

#include "External/GLib/GLib.h"
#include "Vector3.h"

namespace Math {

	static const bool SIMD = true;
	static const bool SIMD_Vector = true;
	static const bool SIMD_Matrix = true;

	float random(float a, float b)
	{
		return a + (rand() / (RAND_MAX / (b - a)));
	}

	class Vector4
	{

	public:

		float x;
		float z;
		float y;
		float w;
		__m128 	m_vec;
		
		Vector4() {
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}
		Vector4(float _x, float _y, float _z, float _w) {
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}
		Vector4(Vector3 vector) {
			x = vector.x;
			y = vector.y;
			z = vector.z;
			w = 0;
		}

		Vector4(float i_x, float i_y, float i_z) : x(i_x), y(i_y), z(i_z)
		{}

		~Vector4() {

		}

		void SetVec() {
			m_vec = { x,y,z,w };
		}

		float const& operator[](int index) const
		{
			if (index == 0) {
				return x;
			}
			else if (index == 1) {
				return y;
			}
			else if (index == 2) {
				return z;
			}
			else if (index == 3) {
				return w;
			}
			else {
				return NULL;
			}
		}

		GLib::Point2D toPoint2D() {

			GLib::Point2D result;
			result.x = x;
			result.y = y;

			return result;
		}

		float DistanceTo(const Vector4 &target) {

			float x_dist = powf((target.x - x), 2.0f);
			float y_dist = powf((target.y - y), 2.0f);
			float z_dist = powf((target.z - z), 2.0f);

			return sqrtf(x_dist + y_dist + z_dist);
		}

		Vector4 operator + (const Vector4 & B) {

			return Vector4(this->x + B.x, this->y + B.y, this->z + B.z, this->w + B.w);
		}

		Vector4 operator * (const float & B) {

			return Vector4(this->x * B, this->y * B, this->z * B, this->w * B);
		}

		Vector3 toVector3() {

			Vector3 result;
			result.x = x;
			result.y = y;
			result.z = z;

			return result;
		}

		void printVector() {

			printf("{ %f, %f, %f, %f }\n", x, y, z, w);
		}

		float Magnitude() {

			float sum = 0;
			sum += powf(x, 2);
			sum += powf(y, 2);
			sum += powf(z, 2);
			sum += powf(w, 2);
			sum = sqrtf(sum);

			return sum;


		}
	};

	static Vector4 Zero4() {

		return Vector4(0, 0, 0, 0);
	}

	class Matrix4
	{

		/*
			Matrices have the following layout:

			--												--
			|	zero_zero  one_zero  two_zero  three_zero	 |
			|	zero_one   one_one   two_one   three_one     |
			|   zero_two   one_two   two_two   three_two     |
			|	zero_three one_three two_three three_three   |
			--												--

		*/


	public:

		float matrix[4][4];


		Matrix4() {

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					matrix[i][j] = 0;
				}
			}
		}

		Matrix4(
			float zero_zero, float one_zero, float two_zero, float three_zero,
			float zero_one, float one_one, float two_one, float three_one,
			float zero_two, float one_two, float two_two, float three_two,
			float zero_three, float one_three, float two_three, float three_three
		) {

			matrix[0][0] = zero_zero;
			matrix[0][1] = zero_one;
			matrix[0][2] = zero_two;
			matrix[0][3] = zero_three;

			matrix[1][0] = one_zero;
			matrix[1][1] = one_one;
			matrix[1][2] = one_two;
			matrix[1][3] = one_three;

			matrix[2][0] = two_zero;
			matrix[2][1] = two_one;
			matrix[2][2] = two_two;
			matrix[2][3] = two_three;

			matrix[3][0] = three_zero;
			matrix[3][1] = three_one;
			matrix[3][2] = three_two;
			matrix[3][3] = three_three;

		}

		Matrix4(float input[4][4]) {

			if (input != NULL) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						matrix[i][j] = input[i][j];
					}
				}
			}
			else {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						matrix[i][j] = 0.0f;
					}
				}
			}
			

		}

		~Matrix4() {
		}


		float operator[](const std::pair<int, int>& coords)
		{
			return matrix[coords.first][coords.second];
		}

		void printMatrix() {

			printf("--									--\n");
			printf("|	%f	%f	%f	%f	|\n", matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0]);
			printf("|	%f	%f	%f	%f	|\n", matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1]);
			printf("|	%f	%f	%f	%f	|\n", matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2]);
			printf("|	%f	%f	%f	%f	|\n", matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]);
			printf("--									--\n");
		}

	};

	static float Dot_Product_Vector4(const Vector4 &A, const Vector4 &B) {

		if (Math::SIMD && Math::SIMD_Vector) {

			__m128 vec0 = { A.x, A.y, A.z, 0.0f };
			__m128 vec1 = {B.x, B.y, B.z, 0.0f };

			// do the dot product of the first 3 components – the 7 part of 0x71
			// output result to only lower float of output  - the 1 part of 0x71
			__m128 vec2 = _mm_dp_ps(vec0, vec1, 0x71);

			float dot = _mm_cvtss_f32(vec2) + (A.w*B.w);
			float compare = (A.x *B.x) + (A.y * B.y) + (A.z * B.z) + (A.w*B.w);
			printf("SIMD Dot: %f	Normal Dot: %f\n", dot, compare);

			return dot;

		}
		else {
			return (A.x *B.x) + (A.y * B.y) + (A.z * B.z) + (A.w*B.w);
		}

	}


	/*/
	Matrix4 Helper Functions
	*/

	static Matrix4 Transpose_4x4(const Matrix4 &A) {

		Matrix4 result;

		for (int row = 0; row < 4; row++) {

			for (int col = 0; col < 4; col++) {

				result.matrix[col][row] = A.matrix[row][col];
			}
		}

		return result;
	}

	static Matrix4 Multiply_4x4(const Matrix4 &A, const float &factor) {


		Matrix4 result = Matrix4(A);

		for (int row = 0; row < 4; row++) {

			for (int col = 0; col < 4; col++) {

				result.matrix[row][col] *= factor;
			}
		}

		return result;
		
	}

	static Matrix4 MultiplySSE(const Matrix4 & A, const Matrix4 & B) 

	{

		Matrix4 o_out = Matrix4();

		// load B
		__m128 rhs_row1 = _mm_load_ps(&B.matrix[0][0]);
		__m128 rhs_row2 = _mm_load_ps(&B.matrix[1][0]);
		__m128 rhs_row3 = _mm_load_ps(&B.matrix[2][0]);
		__m128 rhs_row4 = _mm_load_ps(&B.matrix[3][0]);

		__m128 acc;



		// (*this).col1 * B
		// m_11 * B.row1
		acc = _mm_mul_ps(_mm_load1_ps(&A.matrix[0][0]), rhs_row1);

		// m_12 * B.row2
		acc = _mm_add_ps(acc, _mm_mul_ps(_mm_load1_ps(&A.matrix[0][1]), rhs_row2));

		// m_13 * B.row3
		acc = _mm_add_ps(acc, _mm_mul_ps(_mm_load1_ps(&A.matrix[0][2]), rhs_row3));

		// m_14 * B.row4
		acc = _mm_add_ps(acc, _mm_mul_ps(_mm_load1_ps(&A.matrix[0][3]), rhs_row4));

		// write result
		_mm_storel_pi(reinterpret_cast<__m64 *>(&o_out.matrix[0][0]), acc);
		_mm_storeh_pi(reinterpret_cast<__m64 *>(&o_out.matrix[0][2]), acc);

		// (*this).col2 * B
		// m_21 * B.row1
		acc = _mm_mul_ps(_mm_load1_ps(&A.matrix[1][0]), rhs_row1);

		// m_22 * B.row2
		acc = _mm_add_ps(acc, _mm_mul_ps(_mm_load1_ps(&A.matrix[1][1]), rhs_row2));

		// m_23 * B.row3
		acc = _mm_add_ps(acc, _mm_mul_ps(_mm_load1_ps(&A.matrix[1][2]), rhs_row3));

		// m_24 * B.row4
		acc = _mm_add_ps(acc, _mm_mul_ps(_mm_load1_ps(&A.matrix[1][3]), rhs_row4));

		// write result
		_mm_storel_pi(reinterpret_cast<__m64 *>(&o_out.matrix[1][0]), acc);
		_mm_storeh_pi(reinterpret_cast<__m64 *>(&o_out.matrix[1][2]), acc);

		// (*this).col3 * B
		// m_31 * B.row1
		acc = _mm_mul_ps(_mm_load1_ps(&A.matrix[2][0]), rhs_row1);

		// m_32 * B.row2
		acc = _mm_add_ps(acc, _mm_mul_ps(_mm_load1_ps(&A.matrix[2][1]), rhs_row2));

		// m_33 * B.row3
		acc = _mm_add_ps(acc, _mm_mul_ps(_mm_load1_ps(&A.matrix[2][2]), rhs_row3));

		// m_34 * B.row4
		acc = _mm_add_ps(acc, _mm_mul_ps(_mm_load1_ps(&A.matrix[2][3]), rhs_row4));

		// write result
		_mm_storel_pi(reinterpret_cast<__m64 *>(&o_out.matrix[2][0]), acc);
		_mm_storeh_pi(reinterpret_cast<__m64 *>(&o_out.matrix[2][2]), acc);

		// (*this).col4 * B
		// m_41 * B.row1
		acc = _mm_mul_ps(_mm_load1_ps(&A.matrix[3][0]), rhs_row1);

		// m_42 * B.row2
		acc = _mm_add_ps(acc, _mm_mul_ps(_mm_load1_ps(&A.matrix[3][1]), rhs_row2));

		// m_43 * B.row3
		acc = _mm_add_ps(acc, _mm_mul_ps(_mm_load1_ps(&A.matrix[3][2]), rhs_row3));

		// m_44 * B.row4
		acc = _mm_add_ps(acc, _mm_mul_ps(_mm_load1_ps(&A.matrix[3][3]), rhs_row4));

		// write result
		_mm_storel_pi(reinterpret_cast<__m64 *>(&o_out.matrix[3][0]), acc);
		_mm_storeh_pi(reinterpret_cast<__m64 *>(&o_out.matrix[3][2]), acc);

		return o_out;

	}

	static Matrix4 Multiply_4x4(const Matrix4 &A, const Matrix4 &B) {

		if (Math::SIMD && Math::SIMD_Matrix) {

			return MultiplySSE(A, B);
		}
		Matrix4 result;

		//Iterate through each row of A, multiply by each column of B
		for (int row = 0; row < 4; row++) {

			for (int col = 0; col < 4; col++) {

				float dot_product = 0;
				Vector4 vec_A = Vector4(A.matrix[row][0], A.matrix[row][1], A.matrix[row][2], A.matrix[row][3]);
				Vector4 vec_B = Vector4(B.matrix[0][col], B.matrix[1][col], B.matrix[2][col], B.matrix[3][col]);

				result.matrix[row][col] = Dot_Product_Vector4(vec_A, vec_B);
			}
		}

		return result;

	}

	static Matrix4 InverseSSE(const Matrix4 & input )
	{
		__m128 temp = { 0.0f };
		__m128 row0, row1, row2, row3;
		__m128 minor0, minor1, minor2, minor3;

		Matrix4 o_out = Matrix4();

		temp = _mm_loadh_pi(_mm_loadl_pi(temp, reinterpret_cast<const __m64 *>(&input.matrix[0][0])), reinterpret_cast<const __m64 *>(&input.matrix[1][0]));
		row1 = _mm_loadh_pi(_mm_loadl_pi(temp, reinterpret_cast<const __m64 *>(&input.matrix[2][0])), reinterpret_cast<const __m64 *>(&input.matrix[3][0]));
		row0 = _mm_shuffle_ps(temp, row1, 0x88);
		row1 = _mm_shuffle_ps(row1, temp, 0xDD);
		temp = _mm_loadh_pi(_mm_loadl_pi(temp, reinterpret_cast<const __m64 *>(&input.matrix[0][2])), reinterpret_cast<const __m64 *>(&input.matrix[1][2]));
		row3 = _mm_loadh_pi(_mm_loadl_pi(temp, reinterpret_cast<const __m64 *>(&input.matrix[2][2])), reinterpret_cast<const __m64 *>(&input.matrix[3][2]));
		row2 = _mm_shuffle_ps(temp, row3, 0x88);
		row3 = _mm_shuffle_ps(row3, temp, 0xDD);

		// -----------------------------------------------
		temp = _mm_mul_ps(row2, row3);
		temp = _mm_shuffle_ps(temp, temp, 0xB1);
		minor0 = _mm_mul_ps(row1, temp);
		minor1 = _mm_mul_ps(row0, temp);
		temp = _mm_shuffle_ps(temp, temp, 0x4E);
		minor0 = _mm_sub_ps(_mm_mul_ps(row1, temp), minor0);
		minor1 = _mm_sub_ps(_mm_mul_ps(row0, temp), minor1);
		minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);

		// -----------------------------------------------
		temp = _mm_mul_ps(row1, row2);
		temp = _mm_shuffle_ps(temp, temp, 0xB1);
		minor0 = _mm_add_ps(_mm_mul_ps(row3, temp), minor0);
		minor3 = _mm_mul_ps(row0, temp);
		temp = _mm_shuffle_ps(temp, temp, 0x4E);
		minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, temp));
		minor3 = _mm_sub_ps(_mm_mul_ps(row0, temp), minor3);
		minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);

		// -----------------------------------------------
		temp = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
		temp = _mm_shuffle_ps(temp, temp, 0xB1);
		row2 = _mm_shuffle_ps(row2, row2, 0x4E);
		minor0 = _mm_add_ps(_mm_mul_ps(row2, temp), minor0);
		minor2 = _mm_mul_ps(row0, temp);
		temp = _mm_shuffle_ps(temp, temp, 0x4E);
		minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, temp));
		minor2 = _mm_sub_ps(_mm_mul_ps(row0, temp), minor2);
		minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);

		// -----------------------------------------------
		temp = _mm_mul_ps(row0, row1);
		temp = _mm_shuffle_ps(temp, temp, 0xB1);
		minor2 = _mm_add_ps(_mm_mul_ps(row3, temp), minor2);
		minor3 = _mm_sub_ps(_mm_mul_ps(row2, temp), minor3);
		temp = _mm_shuffle_ps(temp, temp, 0x4E);
		minor2 = _mm_sub_ps(_mm_mul_ps(row3, temp), minor2);
		minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, temp));

		// -----------------------------------------------
		temp = _mm_mul_ps(row0, row3);
		temp = _mm_shuffle_ps(temp, temp, 0xB1);
		minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, temp));
		minor2 = _mm_add_ps(_mm_mul_ps(row1, temp), minor2);
		temp = _mm_shuffle_ps(temp, temp, 0x4E);
		minor1 = _mm_add_ps(_mm_mul_ps(row2, temp), minor1);
		minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, temp));

		// -----------------------------------------------
		temp = _mm_mul_ps(row0, row2);
		temp = _mm_shuffle_ps(temp, temp, 0xB1);
		minor1 = _mm_add_ps(_mm_mul_ps(row3, temp), minor1);
		minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, temp));
		temp = _mm_shuffle_ps(temp, temp, 0x4E);
		minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, temp));
		minor3 = _mm_add_ps(_mm_mul_ps(row1, temp), minor3);

		// -----------------------------------------------
		__m128 det;

		det = _mm_mul_ps(row0, minor0);
		det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
		det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
		temp = _mm_rcp_ss(det);
		det = _mm_sub_ss(_mm_add_ss(temp, temp), _mm_mul_ss(det, _mm_mul_ss(temp, temp)));
		det = _mm_shuffle_ps(det, det, 0x00);

		minor0 = _mm_mul_ps(det, minor0);
		_mm_storel_pi(reinterpret_cast<__m64 *>(&o_out.matrix[0][0]), minor0);
		_mm_storeh_pi(reinterpret_cast<__m64 *>(&o_out.matrix[0][2]), minor0);

		minor1 = _mm_mul_ps(det, minor1);
		_mm_storel_pi(reinterpret_cast<__m64 *>(&o_out.matrix[1][0]), minor1);
		_mm_storeh_pi(reinterpret_cast<__m64 *>(&o_out.matrix[1][2]), minor1);

		minor2 = _mm_mul_ps(det, minor2);
		_mm_storel_pi(reinterpret_cast<__m64 *>(&o_out.matrix[2][0]), minor2);
		_mm_storeh_pi(reinterpret_cast<__m64 *>(&o_out.matrix[2][2]), minor2);

		minor3 = _mm_mul_ps(det, minor3);
		_mm_storel_pi(reinterpret_cast<__m64 *>(&o_out.matrix[3][0]), minor3);
		_mm_storeh_pi(reinterpret_cast<__m64 *>(&o_out.matrix[3][2]), minor3);

		return o_out;

	}

	static Matrix4 Invert_4x4(const Matrix4 &A)
	{
		if (Math::SIMD && Math::SIMD_Matrix) {

			return InverseSSE(A);
		}

		Matrix4 result = Matrix4(A);
		float determinant = 0;

		//Get the transpose of the adjoin matrix

		result.matrix[0][0] = A.matrix[1][1] * A.matrix[2][2] * A.matrix[3][3] -
			A.matrix[1][1] * A.matrix[2][3] * A.matrix[3][2] -
			A.matrix[2][1] * A.matrix[1][2] * A.matrix[3][3] +
			A.matrix[2][1] * A.matrix[1][3] * A.matrix[3][2] +
			A.matrix[3][1] * A.matrix[1][2] * A.matrix[2][3] -
			A.matrix[3][1] * A.matrix[1][3] * A.matrix[2][2];

		result.matrix[1][0] = -A.matrix[1][0] * A.matrix[2][2] * A.matrix[3][3] +
			A.matrix[1][0] * A.matrix[2][3] * A.matrix[3][2] +
			A.matrix[2][0] * A.matrix[1][2] * A.matrix[3][3] -
			A.matrix[2][0] * A.matrix[1][3] * A.matrix[3][2] -
			A.matrix[3][0] * A.matrix[1][2] * A.matrix[2][3] +
			A.matrix[3][0] * A.matrix[1][3] * A.matrix[2][2];

		result.matrix[2][0] = A.matrix[1][0] * A.matrix[2][1] * A.matrix[3][3] -
			A.matrix[1][0] * A.matrix[2][3] * A.matrix[3][1] -
			A.matrix[2][0] * A.matrix[1][1] * A.matrix[3][3] +
			A.matrix[2][0] * A.matrix[1][3] * A.matrix[3][1] +
			A.matrix[3][0] * A.matrix[1][1] * A.matrix[2][3] -
			A.matrix[3][0] * A.matrix[1][3] * A.matrix[2][1];

		result.matrix[3][0] = -A.matrix[1][0] * A.matrix[2][1] * A.matrix[3][2] +
			A.matrix[1][0] * A.matrix[2][2] * A.matrix[3][1] +
			A.matrix[2][0] * A.matrix[1][1] * A.matrix[3][2] -
			A.matrix[2][0] * A.matrix[1][2] * A.matrix[3][1] -
			A.matrix[3][0] * A.matrix[1][1] * A.matrix[2][2] +
			A.matrix[3][0] * A.matrix[1][2] * A.matrix[2][1];

		result.matrix[0][1] = -A.matrix[0][1] * A.matrix[2][2] * A.matrix[3][3] +
			A.matrix[0][1] * A.matrix[2][3] * A.matrix[3][2] +
			A.matrix[2][1] * A.matrix[0][2] * A.matrix[3][3] -
			A.matrix[2][1] * A.matrix[0][3] * A.matrix[3][2] -
			A.matrix[3][1] * A.matrix[0][2] * A.matrix[2][3] +
			A.matrix[3][1] * A.matrix[0][3] * A.matrix[2][2];

		result.matrix[1][1] = A.matrix[0][0] * A.matrix[2][2] * A.matrix[3][3] -
			A.matrix[0][0] * A.matrix[2][3] * A.matrix[3][2] -
			A.matrix[2][0] * A.matrix[0][2] * A.matrix[3][3] +
			A.matrix[2][0] * A.matrix[0][3] * A.matrix[3][2] +
			A.matrix[3][0] * A.matrix[0][2] * A.matrix[2][3] -
			A.matrix[3][0] * A.matrix[0][3] * A.matrix[2][2];

		result.matrix[2][1] = -A.matrix[0][0] * A.matrix[2][1] * A.matrix[3][3] +
			A.matrix[0][0] * A.matrix[2][3] * A.matrix[3][1] +
			A.matrix[2][0] * A.matrix[0][1] * A.matrix[3][3] -
			A.matrix[2][0] * A.matrix[0][3] * A.matrix[3][1] -
			A.matrix[3][0] * A.matrix[0][1] * A.matrix[2][3] +
			A.matrix[3][0] * A.matrix[0][3] * A.matrix[2][1];

		result.matrix[3][1] = A.matrix[0][0] * A.matrix[2][1] * A.matrix[3][2] -
			A.matrix[0][0] * A.matrix[2][2] * A.matrix[3][1] -
			A.matrix[2][0] * A.matrix[0][1] * A.matrix[3][2] +
			A.matrix[2][0] * A.matrix[0][2] * A.matrix[3][1] +
			A.matrix[3][0] * A.matrix[0][1] * A.matrix[2][2] -
			A.matrix[3][0] * A.matrix[0][2] * A.matrix[2][1];

		result.matrix[0][2] = A.matrix[0][1] * A.matrix[1][2] * A.matrix[3][3] -
			A.matrix[0][1] * A.matrix[1][3] * A.matrix[3][2] -
			A.matrix[1][1] * A.matrix[0][2] * A.matrix[3][3] +
			A.matrix[1][1] * A.matrix[0][3] * A.matrix[3][2] +
			A.matrix[3][1] * A.matrix[0][2] * A.matrix[1][3] -
			A.matrix[3][1] * A.matrix[0][3] * A.matrix[1][2];

		result.matrix[1][2] = -A.matrix[0][0] * A.matrix[1][2] * A.matrix[3][3] +
			A.matrix[0][0] * A.matrix[1][3] * A.matrix[3][2] +
			A.matrix[1][0] * A.matrix[0][2] * A.matrix[3][3] -
			A.matrix[1][0] * A.matrix[0][3] * A.matrix[3][2] -
			A.matrix[3][0] * A.matrix[0][2] * A.matrix[1][3] +
			A.matrix[3][0] * A.matrix[0][3] * A.matrix[1][2];

		result.matrix[2][2] = A.matrix[0][0] * A.matrix[1][1] * A.matrix[3][3] -
			A.matrix[0][0] * A.matrix[1][3] * A.matrix[3][1] -
			A.matrix[1][0] * A.matrix[0][1] * A.matrix[3][3] +
			A.matrix[1][0] * A.matrix[0][3] * A.matrix[3][1] +
			A.matrix[3][0] * A.matrix[0][1] * A.matrix[1][3] -
			A.matrix[3][0] * A.matrix[0][3] * A.matrix[1][1];

		result.matrix[3][2] = -A.matrix[0][0] * A.matrix[1][1] * A.matrix[3][2] +
			A.matrix[0][0] * A.matrix[1][2] * A.matrix[3][1] +
			A.matrix[1][0] * A.matrix[0][1] * A.matrix[3][2] -
			A.matrix[1][0] * A.matrix[0][2] * A.matrix[3][1] -
			A.matrix[3][0] * A.matrix[0][1] * A.matrix[1][2] +
			A.matrix[3][0] * A.matrix[0][2] * A.matrix[1][1];

		result.matrix[0][3] = -A.matrix[0][1] * A.matrix[1][2] * A.matrix[2][3] +
			A.matrix[0][1] * A.matrix[1][3] * A.matrix[2][2] +
			A.matrix[1][1] * A.matrix[0][2] * A.matrix[2][3] -
			A.matrix[1][1] * A.matrix[0][3] * A.matrix[2][2] -
			A.matrix[2][1] * A.matrix[0][2] * A.matrix[1][3] +
			A.matrix[2][1] * A.matrix[0][3] * A.matrix[1][2];

		result.matrix[1][3] = A.matrix[0][0] * A.matrix[1][2] * A.matrix[2][3] -
			A.matrix[0][0] * A.matrix[1][3] * A.matrix[2][2] -
			A.matrix[1][0] * A.matrix[0][2] * A.matrix[2][3] +
			A.matrix[1][0] * A.matrix[0][3] * A.matrix[2][2] +
			A.matrix[2][0] * A.matrix[0][2] * A.matrix[1][3] -
			A.matrix[2][0] * A.matrix[0][3] * A.matrix[1][2];

		result.matrix[2][3] = -A.matrix[0][0] * A.matrix[1][1] * A.matrix[2][3] +
			A.matrix[0][0] * A.matrix[1][3] * A.matrix[2][1] +
			A.matrix[1][0] * A.matrix[0][1] * A.matrix[2][3] -
			A.matrix[1][0] * A.matrix[0][3] * A.matrix[2][1] -
			A.matrix[2][0] * A.matrix[0][1] * A.matrix[1][3] +
			A.matrix[2][0] * A.matrix[0][3] * A.matrix[1][1];

		result.matrix[3][3] = A.matrix[0][0] * A.matrix[1][1] * A.matrix[2][2] -
			A.matrix[0][0] * A.matrix[1][2] * A.matrix[2][1] -
			A.matrix[1][0] * A.matrix[0][1] * A.matrix[2][2] +
			A.matrix[1][0] * A.matrix[0][2] * A.matrix[2][1] +
			A.matrix[2][0] * A.matrix[0][1] * A.matrix[1][2] -
			A.matrix[2][0] * A.matrix[0][2] * A.matrix[1][1];

		//get determinant 
		determinant = A.matrix[0][0] * result.matrix[0][0] + A.matrix[0][1] * result.matrix[1][0] + A.matrix[0][2] * result.matrix[2][0] + A.matrix[0][3] * result.matrix[3][0];

		if (determinant == 0)
			return NULL;

		determinant = 1.0f / determinant;

		//inverse = 1/det * |adj|
		return Multiply_4x4(result, determinant);

	}

	static Matrix4 Add_4x4(const Matrix4 &A, const Matrix4 &B) {

		Matrix4 result;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result.matrix[i][j] = A.matrix[i][j] + B.matrix[i][j];
			}
		}

		return result;
	}

	static Matrix4 Subtract_4x4(const Matrix4 &A, const Matrix4 &B) {

		Matrix4 result;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result.matrix[i][j] = A.matrix[i][j] - B.matrix[i][j];
			}
		}

		return result;
	}

	static Matrix4 IdentityMatrix() {

		Matrix4 result;

		result.matrix[0][0] = 1;
		result.matrix[0][1] = 0;
		result.matrix[0][2] = 0;
		result.matrix[0][3] = 0;

		result.matrix[1][0] = 0;
		result.matrix[1][1] = 1;
		result.matrix[1][2] = 0;
		result.matrix[1][3] = 0;

		result.matrix[2][0] = 0;
		result.matrix[2][1] = 0;
		result.matrix[2][2] = 1;
		result.matrix[2][3] = 0;

		result.matrix[3][0] = 0;
		result.matrix[3][1] = 0;
		result.matrix[3][2] = 0;
		result.matrix[3][3] = 1;

		return result;
	}

	static Matrix4 RotationMatrix_X(const float &angle) {

		Matrix4 result;

		result.matrix[0][0] = 1;
		result.matrix[0][1] = 0;
		result.matrix[0][2] = 0;
		result.matrix[0][3] = 0;

		result.matrix[1][0] = 0;
		result.matrix[1][1] = cosf(angle);
		result.matrix[1][2] = -sinf(angle);
		result.matrix[1][3] = 0;

		result.matrix[2][0] = 0;
		result.matrix[2][1] = sinf(angle);
		result.matrix[2][2] = cosf(angle);
		result.matrix[2][3] = 0;

		result.matrix[3][0] = 0;
		result.matrix[3][1] = 0;
		result.matrix[3][2] = 0;
		result.matrix[3][3] = 1;

		return result;
	}

	static Matrix4 RotationMatrix_Y(const float &angle) {

		Matrix4 result;

		result.matrix[0][0] = cosf(angle);
		result.matrix[0][1] = 0;
		result.matrix[0][2] = sinf(angle);
		result.matrix[0][3] = 0;

		result.matrix[1][0] = 0;
		result.matrix[1][1] = 1;
		result.matrix[1][2] = 0;
		result.matrix[1][3] = 0;

		result.matrix[2][0] = -sinf(angle);
		result.matrix[2][1] = 0;
		result.matrix[2][2] = cosf(angle);
		result.matrix[2][3] = 0;

		result.matrix[3][0] = 0;
		result.matrix[3][1] = 0;
		result.matrix[3][2] = 0;
		result.matrix[3][3] = 1;

		return result;
	}

	static Matrix4 RotationMatrix_Z(const float &angle) {

		Matrix4 result;

		result.matrix[0][0] = cosf(angle);
		result.matrix[0][1] = -sinf(angle);
		result.matrix[0][2] = 0;
		result.matrix[0][3] = 0;

		result.matrix[1][0] = sinf(angle);
		result.matrix[1][1] = cosf(angle);
		result.matrix[1][2] = 0;
		result.matrix[1][3] = 0;

		result.matrix[2][0] = 0;
		result.matrix[2][1] = 0;
		result.matrix[2][2] = 1;
		result.matrix[2][3] = 0;

		result.matrix[3][0] = 0;
		result.matrix[3][1] = 0;
		result.matrix[3][2] = 0;
		result.matrix[3][3] = 1;

		return result;
	}

	static Matrix4 ScaleMatrix(const Vector4 &scale) {

		Matrix4 result;

		result.matrix[0][0] = scale.x;
		result.matrix[0][1] = 0;
		result.matrix[0][2] = 0;
		result.matrix[0][3] = 0;

		result.matrix[1][0] = 0;
		result.matrix[1][1] = scale.y;
		result.matrix[1][2] = 0;
		result.matrix[1][3] = 0;

		result.matrix[2][0] = 0;
		result.matrix[2][1] = 0;
		result.matrix[2][2] = scale.z;
		result.matrix[2][3] = 0;

		result.matrix[3][0] = 0;
		result.matrix[3][1] = 0;
		result.matrix[3][2] = 0;
		result.matrix[3][3] = 1;

		return result;
	}

	static Matrix4 TranslationMatrix_Col(const Vector4 &translation) {

		Matrix4 result;

		result.matrix[0][0] = 1;
		result.matrix[0][1] = 0;
		result.matrix[0][2] = 0;
		result.matrix[0][3] = translation.x;

		result.matrix[1][0] = 0;
		result.matrix[1][1] = 1;
		result.matrix[1][2] = 0;
		result.matrix[1][3] = translation.y;

		result.matrix[2][0] = 0;
		result.matrix[2][1] = 0;
		result.matrix[2][2] = 1;
		result.matrix[2][3] = translation.z;

		result.matrix[3][0] = 0;
		result.matrix[3][1] = 0;
		result.matrix[3][2] = 0;
		result.matrix[3][3] = 1;

		return result;

	}

	static Matrix4 TranslationMatrix_Row(const Vector4 &translation) {

		Matrix4 result;

		result.matrix[0][0] = 1;
		result.matrix[0][1] = 0;
		result.matrix[0][2] = 0;
		result.matrix[0][3] = 0;

		result.matrix[1][0] = 0;
		result.matrix[1][1] = 1;
		result.matrix[1][2] = 0;
		result.matrix[1][3] = 0;

		result.matrix[2][0] = 0;
		result.matrix[2][1] = 0;
		result.matrix[2][2] = 1;
		result.matrix[2][3] = 0;

		result.matrix[3][0] = translation.x;
		result.matrix[3][1] = translation.y;
		result.matrix[3][2] = translation.z;
		result.matrix[3][3] = 1;

		return result;
	}

	static glm::mat4 Matrix4ToGLM(Math::Matrix4 matrix) {

		glm::mat4 result = glm::mat4();

		for (int i = 0; i < 4; i++) {

			for (int j = 0; j < 4; j++) {

				result[i][j] = matrix.matrix[i][j];
			}
		}

		return result;
	}

	static Math::Matrix4 GLMtoMatrix4(glm::mat4 matrix) {

		Math::Matrix4 result = Math::Matrix4();

		for (int i = 0; i < 4; i++) {

			for (int j = 0; j < 4; j++) {

				result.matrix[i][j] = matrix[i][j];
			}
		}

		return result;
	}

	/*/
	Vector4 Helper Functions
	*/


	static Vector4 Multiply_RowVector(const Vector4 &A, const Matrix4 &B) {


		Vector4 result;

		//iterate through each column of Matrix A
		result.x = (A.x * B.matrix[0][0]) + (A.y * B.matrix[0][1]) + (A.z * B.matrix[0][2]) + (A.w * B.matrix[0][3]);
		result.y = (A.x * B.matrix[1][0]) + (A.y * B.matrix[1][1]) + (A.z * B.matrix[1][2]) + (A.w * B.matrix[1][3]);
		result.z = (A.x * B.matrix[2][0]) + (A.y * B.matrix[2][1]) + (A.z * B.matrix[2][2]) + (A.w * B.matrix[2][3]);
		result.w = (A.x * B.matrix[3][0]) + (A.y * B.matrix[3][1]) + (A.z * B.matrix[3][2]) + (A.w * B.matrix[3][3]);

		return result;

	}

	static Vector4 Multiply_ColVector(const Matrix4 &A, const Vector4 &B) {


		Vector4 result;

		//iterate through each column of Matrix A
		result.x = (A.matrix[0][0] * B.x) + (A.matrix[0][1] * B.y) + (A.matrix[0][2] * B.z) + (A.matrix[0][3] * B.w);
		result.y = (A.matrix[1][0] * B.x) + (A.matrix[1][1] * B.y) + (A.matrix[1][2] * B.z) + (A.matrix[1][3] * B.w);
		result.z = (A.matrix[2][0] * B.x) + (A.matrix[2][1] * B.y) + (A.matrix[2][2] * B.z) + (A.matrix[2][3] * B.w);
		result.w = (A.matrix[3][0] * B.x) + (A.matrix[3][1] * B.y) + (A.matrix[3][2] * B.z) + (A.matrix[3][3] * B.w);

		return result;

	}

	static Vector4 Add_Vector4(const Vector4 &A, const Vector4 &B) {

		Vector4 result;

		result.x = A.x + B.x;
		result.y = A.y + B.y;
		result.z = A.z + B.z;
		result.w = A.w + B.w;

		return result;

	}

	static Vector4 Subtract_Vector4(const Vector4 &A, const Vector4 &B) {

		Vector4 result;

		result.x = A.x - B.x;
		result.y = A.y - B.y;
		result.z = A.z - B.z;
		result.w = A.w - B.w;

		return result;

	}

	static bool Equals(const Matrix4 &A, const Matrix4 &B) {

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (A.matrix[i][j] != B.matrix[i][j]) {
					return false;
				}
			}
		}

		return true;
	}

	/*/
		Operator Overloads
	*/

	Matrix4 operator + (Matrix4 const &A, Matrix4 const &B) {

		return Add_4x4(A, B);
	}

	Vector4 operator + (Vector4 const &A, Vector4 const &B) {

		return Add_Vector4(A, B);
	}

	Vector4 operator - (Vector4 const &A, Vector4 const &B) {

		return Subtract_Vector4(A, B);
	}

	float operator * (Vector4 const &A, Vector4 const &B) {

		return Dot_Product_Vector4(A, B);

	}

	Matrix4 operator * (Matrix4 const &A, float const &scalar) {

		return Multiply_4x4(A, scalar);
	}

	Matrix4 operator * (float const &scalar, Matrix4 const &B) {

		return Multiply_4x4(B, scalar);
	}

	Matrix4 operator - (Matrix4 const &A, Matrix4 const &B) {

		return Subtract_4x4(A, B);
	}



	Matrix4 operator * (Matrix4 const &A, Matrix4 const &B) {

		return Multiply_4x4(A, B);

	}

	Vector4 operator * (Vector4 const &A, Matrix4 const &B) {

		return Multiply_RowVector(A, B);

	}

	Vector4 operator * (Matrix4 const &A, Vector4 const &B) {

		return Multiply_ColVector(A, B);

	}

	bool operator == (Matrix4 const& A, Matrix4 const &B) {

		return Equals(A, B);
	}

	

}