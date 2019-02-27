#ifndef SPICE_PBM
#define SPICE_PBM

#include <string>
#include <algorithm>
#include <fstream>

#include "../containers/matrix.h"
#include "../containers/pixel.h"

template<size_t channels = 3>
struct ppm
{
    char magic_number[2] = { 'P', '6' };
    unsigned int width;
    unsigned int height;
    unsigned int maxval;
    uint8_t* data;

    /**
     * Constructs a 1x1 ppm with maxval 256
     */
    ppm():
    width(1),
    height(1),
    maxval(256)
    {
        data = new uint8_t[1];
    }
    
    /**
     * Constructs a ppm with data contained in matrix m and maxval 256 unless specified otherwise
     */
    ppm(matrix<pixel<channels>> const m, unsigned int const maxval = 256) {
        width = m.get_width();
        height = m.get_height();
        this->maxval = maxval;

        // calculate and allocate necessary space
        size_t channel_size = maxval <= 256 ? 1 : 2;
        size_t data_length = channel_size * sizeof(uint8_t) * 3 * m.get_width() * m.get_height();
        data = new uint8_t[data_length];
        // write the image data
        unsigned int idx = 0;   // the current position in the ppm data array
        for (auto & pxl: m.data) {
            for (unsigned int channel = 0; channel < 3; ++channel) {
                // if there is data for this particular channel in the matrix, clamp it and multiply by maxval
                if (channel <= channels)
                    data[idx] = static_cast<uint8_t>(std::min(pxl[channel], 1.f) * (maxval - 1));
                // otherwise just write 0
                else
                    data[idx] = 0;
                ++idx;
            }
        }
    }

    /**
     * Reads the file at the specified path and constructs a ppm object according to its contents.
     * This will throw if the file cannot be opened.
     */
    ppm(char const * filename) {
        std::ifstream file;
        file.open(filename, std::ios::binary);

        try {
            if (file.fail()) { throw ("Cannot open input file."); }

            // check magic number - will be discarded right away
            std::string header;
            file >> header;
            if (header != "P6") throw (std::string(filename) + " is not a valid PPM file.");

            file >> width >> height >> maxval;
            file.ignore(256, '\n');

            size_t channel_size = maxval <= 256 ? 1 : 2;
            size_t data_length = channel_size * sizeof(uint8_t) * 3 * width * height;
            data = new uint8_t[data_length];
            for (unsigned int i = 0; i < data_length; ++i) {
                file >> data[i];
            }
        } catch (char const * err) {
            fprintf(stderr, "%s\n", err); 
            file.close();
        }
        
        

        file.close();
    }

    ~ppm() {
        delete[] data;
    }

    // template<size_t channels>
    [[nodiscard]] static ppm from_pixel_matrix(matrix<pixel<channels>> m, unsigned int maxval = 256) {
        ppm img(m, maxval);

        return img;
    }

    template<size_t matrix_channels = 3>
    [[nodiscard]] matrix<pixel<matrix_channels>> to_pixel_matrix() {
        matrix<pixel<matrix_channels>> m(width, height);

        // do data parsing
        for (unsigned int x = 0; x < m.get_width(); ++x) {
            for (unsigned int y = 0; y < m.get_height(); ++y) {
                for (unsigned int channel = 0; channel < matrix_channels; ++channel) {
                    // set uncovered channels to 0
                    if (channel >= channels) m.at(x, y)[channel] = 0;
                    // get other data from data array
                    else m.at(x, y)[channel] = data[y * m.get_width() * 3 + x * 3 + channel] / maxval;
                }
            }
        }

        return m;
    }
};

template<size_t channels = 3>
std::ostream & operator<<(std::ostream & stream, ppm<channels> const & input) {
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

#endif // SPICE_PBM
