/**
 * 自定义按钮弹出窗口控件
 *
 * mainwidget.cpp
 * 测试主窗体。
 *
 * FlyWM_
 * GitHub: https://github.com/FlyWM
 * CSDN: https://blog.csdn.net/a844651990
 *
 */
#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "popupwidgetbutton.h"
#include "testpopupwidget.h"
#include <QMoveEvent>
#include <QDebug>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    PopupWidgetButton *pBtn = new PopupWidgetButton(PWB::Vertical, this, this);
    pBtn->move(100, 250);
    TestPopupWidget *pWidget = new TestPopupWidget(this);
    pBtn->setMainWidget(pWidget);
    pBtn->button()->setText("Vertical");

    PopupWidgetButton *pBtn1 = new PopupWidgetButton(PWB::Horizontal, this, this);
    pBtn1->move(180, 250);
    TestPopupWidget *pWidget1 = new TestPopupWidget(this);
    pBtn1->setMainWidget(pWidget1);
    pBtn1->button()->setText("Horizontal");
}

MainWidget::~MainWidget()
{
    delete ui;
}
