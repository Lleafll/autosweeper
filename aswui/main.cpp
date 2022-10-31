#include <QApplication>
#include <QHBoxLayout>
#include <asw/InMemoryPlayingField.h>
#include <aswui/ConstCellSpanWidgetQt.h>
#include <aswui/MinePredictionsWidgetQt.h>

int main(int argc, char** argv) {
    asw::InMemoryPlayingField field{asw::generate_random_mines(5, 5, 5).cspan()};
    QApplication application{argc, argv};
    QWidget main_widget;
    aswui::ConstCellSpanWidgetQt field_widget{field.cspan(), nullptr};
    aswui::MinePredictionsWidgetQt predictions_widget{
            asw::predict_mines_field(field.cspan()).cspan(), nullptr};
    QHBoxLayout layout{&main_widget};
    layout.addWidget(&field_widget);
    layout.addWidget(&predictions_widget);
    auto recalculate = [&field, &field_widget, &predictions_widget](
                               int const row, int const column) {
        field.reveal(row, column);
        field_widget.set(field.cspan());
        predictions_widget.set(asw::predict_mines_field(field.cspan()).cspan());
    };
    QObject::connect(
            &field_widget, &aswui::ConstCellSpanWidgetQt::clicked, recalculate);
    QObject::connect(
            &predictions_widget,
            &aswui::MinePredictionsWidgetQt::clicked,
            recalculate);
    main_widget.show();
    return QApplication::exec();
}