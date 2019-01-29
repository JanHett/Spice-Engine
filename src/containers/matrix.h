#include <iostream>

#include <array>
#include <vector>
#include <type_traits>

/**
 * Generic matrix class.
 * This class assumes that `T` is a number-like type providing the mathematical operators +, -, * and /.
 * Empty value initialisation T{} should initialise the type with a 'zero value'.
 */
template <class T>
class Matrix {
public:
    std::vector<T> data;

    const unsigned int width;
    const unsigned int height;

    Matrix(unsigned int w = 1, unsigned int h = 1):
	data(std::vector<T>(w * h)),
	width(w),
	height(h)
    {
    }

    ~Matrix() {

    }

    /**
     * Statically gets a reference to the element at the specified coordinates.
     * Invoked as `T element = matrix.at<42, 47>();`
     */
    template <const unsigned int x, const unsigned int y>
    constexpr const T& at () const {
    	// bounds checking
    	// static_assert(y * width + x + 1 <= width * height, "Cannot access coordinates outside Matrix.");
    	return data[y * width + x];
    }

    /**
     * Statically gets a reference to the element at the specified coordinates.
     * Invoked as `T element = matrix.at<42, 47>();`
     */
    template <const unsigned int x, const unsigned int y>
    constexpr T& at () {
    	// bounds checking
    	// static_assert(y * width + x + 1 <= width * height, "Cannot access coordinates outside Matrix.");
    	return data[y * width + x];
    }

    /**
     * Gets a const reference to an element at specified coordinates.
     */
    const T& at (const unsigned int x, const unsigned int y) const {
    	// TODO: bounds checking
    	return data[y * width + x];
    }

	/**
     * Gets a reference to an element at specified coordinates.
     */
    T& at (const unsigned int x, const unsigned int y) {
    	// TODO: bounds checking
    	return data[y * width + x];
    }

    /**
	 * Adds a matrix to another.
	 * For now, adding matrices off differing sizes is undefined behavior/unsupported.
	 */
    Matrix& operator+= (const Matrix<T>& rhs) {
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
	friend Matrix operator+ (Matrix<T> lhs, const Matrix<T>& rhs) {
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
    Matrix& operator+= (const N& rhs) {
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
	friend Matrix operator+ (Matrix<T> lhs, const N& rhs) {
		lhs += rhs;
		return lhs;
	}

	/**
	 * Subtracts a matrix from another.
	 * For now, subtracting matrices of differing sizes is undefined behavior/unsupported.
	 */
    Matrix& operator-= (const Matrix<T>& rhs) {
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
	friend Matrix operator- (Matrix<T> lhs, const Matrix<T>& rhs) {
		lhs -= rhs;
		return lhs;
	}

	/**
	 * Multiplies two matrices together.
	 * TODO: check if this can be optimised.
	 */
	friend Matrix<T> operator* (
		const Matrix<T>& lhs, const Matrix<T>& rhs
	) {
    	// static_assert(rhs.height == lhs.width, "Righthand side Matrix height must equal lefthand side Matrix width.");

    	Matrix<T> result(rhs.width, lhs.height);

    	// TODO: optimise cache locality of this

		// step through all the lines of the lhs...
		for (unsigned int line = 0; line < lhs.height; ++line) {
			// and for each, go through all the columns of the rhs
	    	for (unsigned int column = 0; column < rhs.width; ++column) {
    			T entry{}; // value-initialising type here to start off with something neutral
    			// go through each entry in their common dimension (lhs.widht == rhs.height) in other dimension that's currently being operated on
    			for (unsigned int i = 0; i < lhs.width; ++i) {
    				entry += lhs.at(i, line) * rhs.at(column, i);
    			}
    			result.at(column, line) = entry;
    		}
    	}
		return result;
	}

	void print() const {
		for (unsigned int y = 0; y < height; ++y) {
			for (unsigned int x = 0; x < width; ++x) {
				std::cout << at(x, y) << " ";
			}
			std::cout << std::endl;
		}
	}
};

/**
 * Generic matrix class.
 * This class assumes that `T` is a number-like type providing the mathematical operators +, -, * and /.
 * Empty value initialisation T{} should initialise the type with a 'zero value'.
 */
template <class T, unsigned int w = 1, unsigned int h = 1>
class StaticMatrix {
public:
    std::array<T, w * h> data;

    const unsigned int width = w;
    const unsigned int height = h;

    StaticMatrix():
	data(std::array<T, w * h>())
    {
    }

    ~StaticMatrix() {

    }

    /**
     * Statically gets a reference to the element at the specified coordinates.
     * Invoked as `T element = matrix.at<42, 47>();`
     */
    template <const unsigned int x, const unsigned int y>
    constexpr const T& at () const {
    	// bounds checking
    	static_assert(y * w + x + 1 <= w * h, "Cannot access coordinates outside StaticMatrix.");
    	return data[y * w + x];
    }

    /**
     * Statically gets a reference to the element at the specified coordinates.
     * Invoked as `T element = matrix.at<42, 47>();`
     */
    template <const unsigned int x, const unsigned int y>
    constexpr T& at () {
    	// bounds checking
    	static_assert(y * w + x + 1 <= w * h, "Cannot access coordinates outside StaticMatrix.");
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
    StaticMatrix& operator+= (const StaticMatrix<T, w, h>& rhs) {
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
	friend StaticMatrix operator+ (StaticMatrix<T, w, h> lhs, const StaticMatrix<T, w, h>& rhs) {
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
    StaticMatrix& operator+= (const N& rhs) {
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
	friend StaticMatrix operator+ (StaticMatrix<T, w, h> lhs, const N& rhs) {
		lhs += rhs;
		return lhs;
	}

	/**
	 * Subtracts a matrix from another.
	 * For now, subtracting matrices of differing sizes is undefined behavior/unsupported.
	 */
    StaticMatrix& operator-= (const StaticMatrix<T, w, h>& rhs) {
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
	friend StaticMatrix operator- (StaticMatrix<T, w, h> lhs, const StaticMatrix<T, w, h>& rhs) {
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
	friend StaticMatrix<T, rhs_width, lhs_height> operator* (
		const StaticMatrix<T, lhs_width, lhs_height>& lhs, const StaticMatrix<T, rhs_width, rhs_height>& rhs
	) {
    	static_assert(rhs_height == lhs_width, "Righthand side StaticMatrix height must equal lefthand side StaticMatrix width.");

    	StaticMatrix<T, rhs_width, lhs_height> result;

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