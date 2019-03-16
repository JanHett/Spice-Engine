#ifndef SPICE_PIXEL
#define SPICE_PIXEL

#include <array>

enum class rgb_channel {
    red,
    green,
    blue,
    alpha
};

enum class cmyk_channel {
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
 * The enums rgb_channel and cmyk_channel are provided for convenience to access a particular channel.
 */
template <unsigned int num_channels>
class pixel {
public:
    pixel():
    data{}
    {}

    pixel(std::array<float, num_channels> initial_data):
    data{initial_data}
    {}

    std::array<float, num_channels> data;

    float& operator[](std::size_t idx)                  { return data[idx]; }
    const float& operator[](std::size_t idx) const      { return data[idx]; }
    float& operator[](rgb_channel channel)               { return data[static_cast<int>(channel)]; }
    const float& operator[](rgb_channel channel) const   { return data[static_cast<int>(channel)]; }
    float& operator[](cmyk_channel channel)              { return data[static_cast<int>(channel)]; }
    const float& operator[](cmyk_channel channel) const  { return data[static_cast<int>(channel)]; }

    pixel<num_channels>& operator+=(const pixel<num_channels>& rhs) {
        for (unsigned int i = 0; i < data.size(); ++i) {
            data[i] += rhs.data[i];
        }
        return *this;
    }

    friend pixel operator+(pixel lhs, const pixel& rhs) {
        lhs += rhs;
        return lhs;
    }

    pixel<num_channels>& operator-=(const pixel<num_channels>& rhs) {
        for (unsigned int i = 0; i < data.size(); ++i) {
            data[i] -= rhs.data[i];
        }
        return *this;
    }

    friend pixel operator-(pixel lhs, const pixel& rhs) {
        lhs -= rhs;
        return lhs;
    }

    pixel<num_channels>& operator*=(const pixel<num_channels>& rhs) {
        for (unsigned int i = 0; i < data.size(); ++i) {
            data[i] *= rhs.data[i];
        }
        return *this;
    }

    friend pixel operator*(pixel lhs, const pixel& rhs) {
        lhs *= rhs;
        return lhs;
    }

    pixel<num_channels>& operator/=(const pixel<num_channels>& rhs) {
        for (unsigned int i = 0; i < data.size(); ++i) {
            data[i] /= rhs.data[i];
        }
        return *this;
    }

    friend pixel operator/(pixel lhs, const pixel& rhs) {
        lhs /= rhs;
        return lhs;
    }
};

template <unsigned int num_channels>
std::ostream& operator<<(std::ostream& os, const pixel<num_channels>& pxl)
{
    os << "pixel(";
    for(auto channel: pxl.data)
        os << channel << ", ";
    os << ")";
    return os;
}

#endif // SPICE_PIXEL
