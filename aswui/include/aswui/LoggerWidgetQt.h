#pragma once

#include <QTabWidget>
#include <asw/Image.h>

namespace aswui {

class LoggerWidgetQt final : public QTabWidget {
    Q_OBJECT
  public:
    using QTabWidget::QTabWidget;

    ~LoggerWidgetQt() override = default;

    void log_image(std::string_view message, asw::Image const& image);
};

}  // namespace aswui