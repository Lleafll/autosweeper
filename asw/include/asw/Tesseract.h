#pragma once

#include "ITesseract.h"
#include <memory>
#include <vector>

namespace asw {

class Tesseract final : public ITesseract {
  public:
    Tesseract();

    ~Tesseract() override;

    void set_image(
            std::experimental::mdspan<
                    unsigned char const,
                    std::experimental::dextents<std::size_t, 2>> image,
            int bytes_per_pixel,
            int bytes_per_line) override;
    void recognize() override;
    [[nodiscard]] std::unique_ptr<ITesseractResultIterator>
    get_iterator() override;

  private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

}  // namespace asw