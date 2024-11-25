#ifndef FILL_ASCII_EFFECT_HPP
#define FILL_ASCII_EFFECT_HPP

#include <cstring>
#include <numeric>

#include "ascii_effect.hpp"
#include "conviniences.hpp"
#include "image/image.hpp"

class FillAsciiEffect : public AsciiEffect {
   public:
    FillAsciiEffect(char const *const palette = " .:-;=+*#B%@")
    : palette(palette), palette_len(strlen(palette)) {}

    void operator()(AsciiArt &dst) const override {
        let &image = dst.get_image();
        let char_size = image.size() / dst.get_size();

        for (letmut it = dst.begin(); it != dst.end(); it++) {
            let region =
                Image::Region(image, it.get_pos() * char_size, char_size);
            let avg = std::accumulate(region.begin(), region.end(), Color()) /
                      char_size.area();
            let avg_lum = avg.luminance();

            letmut i = size_t(avg_lum * palette_len);
            if (i >= palette_len) i = palette_len - 1;

            (*it) = AsciiEl(palette[i]);
        }
    }

   private:
    char const *const palette;
    size_t const palette_len;
};

#endif
