#ifndef QTERMWIDGETFORQML_H
#define QTERMWIDGETFORQML_H

#include <QQuickPaintedItem>
#include <qcoreevent.h>
#include <qobject.h>
#include <qquickitem.h>
#include <qtermwidget6/qtermwidget.h>
#include <qtmetamacros.h>
#include <qwidget.h>

class QTermWidgetForQml;

class TermWidget : public QTermWidget
{
    Q_OBJECT
    friend QTermWidgetForQml;
public:
    explicit TermWidget(QWidget *parent = nullptr);
signals:
    void widgetPaint(const QRect &rect);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    bool m_painting = false;
};

class QTermWidgetForQml : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit QTermWidgetForQml(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

private:
    TermWidget *m_terminal;
};

#endif // QTERMWIDGETFORQML_H