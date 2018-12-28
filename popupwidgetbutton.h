/**
 * 自定义按钮弹出窗口控件
 *
 * popupwidgetbutton.h
 * 自定义按钮弹出窗口控件头文件。
 *
 * FlyWM_
 * GitHub: https://github.com/FlyWM
 * CSDN: https://blog.csdn.net/a844651990
 *
 */

#ifndef POPUPWIDGETBUTTON_H
#define POPUPWIDGETBUTTON_H

#include <QWidget>
#include <QPushButton>

namespace PWB {
enum WidgetOrientation {
    Horizontal,
    Vertical,
};
}

class QPushButton;
class PopupWidget;
class PopupWidgetButton : public QWidget
{
    Q_OBJECT
public:
    explicit PopupWidgetButton(PWB::WidgetOrientation orien, QWidget *rootWindow, QWidget *parent = nullptr);
    ~PopupWidgetButton();

    /**
     * @brief setMainWidget
     *  设置弹出的widget
     * @param widget
     */
    void setMainWidget(QWidget *widget);
    /**
     * @brief setButtonObjectName
     *  设置按钮 objectname，方便设置样式表
     * @param name
     */
    void setButtonObjectName(const QString &name);

    /**
     * @brief button
     *  方便设置按钮的一些属性
     * @return
     */
    QPushButton *button() { return m_pButton; }

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);

signals:
    void ButtonClicked();
    void OthersClicked();

public slots:

private:
    QPushButton *m_pButton;
    PWB::WidgetOrientation m_orien;
    PopupWidget *m_pMainWidget;
    bool m_mainWidgetClicked;
    QWidget *m_pRootWindow;
    static QList<PopupWidget *> m_pWidgets;
};

#endif // POPUPWIDGETBUTTON_H
