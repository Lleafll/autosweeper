#pragma once

#include <QWidget>
#include <memory>
#include <gsl/pointers>

namespace aswui {

class PlayingFieldWidgetQt final : public QWidget {
    Q_OBJECT

public:
    explicit PlayingFieldWidgetQt(QWidget* parent);

    ~PlayingFieldWidgetQt() override;

private:
    class Impl;

    gsl::not_null<std::unique_ptr<Impl>> impl_;
};

}  // namespace aswui