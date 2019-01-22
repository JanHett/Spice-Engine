#include <iostream>

#include <vector>
#include <type_traits>

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