#include "widget.h"
#include <QApplication>
#include <cstdlib>

int main(int argc, char *argv[])
{
    srand(time(NULL));

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
