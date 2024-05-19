#pragma once

#include <QQuickView>

#include "core/include/QtScrcpyCore.h"
#include "core/include/adbprocess.h"

#include "service/ResourceService.h"
#include "service/WebSocketService.h"
#include "service/udp/Broadcast.h"

class MainWindow : public QQuickView {

public:
    MainWindow(QQuickView *parent = nullptr);
    ~MainWindow();

    Q_INVOKABLE void test(QString name);


protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:

    ResourceService *resourceService = nullptr;
    Broadcast *m_broadcast = nullptr;
};

