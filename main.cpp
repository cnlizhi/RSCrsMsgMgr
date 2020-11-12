//Remote Sensing Courses Message Manager Program

#include "QRSCMMWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QRSCMMWindow RSCMMWindow;
    RSCMMWindow.show();
    return app.exec();
}
