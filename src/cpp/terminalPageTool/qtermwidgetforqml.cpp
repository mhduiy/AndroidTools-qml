#include "qtermwidgetforqml.h"
#include <QEvent>
#include <QLoggingCategory>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QTimer>
#include <qlogging.h>
#include <qquickitem.h>
#include <qtermwidget6/qtermwidget.h>
#include <qwidget.h>

#define Paint_FPS 30

TermWidget::TermWidget(QWidget *parent) : QTermWidget(parent) {
    setAttribute(Qt::WA_NativeWindow, true);
    setAttribute(Qt::WA_DontCreateNativeAncestors, true);
}

void TermWidget::paintEvent(QPaintEvent *e) {
    QTermWidget::paintEvent(e);
}

QTermWidgetForQml::QTermWidgetForQml(QQuickItem *parent)
: QQuickPaintedItem(parent)
, m_terminal(new TermWidget())
{
    m_terminal->setTerminalOpacity(0);
    QFont font;
    font.setFamilies({"Source Code Pro"});
    m_terminal->setTerminalFont(font);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        m_terminal->sendText("date\n");
        update(boundingRect().toRect());
    });
    timer->start(1000 / Paint_FPS);
}

void QTermWidgetForQml::paint(QPainter *painter) {
    m_terminal->render(painter, QPoint(), QRegion(), QWidget::RenderFlag::DrawChildren);
}
