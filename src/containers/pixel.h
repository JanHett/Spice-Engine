#ifndef SPICE_PIXEL
#define SPICE_PIXEL

#include <array>

template <unsigned int num_channels>
class Pixel {
public:
	Pixel(std::array<float, num_channels> initial_data):
	data{initial_data}
	{}

	std::array<float, num_channels> data;

	float& operator[](std::size_t idx) 			   { return data[idx]; }
    const float& operator[](std::size_t idx) const { return data[idx]; }

	Pixel<num_channels>& operator+=(const Pixel<num_channels>& rhs) {
		for (unsigned int i = 0; i < data.size(); ++i) {
			data[i] += rhs.data[i];
		}
		return *this;
	}

	friend Pixel operator+(Pixel lhs, const Pixel& rhs) {
		lhs += rhs;
		return lhs;
	}
};

#endif // SPICE_PIXEL
