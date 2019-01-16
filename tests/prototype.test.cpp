#include <gtest/gtest.h>
#include <iostream>
#include <array>
#include <cstdlib>

#include "../src/prototype.h"

TEST(Matrix, MatrixConstructor) {
	Matrix<float, 3> m(std::array<const unsigned int, 3>{3,3,3});

	EXPECT_EQ(27, m.data.size());
}

TEST(Matrix, OperatorAddEquals) {
	Matrix<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	Matrix<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 4;
	n.data[0] = 2;

	EXPECT_EQ(m.dimensions, n.dimensions);

	m += n;

	EXPECT_NE(&n, &m);
	EXPECT_EQ(6, m.data[0]);
	EXPECT_EQ(m.dimensions, n.dimensions);
	EXPECT_NE(m.data, n.data);
}

TEST(Matrix, OperatorAdd) {
	Matrix<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	Matrix<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 4;
	n.data[0] = 2;

	EXPECT_EQ(m.dimensions, n.dimensions);

	Matrix<float, 3> res = m + n;

	EXPECT_NE(&m, &res);
	EXPECT_NE(&n, &res);
	EXPECT_EQ(6, res.data[0]);
	EXPECT_EQ(m.dimensions, res.dimensions);
	EXPECT_NE(m.data, res.data);
}

TEST(Matrix, OperatorScalarAddEquals) {
	Matrix<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	unsigned int n = 42;

	m.data[0] = 5;

	m += n;

	EXPECT_EQ(47, m.data[0]);
}

TEST(Matrix, OperatorScalarAdd) {
	Matrix<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	unsigned int n = 42;

	m.data[0] = 5;

	Matrix<float, 3> res = m + n;

	EXPECT_NE(&m, &res);
	EXPECT_EQ(47, res.data[0]);
	EXPECT_EQ(m.dimensions, res.dimensions);
	EXPECT_NE(m.data, res.data);
}

TEST(Matrix, OperatorSubtractEquals) {
	Matrix<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	Matrix<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 42;
	n.data[0] = 2;

	EXPECT_EQ(m.dimensions, n.dimensions);

	m -= n;

	EXPECT_NE(&n, &m);
	EXPECT_EQ(40, m.data[0]);
	EXPECT_EQ(m.dimensions, n.dimensions);
	EXPECT_NE(m.data, n.data);
}

TEST(Matrix, OperatorSubtract) {
	Matrix<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	Matrix<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 42;
	n.data[0] = 2;

	EXPECT_EQ(m.dimensions, n.dimensions);

	Matrix<float, 3> res = m - n;

	EXPECT_NE(&m, &res);
	EXPECT_NE(&n, &res);
	EXPECT_EQ(40, res.data[0]);
	EXPECT_EQ(m.dimensions, res.dimensions);
	EXPECT_NE(m.data, res.data);
}

TEST(Matrix, OperatorMultiplyEquals) {
	Matrix<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	Matrix<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 4;
	n.data[0] = 2;

	EXPECT_EQ(m.dimensions, n.dimensions);

	m *= n;

	EXPECT_NE(&n, &m);
	EXPECT_EQ(8, m.data[0]);
	EXPECT_EQ(m.dimensions, n.dimensions);
	EXPECT_NE(m.data, n.data);
}

TEST(Matrix, OperatorMultiply) {
	Matrix<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	Matrix<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 4;
	n.data[0] = 2;

	EXPECT_EQ(m.dimensions, n.dimensions);

	Matrix<float, 3> res = m * n;

	EXPECT_NE(&m, &res);
	EXPECT_NE(&n, &res);
	EXPECT_EQ(8, res.data[0]);
	EXPECT_EQ(m.dimensions, res.dimensions);
	EXPECT_NE(m.data, res.data);
}

TEST(Matrix, OperatorDivideEquals) {
	Matrix<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	Matrix<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 4;
	for (int i = 0; i < n.data.size(); ++i) n.data[i] = 2;	// make sure we're not dividing by 0

	EXPECT_EQ(m.dimensions, n.dimensions);

	m /= n;

	EXPECT_NE(&n, &m);
	EXPECT_EQ(2, m.data[0]);
	EXPECT_EQ(m.dimensions, n.dimensions);
	EXPECT_NE(m.data, n.data);
}

TEST(Matrix, OperatorDivide) {
	Matrix<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	Matrix<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 4;
	for (int i = 0; i < n.data.size(); ++i) n.data[i] = 2;	// make sure we're not dividing by 0

	EXPECT_EQ(m.dimensions, n.dimensions);

	Matrix<float, 3> res = m / n;

	EXPECT_NE(&m, &res);
	EXPECT_NE(&n, &res);
	EXPECT_EQ(2, res.data[0]);
	EXPECT_EQ(m.dimensions, res.dimensions);
	EXPECT_NE(m.data, res.data);
}

// TEST(Matrix, MatrixAddEqualsPerformance) {
// 	Matrix<float, 2> m(std::array<const unsigned int, 2>{14204, 10652});
// 	Matrix<float, 2> n(std::array<const unsigned int, 2>{14204, 10652});

// 	std::cout << "Randomising data of " << m.data.size() + n.data.size() << " values." << std::endl;
// 	for (int i = 0; i < m.data.size(); ++i) {
// 		m.data[i] = std::rand() * std::rand() / std::rand();
// 	}
// 	for (int i = 0; i < n.data.size(); ++i) {
// 		n.data[i] = std::rand() * std::rand() / std::rand();
// 	}
// 	std::cout << "Done randomising data. Here's the value at index 42: " << m.data[42] << std::endl;

// 	m.data[0] = 4;
// 	n.data[0] = 2;

// 	EXPECT_EQ(m.dimensions, n.dimensions);

// 	m += n;

// 	EXPECT_NE(&n, &m);
// 	EXPECT_EQ(6, m.data[0]);
// 	EXPECT_EQ(m.dimensions, n.dimensions);
// 	EXPECT_NE(m.data, n.data);
// }
