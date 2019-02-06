#ifndef SPICE_PIXEL
#define SPICE_PIXEL

#include <array>

enum class RGBChannel {
    red,
    green,
    blue,
    alpha
};

enum class CMYKChannel {
    cyan,
    magenta,
    yellow,
    black,
    alpha
};

/**
 * Generic class to hold colour information.
 * As a rule, data is organised as follows:
 * for RGB pixels:  [ red, green, blue, alpha, ...arbitrary channels ],
 * for CMYK pixels: [ cyan, magenta, yellow, black, alpha, ...arbitrary channels ]
 *
 * The enums RGBChannel and CMYKChannel are provided for convenience to access a particular channel.
 */
template <unsigned int num_channels>
class Pixel {
public:
    Pixel():
    data{}
    {}

    Pixel(std::array<float, num_channels> initial_data):
    data{initial_data}
    {}

    std::array<float, num_channels> data;

    float& operator[](std::size_t idx)                  { return data[idx]; }
    const float& operator[](std::size_t idx) const      { return data[idx]; }
    float& operator[](RGBChannel channel)               { return data[static_cast<int>(channel)]; }
    const float& operator[](RGBChannel channel) const   { return data[static_cast<int>(channel)]; }
    float& operator[](CMYKChannel channel)              { return data[static_cast<int>(channel)]; }
    const float& operator[](CMYKChannel channel) const  { return data[static_cast<int>(channel)]; }

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

    Pixel<num_channels>& operator-=(const Pixel<num_channels>& rhs) {
        for (unsigned int i = 0; i < data.size(); ++i) {
            data[i] -= rhs.data[i];
        }
        return *this;
    }

    friend Pixel operator-(Pixel lhs, const Pixel& rhs) {
        lhs -= rhs;
        return lhs;
    }

    Pixel<num_channels>& operator*=(const Pixel<num_channels>& rhs) {
        for (unsigned int i = 0; i < data.size(); ++i) {
            data[i] *= rhs.data[i];
        }
        return *this;
    }

    friend Pixel operator*(Pixel lhs, const Pixel& rhs) {
        lhs *= rhs;
        return lhs;
    }

    Pixel<num_channels>& operator/=(const Pixel<num_channels>& rhs) {
        for (unsigned int i = 0; i < data.size(); ++i) {
            data[i] /= rhs.data[i];
        }
        return *this;
    }

    friend Pixel operator/(Pixel lhs, const Pixel& rhs) {
        lhs /= rhs;
        return lhs;
    }
};

#endif // SPICE_PIXEL
