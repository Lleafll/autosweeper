#include "Tesseract.h"
#include <stdexcept>
#include <tesseract/baseapi.h>

namespace asw {

class Tesseract::Impl final {
  public:
    Impl() {
        auto const init_success = api_.Init(
                nullptr,  // Use working directory
                nullptr   // Use "eng", corresponds to eng.traineddata
        );
        if (init_success != 0) {
            throw std::runtime_error{"Failed to initialize Tesseract API"};
        }
        api_.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
    }

    ~Impl() {
        api_.End();
    }

    Impl(Impl const&) = delete;
    Impl(Impl&&) noexcept = delete;
    Impl& operator=(Impl const&) = delete;
    Impl& operator=(Impl&&) noexcept = delete;

  private:
    tesseract::TessBaseAPI api_ = {};
};

Tesseract::Tesseract() : impl_{std::make_unique<Impl>()} {
}

Tesseract::~Tesseract() = default;

void Tesseract::set_image(
        [[maybe_unused]] std::experimental::mdspan<
                unsigned char const,
                std::experimental::dextents<size_t, 2>> image,
        [[maybe_unused]] ImageInfo const& info) {
    throw std::runtime_error{"NYI"};
}

void Tesseract::recognize() {
    throw std::runtime_error{"NYI"};
}

std::unique_ptr<ITesseractResultIterator> Tesseract::get_iterator() {
    throw std::runtime_error{"NYI"};
}

}  // namespace asw