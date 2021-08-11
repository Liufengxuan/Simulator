#include "mainform.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainForm w;
    w.parm_argc=argc;
    w.parm_argv=argv;
    w.show();




    return a.exec();
}
