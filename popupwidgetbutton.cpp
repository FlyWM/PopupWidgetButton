/**
 * 自定义按钮弹出窗口控件
 *
 * popupwidgetbutton.cpp
 * 自定义按钮弹出窗口控件cpp文件。
 *
 * FlyWM_
 * GitHub: https://github.com/FlyWM
 * CSDN: https://blog.csdn.net/a844651990
 * Email: fwm17918@163.com
 */
#include "popupwidgetbutton.h"
#include "mainwidget.h"
#include <QPushButton>
#include <QLayout>
#include <QPainter>
#include <QDebug>
#include <QStyleOption>
#include <QEvent>
#include <QApplication>
#include <QMouseEvent>
#include <QLayout>

class PopupTriangleWidget : public QWidget
{
public:
    PopupTriangleWidget(PWB::WidgetOrientation orien, QWidget *parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *e);

private:
    PWB::WidgetOrientation m_orien;
};

class PopupMainWidget : public QWidget
{
public:
    PopupMainWidget(QWidget *parent = nullptr);
    void setMainWidget(QWidget *widget);

protected:
    virtual void paintEvent(QPaintEvent *e);

private:
    QHBoxLayout *m_pMainLayout;
    QWidget *m_pMainWidget;
};

class PopupWidget : public QWidget
{
public:
    explicit PopupWidget(PWB::WidgetOrientation orien, QWidget *parent = nullptr);
    void setMainWidget(QWidget *widget);

protected:
    virtual void paintEvent(QPaintEvent *e);

signals:
    void Hide();
    void Show();

private:
    PopupMainWidget *m_pMainWidget;
};


PopupTriangleWidget::PopupTriangleWidget(PWB::WidgetOrientation orien, QWidget *parent)
    : QWidget(parent),
      m_orien(orien)
{
    setFixedHeight(10);
}

void PopupTriangleWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter painter(this);
    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QColor("#548B54"));
    painter.setBrush(QBrush(QColor("#548B54")));
    QVector<QPointF> points;
    if (m_orien == PWB::Horizontal) {
        points << QPointF(0, height()/2) << QPointF(width(), 0) << QPointF(width(), height());
    } else if (m_orien == PWB::Vertical) {
        points << QPointF(0, height()) << QPointF(width()/2, 0) << QPointF(width(), height());
    }
    painter.drawPolygon(QPolygonF(points));
}

PopupMainWidget::PopupMainWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #548B54; border: 2px solid #548B54; border-radius: 10px;");
    m_pMainWidget = new QWidget(this);
    m_pMainLayout = new QHBoxLayout(this);
    m_pMainLayout->addWidget(m_pMainWidget);
    m_pMainLayout->setContentsMargins(15, 15, 15, 15);
}

void PopupMainWidget::setMainWidget(QWidget *widget)
{
    if (widget == nullptr) return;
    m_pMainWidget->deleteLater();
    m_pMainWidget = widget;
    m_pMainLayout->addWidget(m_pMainWidget);
}

void PopupMainWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter painter(this);

    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
PopupWidget::PopupWidget(PWB::WidgetOrientation orien, QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags((Qt::Dialog | Qt::FramelessWindowHint | this->windowFlags() | Qt::NoDropShadowWindowHint));
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
    PopupTriangleWidget *pTriangleWidget = new PopupTriangleWidget(orien, this);
    m_pMainWidget = new PopupMainWidget(this);
    if (orien == PWB::Horizontal) {
        pTriangleWidget->setFixedSize(12, 20);
    } else if (orien == PWB::Vertical) {
        pTriangleWidget->setFixedSize(20, 12);
    }

    pTriangleWidget->setStyleSheet("border: none;");
    m_pMainWidget->setMinimumSize(270, 210);

    if (orien == PWB::Horizontal) {
        QVBoxLayout *pLeftLayout = new QVBoxLayout();
        pLeftLayout->addSpacing(35);
        pLeftLayout->addWidget(pTriangleWidget);
        pLeftLayout->addStretch();
        QHBoxLayout *pLayout = new QHBoxLayout(this);
        pLayout->addLayout(pLeftLayout);
        pLayout->addWidget(m_pMainWidget);
//        pLayout->addStretch();
        pLayout->setContentsMargins(0, 0, 0, 0);
        pLayout->setSpacing(0);
    } else if (orien == PWB::Vertical) {
        QVBoxLayout *pLayout = new QVBoxLayout(this);
        pLayout->addWidget(pTriangleWidget, 0, Qt::AlignHCenter);
        pLayout->addWidget(m_pMainWidget);
//        pLayout->addStretch();
        pLayout->setContentsMargins(0, 0, 0, 0);
        pLayout->setSpacing(0);
    }
}

void PopupWidget::setMainWidget(QWidget *widget)
{
    if (widget == nullptr) return;
    m_pMainWidget->setMainWidget(widget);
}

void PopupWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter painter(this);

    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

QList<PopupWidget *> PopupWidgetButton::m_pWidgets;
PopupWidgetButton::PopupWidgetButton(PWB::WidgetOrientation orien, QWidget *parent)
    : QWidget(parent),
      m_orien(orien),
      m_mainWidgetClicked(false)
{
    qApp->installEventFilter(this);
    m_pButton = new QPushButton(this);
    m_pMainWidget = new PopupWidget(orien, this);
    m_pWidgets.append(m_pMainWidget);
    m_pMainWidget->show();
    m_pMainWidget->hide();
    m_pMainWidget->setObjectName("PopupMainWidget");

    connect(this, &PopupWidgetButton::ButtonClicked, [=]() {
        if (m_pMainWidget->isHidden()) {
            QPoint pos;
            if (m_orien == PWB::Horizontal) {
                pos.setX(m_pButton->mapToGlobal(QPoint(0, 0)).x() + m_pButton->width());
                pos.setY(m_pButton->mapToGlobal(QPoint(0, 0)).y() - 30);
            } else if (m_orien == PWB::Vertical) {
                pos.setX(m_pButton->mapToGlobal(QPoint(0, 0)).x() + m_pButton->width()/2 - m_pMainWidget->width()/2);
                pos.setY(m_pButton->mapToGlobal(QPoint(0, 0)).y() + m_pButton->height());
                qDebug() << "pos: " << m_pMainWidget->width();
            }
            // 同时只能显示一个 popupwidget
            foreach (auto widget, m_pWidgets) {
                if (widget != m_pMainWidget) {
                    widget->hide();
                }
            }
            m_pMainWidget->move(pos);
            m_pMainWidget->show();
        } else {
            m_pMainWidget->hide();
        }
    });
    connect(this, &PopupWidgetButton::OthersClicked, m_pMainWidget, &PopupWidget::hide);
}

PopupWidgetButton::~PopupWidgetButton()
{
    if (m_pWidgets.contains(m_pMainWidget)) {
        m_pWidgets.removeOne(m_pMainWidget);
    }
}

void PopupWidgetButton::setMainWidget(QWidget *widget)
{
    if (widget == nullptr) return;
    m_pMainWidget->setMainWidget(widget);
}

void PopupWidgetButton::setButtonObjectName(const QString &name)
{
    m_pButton->setObjectName(name);
}

bool PopupWidgetButton::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        QPoint buttonPoint = m_pButton->mapToGlobal(QPoint(0, 0));
        // 需要获取主窗口的坐标
        QPoint ePoint = e->pos() + MainWidget::getMainWidgetPos();

        /**
          * \warning
          *  Qt5中的事件传递的第一层watched并不是我们自己的主窗体Widget类或者ButtonPopupWidget类，
          *  而是它们的一个私有实现类QWidgetWindow，该类的objectName会自动加上“Window”
          */
        if (watched->objectName() == "MainWidgetWindow") {
            m_mainWidgetClicked = false;
            // 点击按钮
            if (ePoint.x() >= buttonPoint.x() && ePoint.x() <= buttonPoint.x() + m_pButton->width()
                    && ePoint.y() >= buttonPoint.y() && ePoint.y() <= buttonPoint.y() + m_pButton->height()) {
                emit ButtonClicked();
                return true;
            }
        }
        // 点击弹出的widget
        else if (watched->objectName() == "PopupMainWidgetWindow") {
            m_mainWidgetClicked =  true;
            return QWidget::eventFilter(watched, event);
        }

        if (!m_pMainWidget->isHidden() && !m_mainWidgetClicked) {
            emit OthersClicked();
        }

    }
    // 点击标题栏
    else if (event->type() == QEvent::NonClientAreaMouseButtonRelease ||
               event->type() == QEvent::NonClientAreaMouseButtonPress ||
               event->type() == QEvent::NonClientAreaMouseButtonDblClick) {
        emit OthersClicked();
    }
    return QObject::eventFilter(watched, event);
}
