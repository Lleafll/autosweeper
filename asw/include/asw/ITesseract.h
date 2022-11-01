#pragma once

#include <experimental/mdspan>
#include <memory>
#include <vector>

namespace asw {

class ITesseractResultIterator {
  public:
    virtual ~ITesseractResultIterator() = 0;

    /**
     * Implies tesseract::PageIteratorLevel::RIL_SYMBOL
     */
    virtual bool next() = 0;

    /**
     * Implies tesseract::PageIteratorLevel::RIL_SYMBOL
     */
    [[nodiscard]] virtual std::unique_ptr<char[]> get_utf8_text() const = 0;
};

/**
 * Calls to Init(), End(), etc. are expected to be handled by the
 * implementations
 */
class ITesseract {
  public:
    virtual ~ITesseract() = default;

    virtual void set_image(
            std::experimental::mdspan<
                    unsigned char const,
                    std::experimental::dextents<std::size_t, 2>> image,
            int bytes_per_pixel,
            int bytes_per_line) = 0;

    virtual void recognize() = 0;

    /**
     * @return Is allowed to point to nullptr
     */
    virtual std::unique_ptr<ITesseractResultIterator> get_iterator() = 0;
};

}  // namespace asw