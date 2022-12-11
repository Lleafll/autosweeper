#pragma once

#include <experimental/mdspan>
#include <memory>
#include <vector>

namespace asw {

class ITesseractResultIterator {
  public:
    virtual ~ITesseractResultIterator() = default;

    /**
     * Implies tesseract::PageIteratorLevel::RIL_SYMBOL
     */
    virtual bool next() = 0;

    /**
     * Implies tesseract::PageIteratorLevel::RIL_SYMBOL
     */
    [[nodiscard]] virtual std::string get_utf8_text() const = 0;
};

struct ImageInfo {
    int bytes_per_pixel;
    int bytes_per_line;
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
                    std::experimental::dextents<size_t, 2>> const& image,
            ImageInfo const& info) = 0;

    virtual bool recognize() = 0;

    /**
     * @return Is allowed to point to nullptr
     */
    virtual std::unique_ptr<ITesseractResultIterator> get_iterator() = 0;
};

}  // namespace asw