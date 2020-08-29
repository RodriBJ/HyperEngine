#pragma once

#include <iostream>
#include <cmath>

#include "Vector4.h"

namespace Hyperion 
{
    class Matrix4
    {
    public:
        float matrix[4][4];

    public:
        Matrix4();
        Matrix4(float value);
        Matrix4(const Matrix4& matrix4);

		Matrix4 operator+(float value) const;
		Matrix4 operator+=(float value);
		Matrix4 operator+(Matrix4 matrix4) const;
		Matrix4 operator+=(Matrix4 matrix4);
		Matrix4 operator+(Vector4& vector) const;
		Matrix4 operator+=(Vector4& vector);

		Matrix4 operator-(float value) const;
		Matrix4 operator-=(float value);
		Matrix4 operator-(Matrix4 matrix4) const;
		Matrix4 operator-=(Matrix4 matrix4);
		Matrix4 operator-(Vector4& vector) const;
		Matrix4 operator-=(Vector4& vector);

        Matrix4 operator*(float value) const;
		Matrix4 operator*=(float value);
		Matrix4 operator*(Matrix4 matrix4) const;
		Matrix4 operator*=(Matrix4 matrix4);
		Matrix4 operator*(Vector4& vector) const;
		Matrix4 operator*=(Vector4& vector);

		Matrix4 operator/(float value) const;
		Matrix4 operator/=(float value);
		Matrix4 operator/(Matrix4 matrix4) const;
		Matrix4 operator/=(Matrix4 matrix4);
		Matrix4 operator/(Vector4& vector) const;
		Matrix4 operator/=(Vector4& vector);

		float* operator[](size_t i) { return matrix[i]; }

        friend std::ostream& operator<<(std::ostream& os, const Matrix4& matrix);
	};

	inline std::ostream& operator<<(std::ostream& os, const Matrix4& matrix)
	{
		for (int i = 0; i < 4; i++)
		{
			os << "[" << matrix.matrix[i][0] << ", " << matrix.matrix[i][1] << ", " << matrix.matrix[i][2] << ", " << matrix.matrix[i][3] << "]";
			if (i != 3)
				os << std::endl;
		}
		return os;
	}
}