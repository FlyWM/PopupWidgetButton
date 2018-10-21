/**
 * 自定义按钮弹出窗口控件
 *
 * testpopupwidget.cpp
 * 测试用例，自定义弹出的窗体。
 *
 * FlyWM_
 * GitHub: https://github.com/FlyWM
 * CSDN: https://blog.csdn.net/a844651990
 *
 */

#include "testpopupwidget.h"
#include <QStyleOption>
#include <QPainter>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

TestPopupWidget::TestPopupWidget(QWidget *parent)
    : QWidget(parent)
{
    QLabel *pLabel = new QLabel("This is a popup widget...", this);
    QHBoxLayout *pMainLayout = new QHBoxLayout(this);
    pLabel->setStyleSheet("QLabel { color: black; }");

    pMainLayout->addWidget(pLabel);
}

void TestPopupWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter painter(this);

    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
