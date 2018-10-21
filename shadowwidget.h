#ifndef SHADOWWIDGET_H
#define SHADOWWIDGET_H

#include <QWidget>

class QVBoxLayout;
class ShadowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShadowWidget(QWidget *parent = nullptr);

    void setMainWidget(QWidget *widget);

signals:

public slots:


private:
    QWidget *m_pMainWidget;
    QVBoxLayout *m_pMainLayout;
};

#endif // SHADOWWIDGET_H
