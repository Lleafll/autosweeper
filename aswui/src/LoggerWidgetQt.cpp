#include "LoggerWidgetQt.h"
#include <QLabel>
#include <QPixmap>
#include <asw/algorithm2d.h>
#include <gsl/narrow>

namespace aswui {

namespace {

QPixmap to_pixmap(asw::Image const& image) {
    auto const rows = image.cspan().extent(0);
    auto const columns = image.cspan().extent(1);
    QImage qimage{
            gsl::narrow_cast<int>(columns),
            gsl::narrow_cast<int>(rows),
            QImage::Format_RGB32};
    asw::indexed_for_each(
            image.cspan(),
            [&qimage](asw::Position const& position, asw::Color const& color) {
                qimage.setPixelColor(
                        gsl::narrow_cast<int>(position.column),
                        gsl::narrow_cast<int>(position.row),
                        {color.red, color.green, color.green});
            });
    return QPixmap::fromImage(qimage);
}

}  // namespace

void LoggerWidgetQt::log_image(
        std::string_view const message,
        asw::Image const& image) {
    auto* const widget = new QLabel{this};
    widget->setPixmap(to_pixmap(image));
    addTab(widget, QString::fromStdString(std::string{message}));
}

}  // namespace aswui