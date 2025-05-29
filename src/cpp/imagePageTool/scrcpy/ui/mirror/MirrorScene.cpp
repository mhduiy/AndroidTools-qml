#include "MirrorScene.h"
#include <QImage>
#include <QTimerEvent>
#include <QMouseEvent>
#include <QDebug>
#include "service/ServiceManager.h"
#include <QBuffer>
#include <QElapsedTimer>
#include <QtConcurrent/QtConcurrent>

MirrorScene::MirrorScene(QQuickItem *parent)
    : QQuickItem(parent), m_image()
{
    m_resourceService = ServiceManager::getInstance().resourceService();
    connect(m_resourceService, &ResourceService::cppGenerateEvents, this, &MirrorScene::cppGenerateEvents);
    connect(&qsc::IDeviceManage::getInstance(), &qsc::IDeviceManage::onNewFrame, this, &MirrorScene::onYuvFrame);
    setAcceptedMouseButtons(Qt::AllButtons);
}

void MirrorScene::declareQml() {
    qmlRegisterType<MirrorScene>("MirrorScene", 1, 0, "MirrorScene");
}

QImage MirrorScene::image() const {
    return m_image;
}

void MirrorScene::onYuvFrame(int width, int height, uint8_t *dataY, uint8_t *dataU, uint8_t *dataV, int linesizeY, int linesizeU, int linesizeV) {
    // YUV420P格式转换为QImage，使用Qt原生方法
    
    // 创建RGB32格式的图像
    QImage img(width, height, QImage::Format_RGB32);
    
    // 定义行处理函数
    auto processLine = [&](int y) {
        QRgb *rgbLine = reinterpret_cast<QRgb*>(img.scanLine(y));
        const uint8_t *yLine = dataY + y * linesizeY;
        
        // 对于U和V，由于是4:2:0格式，所以行和列都是Y的一半
        const uint8_t *uLine = dataU + (y >> 1) * linesizeU;
        const uint8_t *vLine = dataV + (y >> 1) * linesizeV;
        
        for (int x = 0; x < width; x++) {
            // 预先计算UV索引，避免重复计算
            const int uvIndex = x >> 1;
            
            // 获取YUV值
            const int Y = yLine[x];
            const int U = uLine[uvIndex];
            const int V = vLine[uvIndex];
            
            // YUV到RGB转换公式（BT.601标准）
            // 优化计算，减少中间变量
            const int C = Y - 16;
            const int D = U - 128;
            const int E = V - 128;
            
            // 直接计算并限制在0-255范围内
            const int R = qBound(0, (298 * C + 409 * E + 128) >> 8, 255);
            const int G = qBound(0, (298 * C - 100 * D - 208 * E + 128) >> 8, 255);
            const int B = qBound(0, (298 * C + 516 * D + 128) >> 8, 255);
            
            // 直接设置RGB值
            rgbLine[x] = qRgb(R, G, B);
        }
    };
    
    // 使用QtConcurrent并行处理所有行
    QVector<int> lines;
    for (int y = 0; y < height; y++) {
        lines.append(y);
    }
    QtConcurrent::blockingMap(lines, processLine);
    
    m_image = img;

    emit imageChanged();
}

void MirrorScene::timerEvent(QTimerEvent *event) {
    Q_UNUSED(event);
}

void MirrorScene::mouseProcess(QMouseEvent *event) {
    auto device = qsc::IDeviceManage::getInstance().getDevice(m_resourceService->serial());
    if (!device) {
        return;
    }
    if (m_resourceService->orientation() == 0) {
        emit device->mouseEvent(event, m_resourceService->frameSize(), m_resourceService->portraitSize());
    } else {
        emit device->mouseEvent(event, m_resourceService->frameSize(), m_resourceService->landscapeSize());
    }
}

void MirrorScene::mousePressEvent(QMouseEvent *event) {
    mouseProcess(event);
}

void MirrorScene::mouseReleaseEvent(QMouseEvent *event) {
    mouseProcess(event);
}

void MirrorScene::mouseMoveEvent(QMouseEvent *event) {
    mouseProcess(event);
}

void MirrorScene::mouseDoubleClickEvent(QMouseEvent *event) {
    mouseProcess(event);
}

void MirrorScene::wheelEvent(QWheelEvent *event) {
    // 这里可根据需要实现wheel事件处理
}