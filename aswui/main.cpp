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
    asw::PlayingField playing_field{mines.cspan()};
    aswui::ConstCellSpanWidgetQt widget{playing_field.cspan(), nullptr};
    widget.show();
    return QApplication::exec();
}