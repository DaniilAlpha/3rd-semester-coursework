#ifndef JPEG_IMAGE_HPP
#define JPEG_IMAGE_HPP

#include <cstddef>

#include "conviniences.hpp"
#include "dims.hpp"
#include "image/image.hpp"

class JpegImage : public Image {
   public:
    static JpegImage load(uint8_t const *const src_data, size_t const src_size);

    ~JpegImage();

    Size get_size() const override { return size; }

    Color operator[](Pos const pos) const override {
        if (pos.x >= size.w || pos.y >= size.h) return Color(0x000000);
        let pix = data[pos.x + size.w * pos.y];
        return Color(pix[0], pix[1], pix[2]);
    }

   private:
    typedef uint8_t JpegPixel[3];

    JpegImage(JpegPixel const *const data, Size const size)
    : data(data), size(size) {}

    JpegPixel const *const data;
    Size const size;
};

#endif
