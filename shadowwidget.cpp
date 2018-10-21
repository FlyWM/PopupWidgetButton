#include "shadowwidget.h"
#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>

ShadowWidget::ShadowWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(150, 150);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
//    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    m_pMainLayout = new QVBoxLayout(this);
    m_pMainWidget = new QWidget(this);
    m_pMainWidget->setStyleSheet(" background-color: #fafafa; border-radius: 10px;");
    m_pMainLayout->addWidget(m_pMainWidget);
    m_pMainLayout->setContentsMargins(8, 8, 8, 8);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    //设置阴影颜色
    shadow->setColor(QColor("#000000"));
    //设置阴影圆角
    shadow->setBlurRadius(30);
    //给嵌套QWidget设置阴影
    m_pMainWidget->setGraphicsEffect(shadow);
    //给垂直布局器设置边距(此步很重要, 设置宽度为阴影的宽度)
    m_pMainLayout->setMargin(24);
}

void ShadowWidget::setMainWidget(QWidget *widget)
{

}
