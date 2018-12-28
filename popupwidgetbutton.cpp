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
#include "shadowwidget.h"
#include <QPushButton>
#include <QLayout>
#include <QPainter>
#include <QDebug>
#include <QStyleOption>
#include <QEvent>
#include <QApplication>
#include <QMouseEvent>
#include <QLayout>
#include <QGraphicsDropShadowEffect>
#include <QDialog>
#include <QWindow>
#include <QMessageBox>
#include "popupwidgetbutton.h"
#include "mainwidget.h"
#include "shadowwidget.h"

class PopupTriangleWidget : public QWidget
{
public:
    PopupTriangleWidget(PWB::WidgetOrientation orien, QWidget *parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *e);

private:
    PWB::WidgetOrientation m_orien;
};

/**
 * @brief The PopupMainWidget class
 *  unused
 */
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

/**
 * @brief The PopupWidgetContent class
 *  unused
 */
class PopupWidgetContent : public QWidget
{
public:
    explicit PopupWidgetContent(PWB::WidgetOrientation orien, QWidget *parent = nullptr);

private:
    QWidget *m_pMainWidget;
};

class PopupWidget : public QWidget
{
public:
    explicit PopupWidget(PWB::WidgetOrientation orien, QWidget *parent = nullptr);
    void setCentralWidget(QWidget *widget);

protected:
    virtual void paintEvent(QPaintEvent *e);

signals:
    void Hide();
    void Show();

private:


private:
    QWidget *m_pMainWidget;
    QVBoxLayout *m_pLeftLayout;
    QHBoxLayout *m_pHLayout;
    QVBoxLayout *m_pVLayout;
    QHBoxLayout *m_pCentralLayout;
    QWidget *m_pCentralWidget;
    PWB::WidgetOrientation m_orien;
    QGraphicsDropShadowEffect *m_pShadow;
    PopupTriangleWidget *m_pTriangleWidget;
    int m_shadowWidth;
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
    QPainterPath path;

    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QColor("lightgray"));
    painter.setBrush(QBrush(QColor("#fafafa")));
    QVector<QPointF> points;
    if (m_orien == PWB::Horizontal) {
        points << QPointF(width(), height()) << QPointF(0, height()/2) << QPointF(width(), 0);
        QPolygonF polyF(points);
        path.addPolygon(polyF);
        painter.drawPath(path);
    } else if (m_orien == PWB::Vertical) {
        points << QPointF(0, height()) << QPointF(width()/2, 0) << QPointF(width(), height());
        QPolygonF polyF(points);
        path.addPolygon(polyF);
        painter.drawPath(path);
    }
}

PopupMainWidget::PopupMainWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #548B54; border: 2px solid #548B54; border-radius: 15px;");
    m_pMainWidget = new QWidget(this);
    m_pMainLayout = new QHBoxLayout(this);
    m_pMainLayout->addWidget(m_pMainWidget);
    m_pMainLayout->setContentsMargins(0, 0, 0, 0);
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

PopupWidgetContent::PopupWidgetContent(PWB::WidgetOrientation orien, QWidget *parent)
    : QWidget(parent),
      m_pMainWidget(new QWidget(this))
{
    PopupTriangleWidget *pTriangleWidget = new PopupTriangleWidget(orien, this);

    if (orien == PWB::Horizontal) {
        pTriangleWidget->setFixedSize(12, 20);
    } else if (orien == PWB::Vertical) {
        pTriangleWidget->setFixedSize(20, 12);
    }

    pTriangleWidget->setStyleSheet("border: none;");
    m_pMainWidget->setMinimumSize(270, 210);
    m_pMainWidget->setStyleSheet("background-color: red;");

    if (orien == PWB::Horizontal) {
        QVBoxLayout *pLeftLayout = new QVBoxLayout();
        pLeftLayout->addSpacing(35);
        pLeftLayout->addWidget(pTriangleWidget);
        pLeftLayout->addStretch();
        QHBoxLayout *pLayout = new QHBoxLayout(this);
        pLayout->addLayout(pLeftLayout);
        pLayout->addWidget(m_pMainWidget);
        pLayout->setContentsMargins(0, 0, 0, 0);
        pLayout->setSpacing(0);
    } else if (orien == PWB::Vertical) {
        QVBoxLayout *pLayout = new QVBoxLayout(this);
        pLayout->addWidget(pTriangleWidget, 0, Qt::AlignHCenter);
        pLayout->addWidget(m_pMainWidget);
        pLayout->setContentsMargins(0, 0, 0, 0);
        pLayout->setSpacing(0);
    }
}

PopupWidget::PopupWidget(PWB::WidgetOrientation orien, QWidget *parent)
    : QWidget(parent),
      m_orien(orien),
      m_shadowWidth(4)
{
    setWindowFlags((Qt::Dialog | Qt::FramelessWindowHint | this->windowFlags() /*| Qt::NoDropShadowWindowHint*/));
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
    setStyleSheet(" /*background-color: red;*/ border-radius: 10px;");
    m_pShadow = new QGraphicsDropShadowEffect(this);
    m_pShadow->setOffset(0, 0);
    m_pShadow->setColor(QColor("#cccccc"));
    m_pShadow->setBlurRadius(10);
    m_pMainWidget = new QWidget(this);
    m_pCentralWidget = new QWidget(m_pMainWidget);
    m_pCentralWidget->setStyleSheet("QWidget { border: none; }");
    m_pCentralLayout = new QHBoxLayout(m_pMainWidget);
    m_pCentralLayout->addWidget(m_pCentralWidget);
    m_pMainWidget->setStyleSheet(" border: 1px solid lightgray; background-color: #fafafa; border-radius: 10px;");
    m_pMainWidget->setGraphicsEffect(m_pShadow);
    m_pTriangleWidget = new PopupTriangleWidget(m_orien, this);

    if (m_orien == PWB::Horizontal) {
        m_pTriangleWidget->setFixedSize(12, 20);
    } else if (m_orien == PWB::Vertical) {
        m_pTriangleWidget->setFixedSize(20, 12);
    }

    m_pTriangleWidget->setStyleSheet("border: none;");
    m_pMainWidget->setMinimumSize(270, 210);

    if (m_orien == PWB::Horizontal) {
        m_pHLayout = new QHBoxLayout(this);
        m_pHLayout->addSpacing(m_shadowWidth + 12);
        m_pHLayout->addWidget(m_pMainWidget);
        m_pHLayout->setSpacing(0);
        m_pHLayout->setContentsMargins(m_shadowWidth, m_shadowWidth, m_shadowWidth, m_shadowWidth);
        m_pTriangleWidget->move(m_shadowWidth + 5, 40);
#if 0
        m_pLeftLayout = new QVBoxLayout();
        m_pLeftLayout->addSpacing(35);
        m_pLeftLayout->addWidget(m_pTriangleWidget);
        m_pLeftLayout->addStretch();
        m_pHLayout = new QHBoxLayout(this);
        m_pHLayout->addLayout(m_pLeftLayout);
        m_pHLayout->addWidget(m_pMainWidget);
        m_pHLayout->setContentsMargins(0, 0, 0, 0);
        m_pHLayout->setSpacing(0);
        m_pHLayout->setMargin(m_shadowWidth);
#endif
    } else if (m_orien == PWB::Vertical) {
        m_pVLayout = new QVBoxLayout(this);
        m_pVLayout->addSpacing(m_shadowWidth + 12);
        m_pVLayout->addWidget(m_pMainWidget);
        m_pVLayout->setSpacing(0);
        m_pVLayout->setContentsMargins(m_shadowWidth, m_shadowWidth, m_shadowWidth, m_shadowWidth);
        m_pTriangleWidget->move(m_pMainWidget->width()/2-10 + m_shadowWidth, m_shadowWidth + 5);
#if 0
        m_pVLayout = new QVBoxLayout(this);
        m_pVLayout->addWidget(m_pTriangleWidget, 0, Qt::AlignHCenter);
        m_pVLayout->addWidget(m_pMainWidget);
        m_pVLayout->setContentsMargins(0, 0, 0, 0);
        m_pVLayout->setSpacing(0);
        m_pVLayout->setMargin(m_shadowWidth);
#endif
    }
}

void PopupWidget::setCentralWidget(QWidget *widget)
{
    if (widget == nullptr) return;

    m_pCentralLayout->removeWidget(m_pCentralWidget);
    m_pCentralWidget = widget;
    m_pCentralWidget->setStyleSheet("QWidget { border: none; }");
    m_pCentralLayout->addWidget(widget);
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
PopupWidgetButton::PopupWidgetButton(PWB::WidgetOrientation orien, QWidget *rootWindow, QWidget *parent)
    : QWidget(parent)
    , m_orien(orien)
    , m_mainWidgetClicked(false)
    , m_pRootWindow(rootWindow)
{
    QList<QWidget *> topWidgets = qApp->topLevelWidgets();
    if (!topWidgets.contains(rootWindow) || !qobject_cast<QWidget *>(rootWindow))
        QMessageBox::critical(nullptr, tr("Warning!"), tr("The root window is not a topleavel window!"));

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
                pos.setY(m_pButton->mapToGlobal(QPoint(0, 0)).y() - 40);
            } else if (m_orien == PWB::Vertical) {
                pos.setX(m_pButton->mapToGlobal(QPoint(0, 0)).x() + m_pButton->width()/2 - m_pMainWidget->width()/2);
                pos.setY(m_pButton->mapToGlobal(QPoint(0, 0)).y() + m_pButton->height());
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
    m_pMainWidget->setCentralWidget(widget);
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
        QPoint ePoint = e->pos() + m_pRootWindow->mapToGlobal(QPoint(0, 0));

        /**
          * \warning
          *  主窗口的事件先传递给它一个私有实现类QWidgetWindow，该类的objectName会自动加上“Window”
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


