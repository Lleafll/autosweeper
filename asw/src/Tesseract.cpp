#include "Tesseract.h"
#include <stdexcept>
#include <tesseract/baseapi.h>

namespace asw {

class Tesseract::Impl final {
  public:
    Impl() {
        auto const init_success = api_.Init(nullptr, "eng");
        if (not init_success) {
            throw std::runtime_error{"Failed to initialize Tesseract API"};
        }
        api_.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
    }

    ~Impl() {
        api_.End();
    }

  private:
    tesseract::TessBaseAPI api_ = {};
};

Tesseract::Tesseract() : impl_{std::make_unique<Impl>()} {
}

std::vector<TesseractDetection>
Tesseract::detect([[maybe_unused]] std::experimental::mdspan<
                  unsigned char const,
                  std::experimental::dextents<std::size_t, 2>> const image)
        const {
    throw std::runtime_error{"NYI"};
}

Tesseract::~Tesseract() = default;

}  // namespace asw