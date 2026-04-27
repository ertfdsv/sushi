#include "mainwindow.h"
#include "SushiItem.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    qDebug() << "\n===== 食材类测试 =====";
    
    Rice rice;
    qDebug() << "\n1. 米饭:";
    rice.showInfo();
    
    Salmon salmon;
    qDebug() << "\n2. 三文鱼:";
    salmon.showInfo();
    
    SushiItem *item1 = new Rice();
    SushiItem *item2 = new Salmon();
    qDebug() << "\n3. 多态测试:";
    item1->showInfo();
    item2->showInfo();
    
    delete item1;
    delete item2;
    
    qDebug() << "\n===== 测试结束 =====";
    
    MainWindow w;
    w.show();
    
    return a.exec();
}
