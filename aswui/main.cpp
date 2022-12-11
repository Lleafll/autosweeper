#include <QApplication>
#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QShortcut>
#include <QSpinBox>
#include <asw/InMemoryPlayingField.h>
#include <asw/ScreenDetectionPlayingField.h>
#include <aswui/ConstCellSpanWidgetQt.h>
#include <aswui/MinePredictionsWidgetQt.h>
#include <gsl/narrow>

namespace {

constexpr asw::Size field_size{5, 5};
constexpr int mine_count = 5;

gsl::not_null<std::unique_ptr<QWidget>>
build_widget(std::unique_ptr<asw::PlayingField>& field) {
    auto widget = std::make_unique<QWidget>();
    auto* const field_selection = new QComboBox{widget.get()};
    field_selection->addItems({u"InMemory"_qs, u"Desktop"_qs});
    auto* const rows_widget = new QSpinBox{widget.get()};
    rows_widget->setMinimum(1);
    rows_widget->setValue(gsl::narrow_cast<int>(field->rows()));
    auto* const columns_widget = new QSpinBox{widget.get()};
    columns_widget->setValue(gsl::narrow_cast<int>(field->columns()));
    columns_widget->setMinimum(1);
    auto* const mines_widget = new QSpinBox{widget.get()};
    mines_widget->setValue(field->mine_count());
    mines_widget->setMinimum(1);
    auto* const auto_solver_button = new QPushButton{u"Next"_qs, widget.get()};
    auto* const field_widget =
            new aswui::ConstCellSpanWidgetQt{field->cspan(), nullptr};
    auto* const predictions_widget = new aswui::MinePredictionsWidgetQt{
            asw::predict_mines_field(field->cspan()).cspan(), nullptr};
    auto* const main_layout = new QVBoxLayout{widget.get()};
    auto* const top_layout = new QHBoxLayout;
    main_layout->addLayout(top_layout);
    top_layout->addWidget(field_selection);
    top_layout->addWidget(rows_widget);
    top_layout->addWidget(columns_widget);
    top_layout->addWidget(mines_widget);
    top_layout->addWidget(auto_solver_button);
    auto* const lower_layout = new QHBoxLayout;
    main_layout->addLayout(lower_layout);
    lower_layout->addWidget(field_widget);
    lower_layout->addWidget(predictions_widget);
    auto const refresh = [&field, field_widget, predictions_widget]() {
        field_widget->set(field->cspan());
        predictions_widget->set(
                asw::predict_mines_field(field->cspan()).cspan());
    };
    auto const recalculate = [&field,
                              refresh](int const row, int const column) {
        field->reveal(
                {.row = gsl::narrow_cast<size_t>(row),
                 .column = gsl::narrow_cast<size_t>(column)});
        refresh();
    };
    auto const reinitialize_field = [&field,
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
            field = std::make_unique<asw::InMemoryPlayingField>(
                    asw::generate_random_mines(size, mines).cspan());
        } else {
            field = std::make_unique<asw::ScreenDetectionPlayingField>(size);
        }
        refresh();
    };
    auto const auto_solve = [&field, refresh] {
        auto const prediction = asw::predict_mines_field(field->cspan());
        for (size_t row = 0; row < field->rows(); ++row) {
            for (size_t column = 0; column < field->columns(); ++column) {
                if (prediction(row, column) == asw::Prediction::Safe) {
                    field->reveal({row, column});
                    refresh();
                    return;
                }
            }
        }
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
            field_widget, &aswui::ConstCellSpanWidgetQt::clicked, recalculate);
    QObject::connect(
            predictions_widget,
            &aswui::MinePredictionsWidgetQt::clicked,
            recalculate);
    QObject::connect(auto_solver_button, &QPushButton::clicked, auto_solve);
    return widget;
}

}  // namespace

int main(int argc, char** argv) {
    std::unique_ptr<asw::PlayingField> field =
            std::make_unique<asw::InMemoryPlayingField>(
                    asw::generate_random_mines(field_size, mine_count).cspan());
    QApplication const application{argc, argv};
    auto main_widget = build_widget(field);
    main_widget->show();
    return QApplication::exec();
}