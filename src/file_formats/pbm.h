#include <string>
#include <algorithm>

#include "../containers/matrix.h"
#include "../containers/pixel.h"

struct ppm
{
    char magic_number[2] = { 'P', '6' };
    unsigned int width;
    unsigned int height;
    unsigned int maxval;
    uint8_t* data;

    template<size_t channels>
    ppm(matrix<pixel<channels>> m, unsigned int maxval = 256) {
        width = m.width;
        height = m.height;
        maxval = maxval;

        // calculate necessary space
        size_t channel_size = maxval <= 256 ? 1 : 2;
        size_t data_length = channel_size * sizeof(uint8_t) * 3 * m.width * m.height;
        data = new uint8_t[data_length];
        // write the image data
        for (unsigned int x = 0; x < m.width; ++x) {
            for (unsigned int y = 0; y < m.height; ++y) {
                for (unsigned int channel = 0; channel < 3; ++channel) {
                    data[y * m.width + x + channel] = channel <= channels ?
                        // if there is data for this particular channel in the matrix, clamp it and multiply by maxval
                        static_cast<uint8_t>(std::min(m.at(x, y)[channel], 1.f) * (maxval - 1)) :
                        0; // otherwise just write 0
                }
            }
        }
    }

    ~ppm() {
        delete[] data;
    }

    template<size_t channels>
    // static ppm from_pixel_matrix(matrix<pixel<channels>> m, unsigned int maxval = 256) {
    //     ppm img;

    //     img.width = m.width;
    //     img.height = m.height;
    //     img.maxval = maxval;

    //     // calculate necessary space
    //     size_t channel_size = img.maxval <= 256 ? 1 : 2;
    //     size_t data_length = channel_size * sizeof(uint8_t) * 3 * m.width * m.height;
    //     img.data = new uint8_t[data_length];
    //     // write the image data
    //     for (unsigned int x = 0; x < m.width; ++x) {
    //         for (unsigned int y = 0; y < m.height; ++y) {
    //             for (unsigned int channel = 0; channel < 3; ++channel) {
    //                 img.data[y * m.width + x + channel] = channel <= channels ?
    //                     // if there is data for this particular channel in the matrix, clamp it and multiply by maxval
    //                     static_cast<uint8_t>(std::min(m.at(x, y)[channel], 1.f) * (img.maxval - 1)) :
    //                     0; // otherwise just write 0
    //             }
    //         }
    //     }

    //     return img;
    // }

    matrix<pixel<3>> to_pixel_matrix() {
        matrix<pixel<3>> m(width, height);

        // do data parsing
        for (unsigned int x = 0; x < m.width; ++x) {
            for (unsigned int y = 0; y < m.height; ++y) {
                for (unsigned int channel = 0; channel < 3; ++channel) {
                    m.at(x, y)[channel] = data[y * m.width + x + channel];
                }
            }
        }

        return m;
    }
};

std::ostream & operator<<(std::ostream & stream, ppm const & input) {
    stream
    << input.magic_number[0]
    << input.magic_number[1]
    << '\n'
    << input.width << ' '
    << input.height << '\n'
    << input.maxval << '\n';

    size_t channel_size = input.maxval <= 256 ? 1 : 2;
    size_t data_length = channel_size * sizeof(uint8_t) * 3 * input.width * input.height;
    for (unsigned int i = 0; i < data_length; ++i) {
        stream << input.data[i];
    }
    return stream;
}

std::istream & operator>>(std::istream & stream, ppm & output) {
    stream >> output.width;
    stream >> output.height;
    stream >> output.maxval;

    size_t channel_size = output.maxval <= 256 ? 1 : 2;
    size_t data_length = channel_size * sizeof(uint8_t) * 3 * output.width * output.height;
    output.data = new uint8_t[data_length];
    for (unsigned int i = 0; i < data_length; ++i) {
        stream >> output.data[i];
    }
    return stream;
}
