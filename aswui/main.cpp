#include <QApplication>
#include <asw/PlayingField.h>
#include <aswui/ConstCellSpanWidgetQt.h>

int main(int argc, char** argv) {
    QApplication application{argc, argv};
    asw::MineCellArray<8, 8> mines;
    {
        using enum asw::MineCell;
        mines(1, 1) = Mine;
        mines(6, 3) = Mine;
        mines(7, 7) = Mine;
    }
    asw::PlayingField field{mines.cspan()};
    aswui::ConstCellSpanWidgetQt widget{field.cspan(), nullptr};
    widget.show();
    QObject::connect(
            &widget,
            &aswui::ConstCellSpanWidgetQt::clicked,
            [&field, &widget](int const row, int const column) {
                field.reveal(row, column);
                widget.set(field.cspan());
            });
    return QApplication::exec();
}