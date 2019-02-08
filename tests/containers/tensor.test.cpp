#include <gtest/gtest.h>
#include <iostream>
#include <array>
#include <cstdlib>

#include "../../src/containers/tensor.h"

TEST(Tensor, TensorConstructor) {
	tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});

	EXPECT_EQ(27, m.data.size());
}

TEST(Tensor, OperatorAddEquals) {
	tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	tensor<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

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
	tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	tensor<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 4;
	n.data[0] = 2;

	EXPECT_EQ(m.dimensions, n.dimensions);

	tensor<float, 3> res = m + n;

	EXPECT_NE(&m, &res);
	EXPECT_NE(&n, &res);
	EXPECT_EQ(6, res.data[0]);
	EXPECT_EQ(m.dimensions, res.dimensions);
	EXPECT_NE(m.data, res.data);
}

TEST(Tensor, OperatorScalarAddEquals) {
	tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	unsigned int n = 42;

	m.data[0] = 5;

	m += n;

	EXPECT_EQ(47, m.data[0]);
}

TEST(Tensor, OperatorScalarAdd) {
	tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	unsigned int n = 42;

	m.data[0] = 5;

	tensor<float, 3> res = m + n;

	EXPECT_NE(&m, &res);
	EXPECT_EQ(47, res.data[0]);
	EXPECT_EQ(m.dimensions, res.dimensions);
	EXPECT_NE(m.data, res.data);
}

TEST(Tensor, OperatorSubtractEquals) {
	tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	tensor<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

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
	tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	tensor<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 42;
	n.data[0] = 2;

	EXPECT_EQ(m.dimensions, n.dimensions);

	tensor<float, 3> res = m - n;

	EXPECT_NE(&m, &res);
	EXPECT_NE(&n, &res);
	EXPECT_EQ(40, res.data[0]);
	EXPECT_EQ(m.dimensions, res.dimensions);
	EXPECT_NE(m.data, res.data);
}

TEST(Tensor, OperatorMultiplyEquals) {
	tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	tensor<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

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
	tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	tensor<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 4;
	n.data[0] = 2;

	EXPECT_EQ(m.dimensions, n.dimensions);

	tensor<float, 3> res = m * n;

	EXPECT_NE(&m, &res);
	EXPECT_NE(&n, &res);
	EXPECT_EQ(8, res.data[0]);
	EXPECT_EQ(m.dimensions, res.dimensions);
	EXPECT_NE(m.data, res.data);
}

TEST(Tensor, OperatorDivideEquals) {
	tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	tensor<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 4;
	for (size_t i = 0; i < n.data.size(); ++i) n.data[i] = 2;	// make sure we're not dividing by 0

	EXPECT_EQ(m.dimensions, n.dimensions);

	m /= n;

	EXPECT_NE(&n, &m);
	EXPECT_EQ(2, m.data[0]);
	EXPECT_EQ(m.dimensions, n.dimensions);
	EXPECT_NE(m.data, n.data);
}

TEST(Tensor, OperatorDivide) {
	tensor<float, 3> m(std::array<const unsigned int, 3>{3,3,3});
	tensor<float, 3> n(std::array<const unsigned int, 3>{3,3,3});

	m.data[0] = 4;
	for (size_t i = 0; i < n.data.size(); ++i) n.data[i] = 2;	// make sure we're not dividing by 0

	EXPECT_EQ(m.dimensions, n.dimensions);

	tensor<float, 3> res = m / n;

	EXPECT_NE(&m, &res);
	EXPECT_NE(&n, &res);
	EXPECT_EQ(2, res.data[0]);
	EXPECT_EQ(m.dimensions, res.dimensions);
	EXPECT_NE(m.data, res.data);
}
