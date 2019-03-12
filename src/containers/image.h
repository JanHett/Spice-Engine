#ifndef SPICE_IMAGE
#define SPICE_IMAGE

#include "matrix.h"
#include "pixel.h"

using rgb_image		= matrix<pixel<3>>;
using rgba_image	= matrix<pixel<4>>;
using cmyk_image	= matrix<pixel<4>>;
using cmyka_image	= matrix<pixel<5>>;

#endif // SPICE_IMAGE
