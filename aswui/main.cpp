#include <QApplication>
#include <QHBoxLayout>
#include <QSpinBox>
#include <asw/InMemoryPlayingField.h>
#include <aswui/ConstCellSpanWidgetQt.h>
#include <aswui/MinePredictionsWidgetQt.h>
#include <gsl/narrow>

namespace {

gsl::not_null<std::unique_ptr<QWidget>>
build_widget(asw::InMemoryPlayingField& field) {
    auto widget = std::make_unique<QWidget>();
    auto* const rows_widget = new QSpinBox{widget.get()};
    rows_widget->setMinimum(1);
    rows_widget->setValue(gsl::narrow_cast<int>(field.rows()));
    auto* const columns_widget = new QSpinBox{widget.get()};
    columns_widget->setValue(gsl::narrow_cast<int>(field.columns()));
    columns_widget->setMinimum(1);
    auto* const mines_widget = new QSpinBox{widget.get()};
    mines_widget->setValue(field.mine_count());
    columns_widget->setMinimum(1);
    auto* const field_widget =
            new aswui::ConstCellSpanWidgetQt{field.cspan(), nullptr};
    auto* const predictions_widget = new aswui::MinePredictionsWidgetQt{
            asw::predict_mines_field(field.cspan()).cspan(), nullptr};
    auto* const main_layout = new QVBoxLayout{widget.get()};
    auto* const top_layout = new QHBoxLayout{};
    main_layout->addLayout(top_layout);
    top_layout->addWidget(rows_widget);
    top_layout->addWidget(columns_widget);
    top_layout->addWidget(mines_widget);
    auto* const lower_layout = new QHBoxLayout{};
    main_layout->addLayout(lower_layout);
    lower_layout->addWidget(field_widget);
    lower_layout->addWidget(predictions_widget);
    auto const refresh = [&field, field_widget, predictions_widget]() {
        field_widget->set(field.cspan());
        predictions_widget->set(
                asw::predict_mines_field(field.cspan()).cspan());
    };
    auto const recalculate = [&field,
                              refresh](int const row, int const column) {
        field.reveal(row, column);
        refresh();
    };
    auto const reinitialize_field = [&field, refresh](
                                            std::size_t const rows,
                                            std::size_t const columns,
                                            int const mines) {
        field = asw::InMemoryPlayingField{
                asw::generate_random_mines(rows, columns, mines).cspan()};
        refresh();
    };
    QObject::connect(
            rows_widget,
            &QSpinBox::valueChanged,
            [&field, reinitialize_field](int const rows) {
                reinitialize_field(
                        gsl::narrow_cast<std::size_t>(rows),
                        field.columns(),
                        field.mine_count());
            });
    QObject::connect(
            columns_widget,
            &QSpinBox::valueChanged,
            [&field, reinitialize_field](int const columns) {
                reinitialize_field(
                        field.rows(),
                        gsl::narrow_cast<std::size_t>(columns),
                        field.mine_count());
            });
    QObject::connect(
            mines_widget,
            &QSpinBox::valueChanged,
            [&field, reinitialize_field](int const mines) {
                reinitialize_field(field.rows(), field.columns(), mines);
            });
    QObject::connect(
            field_widget, &aswui::ConstCellSpanWidgetQt::clicked, recalculate);
    QObject::connect(
            predictions_widget,
            &aswui::MinePredictionsWidgetQt::clicked,
            recalculate);
    return widget;
}

}  // namespace

int main(int argc, char** argv) {
    asw::InMemoryPlayingField field{
            asw::generate_random_mines(5, 5, 5).cspan()};
    QApplication application{argc, argv};
    auto main_widget = build_widget(field);
    main_widget->show();
    return QApplication::exec();
}