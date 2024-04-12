#include "QtViewer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtViewer w;
    w.show();
    return a.exec();
}
