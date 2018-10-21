/**
 * 自定义按钮弹出窗口控件
 *
 * main.cpp
 *
 * FlyWM_
 * GitHub: https://github.com/FlyWM
 * CSDN: https://blog.csdn.net/a844651990
 * Email: fwm17918@163.com
 */
#include "mainwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    w.show();
    w.initPos();
    w.resize(800, 600);

    return a.exec();
}
