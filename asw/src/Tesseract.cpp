#include "Tesseract.h"
#include <gsl/narrow>
#include <stdexcept>
#include <tesseract/baseapi.h>

namespace stdex = std::experimental;

namespace asw {

class TesseractIterator final : public ITesseractResultIterator {
  public:
    explicit TesseractIterator(tesseract::ResultIterator* const iterator)
        : iterator_{iterator} {
    }

    ~TesseractIterator() override {
        delete iterator_;
    }

    TesseractIterator(TesseractIterator const&) = delete;
    TesseractIterator& operator=(TesseractIterator const&) = delete;

    TesseractIterator(TesseractIterator&& other) noexcept
        : iterator_{std::exchange(other.iterator_, nullptr)} {
    }

    TesseractIterator& operator=(TesseractIterator&& other) noexcept {
        iterator_ = std::exchange(other.iterator_, nullptr);
        return *this;
    }

    bool next() override {
        return iterator_->Next(tesseract::PageIteratorLevel::RIL_SYMBOL);
    }

    [[nodiscard]] std::string get_utf8_text() const override {
        auto* chars = iterator_->GetUTF8Text(
                tesseract::PageIteratorLevel::RIL_SYMBOL);
        std::string string{chars};
        delete[] chars;
        return string;
    }

  private:
    tesseract::ResultIterator* iterator_ = nullptr;
};

class Tesseract::Impl final {
  public:
    Impl() {
        auto const init_success = api_.Init(
                nullptr,  // Use working directory for looking up training data
                "eng");
        if (init_success != 0) {
            throw std::runtime_error{"Failed to initialize Tesseract API"};
        }
        api_.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
    }

    void set_image(
            stdex::mdspan<
                    unsigned char const,
                    stdex::dextents<size_t, 2>> const& image,
            ImageInfo const& info) {
        api_.SetImage(
                image.data(),
                gsl::narrow_cast<int>(image.extent(0)),
                gsl::narrow_cast<int>(image.extent(1)),
                info.bytes_per_pixel,
                info.bytes_per_line);
    }

    bool recognize() {
        return api_.Recognize(nullptr) == 0;
    }

    std::unique_ptr<ITesseractResultIterator> get_iterator() {
        auto* const iterator = api_.GetIterator();
        if (iterator == nullptr) {
            return nullptr;
        }
        return std::make_unique<TesseractIterator>(iterator);
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
        stdex::mdspan<unsigned char const, stdex::dextents<size_t, 2>> const&
                image,
        ImageInfo const& info) {
    impl_->set_image(image, info);
}

bool Tesseract::recognize() {
    return impl_->recognize();
}

std::unique_ptr<ITesseractResultIterator> Tesseract::get_iterator() {
    return impl_->get_iterator();
}

}  // namespace asw