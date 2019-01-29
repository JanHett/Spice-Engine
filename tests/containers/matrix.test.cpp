#include <gtest/gtest.h>
#include <iostream>
#include <array>
#include <cstdlib>

#include "../../src/containers/matrix.h"

TEST(Matrix, StaticConstAt) {
	Matrix<int> _m(2, 2);
	_m.data[1] = 42;

	// make a const copy to force using const version of the function
	const Matrix<int> m = _m;

	// check if accessing works
	EXPECT_EQ(0, (m.at<0, 0>()));
	EXPECT_EQ(42, (m.at<1, 0>()));
	EXPECT_EQ(0, (m.at<0, 1>()));
	EXPECT_EQ(0, (m.at<1, 1>()));
}

TEST(Matrix, StaticAt) {
	Matrix<int> m(2,2);
	m.data[1] = 42;

	// check if accessing works
	EXPECT_EQ(0, (m.at<0, 0>()));
	EXPECT_EQ(42, (m.at<1, 0>()));
	EXPECT_EQ(0, (m.at<0, 1>()));
	EXPECT_EQ(0, (m.at<1, 1>()));
	
	// check if assigning works
	m.at<0,1>() = 47;
	EXPECT_EQ(47, (m.at<0, 1>()));
}

TEST(Matrix, ConstAt) {
	Matrix<int> _m(2, 2);
	_m.data[1] = 42;

	// make a const copy to force using const version of the function
	const Matrix<int> m = _m;

	// check if accessing works
	EXPECT_EQ(0, m.at(0, 0));
	EXPECT_EQ(42, m.at(1, 0));
	EXPECT_EQ(0, m.at(0, 1));
	EXPECT_EQ(0, m.at(1, 1));
}

TEST(Matrix, At) {
	Matrix<int> m(2, 2);
	m.data[1] = 42;

	// check if accessing works
	EXPECT_EQ(0, m.at(0, 0));
	EXPECT_EQ(42, m.at(1, 0));
	EXPECT_EQ(0, m.at(0, 1));
	EXPECT_EQ(0, m.at(1, 1));
	
	// check if assigning works
	m.at(0,1) = 47;
	EXPECT_EQ(47, m.at(0, 1));
}

TEST(Matrix, OperatorAddEquals) {
	Matrix<float> m(3, 3);
	Matrix<float> n(3, 3);

	m.data[0] = 4;
	n.data[0] = 2;

	EXPECT_EQ(m.data.size(), n.data.size());

	m += n;

	EXPECT_NE(&n, &m);
	EXPECT_EQ(6, m.data[0]);
	EXPECT_EQ(m.data.size(), n.data.size());
	EXPECT_NE(m.data, n.data);
}

TEST(Matrix, OperatorAdd) {
	Matrix<float> m(3, 3);
	Matrix<float> n(3, 3);

	m.data[0] = 4;
	n.data[0] = 2;

	EXPECT_EQ(m.data.size(), n.data.size());

	Matrix<float> res = m + n;

	EXPECT_NE(&m, &res);
	EXPECT_NE(&n, &res);
	EXPECT_EQ(6, res.data[0]);
	EXPECT_EQ(m.data.size(), res.data.size());
	EXPECT_NE(m.data, res.data);
}

TEST(Matrix, OperatorScalarAddEquals) {
	Matrix<float> m(3, 3);
	unsigned int n = 42;

	m.data[0] = 5;

	m += n;

	EXPECT_EQ(47, m.data[0]);
}

TEST(Matrix, OperatorScalarAdd) {
	Matrix<float> m(3, 3);
	unsigned int n = 42;

	m.data[0] = 5;

	Matrix<float> res = m + n;

	EXPECT_NE(&m, &res);
	EXPECT_EQ(47, res.data[0]);
	EXPECT_EQ(m.data.size(), res.data.size());
	EXPECT_NE(m.data, res.data);
}

TEST(Matrix, OperatorSubtractEquals) {
	Matrix<float> m(3, 3);
	Matrix<float> n(3, 3);

	m.data[0] = 42;
	n.data[0] = 2;

	EXPECT_EQ(m.data.size(), n.data.size());

	m -= n;

	EXPECT_NE(&n, &m);
	EXPECT_EQ(40, m.data[0]);
	EXPECT_EQ(m.data.size(), n.data.size());
	EXPECT_NE(m.data, n.data);
}

TEST(Matrix, OperatorSubtract) {
	Matrix<float> m(3, 3);
	Matrix<float> n(3, 3);

	m.data[0] = 42;
	n.data[0] = 2;

	EXPECT_EQ(m.data.size(), n.data.size());

	Matrix<float> res = m - n;

	EXPECT_NE(&m, &res);
	EXPECT_NE(&n, &res);
	EXPECT_EQ(40, res.data[0]);
	EXPECT_EQ(m.data.size(), res.data.size());
	EXPECT_NE(m.data, res.data);
}

TEST(Matrix, OperatorMultiply) {
	Matrix<int> m(2, 3);
	m.at<0, 0>() = 42;
	m.at<0, 1>() = 47;
	m.at<0, 2>() = 42;
	m.at<1, 0>() = 47;
	m.at<1, 1>() = 42;
	m.at<1, 2>() = 47;

	Matrix<int> n(3, 2);
	n.at<0, 0>() = 47;
	n.at<0, 1>() = 42;
	n.at<1, 0>() = 47;
	n.at<1, 1>() = 42;
	n.at<2, 0>() = 47;
	n.at<2, 1>() = 42;

	const Matrix result = m * n;
	EXPECT_EQ(result.height, m.height);
	EXPECT_EQ(result.width, n.width);

	EXPECT_EQ(3948, (result.at<0, 0>()));
	EXPECT_EQ(3973, (result.at<0, 1>()));
	EXPECT_EQ(3948, (result.at<0, 2>()));
	EXPECT_EQ(3948, (result.at<1, 0>()));
	EXPECT_EQ(3973, (result.at<1, 1>()));
	EXPECT_EQ(3948, (result.at<1, 2>()));
	EXPECT_EQ(3948, (result.at<2, 0>()));
	EXPECT_EQ(3973, (result.at<2, 1>()));
	EXPECT_EQ(3948, (result.at<2, 2>()));
}

// --------------------

TEST(StaticMatrix, StaticConstAt) {
	StaticMatrix<int, 2, 2> _m;
	_m.data[1] = 42;

	// make a const copy to force using const version of the function
	const StaticMatrix<int, 2, 2> m = _m;

	// check if accessing works
	EXPECT_EQ(0, (m.at<0, 0>()));
	EXPECT_EQ(42, (m.at<1, 0>()));
	EXPECT_EQ(0, (m.at<0, 1>()));
	EXPECT_EQ(0, (m.at<1, 1>()));
}

TEST(StaticMatrix, StaticAt) {
	StaticMatrix<int, 2, 2> m;
	m.data[1] = 42;

	// check if accessing works
	EXPECT_EQ(0, (m.at<0, 0>()));
	EXPECT_EQ(42, (m.at<1, 0>()));
	EXPECT_EQ(0, (m.at<0, 1>()));
	EXPECT_EQ(0, (m.at<1, 1>()));
	
	// check if assigning works
	m.at<0,1>() = 47;
	EXPECT_EQ(47, (m.at<0, 1>()));
}

TEST(StaticMatrix, ConstAt) {
	StaticMatrix<int, 2, 2> _m;
	_m.data[1] = 42;

	// make a const copy to force using const version of the function
	const StaticMatrix<int, 2, 2> m = _m;

	// check if accessing works
	EXPECT_EQ(0, m.at(0, 0));
	EXPECT_EQ(42, m.at(1, 0));
	EXPECT_EQ(0, m.at(0, 1));
	EXPECT_EQ(0, m.at(1, 1));
}

TEST(StaticMatrix, At) {
	StaticMatrix<int, 2, 2> m;
	m.data[1] = 42;

	// check if accessing works
	EXPECT_EQ(0, m.at(0, 0));
	EXPECT_EQ(42, m.at(1, 0));
	EXPECT_EQ(0, m.at(0, 1));
	EXPECT_EQ(0, m.at(1, 1));
	
	// check if assigning works
	m.at(0,1) = 47;
	EXPECT_EQ(47, m.at(0, 1));
}

TEST(StaticMatrix, OperatorAddEquals) {
	StaticMatrix<float, 3, 3> m;
	StaticMatrix<float, 3, 3> n;

	m.data[0] = 4;
	n.data[0] = 2;

	EXPECT_EQ(m.data.size(), n.data.size());

	m += n;

	EXPECT_NE(&n, &m);
	EXPECT_EQ(6, m.data[0]);
	EXPECT_EQ(m.data.size(), n.data.size());
	EXPECT_NE(m.data, n.data);
}

TEST(StaticMatrix, OperatorAdd) {
	StaticMatrix<float, 3, 3> m;
	StaticMatrix<float, 3, 3> n;

	m.data[0] = 4;
	n.data[0] = 2;

	EXPECT_EQ(m.data.size(), n.data.size());

	StaticMatrix<float, 3, 3> res = m + n;

	EXPECT_NE(&m, &res);
	EXPECT_NE(&n, &res);
	EXPECT_EQ(6, res.data[0]);
	EXPECT_EQ(m.data.size(), res.data.size());
	EXPECT_NE(m.data, res.data);
}

TEST(StaticMatrix, OperatorScalarAddEquals) {
	StaticMatrix<float, 3, 3> m;
	unsigned int n = 42;

	m.data[0] = 5;

	m += n;

	EXPECT_EQ(47, m.data[0]);
}

TEST(StaticMatrix, OperatorScalarAdd) {
	StaticMatrix<float, 3, 3> m;
	unsigned int n = 42;

	m.data[0] = 5;

	StaticMatrix<float, 3, 3> res = m + n;

	EXPECT_NE(&m, &res);
	EXPECT_EQ(47, res.data[0]);
	EXPECT_EQ(m.data.size(), res.data.size());
	EXPECT_NE(m.data, res.data);
}

TEST(StaticMatrix, OperatorSubtractEquals) {
	StaticMatrix<float, 3, 3> m;
	StaticMatrix<float, 3, 3> n;

	m.data[0] = 42;
	n.data[0] = 2;

	EXPECT_EQ(m.data.size(), n.data.size());

	m -= n;

	EXPECT_NE(&n, &m);
	EXPECT_EQ(40, m.data[0]);
	EXPECT_EQ(m.data.size(), n.data.size());
	EXPECT_NE(m.data, n.data);
}

TEST(StaticMatrix, OperatorSubtract) {
	StaticMatrix<float, 3, 3> m;
	StaticMatrix<float, 3, 3> n;

	m.data[0] = 42;
	n.data[0] = 2;

	EXPECT_EQ(m.data.size(), n.data.size());

	StaticMatrix<float, 3, 3> res = m - n;

	EXPECT_NE(&m, &res);
	EXPECT_NE(&n, &res);
	EXPECT_EQ(40, res.data[0]);
	EXPECT_EQ(m.data.size(), res.data.size());
	EXPECT_NE(m.data, res.data);
}

TEST(StaticMatrix, OperatorMultiply) {
	StaticMatrix<int, 2, 3> m;
	m.at<0, 0>() = 42;
	m.at<0, 1>() = 47;
	m.at<0, 2>() = 42;
	m.at<1, 0>() = 47;
	m.at<1, 1>() = 42;
	m.at<1, 2>() = 47;

	StaticMatrix<int, 3, 2> n;
	n.at<0, 0>() = 47;
	n.at<0, 1>() = 42;
	n.at<1, 0>() = 47;
	n.at<1, 1>() = 42;
	n.at<2, 0>() = 47;
	n.at<2, 1>() = 42;

	const StaticMatrix result = m * n;
	EXPECT_EQ(result.height, m.height);
	EXPECT_EQ(result.width, n.width);

	EXPECT_EQ(3948, (result.at<0, 0>()));
	EXPECT_EQ(3973, (result.at<0, 1>()));
	EXPECT_EQ(3948, (result.at<0, 2>()));
	EXPECT_EQ(3948, (result.at<1, 0>()));
	EXPECT_EQ(3973, (result.at<1, 1>()));
	EXPECT_EQ(3948, (result.at<1, 2>()));
	EXPECT_EQ(3948, (result.at<2, 0>()));
	EXPECT_EQ(3973, (result.at<2, 1>()));
	EXPECT_EQ(3948, (result.at<2, 2>()));
}