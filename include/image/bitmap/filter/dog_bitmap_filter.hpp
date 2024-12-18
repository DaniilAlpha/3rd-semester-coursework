#ifndef DOG_BITMAP_FILTER_HPP
#define DOG_BITMAP_FILTER_HPP

#include <thread>

#include "image/bitmap/filter/bitmap_filter.hpp"

class DoGBitmapFilter : public BitmapFilter {
   public:
    template <uint16_t S>
    struct GaussianKernel : public CnvlKernel<float, S, S> {
        GaussianKernel()
        : CnvlKernel<float, S, S>([](uint16_t const i, uint16_t const j) {
              let x = i - 0.5 * S, y = j - 0.5 * S;
              return 1.0 / (M_2_PI * S * S) *
                     expf(-(x * x + y * y) / (2 * S * S));
          }) {}
    };

    DoGBitmapFilter(float const eps, float const p = 1) : eps(eps), p(p) {}

   private:
    float const eps, p;

    void operator()(Bitmap &dst) const override {
        static let g_s = GaussianKernel<7>();
        static let g_ks = GaussianKernel<5>();

        letmut a = Bitmap(dst.size()), b = Bitmap(dst.size());

        letmut a_thread = std::thread([&a, &dst]() { g_s.apply(a, dst); }),
               b_thread = std::thread([&b, &dst]() { g_ks.apply(b, dst); });
        a_thread.join();
        b_thread.join();

        for (letmut it = dst.begin(); it != dst.end(); it++) {
            let px = a[it.pos()] * (1 + p) - b[it.pos()] * p;
            dst[it.pos()] = px.sqr_magnitude() > eps * eps ? Color(0xFFFFFF)
                                                           : Color(0x000000);
        }
    }
};

#endif
