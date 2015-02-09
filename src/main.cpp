#include "mainwindow.h"
#include <QApplication>

#include <QDebug>

#include "data_types.h"
#include "datacontainer.h"

#define TEST

int main(int argc, char *argv[])
{
#ifndef TEST
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
#else
    std::vector<CloudPoint> v;
 v.push_back(CloudPoint(-1.09700000, 0.27380000, 134.18430000));
 v.push_back(CloudPoint(-1.09640000, 0.27460000, 134.18350000));
 v.push_back(CloudPoint(-1.09660000, 0.27550000, 134.18430000));
 v.push_back(CloudPoint(-1.09630000, 0.27640000, 134.18430000));
 v.push_back(CloudPoint(-1.09580000, 0.27720000, 134.18380000));
 v.push_back(CloudPoint(-1.09600000, 0.27820000, 134.18480000));
 v.push_back(CloudPoint(-1.09600000, 0.27910000, 134.18510000));
 v.push_back(CloudPoint(-1.09610000, 0.28010000, 134.18570000));
 v.push_back(CloudPoint(-1.09580000, 0.28090000, 134.18570000));
 v.push_back(CloudPoint(-1.09520000, 0.28180000, 134.18510000));
    qDebug() << "Hello";

    DataContainer::instance().setCloud(v);
    DataContainer::instance().showCloud();
#endif
}
