/**
 * 自定义按钮弹出窗口控件
 *
 * mainwidget.h
 * 测试主窗体。
 *
 * FlyWM_
 * GitHub: https://github.com/FlyWM
 * CSDN: https://blog.csdn.net/a844651990
 * Email: fwm17918@163.com
 */

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();


private:
    Ui::MainWidget *ui;
    static QPoint m_pos;
};

#endif // MAINWIDGET_H
