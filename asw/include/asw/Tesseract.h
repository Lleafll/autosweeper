#pragma once

#include "ITesseract.h"
#include <memory>
#include <vector>

namespace asw {

class Tesseract final : public ITesseract {
  public:
    Tesseract();

    ~Tesseract() override;
    Tesseract(Tesseract const&) = delete;
    Tesseract(Tesseract&&) noexcept = delete;
    Tesseract& operator=(Tesseract const&) = delete;
    Tesseract& operator=(Tesseract&&) noexcept = delete;

    void set_image(
            std::experimental::mdspan<
                    unsigned char const,
                    std::experimental::dextents<size_t, 2>> image,
            ImageInfo const& info) override;
    void recognize() override;
    [[nodiscard]] std::unique_ptr<ITesseractResultIterator>
    get_iterator() override;

  private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

}  // namespace asw