#include <gtest/gtest.h>
#include <iostream>
#include <array>
#include <cstdlib>

#include "../src/prototype.h"

TEST(Matrix, StaticConstAt) {
	Matrix<int, 2, 2> _m;
	_m.data[1] = 42;

	// make a const copy to force using const version of the function
	const Matrix<int, 2, 2> m = _m;

	// check if accessing works
	EXPECT_EQ(0, (m.at<0, 0>()));
	EXPECT_EQ(42, (m.at<1, 0>()));
	EXPECT_EQ(0, (m.at<0, 1>()));
	EXPECT_EQ(0, (m.at<1, 1>()));
}

TEST(Matrix, StaticAt) {
	Matrix<int, 2, 2> m;
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
	Matrix<int, 2, 2> _m;
	_m.data[1] = 42;

	// make a const copy to force using const version of the function
	const Matrix<int, 2, 2> m = _m;

	// check if accessing works
	EXPECT_EQ(0, m.at(0, 0));
	EXPECT_EQ(42, m.at(1, 0));
	EXPECT_EQ(0, m.at(0, 1));
	EXPECT_EQ(0, m.at(1, 1));
}

TEST(Matrix, At) {
	Matrix<int, 2, 2> m;
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
	Matrix<float, 3, 3> m;
	Matrix<float, 3, 3> n;

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
	Matrix<float, 3, 3> m;
	Matrix<float, 3, 3> n;

	m.data[0] = 4;
	n.data[0] = 2;

	EXPECT_EQ(m.data.size(), n.data.size());

	Matrix<float, 3, 3> res = m + n;

	EXPECT_NE(&m, &res);
	EXPECT_NE(&n, &res);
	EXPECT_EQ(6, res.data[0]);
	EXPECT_EQ(m.data.size(), res.data.size());
	EXPECT_NE(m.data, res.data);
}

TEST(Matrix, OperatorScalarAddEquals) {
	Matrix<float, 3, 3> m;
	unsigned int n = 42;

	m.data[0] = 5;

	m += n;

	EXPECT_EQ(47, m.data[0]);
}

TEST(Matrix, OperatorScalarAdd) {
	Matrix<float, 3, 3> m;
	unsigned int n = 42;

	m.data[0] = 5;

	Matrix<float, 3, 3> res = m + n;

	EXPECT_NE(&m, &res);
	EXPECT_EQ(47, res.data[0]);
	EXPECT_EQ(m.data.size(), res.data.size());
	EXPECT_NE(m.data, res.data);
}

TEST(Matrix, OperatorSubtractEquals) {
	Matrix<float, 3, 3> m;
	Matrix<float, 3, 3> n;

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
	Matrix<float, 3, 3> m;
	Matrix<float, 3, 3> n;

	m.data[0] = 42;
	n.data[0] = 2;

	EXPECT_EQ(m.data.size(), n.data.size());

	Matrix<float, 3, 3> res = m - n;

	EXPECT_NE(&m, &res);
	EXPECT_NE(&n, &res);
	EXPECT_EQ(40, res.data[0]);
	EXPECT_EQ(m.data.size(), res.data.size());
	EXPECT_NE(m.data, res.data);
}

TEST(Matrix, OperatorMultiply) {
	Matrix<int, 2, 3> m;
	m.at<0, 0>() = 42;
	m.at<0, 1>() = 47;
	m.at<0, 2>() = 42;
	m.at<1, 0>() = 47;
	m.at<1, 1>() = 42;
	m.at<1, 2>() = 47;
	m.print();

	Matrix<int, 3, 2> n;
	n.at<0, 0>() = 47;
	n.at<0, 1>() = 42;
	n.at<1, 0>() = 47;
	n.at<1, 1>() = 42;
	n.at<2, 0>() = 47;
	n.at<2, 1>() = 42;
	n.print();

	const Matrix result = n * m ;
	EXPECT_EQ(result.height, m.height);
	EXPECT_EQ(result.width, n.width);

	result.print();

	EXPECT_EQ(6157, (result.at<0, 0>()));
	EXPECT_EQ(5502, (result.at<0, 1>()));
	EXPECT_EQ(6392, (result.at<1, 0>()));
	EXPECT_EQ(5712, (result.at<1, 1>()));
}

// ------------------

TEST(Tensor, TensorConstructor) {
	Tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});

	EXPECT_EQ(27, m.data.size());
}

TEST(Tensor, OperatorAddEquals) {
	Tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	Tensor<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 4;
	n.data[0] = 2;

	EXPECT_EQ(m.dimensions, n.dimensions);

	m += n;

	EXPECT_NE(&n, &m);
	EXPECT_EQ(6, m.data[0]);
	EXPECT_EQ(m.dimensions, n.dimensions);
	EXPECT_NE(m.data, n.data);
}

TEST(Tensor, OperatorAdd) {
	Tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	Tensor<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 4;
	n.data[0] = 2;

	EXPECT_EQ(m.dimensions, n.dimensions);

	Tensor<float, 3> res = m + n;

	EXPECT_NE(&m, &res);
	EXPECT_NE(&n, &res);
	EXPECT_EQ(6, res.data[0]);
	EXPECT_EQ(m.dimensions, res.dimensions);
	EXPECT_NE(m.data, res.data);
}

TEST(Tensor, OperatorScalarAddEquals) {
	Tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	unsigned int n = 42;

	m.data[0] = 5;

	m += n;

	EXPECT_EQ(47, m.data[0]);
}

TEST(Tensor, OperatorScalarAdd) {
	Tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	unsigned int n = 42;

	m.data[0] = 5;

	Tensor<float, 3> res = m + n;

	EXPECT_NE(&m, &res);
	EXPECT_EQ(47, res.data[0]);
	EXPECT_EQ(m.dimensions, res.dimensions);
	EXPECT_NE(m.data, res.data);
}

TEST(Tensor, OperatorSubtractEquals) {
	Tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	Tensor<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 42;
	n.data[0] = 2;

	EXPECT_EQ(m.dimensions, n.dimensions);

	m -= n;

	EXPECT_NE(&n, &m);
	EXPECT_EQ(40, m.data[0]);
	EXPECT_EQ(m.dimensions, n.dimensions);
	EXPECT_NE(m.data, n.data);
}

TEST(Tensor, OperatorSubtract) {
	Tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	Tensor<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 42;
	n.data[0] = 2;

	EXPECT_EQ(m.dimensions, n.dimensions);

	Tensor<float, 3> res = m - n;

	EXPECT_NE(&m, &res);
	EXPECT_NE(&n, &res);
	EXPECT_EQ(40, res.data[0]);
	EXPECT_EQ(m.dimensions, res.dimensions);
	EXPECT_NE(m.data, res.data);
}

TEST(Tensor, OperatorMultiplyEquals) {
	Tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	Tensor<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 4;
	n.data[0] = 2;

	EXPECT_EQ(m.dimensions, n.dimensions);

	m *= n;

	EXPECT_NE(&n, &m);
	EXPECT_EQ(8, m.data[0]);
	EXPECT_EQ(m.dimensions, n.dimensions);
	EXPECT_NE(m.data, n.data);
}

TEST(Tensor, OperatorMultiply) {
	Tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	Tensor<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 4;
	n.data[0] = 2;

	EXPECT_EQ(m.dimensions, n.dimensions);

	Tensor<float, 3> res = m * n;

	EXPECT_NE(&m, &res);
	EXPECT_NE(&n, &res);
	EXPECT_EQ(8, res.data[0]);
	EXPECT_EQ(m.dimensions, res.dimensions);
	EXPECT_NE(m.data, res.data);
}

TEST(Tensor, OperatorDivideEquals) {
	Tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	Tensor<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 4;
	for (int i = 0; i < n.data.size(); ++i) n.data[i] = 2;	// make sure we're not dividing by 0

	EXPECT_EQ(m.dimensions, n.dimensions);

	m /= n;

	EXPECT_NE(&n, &m);
	EXPECT_EQ(2, m.data[0]);
	EXPECT_EQ(m.dimensions, n.dimensions);
	EXPECT_NE(m.data, n.data);
}

TEST(Tensor, OperatorDivide) {
	Tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	Tensor<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 4;
	for (int i = 0; i < n.data.size(); ++i) n.data[i] = 2;	// make sure we're not dividing by 0

	EXPECT_EQ(m.dimensions, n.dimensions);

	Tensor<float, 3> res = m / n;

	EXPECT_NE(&m, &res);
	EXPECT_NE(&n, &res);
	EXPECT_EQ(2, res.data[0]);
	EXPECT_EQ(m.dimensions, res.dimensions);
	EXPECT_NE(m.data, res.data);
}
