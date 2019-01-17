#include <iostream>

#include <vector>
#include <array>
#include <type_traits>

using namespace std;

template <class T, unsigned int w = 1, unsigned int h = 1>
class Matrix {
public:
    std::vector<T> data;

    const unsigned int width = w;
    const unsigned int height = h;

    Matrix():
	data(std::vector<T>(w * h))
    {
    }

    ~Matrix() {

    }

    T& at (const unsigned int x, const unsigned int y) {
    	// TODO: bounds checking
    	// static_assert(x * w + y <= w * h, "Cannot access coordinates outside Matrix.");
    	return data[x * w + y];
    }

    T at (const unsigned int x, const unsigned int y) const {
    	// TODO: bounds checking
    	// static_assert(x * w + y <= w * h, "Cannot access coordinates outside Matrix.");
    	return data[x * w + y];
    }

    /**
	 * Adds a matrix to another.
	 * For now, adding matrices off differing sizes is undefined behavior/unsupported.
	 */
    Matrix& operator+= (const Matrix<T, w, h>& rhs) {
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
	friend Matrix operator+ (Matrix<T, w, h> lhs, const Matrix<T, w, h>& rhs) {
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
    	// todo: iterate in a way that allows checking if rhs is defined at that particular position and to use 0 in that case
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
	friend Matrix operator+ (Matrix<T, w, h> lhs, const N& rhs) {
		lhs += rhs;
		return lhs;
	}

	/**
	 * Subtracts a matrix from another.
	 * For now, subtracting matrices of differing sizes is undefined behavior/unsupported.
	 */
    Matrix& operator-= (const Matrix<T, w, h>& rhs) {
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
	friend Matrix operator- (Matrix<T, w, h> lhs, const Matrix<T, w, h>& rhs) {
		lhs -= rhs;
		return lhs;
	}

	/**
	 * Multiplies two matrices together.
	 * TODO: check if this can be optimised.
	 */
	template<
		unsigned int lhs_height, unsigned int lhs_width,
		unsigned int rhs_height, unsigned int rhs_width
	>
	friend Matrix<T, rhs_width, lhs_height> operator* (
		const Matrix<T, lhs_width, lhs_height>& lhs, const Matrix<T, rhs_width, rhs_height>& rhs
	) {
    	static_assert(rhs_height == lhs_width, "Righthand side Matrix height must equal lefthand side Matrix width.");

    	Matrix<T, rhs_width, lhs_height> result;

    	for (unsigned int x = 0; x < rhs_width; ++x) {
    		for (unsigned int y = 0; y < lhs_height; ++y) {
    			T entry;
    			for (unsigned int i = 0; i < lhs_width; ++i) {
    				entry += lhs.at(y, i) * rhs.at(i, x);
    			}
    			result.at(x, y) = entry;
    		}
    	}
		return result;
	}
};

template <class T, unsigned int num_dims = 1>
class Tensor {
public:
	/**
	 * Contains the size of each of the dimensions.
	 * e.g.: {512, 512, 4} for a 512 x 512 pixel RGBA image
	 * or { 4, 4 } for a transformation matrix in 3D space
	 *
	 * Must be assigned in the constructor.
	 */
    const std::array<const unsigned int, num_dims> dimensions;
    
    std::vector<T> data;

    Tensor(const std::array<const unsigned int, num_dims>&& dimensions):
    dimensions(dimensions)
    {
    	unsigned int size = 1;
    	for (unsigned int dim: dimensions) size *= dim;
    	data = std::vector<T>(size);
    }

    ~Tensor() {

    }

	/**
	 * Adds a matrix to another.
	 * For now, adding matrices of differing sizes is undefined behavior.
	 */
    Tensor& operator+= (const Tensor<T, num_dims>& rhs) {
    	// todo: iterate in a way that allows checking if rhs is defined at that particular position and to use 0 in that case
		for (unsigned int i = 0; i < data.size(); ++i) {
			data[i] += rhs.data[i];
		}
		return *this;
	}

	/**
	 * Adds two matrices together.
	 * For now, adding matrices of differing sizes is undefined behavior.
	 */
	friend Tensor operator+ (Tensor lhs, const Tensor& rhs) {
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
    Tensor& operator+= (const N& rhs) {
    	// todo: iterate in a way that allows checking if rhs is defined at that particular position and to use 0 in that case
		for (unsigned int i = 0; i < data.size(); ++i) {
			data[i] += rhs;
		}
		return *this;
	}

	/**
	 * Adds two matrices together.
	 * For now, adding matrices of differing sizes is undefined behavior.
	 */
	template<
    	typename N,
    	typename = typename std::enable_if<std::is_arithmetic<N>::value, N>::type
	>
	friend Tensor operator+ (Tensor lhs, const N& rhs) {
		lhs += rhs;
		return lhs;
	}

	/**
	 * Subtracts a matrix from another.
	 * For now, subtracting matrices of differing sizes is undefined behavior.
	 */
    Tensor& operator-= (const Tensor<T, num_dims>& rhs) {
    	// todo: iterate in a way that allows checking if rhs is defined at that particular position and to use 0 in that case
		for (unsigned int i = 0; i < data.size(); ++i) {
			data[i] -= rhs.data[i];
		}
		return *this;
	}

	/**
	 * Subtracts two matrices from one another.
	 * For now, subtracting matrices of differing sizes is undefined behavior.
	 */
	friend Tensor operator- (Tensor lhs, const Tensor& rhs) {
		lhs -= rhs;
		return lhs;
	}

	/**
	 * TODO: Implement ACTUAL matrix multiplication
	 * Multiplies a matrix with another.
	 * For now, multiplying matrices of differing sizes is undefined behavior.
	 */
    Tensor& operator*= (const Tensor<T, num_dims>& rhs) {
    	// todo: iterate in a way that allows checking if rhs is defined at that particular position and to use 0 in that case
		for (unsigned int i = 0; i < data.size(); ++i) {
			data[i] *= rhs.data[i];
		}
		return *this;
	}

	/**
	 * Multiplies two matrices together.
	 * For now, multiplying matrices of differing sizes is undefined behavior.
	 */
	friend Tensor operator* (Tensor lhs, const Tensor& rhs) {
		lhs *= rhs;
		return lhs;
	}

	/**
	 * Divides a matrix by another.
	 * For now, dividing matrices of differing sizes as well as dividing by matrices containing 0 is undefined behavior.
	 */
    Tensor& operator/= (const Tensor<T, num_dims>& rhs) {
    	// todo: iterate in a way that allows checking if rhs is defined at that particular position and to use 0 in that case
		for (unsigned int i = 0; i < data.size(); ++i) {
			data[i] /= rhs.data[i];
		}
		return *this;
	}

	/**
	 * Divides one matrix by another.
	 * For now, dividing matrices of differing sizes as well as dividing by matrices containing 0 is undefined behavior.
	 */
	friend Tensor operator/ (Tensor lhs, const Tensor& rhs) {
		lhs /= rhs;
		return lhs;
	}

	unsigned int indexAt(const std::vector<unsigned int> &coordinates) const {
		unsigned int idx = 0;
		for (unsigned int i = 0; i < coordinates.size(); ++i) {
			// increment index by coordinate (aka line/colomn number) multiplied by the size of the Tensor in that dimension
			idx += coordinates[i] * dimensions[i];
		}
		return idx;
	}

	std::vector<unsigned int> coordinatesAt(const unsigned int index) const {
		std::vector<unsigned int> coordinates = std::vector<unsigned int>(dimensions.size());
		unsigned int remainingIndex = index;
		for (int i = 0; i < coordinates.size(); ++i)
		{
			unsigned int coord = remainingIndex / dimensions[i];
			remainingIndex -= coord;
		}

		return coordinates;
	}
};

// class Image: public Matrix {
// public:
// 	// TODO: provide constructor that allows for setting channels etc.
// 	Image(const unsigned int x = 1, const unsigned int y = 1):
// 	Matrix({x, y, 3})
// 	{
// 	}
// };
