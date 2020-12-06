#include "vkapi.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    vkapi w;
    w.show();
    return a.exec();
}
