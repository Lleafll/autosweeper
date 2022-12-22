#pragma once

#include <QTabWidget>
#include <asw/Logger.h>

namespace aswui {

class LoggerWidgetQt final : public QTabWidget, public asw::Logger {
    Q_OBJECT
  public:
    using QTabWidget::QTabWidget;

    ~LoggerWidgetQt() override = default;

    void log_image(std::string_view message, asw::Image const& image) override;
};

}  // namespace aswui