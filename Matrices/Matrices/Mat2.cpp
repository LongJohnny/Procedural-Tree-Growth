#include "Mat2.h"
#include "MathAux.h"

Mat2::Mat2() : Mat2(0) { }
Mat2::Mat2(const float fill) : Mat2(fill,fill,fill,fill) { }
Mat2::Mat2(const float l1c1, const float l1c2, const float l2c1, const float l2c2) 
{
	m[0][0] = l1c1;
	m[0][1] = l1c2;
	m[1][0] = l2c1;
	m[1][1] = l2c2;
}
Mat2::Mat2(const Mat2& other) {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			m[l][c] = other.m[l][c];
		}
	}
}

Mat2 Mat2::IDENTITY = {1, 0, 
						0, 1};

Mat2& Mat2::operator=(const Mat2& other) {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			m[l][c] = other.m[l][c];
		}
	}
	return *this;
}
Mat2& Mat2::operator+=(const Mat2& other) {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			m[l][c] += other.m[l][c];
		}
	}
	return *this;
}
Mat2& Mat2::operator-=(const Mat2& other) {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			m[l][c] -= other.m[l][c];
		}
	}
	return *this;
}
Mat2& Mat2::operator*=(const Mat2& other) {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			m[l][c] = m[l][0] * other.m[0][l] + 
					  m[l][1] * other.m[1][l];
		}
	}
	return *this;
}

Mat2& Mat2::operator*=(const float s) {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			m[l][c] *= s;
		}
	}
	return *this;
}

Mat2& Mat2::operator/=(const float s) {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			m[l][c] /= s;
		}
	}
	return *this;
}

bool Mat2::operator==(const Mat2& other) const {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			if (!cmpf(m[l][c], other.m[l][c]))
				return false;
		}
	}
	return true;
}
bool Mat2::operator!=(const Mat2& other) const {
	return !(*this == other);
}

Mat2 Mat2::operator+(const Mat2& other) {
	Mat2 sum;
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			sum.m[l][c] = m[l][c] + other.m[l][c];
		}
	}
	return sum;
}
Mat2 Mat2::operator-(const Mat2& other) {
	Mat2 diff;
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			diff.m[l][c] = m[l][c] - other.m[l][c];
		}
	}
	return diff;
}
Mat2 Mat2::operator*(const Mat2& other) {
	Mat2 prod;
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			prod.m[l][c] = m[l][0] * other.m[0][l] +
						   m[l][1] * other.m[1][l];
		}
	}
	return prod;
}
Mat2 Mat2::operator*(const float s) {
	Mat2 prod;
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			prod.m[l][c] = m[l][c] * s;
		}
	}
	return prod;
}
Mat2 Mat2::operator/(const float s) {
	Mat2 divid;
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			divid.m[l][c] = m[l][c] / s;
		}
	}
	return *this;
}


Mat2 operator*(const float s, const Mat2& mat2) {
	Mat2 prod;
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			prod.m[l][c] = mat2.m[l][c] * s;
		}
	}
	return prod;
}
Vec2 Mat2::operator*(const Vec2& v) {
	Vec2 prod;
	prod.x = m[0][0] * v.x;
	prod.y = m[1][1] * v.y;
	return prod;
}

Mat2 Mat2::transpose() {
	Mat2 trans;
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			trans.m[l][c] = m[c][l];
		}
	}
	return trans;
}
bool Mat2::inverse(Mat2& toInvert) {
	float det = toInvert.determinant();

	if (cmpf(det,0)) // If the determinant is 0 the matrix is not invertible
		return false;

	toInvert = (1 / det) * Mat2(toInvert.m[1][1], -toInvert.m[0][1],
							   -toInvert.m[1][0], toInvert.m[0][0]);
	return true;
}
float Mat2::determinant() {
	return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}
float* Mat2::toOpenGLFormat() {
	float mat[4];
	int i = 0;
	for (int c = 0; c < 2; c++) {
		for (int l = 0; l < 2; l++) {
			mat[i] = m[l][c];
			++i;
		}
	}
	return mat;
}

std::ostream& operator<<(std::ostream& os, const Mat2& mat2) {
	for (int l = 0; l < 2; l++) {
		os << "[ ";
		for (int c = 0; c < 2; c++) {
			os << mat2.m[l][c];
			if (c < 1) { // Only print on the first column
				os << " , ";
			}
		}
		os << " ]" << std::endl;
	}
	return os;
}
std::istream& operator>>(std::istream& is, Mat2& mat2) {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			is >> mat2.m[l][c];
		}
	}
	return is;
}
