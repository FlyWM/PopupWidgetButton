/**
 * 自定义按钮弹出窗口控件
 *
 * testpopupwidget.h
 * 测试用例，自定义弹出的窗体。
 *
 * FlyWM_
 * GitHub: https://github.com/FlyWM
 * CSDN: https://blog.csdn.net/a844651990
 *
 */

#ifndef TESTPOPUPWIDGET_H
#define TESTPOPUPWIDGET_H

#include <QWidget>

class TestPopupWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TestPopupWidget(QWidget *parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *e);

signals:

public slots:
};

#endif // TESTPOPUPWIDGET_H
