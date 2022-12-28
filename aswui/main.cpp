#include <QApplication>
#include <QComboBox>
#include <QHBoxLayout>
#include <QPointer>
#include <QPushButton>
#include <QShortcut>
#include <QSpinBox>
#include <asw/ImageMatchingPlayingField.h>
#include <asw/InMemoryPlayingField.h>
#include <asw/Logger.h>
#include <asw/algorithm2d.h>
#include <aswui/CellConstSpanWidgetQt.h>
#include <aswui/LoggerWidgetQt.h>
#include <aswui/MinePredictionsWidgetQt.h>
#include <gsl/narrow>

namespace {

constexpr asw::Size field_size{5, 5};
constexpr int mine_count = 5;

gsl::not_null<std::unique_ptr<QWidget>> build_widget(
        pro::proxy<asw::PlayingField>& field,
        pro::proxy<asw::Logger>& logger) {
    auto widget = std::make_unique<QWidget>();
    auto* const field_selection = new QComboBox{widget.get()};
    field_selection->addItems({u"InMemory"_qs, u"Desktop"_qs});
    auto* const rows_widget = new QSpinBox{widget.get()};
    rows_widget->setMinimum(1);
    auto const size = field.invoke<asw::GetSize>();
    rows_widget->setValue(gsl::narrow_cast<int>(size.rows));
    auto* const columns_widget = new QSpinBox{widget.get()};
    columns_widget->setValue(gsl::narrow_cast<int>(size.columns));
    columns_widget->setMinimum(1);
    auto* const mines_widget = new QSpinBox{widget.get()};
    mines_widget->setValue(mine_count);
    mines_widget->setMinimum(1);
    auto* const restart_button = new QPushButton{u"Restart"_qs, widget.get()};
    auto* const auto_solver_button = new QPushButton{u"Next"_qs, widget.get()};
    auto const field_span = field.invoke<asw::Cspan>();
    auto* const field_widget =
            new aswui::CellConstSpanWidgetQt{field_span, nullptr};
    auto* const predictions_widget = new aswui::MinePredictionsWidgetQt{
            asw::predict_mines_field(field_span).cspan(), nullptr};
    auto* const main_layout = new QVBoxLayout{widget.get()};
    auto* const top_layout = new QHBoxLayout;
    main_layout->addLayout(top_layout);
    top_layout->addWidget(field_selection);
    top_layout->addWidget(rows_widget);
    top_layout->addWidget(columns_widget);
    top_layout->addWidget(mines_widget);
    top_layout->addWidget(restart_button);
    top_layout->addWidget(auto_solver_button);
    auto* const lower_layout = new QHBoxLayout;
    main_layout->addLayout(lower_layout);
    lower_layout->addWidget(field_widget);
    lower_layout->addWidget(predictions_widget);
    auto const refresh = [&field, field_widget, predictions_widget]() {
        field.invoke<asw::Update>();
        auto const field_span = field.invoke<asw::Cspan>();
        field_widget->set(field_span);
        predictions_widget->set(asw::predict_mines_field(field_span).cspan());
    };
    auto const recalculate = [&field,
                              refresh](int const row, int const column) {
        field.invoke<asw::Reveal>(asw::Position{
                .row = gsl::narrow_cast<size_t>(row),
                .column = gsl::narrow_cast<size_t>(column)});
        refresh();
    };
    auto const reinitialize_field = [&field,
                                     &logger,
                                     field_selection,
                                     rows_widget,
                                     columns_widget,
                                     mines_widget,
                                     refresh]() {
        auto const field_type = field_selection->currentIndex();
        auto const rows = rows_widget->value();
        auto const columns = columns_widget->value();
        asw::Size const size{
                .rows = gsl::narrow_cast<size_t>(rows),
                .columns = gsl::narrow_cast<size_t>(columns)};
        auto const mines = mines_widget->value();
        if (field_type == 0) {
            field = pro::
                    make_proxy<asw::PlayingField, asw::InMemoryPlayingField>(
                            asw::generate_random_mines(size, mines).cspan());
        } else {
            field = pro::make_proxy<
                    asw::PlayingField,
                    asw::ImageMatchingPlayingField<
                            asw::MinesweeperScreen,
                            asw::Matcher>>(
                    asw::MinesweeperScreen{logger}, asw::Matcher{logger}, 16);
        }
        refresh();
    };
    auto const auto_solve = [&field, refresh] {
        auto const prediction =
                asw::predict_mines_field(field.invoke<asw::Cspan>());
        auto safe_clicked = false;
        asw::indexed_for_each(
                prediction,
                [&safe_clicked, &field](
                        asw::Position const& position,
                        asw::Prediction const prediction) {
                    if (prediction == asw::Prediction::Safe) {
                        field.invoke<asw::Reveal>(position);
                        safe_clicked = true;
                    }
                });
        if (not safe_clicked) {
            auto break_out = false;
            auto const size = field.invoke<asw::GetSize>();
            for (size_t row = 0; row < size.rows; ++row) {
                for (size_t column = 0; column < size.columns; ++column) {
                    if (prediction(row, column) == asw::Prediction::Unknown) {
                        field.invoke<asw::Reveal>(asw::Position{row, column});
                        break_out = true;
                        break;
                    }
                }
                if (break_out) {
                    break;
                }
            }
        }
        refresh();
    };
    new QShortcut{QKeySequence{Qt::Key_Space}, widget.get(), auto_solve};
    QObject::connect(
            field_selection,
            &QComboBox::currentIndexChanged,
            reinitialize_field);
    QObject::connect(rows_widget, &QSpinBox::valueChanged, reinitialize_field);
    QObject::connect(
            columns_widget, &QSpinBox::valueChanged, reinitialize_field);
    QObject::connect(mines_widget, &QSpinBox::valueChanged, reinitialize_field);
    QObject::connect(
            field_widget, &aswui::CellConstSpanWidgetQt::clicked, recalculate);
    QObject::connect(
            predictions_widget,
            &aswui::MinePredictionsWidgetQt::clicked,
            recalculate);
    QObject::connect(restart_button, &QPushButton::clicked, reinitialize_field);
    QObject::connect(auto_solver_button, &QPushButton::clicked, auto_solve);
    return widget;
}

}  // namespace

int main(int argc, char** argv) {
    QApplication const application{argc, argv};
#ifdef _DEBUG
    aswui::LoggerWidgetQt logger_widget;
    logger_widget.setWindowFlag(Qt::Window);
    logger_widget.show();
    pro::proxy<asw::Logger> logger{&logger_widget};
#else
    auto logger = pro::make_proxy<asw::Logger, asw::NullLogger>();
#endif
    auto field = pro::make_proxy<asw::PlayingField, asw::InMemoryPlayingField>(
            asw::generate_random_mines(field_size, mine_count).cspan());
    auto main_widget = build_widget(field, logger);
    main_widget->show();
    return QApplication::exec();
}