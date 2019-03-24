#ifndef SPICE_MATRIX
#define SPICE_MATRIX

#include <iostream>

#include <array>
#include <vector>
#include <type_traits>
#include <cmath>

#include "pixel.h"

enum class overscan_mode {
    none,
    exception,
    repeat
};

/**
 * Generic matrix class.
 * This class assumes that `T` is a number-like type providing the mathematical operators +, -, * and /.
 * Empty value initialisation T{} should initialise the type with a 'zero value'.
 */
template <class T>
class matrix {
private:
    unsigned int p_width;
    unsigned int p_height;
public:
    std::vector<T> data;

    unsigned int const & width() const { return p_width; }
    unsigned int const & height() const { return p_height; }

    matrix(unsigned int w = 1, unsigned int h = 1):
    data(w * h),
    p_width(w),
    p_height(h)
    {
    }

    matrix(const matrix<T> & other):
    data(other.data),
    p_width(other.p_width),
    p_height(other.p_height)
    {
    }

    matrix(const matrix<T> && other):
    data(std::move(other.data)),
    p_width(std::move(other.p_width)),
    p_height(std::move(other.p_height))
    {
    }

    ~matrix() {}

    /**
     * Statically gets a reference to the element at the specified coordinates.
     * Invoked as `T element = matrix.at<42, 47>();`
     */
    template <const unsigned int x, const unsigned int y>
    constexpr const T& at () const {
        // bounds checking
        // static_assert(y * p_width + x + 1 <= p_width * p_height, "Cannot access coordinates outside matrix.");
        return data[y * p_width + x];
    }

    /**
     * Statically gets a reference to the element at the specified coordinates.
     * Invoked as `T element = matrix.at<42, 47>();`
     */
    template <const unsigned int x, const unsigned int y>
    constexpr T& at () {
        // bounds checking
        // static_assert(y * p_width + x + 1 <= p_width * p_height, "Cannot access coordinates outside matrix.");
        return data[y * p_width + x];
    }

    /**
     * Gets a const reference to an element at specified coordinates.
     */
    const T& at (const int x,
        const int y,
        const overscan_mode overscan = overscan_mode::exception) const
    {
        // bounds checking
        if (x < 0 || x >= p_width || y < 0 || y >= p_height) {
            switch(overscan) {
                case overscan_mode::repeat:
                {
                    int x_fallback = x < 0 ? 0 : x >= p_width ? p_width - 1 : x;
                    int y_fallback = y < 0 ? 0 : y >= p_height ? p_height - 1 : y;
                    return data[y_fallback * p_width + x_fallback];
                }
                case overscan_mode::exception:
                {
                    throw std::out_of_range(
                        "Cannot access " + std::to_string(p_width) + "x" + std::to_string(p_height) +
                        " matrix at (" + std::to_string(x) + ", " + std::to_string(y) + ")."
                    );
                }
                default: break;
            }
        }
        return data[y * p_width + x];
    }

    /**
     * Gets a reference to an element at specified coordinates.
     */
    T& at (const int x,
        const int y,
        const overscan_mode overscan = overscan_mode::exception)
    {
        // bounds checking
        if (x < 0 || x >= p_width || y < 0 || y >= p_height) {
            switch(overscan) {
                case overscan_mode::repeat:
                {
                    int x_fallback = x < 0 ? 0 : x >= p_width ? p_width - 1 : x;
                    int y_fallback = y < 0 ? 0 : y >= p_height ? p_height - 1 : y;
                    return data[y_fallback * p_width + x_fallback];
                }
                case overscan_mode::exception:
                {
                    throw std::out_of_range(
                        "Cannot access " + std::to_string(p_width) + "x" + std::to_string(p_height) +
                        " matrix at (" + std::to_string(x) + ", " + std::to_string(y) + ")."
                    );
                }
                default: break;
            }
        }
        return data[y * p_width + x];
    }

    /**
     * Gets a const reference to an element at specified coordinates.
     * operator() does not perform bounds checking.
     */
    const T& operator() (const unsigned int x, const unsigned int y) const {
        return data[y * p_width + x];
    }

    /**
     * Gets a reference to an element at specified coordinates.
     * operator() does not perform bounds checking.
     */
    T& operator() (const unsigned int x, const unsigned int y) {
        return data[y * p_width + x];
    }

    /** copy assignment */
    matrix& operator=(const matrix<T>& other) {
        if (this != &other) {
            p_width = other.p_width;
            p_height = other.p_height;
            // TODO: can storage be reused?
            data = other.data;
        }
        return *this;
    }

    /** move assignment */
    matrix& operator=(matrix<T>&& other) noexcept {
        if (this != &other) {
            p_width = other.p_width;
            p_height = other.p_height;
            data = std::move(other.data);
        }
        return *this;
    }

    /**
     * Adds a matrix to another.
     * For now, adding matrices off differing sizes is undefined behavior/unsupported.
     */
    matrix& operator+= (const matrix<T>& rhs) {
        // todo: iterate in a way that allows checking if rhs is defined at that particular position and to use 0 in that case
        for (unsigned int i = 0; i < data.size(); ++i) {
            data[i] += rhs.data[i];
        }
        return *this;
    }

    /**
     * Adds two matrices together.
     * For now, adding matrices of differing sizes is undefined behavior/unsupported.
     */
    friend matrix operator+ (matrix<T> lhs, const matrix<T>& rhs) {
        lhs += rhs;
        return lhs;
    }

    /**
     * Adds a scalar to all components of a matrix.
     */
    template<
        typename N,
        typename = typename std::enable_if<std::is_arithmetic<N>::value, N>::type
    >
    matrix& operator+= (const N& rhs) {
        for (unsigned int i = 0; i < data.size(); ++i) {
            data[i] += rhs;
        }
        return *this;
    }

    /**
     * Adds two matrices together.
     * For now, adding matrices of differing sizes is undefined behavior/unsupported.
     */
    template<
        typename N,
        typename = typename std::enable_if<std::is_arithmetic<N>::value, N>::type
    >
    friend matrix operator+ (matrix<T> lhs, const N& rhs) {
        lhs += rhs;
        return lhs;
    }

    /**
     * Subtracts a matrix from another.
     * For now, subtracting matrices of differing sizes is undefined behavior/unsupported.
     */
    matrix& operator-= (const matrix<T>& rhs) {
        // todo: iterate in a way that allows checking if rhs is defined at that particular position and to use 0 in that case
        for (unsigned int i = 0; i < data.size(); ++i) {
            data[i] -= rhs.data[i];
        }
        return *this;
    }

    /**
     * Subtracts two matrices from one another.
     * For now, subtracting matrices of differing sizes is undefined behavior/unsupported.
     */
    friend matrix operator- (matrix<T> lhs, const matrix<T>& rhs) {
        lhs -= rhs;
        return lhs;
    }

    /**
     * Multiplies two matrices together.
     * TODO: check if this can be optimised, add *= operator.
     */
    friend matrix<T> operator* (
        const matrix<T>& lhs, const matrix<T>& rhs
    ) {
        // static_assert(rhs.p_height == lhs.p_width, "Righthand side matrix height must equal lefthand side matrix width.");

        matrix<T> result(rhs.p_width, lhs.p_height);

        // TODO: optimise cache locality of this

        // step through all the lines of the lhs...
        for (unsigned int line = 0; line < lhs.p_height; ++line) {
            // and for each, go through all the columns of the rhs
            for (unsigned int column = 0; column < rhs.p_width; ++column) {
                T entry{}; // value-initialising type here to start off with something neutral
                // go through each entry in their common dimension (lhs.widht == rhs.p_height) in other dimension that's currently being operated on
                for (unsigned int i = 0; i < lhs.p_width; ++i) {
                    entry += lhs.at(i, line) * rhs.at(column, i);
                }
                result.at(column, line) = entry;
            }
        }
        return result;
    }

    /**
     * Returns a transposed copy of the matrix.
     * TODO: in-place implementation?
     */
    matrix<T> transpose() const {
        matrix<T> result(height(), width());
        for (unsigned int y = 0; y < height(); ++y) {
            for (unsigned int x = 0; x < width(); ++x) {
                result.at(y, x) = at(x, y);
            }
        }
        return result;
    }

    /**
     * Runs a series of box blur filters over the image to approximate a gaussian blur.
     */
    matrix<T> fast_blur(float radius, unsigned int passes = 3) const {
        // algorithm described here: http://blog.ivank.net/fastest-gaussian-blur.html (MIT license)
        
        // creates a vector of box-blur sizes for a given standard deviation sigma
        // increasing n will approximate a true gaussian blur better but decrease performance
        // TODO: get the box size right so a small sigma doesn't result in a massive blur radius
        auto box_sizes = [=](float sigma, unsigned int n) {
            float w_ideal = std::sqrt((12 * sigma * sigma / n) + 1);
            float wl = std::floorf(w_ideal);
            if (static_cast<int>(wl) % 2 == 0) --wl;
            float wu = wl + 2;

            float m_ideal = (12 * sigma * sigma - n * wl * wl - 4 * n * wl - 3 * n)/(-4 * wl - 4);
            float m = std::roundf(m_ideal);

            std::vector<float> sizes(n);
            for (unsigned int i = 0; i < n; ++i)
                sizes[i] = i < m ? wl : wu;
            return sizes;
        };

        auto horizontal_blur = [=](matrix<T> const & mtx, float radius) {
            auto mtx_to_blur = mtx;
            auto r = std::round(radius);
            auto diameter = r + r + 1;
            for (unsigned int line = 0; line < mtx.height(); ++line) {
                T accumulator{};
                // calculate the first pixel's value -- TODO: can we avoid going over the negative values?
                for (int offset = -r; offset <= r; ++offset) {
                    accumulator += mtx.at(offset, line, overscan_mode::repeat);
                }
                accumulator /= diameter;
                mtx_to_blur.at(0, line, overscan_mode::repeat) = accumulator;

                // calculate following pixel's values by subtracting left-most pixel within the radius
                // and adding the one to the right of the right-most for each of the remaining pixels of the row
                for (unsigned int column = 1; column < mtx.width(); ++column) {
                    accumulator = accumulator -
                        (mtx.at(column - r - 1, line, overscan_mode::repeat) / diameter) +
                        (mtx.at(column + r, line, overscan_mode::repeat) / diameter);
                    mtx_to_blur.at(column, line, overscan_mode::repeat) = accumulator;
                }
            }
            return mtx_to_blur;
        };

        const std::vector<float> radii = box_sizes(radius, passes);

        // compute horizontal blurs
        // copy is implicit in horizontal_blur lambda
        auto blurred = horizontal_blur(*this, radii[0]);
        for (unsigned int radius_i = 1; radius_i < radii.size(); ++radius_i) {
            blurred = horizontal_blur(blurred, radii[radius_i]);
        }
        // transpose
        blurred = blurred.transpose();
        // compute vertical blurs (which are horizontal on the transpose)
        for (unsigned int radius_i = 0; radius_i < radii.size(); ++radius_i) {
            blurred = horizontal_blur(blurred, radii[radius_i]);
        }
        // return transpose of traspose
        return blurred.transpose();
    }

    void print() const {
        for (unsigned int y = 0; y < height(); ++y) {
            for (unsigned int x = 0; x < width(); ++x) {
                std::cout << at(x, y) << " ";
            }
            std::cout << std::endl;
        }
    }
};

inline void print_greyscale(matrix<float> const & mtx) {
    // \033[48;2;<r>;<g>;<b>m
    for (unsigned int y = 0; y < mtx.height(); ++y) {
        for (unsigned int x = 0; x < mtx.width(); ++x) {
            std::cout << "\033[48;2;" <<
            // set BG-colours
            static_cast<int>(std::floor(mtx.at(x, y) * 255)) << ";" <<
            static_cast<int>(std::floor(mtx.at(x, y) * 255)) << ";" <<
            static_cast<int>(std::floor(mtx.at(x, y) * 255)) <<
            // set FG-colours
            ";38;2;" <<
            static_cast<int>(std::floor(mtx.at(x, y) * 255)) << ";" <<
            static_cast<int>(std::floor(mtx.at(x, y) * 255)) << ";" <<
            static_cast<int>(std::floor(mtx.at(x, y) * 255)) << "m" << "  ";
        }
        std::cout << "\033[0m" << std::endl;
    }
}

template<unsigned int num_channels = 3>
inline void print_color(matrix<pixel<num_channels>> const & mtx) {
    // \033[48;2;<r>;<g>;<b>m
    for (unsigned int y = 0; y < mtx.height(); ++y) {
        for (unsigned int x = 0; x < mtx.width(); ++x) {
            std::cout << "\033[48;2;" <<
            // set BG-colours
            static_cast<int>(std::floor(mtx.at(x, y)[rgb_channel::red] * 255)) << ";" <<
            static_cast<int>(std::floor(mtx.at(x, y)[rgb_channel::green] * 255)) << ";" <<
            static_cast<int>(std::floor(mtx.at(x, y)[rgb_channel::blue] * 255)) <<
            // set FG-colours
            ";38;2;" <<
            static_cast<int>(std::floor(mtx.at(x, y)[rgb_channel::red] * 255)) << ";" <<
            static_cast<int>(std::floor(mtx.at(x, y)[rgb_channel::green] * 255)) << ";" <<
            static_cast<int>(std::floor(mtx.at(x, y)[rgb_channel::blue] * 255)) << "m" << "  ";
        }
        std::cout << "\033[0m" << std::endl;
    }
}

/**
 * Generic matrix class.
 * This class assumes that `T` is a number-like type providing the mathematical operators +, -, * and /.
 * Empty value initialisation T{} should initialise the type with a 'zero value'.
 */
template <class T, unsigned int w = 1, unsigned int h = 1>
class static_matrix {
public:
    std::array<T, w * h> data;

    const unsigned int width = w;
    const unsigned int height = h;

    static_matrix():
    data(std::array<T, w * h>())
    {
    }

    ~static_matrix() {

    }

    /**
     * Statically gets a reference to the element at the specified coordinates.
     * Invoked as `T element = matrix.at<42, 47>();`
     */
    template <const unsigned int x, const unsigned int y>
    constexpr const T& at () const {
        // bounds checking
        static_assert(y * w + x + 1 <= w * h, "Cannot access coordinates outside static_matrix.");
        return data[y * w + x];
    }

    /**
     * Statically gets a reference to the element at the specified coordinates.
     * Invoked as `T element = matrix.at<42, 47>();`
     */
    template <const unsigned int x, const unsigned int y>
    constexpr T& at () {
        // bounds checking
        static_assert(y * w + x + 1 <= w * h, "Cannot access coordinates outside static_matrix.");
        return data[y * w + x];
    }

    /**
     * Gets a const reference to an element at specified coordinates.
     */
    const T& at (const unsigned int x, const unsigned int y) const {
        // TODO: bounds checking
        return data[y * w + x];
    }

    /**
     * Gets a reference to an element at specified coordinates.
     */
    T& at (const unsigned int x, const unsigned int y) {
        // TODO: bounds checking
        return data[y * w + x];
    }

    /**
     * Adds a matrix to another.
     * For now, adding matrices off differing sizes is undefined behavior/unsupported.
     */
    static_matrix& operator+= (const static_matrix<T, w, h>& rhs) {
        // todo: iterate in a way that allows checking if rhs is defined at that particular position and to use 0 in that case
        for (unsigned int i = 0; i < data.size(); ++i) {
            data[i] += rhs.data[i];
        }
        return *this;
    }

    /**
     * Adds two matrices together.
     * For now, adding matrices of differing sizes is undefined behavior/unsupported.
     */
    friend static_matrix operator+ (static_matrix<T, w, h> lhs, const static_matrix<T, w, h>& rhs) {
        lhs += rhs;
        return lhs;
    }

    /**
     * Adds a scalar to all components of a matrix.
     */
    template<
        typename N,
        typename = typename std::enable_if<std::is_arithmetic<N>::value, N>::type
    >
    static_matrix& operator+= (const N& rhs) {
        for (unsigned int i = 0; i < data.size(); ++i) {
            data[i] += rhs;
        }
        return *this;
    }

    /**
     * Adds two matrices together.
     * For now, adding matrices of differing sizes is undefined behavior/unsupported.
     */
    template<
        typename N,
        typename = typename std::enable_if<std::is_arithmetic<N>::value, N>::type
    >
    friend static_matrix operator+ (static_matrix<T, w, h> lhs, const N& rhs) {
        lhs += rhs;
        return lhs;
    }

    /**
     * Subtracts a matrix from another.
     * For now, subtracting matrices of differing sizes is undefined behavior/unsupported.
     */
    static_matrix& operator-= (const static_matrix<T, w, h>& rhs) {
        // todo: iterate in a way that allows checking if rhs is defined at that particular position and to use 0 in that case
        for (unsigned int i = 0; i < data.size(); ++i) {
            data[i] -= rhs.data[i];
        }
        return *this;
    }

    /**
     * Subtracts two matrices from one another.
     * For now, subtracting matrices of differing sizes is undefined behavior/unsupported.
     */
    friend static_matrix operator- (static_matrix<T, w, h> lhs, const static_matrix<T, w, h>& rhs) {
        lhs -= rhs;
        return lhs;
    }

    /**
     * Multiplies two matrices together.
     * TODO: check if this can be optimised.
     */
    template<
        unsigned int lhs_width, unsigned int lhs_height,
        unsigned int rhs_width, unsigned int rhs_height
    >
    friend static_matrix<T, rhs_width, lhs_height> operator* (
        const static_matrix<T, lhs_width, lhs_height>& lhs, const static_matrix<T, rhs_width, rhs_height>& rhs
    ) {
        static_assert(rhs_height == lhs_width, "Righthand side static_matrix height must equal lefthand side static_matrix width.");

        static_matrix<T, rhs_width, lhs_height> result;

        // TODO: optimise cache locality of this

        // step through all the lines of the lhs...
        for (unsigned int line = 0; line < lhs_height; ++line) {
            // and for each, go through all the columns of the rhs
            for (unsigned int column = 0; column < rhs_width; ++column) {
                T entry{}; // value-initialising type here to start off with something neutral
                // go through each entry in their common dimension (lhs_widht == rhs_height) in other dimension that's currently being operated on
                for (unsigned int i = 0; i < lhs_width; ++i) {
                    entry += lhs.at(i, line) * rhs.at(column, i);
                }
                result.at(column, line) = entry;
            }
        }
        return result;
    }

    void print() const {
        for (unsigned int y = 0; y < h; ++y) {
            for (unsigned int x = 0; x < w; ++x) {
                std::cout << at(x, y) << " ";
            }
            std::cout << std::endl;
        }
    }
};

#endif // SPICE_MATRIX
