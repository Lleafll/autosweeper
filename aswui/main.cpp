#include <QApplication>
#include <QHBoxLayout>
#include <asw/PlayingField.h>
#include <aswui/ConstCellSpanWidgetQt.h>
#include <aswui/MinePredictionsWidgetQt.h>

int main(int argc, char** argv) {
    asw::MineCellArray<8, 8> mines;
    {
        using enum asw::MineCell;
        mines(1, 1) = Mine;
        mines(6, 3) = Mine;
        mines(7, 7) = Mine;
    }
    asw::PlayingField field{mines.cspan()};
    QApplication application{argc, argv};
    QWidget main_widget;
    aswui::ConstCellSpanWidgetQt field_widget{field.cspan(), nullptr};
    aswui::MinePredictionsWidgetQt predictions_widget{
            asw::predict_mines_field(field.cspan()).cspan(), nullptr};
    QHBoxLayout layout{&main_widget};
    layout.addWidget(&field_widget);
    layout.addWidget(&predictions_widget);
    QObject::connect(
            &field_widget,
            &aswui::ConstCellSpanWidgetQt::clicked,
            [&field, &field_widget, &predictions_widget](
                    int const row, int const column) {
                field.reveal(row, column);
                field_widget.set(field.cspan());
                predictions_widget.set(
                        asw::predict_mines_field(field.cspan()).cspan());
            });
    main_widget.show();
    return QApplication::exec();
}