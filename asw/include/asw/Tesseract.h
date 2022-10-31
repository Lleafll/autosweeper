#pragma once

#include "ITesseract.h"
#include <memory>
#include <vector>

namespace asw {

class Tesseract final : public ITesseract {
  public:
    Tesseract();

    ~Tesseract() override;

    [[nodiscard]] std::vector<TesseractDetection>
    detect(std::experimental::mdspan<const unsigned char,
            std::experimental::dextents<std::size_t, 2>> image) const override;

  private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

}  // namespace asw