#include <gtest/gtest.h>
#include <iostream>
#include <array>
#include <cstdlib>

#include "../../src/containers/matrix.h"

TEST(Matrix, ConstructorEmpty) {
    matrix<float> m;
    EXPECT_EQ(1, m.width());
    EXPECT_EQ(1, m.height());
    EXPECT_EQ(0, (m.at<0, 0>()));
}

TEST(Matrix, ConstructorDimensions) {
    matrix<float> m(2, 3);
    EXPECT_EQ(2, m.width());
    EXPECT_EQ(3, m.height());
    EXPECT_EQ(0, (m.at<0, 0>()));
    EXPECT_EQ(0, (m.at<1, 0>()));
    EXPECT_EQ(0, (m.at<0, 1>()));
    EXPECT_EQ(0, (m.at<1, 1>()));
    EXPECT_EQ(0, (m.at<0, 2>()));
    EXPECT_EQ(0, (m.at<1, 2>()));
}

TEST(Matrix, CopyConstructor) {
    matrix<int> m(2, 2);
    m.data[1] = 42;

    matrix<int> copied(m);

    EXPECT_EQ(0, copied.at(0, 0));
    EXPECT_EQ(42, copied.at(1, 0));
    EXPECT_EQ(0, copied.at(0, 1));
    EXPECT_EQ(0, copied.at(1, 1));
    
    // check if it's really been copied
    m.at(0,1) = 47;
    EXPECT_EQ(0, copied.at(0, 1));
    EXPECT_NE(&m, &copied);
}

TEST(Matrix, MoveConstructor) {
    matrix<int> m(2, 2);
    m.data[1] = 42;

    // create an rvalue matrix and move it to m_moved
    auto m_moved = std::move(m);

    EXPECT_EQ(0, m_moved.at(0, 0));
    EXPECT_EQ(42, m_moved.at(1, 0));
    EXPECT_EQ(0, m_moved.at(0, 1));
    EXPECT_EQ(0, m_moved.at(1, 1));
    
    // check if it's really been moved from the copy-constructed matrix
    m.at(0,1) = 47;
    EXPECT_EQ(0, m_moved.at(0, 1));
    EXPECT_NE(&m, &m_moved);
}

TEST(Matrix, StaticConstAt) {
    matrix<int> _m(2, 2);
    _m.data[1] = 42;

    // make a const copy to force using const version of the function
    const matrix<int> m = _m;

    // check if accessing works
    EXPECT_EQ(0, (m.at<0, 0>()));
    EXPECT_EQ(42, (m.at<1, 0>()));
    EXPECT_EQ(0, (m.at<0, 1>()));
    EXPECT_EQ(0, (m.at<1, 1>()));
}

TEST(Matrix, StaticAt) {
    matrix<int> m(2,2);
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
    matrix<int> _m(2, 2);
    _m.data[1] = 42;

    // make a const copy to force using const version of the function
    const matrix<int> m = _m;

    // check if accessing works
    EXPECT_EQ(0, m.at(0, 0));
    EXPECT_EQ(42, m.at(1, 0));
    EXPECT_EQ(0, m.at(0, 1));
    EXPECT_EQ(0, m.at(1, 1));

    // check out-of-bounds access
    EXPECT_EQ(0, m.at(2, 0));
    EXPECT_EQ(42, m.at(2, 0, overscan_mode::repeat));
}

TEST(Matrix, At) {
    matrix<int> m(2, 2);
    m.data[1] = 42;

    // check if accessing works
    EXPECT_EQ(0, m.at(0, 0));
    EXPECT_EQ(42, m.at(1, 0));
    EXPECT_EQ(0, m.at(0, 1));
    EXPECT_EQ(0, m.at(1, 1));

    // check out-of-bounds access
    EXPECT_EQ(0, m.at(2, 0));
    EXPECT_EQ(42, m.at(2, 0, overscan_mode::repeat));
    
    // check if assigning works
    m.at(0,1) = 47;
    EXPECT_EQ(47, m.at(0, 1));
}

TEST(Matrix, ConstOperatorCall) {
    matrix<int> _m(2, 2);
    _m.data[1] = 42;

    // make a const copy to force using const version of the function
    const matrix<int> m = _m;

    // check if accessing works
    EXPECT_EQ(0, m(0, 0));
    EXPECT_EQ(42, m(1, 0));
    EXPECT_EQ(0, m(0, 1));
    EXPECT_EQ(0, m(1, 1));
}

TEST(Matrix, OperatorCall) {
    matrix<int> m(2, 2);
    m.data[1] = 42;

    // check if accessing works
    EXPECT_EQ(0, m(0, 0));
    EXPECT_EQ(42, m(1, 0));
    EXPECT_EQ(0, m(0, 1));
    EXPECT_EQ(0, m(1, 1));
    
    // check if assigning works
    m(0,1) = 47;
    EXPECT_EQ(47, m(0, 1));
}

TEST(Matrix, OperatorCopyAssignment) {
    matrix<int> m(2, 2);
    m.data[1] = 42;

    matrix<int>m_copy;
    m_copy = m;

    EXPECT_EQ(0, m_copy.at(0, 0));
    EXPECT_EQ(42, m_copy.at(1, 0));
    EXPECT_EQ(0, m_copy.at(0, 1));
    EXPECT_EQ(0, m_copy.at(1, 1));
    
    // check if it's really a copy
    m.at(0,1) = 47;
    EXPECT_EQ(0, m_copy.at(0, 1));
    EXPECT_NE(&m, &m_copy);
}

TEST(Matrix, OperatorMoveAssignment) {
    // create an rvalue matrix and move it to m_moved
    auto create_rvalue = []() {
        matrix<int> m(2, 2);
        m.data[1] = 42;
        return m;
    };

    matrix<int>m_moved;
    m_moved = create_rvalue();

    EXPECT_EQ(0, m_moved.at(0, 0));
    EXPECT_EQ(42, m_moved.at(1, 0));
    EXPECT_EQ(0, m_moved.at(0, 1));
    EXPECT_EQ(0, m_moved.at(1, 1));
}

TEST(Matrix, OperatorAddEquals) {
    matrix<float> m(3, 3);
    matrix<float> n(3, 3);

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
    matrix<float> m(3, 3);
    matrix<float> n(3, 3);

    m.data[0] = 4;
    n.data[0] = 2;

    EXPECT_EQ(m.data.size(), n.data.size());

    matrix<float> res = m + n;

    EXPECT_NE(&m, &res);
    EXPECT_NE(&n, &res);
    EXPECT_EQ(6, res.data[0]);
    EXPECT_EQ(m.data.size(), res.data.size());
    EXPECT_NE(m.data, res.data);
}

TEST(Matrix, OperatorScalarAddEquals) {
    matrix<float> m(3, 3);
    unsigned int n = 42;

    m.data[0] = 5;

    m += n;

    EXPECT_EQ(47, m.data[0]);
}

TEST(Matrix, OperatorScalarAdd) {
    matrix<float> m(3, 3);
    unsigned int n = 42;

    m.data[0] = 5;

    matrix<float> res = m + n;

    EXPECT_NE(&m, &res);
    EXPECT_EQ(47, res.data[0]);
    EXPECT_EQ(m.data.size(), res.data.size());
    EXPECT_NE(m.data, res.data);
}

TEST(Matrix, OperatorSubtractEquals) {
    matrix<float> m(3, 3);
    matrix<float> n(3, 3);

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
    matrix<float> m(3, 3);
    matrix<float> n(3, 3);

    m.data[0] = 42;
    n.data[0] = 2;

    EXPECT_EQ(m.data.size(), n.data.size());

    matrix<float> res = m - n;

    EXPECT_NE(&m, &res);
    EXPECT_NE(&n, &res);
    EXPECT_EQ(40, res.data[0]);
    EXPECT_EQ(m.data.size(), res.data.size());
    EXPECT_NE(m.data, res.data);
}

TEST(Matrix, OperatorMultiply) {
    matrix<int> m(2, 3);
    m.at<0, 0>() = 42;
    m.at<0, 1>() = 47;
    m.at<0, 2>() = 42;
    m.at<1, 0>() = 47;
    m.at<1, 1>() = 42;
    m.at<1, 2>() = 47;

    matrix<int> n(3, 2);
    n.at<0, 0>() = 47;
    n.at<0, 1>() = 42;
    n.at<1, 0>() = 47;
    n.at<1, 1>() = 42;
    n.at<2, 0>() = 47;
    n.at<2, 1>() = 42;

    const matrix result = m * n;
    EXPECT_EQ(result.height(), m.height());
    EXPECT_EQ(result.width(), n.width());

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

TEST(Matrix, transposeConst) {
    matrix<int> m(2, 3);
    m.at<0, 0>() = 1;
    m.at<0, 1>() = 0;
    m.at<0, 2>() = 1;
    m.at<1, 0>() = 0;
    m.at<1, 1>() = 1;
    m.at<1, 2>() = 0;

    auto const const_m(m);

    auto transposed = const_m.transpose();

    // check that it's a copy
    EXPECT_NE(&const_m, &transposed);

    EXPECT_EQ(3, transposed.width());
    EXPECT_EQ(2, transposed.height());

    EXPECT_EQ(1, (transposed.at<0, 0>()));
    EXPECT_EQ(0, (transposed.at<0, 1>()));
    EXPECT_EQ(0, (transposed.at<1, 0>()));
    EXPECT_EQ(1, (transposed.at<1, 1>()));
    EXPECT_EQ(1, (transposed.at<2, 0>()));
    EXPECT_EQ(0, (transposed.at<2, 1>()));
}

TEST(Matrix, fast_blur) {
    matrix<float> m(30,20);
    // set up a checkerboard like so:
    // 0011
    // 1100
    for (unsigned int y = 0; y < m.height(); ++y) {
        for (unsigned int x = 0; x < m.width(); ++x) {
            if (
                (x >= 15 && y < 10) ||
                (x < 15 && y >= 10)
            ) m.at(x, y) = 1;
        }
    }

    print_greyscale(m);
    m.print();

    auto blurred = m.fast_blur(3);

    print_greyscale(blurred);
    blurred.print();

    // should return a matrix of same type as source
    static_assert(std::is_same<decltype(m), decltype(blurred)>::value,
        "Blurred matrix does not have the same type as source");

    // should leave dimensions intact
    EXPECT_EQ(m.width(), blurred.width());
    EXPECT_EQ(m.height(), blurred.height());

    // should leave source matrix intact
    for (unsigned int y = 0; y < m.height(); ++y) {
        for (unsigned int x = 0; x < m.width(); ++x) {
            if (
                (x < 15 && y < 10) ||
                (x >= 15 && y >= 10)
            ) EXPECT_EQ(0, m.at(x, y));
            else EXPECT_EQ(1, m.at(x, y));
        }
    }

    // should apply a blur
    for (unsigned int y = 0; y < blurred.height(); ++y) {
        for (unsigned int x = 0; x < blurred.width(); ++x) {
            // should not escape the range (0, 1)
            EXPECT_GE(0, blurred.at(x, y));
            EXPECT_LE(1, blurred.at(x, y));
            // at the edges there should be full-on 0s or 1s
            if (
                (x < 12 && y < 7) ||
                (x >= 18 && y >= 13)
            ) EXPECT_EQ(0, blurred.at(x, y));
            else if (
                (x >= 18 && y < 7) ||
                (x < 12 && y >= 13)
            ) EXPECT_EQ(1, blurred.at(x, y));
            // in the centre we should have a relatively neutral value
            else if (x == 15 && y == 10) {
                EXPECT_LT(0.6, blurred.at(x,y));
                EXPECT_GT(0.4, blurred.at(x,y));
            }
        }
    }
}

// --------------------

TEST(StaticMatrix, StaticConstAt) {
    static_matrix<int, 2, 2> _m;
    _m.data[1] = 42;

    // make a const copy to force using const version of the function
    const static_matrix<int, 2, 2> m = _m;

    // check if accessing works
    EXPECT_EQ(0, (m.at<0, 0>()));
    EXPECT_EQ(42, (m.at<1, 0>()));
    EXPECT_EQ(0, (m.at<0, 1>()));
    EXPECT_EQ(0, (m.at<1, 1>()));
}

TEST(StaticMatrix, StaticAt) {
    static_matrix<int, 2, 2> m;
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
    static_matrix<int, 2, 2> _m;
    _m.data[1] = 42;

    // make a const copy to force using const version of the function
    const static_matrix<int, 2, 2> m = _m;

    // check if accessing works
    EXPECT_EQ(0, m.at(0, 0));
    EXPECT_EQ(42, m.at(1, 0));
    EXPECT_EQ(0, m.at(0, 1));
    EXPECT_EQ(0, m.at(1, 1));
}

TEST(StaticMatrix, At) {
    static_matrix<int, 2, 2> m;
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
    static_matrix<float, 3, 3> m;
    static_matrix<float, 3, 3> n;

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
    static_matrix<float, 3, 3> m;
    static_matrix<float, 3, 3> n;

    m.data[0] = 4;
    n.data[0] = 2;

    EXPECT_EQ(m.data.size(), n.data.size());

    static_matrix<float, 3, 3> res = m + n;

    EXPECT_NE(&m, &res);
    EXPECT_NE(&n, &res);
    EXPECT_EQ(6, res.data[0]);
    EXPECT_EQ(m.data.size(), res.data.size());
    EXPECT_NE(m.data, res.data);
}

TEST(StaticMatrix, OperatorScalarAddEquals) {
    static_matrix<float, 3, 3> m;
    unsigned int n = 42;

    m.data[0] = 5;

    m += n;

    EXPECT_EQ(47, m.data[0]);
}

TEST(StaticMatrix, OperatorScalarAdd) {
    static_matrix<float, 3, 3> m;
    unsigned int n = 42;

    m.data[0] = 5;

    static_matrix<float, 3, 3> res = m + n;

    EXPECT_NE(&m, &res);
    EXPECT_EQ(47, res.data[0]);
    EXPECT_EQ(m.data.size(), res.data.size());
    EXPECT_NE(m.data, res.data);
}

TEST(StaticMatrix, OperatorSubtractEquals) {
    static_matrix<float, 3, 3> m;
    static_matrix<float, 3, 3> n;

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
    static_matrix<float, 3, 3> m;
    static_matrix<float, 3, 3> n;

    m.data[0] = 42;
    n.data[0] = 2;

    EXPECT_EQ(m.data.size(), n.data.size());

    static_matrix<float, 3, 3> res = m - n;

    EXPECT_NE(&m, &res);
    EXPECT_NE(&n, &res);
    EXPECT_EQ(40, res.data[0]);
    EXPECT_EQ(m.data.size(), res.data.size());
    EXPECT_NE(m.data, res.data);
}

TEST(StaticMatrix, OperatorMultiply) {
    static_matrix<int, 2, 3> m;
    m.at<0, 0>() = 42;
    m.at<0, 1>() = 47;
    m.at<0, 2>() = 42;
    m.at<1, 0>() = 47;
    m.at<1, 1>() = 42;
    m.at<1, 2>() = 47;

    static_matrix<int, 3, 2> n;
    n.at<0, 0>() = 47;
    n.at<0, 1>() = 42;
    n.at<1, 0>() = 47;
    n.at<1, 1>() = 42;
    n.at<2, 0>() = 47;
    n.at<2, 1>() = 42;

    const static_matrix result = m * n;
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