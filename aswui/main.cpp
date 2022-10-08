#include <QApplication>
#include <aswui/PlayingFieldWidgetQt.h>

int main(int argc, char** argv) {
    QApplication application{argc, argv};
    aswui::PlayingFieldWidgetQt widget{nullptr};
    widget.show();
    return QApplication::exec();
}