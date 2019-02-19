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
        width = m.width;
        height = m.height;
        this->maxval = maxval;

        // calculate and allocate necessary space
        size_t channel_size = maxval <= 256 ? 1 : 2;
        size_t data_length = channel_size * sizeof(uint8_t) * 3 * m.width * m.height;
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
        
        std::string header;
        file >> header;
        std::cout << header << std::endl;
        if (header != "P6") throw (std::string(filename) + " is not a valid PPM file.");
        file >> width >> height >> maxval;
        file.ignore(256, '\n');

        std::cout << "Width: " << width << ", height: " << height << ", maxval: " << maxval << std::endl;

        size_t channel_size = maxval <= 256 ? 1 : 2;
        size_t data_length = channel_size * sizeof(uint8_t) * 3 * width * height;
        data = new uint8_t[data_length];
        for (unsigned int i = 0; i < data_length; ++i) {
            file >> data[i];
            std::cout << "Read value " << data[i] << " into data[" << i << "]" << std::endl;
        }

        file.close();
    }

    ~ppm() {
        delete[] data;
    }

    // template<size_t channels>
    static ppm from_pixel_matrix(matrix<pixel<channels>> m, unsigned int maxval = 256) {
        ppm img(m, maxval);

        return img;
    }

    matrix<pixel<3>> to_pixel_matrix() {
        matrix<pixel<3>> m(width, height);

        // do data parsing
        for (unsigned int x = 0; x < m.width; ++x) {
            for (unsigned int y = 0; y < m.height; ++y) {
                for (unsigned int channel = 0; channel < 3; ++channel) {
                    m.at(x, y)[channel] = data[y * m.width * 3 + x * 3 + channel];
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
