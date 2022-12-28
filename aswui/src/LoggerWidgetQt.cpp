#include "LoggerWidgetQt.h"
#include <QLabel>
#include <QPixmap>
#include <asw/algorithm2d.h>
#include <gsl/narrow>

namespace aswui {

namespace {

QPixmap to_pixmap(asw::Image const& image) {
    QImage qimage{
            gsl::narrow_cast<int>(image.extent(1)),
            gsl::narrow_cast<int>(image.extent(0)),
            QImage::Format_RGB888};
    asw::indexed_for_each(
            image,
            [&qimage](asw::Position const& position, asw::Color const& color) {
                qimage.setPixelColor(
                        gsl::narrow_cast<int>(position.column),
                        gsl::narrow_cast<int>(position.row),
                        {color.red, color.green, color.blue});
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