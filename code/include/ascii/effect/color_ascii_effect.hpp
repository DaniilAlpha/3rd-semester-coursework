#ifndef COLOR_ASCII_EFFECT_HPP
#define COLOR_ASCII_EFFECT_HPP

#include <cmath>
#include <numeric>

#include "ascii_effect.hpp"

struct ColorAsciiEffect : public AsciiEffect {
    void operator()(AsciiArt &dst) const override {
        let &image = dst.image();
        let char_size = image.size() / dst.size();

        for (letmut it = dst.begin(); it != dst.end(); it++) {
            let region =
                Image<Color>::Region(image, it.pos() * char_size, char_size);
            let avg = std::accumulate(region.begin(), region.end(), Color()) /
                      char_size.area();

            (*it).set_true_color(avg);
        }
    }
};

#endif
