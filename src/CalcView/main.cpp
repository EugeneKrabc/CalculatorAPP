#include "CalcView.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CalcModel model;
    CalcControler controler(&model);
    CalcView w(&controler);
    w.show();
    return a.exec();
}
