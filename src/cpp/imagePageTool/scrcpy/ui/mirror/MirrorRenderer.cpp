
#include <QOpenGLPixelTransferOptions>

#include "MirrorRenderer.h"
#include "libavcodec/avcodec.h"
#include "service/ServiceManager.h"
#include "ui/util/config.h"

MirrorRenderer::MirrorRenderer(QObject *parent) {
    Q_UNUSED(parent);
    m_resourceService = ServiceManager::getInstance().resourceService();
    // connect(&qsc::IDeviceManage::getInstance(), &qsc::IDeviceManage::onNewFrame, this, &MirrorRenderer::onFrame);
}

MirrorRenderer::MirrorRenderer() {

}

MirrorRenderer::~MirrorRenderer() {
    qWarning() << __FUNCTION__;
    safeDeleteTexture(mTexY);
    safeDeleteTexture(mTexU);
    safeDeleteTexture(mTexV);
}

void MirrorRenderer::onFrame(int width, int height, uint8_t *dataY, uint8_t *dataU, uint8_t *dataV, int linesizeY, int linesizeU, int linesizeV) {

//    qDebug() << width;
//    qDebug() << height;

    if (width != m_videoWidth) {
        m_videoWidth = width;
        m_videoHeight = height;
        resize(width, height);
        initTexture();
        updateTextureInfo(m_videoWidth, m_videoHeight, AV_PIX_FMT_YUV420P);
        m_resourceService->setFrameSize(QSize(width, height));
    }

    YUVData m_yuvData;

    m_yuvData.Y.resize(linesizeY * height);
    memcpy(m_yuvData.Y.data(), dataY, static_cast<size_t>(m_yuvData.Y.size()));

    m_yuvData.U.resize(linesizeU * height / 2);
    memcpy(m_yuvData.U.data(), dataU, static_cast<size_t>(m_yuvData.U.size()));

    m_yuvData.V.resize(linesizeV * height / 2);
    memcpy(m_yuvData.V.data(), dataV, static_cast<size_t>(m_yuvData.V.size()));

    m_yuvData.height = height;
    m_yuvData.yLineSize = linesizeY;
    m_yuvData.uLineSize = linesizeU;
    m_yuvData.vLineSize = linesizeV;

    updateTextureData(m_yuvData);
}

void MirrorRenderer::init() {
    qWarning() << __FUNCTION__;

    initializeOpenGLFunctions();
    glDepthMask(GL_TRUE);
    glEnable(GL_TEXTURE_2D);

    initShader();
    initTexture();
    initGeometry();

    updateTextureInfo(m_videoWidth, m_videoHeight, AV_PIX_FMT_YUV420P);
    m_resourceService->setFrameSize(QSize(m_videoWidth, m_videoHeight));
}

void MirrorRenderer::resize(int width, int height) {
    //qWarning() << __FUNCTION__;
    glViewport(0, 0, width, height);
    float bottom = -1.0f;
    float top = 1.0f;
    float n = 1.0f;
    float f = 100.0f;
    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.frustum(-1.0, 1.0, bottom, top, n, f);
}

void MirrorRenderer::initShader() {
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/res/shaders/vertex.vsh")) {
        qDebug() << " add vertex shaders file failed.";
        return;
    }
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/res/shaders/fragment.fsh")) {
        qDebug() << " add fragment shaders file failed.";
        return;
    }
    mProgram.bindAttributeLocation("qt_Vertex", 0);
    mProgram.bindAttributeLocation("texCoord", 1);
    mProgram.link();
    mProgram.bind();
}

void MirrorRenderer::initTexture() {
    // yuv420p
    mTexY = new QOpenGLTexture(QOpenGLTexture::Target2D);
    mTexY->setFormat(QOpenGLTexture::LuminanceFormat);
    //mTexY->setFixedSamplePositions(true);
    mTexY->setMinificationFilter(QOpenGLTexture::Nearest);
    mTexY->setMagnificationFilter(QOpenGLTexture::Nearest);
    mTexY->setWrapMode(QOpenGLTexture::ClampToEdge);

    mTexU = new QOpenGLTexture(QOpenGLTexture::Target2D);
    mTexU->setFormat(QOpenGLTexture::LuminanceFormat);
    //mTexU->setFixedSamplePositions(true);
    mTexU->setMinificationFilter(QOpenGLTexture::Nearest);
    mTexU->setMagnificationFilter(QOpenGLTexture::Nearest);
    mTexU->setWrapMode(QOpenGLTexture::ClampToEdge);

    mTexV = new QOpenGLTexture(QOpenGLTexture::Target2D);
    mTexV->setFormat(QOpenGLTexture::LuminanceFormat);
    //mTexV->setFixedSamplePositions(true);
    mTexV->setMinificationFilter(QOpenGLTexture::Nearest);
    mTexV->setMagnificationFilter(QOpenGLTexture::Nearest);
    mTexV->setWrapMode(QOpenGLTexture::ClampToEdge);
}

void MirrorRenderer::initGeometry() {
    mVertices << QVector3D(-1, 1, 0.0f) << QVector3D(1, 1, 0.0f) << QVector3D(1, -1, 0.0f) << QVector3D(-1, -1, 0.0f);
    mTexcoords << QVector2D(0, 1) << QVector2D(1, 1) << QVector2D(1, 0) << QVector2D(0, 0);

    mViewMatrix.setToIdentity();
    mViewMatrix.lookAt(QVector3D(0.0f, 0.0f, 1.001f), QVector3D(0.0f, 0.0f, -5.0f), QVector3D(0.0f, 1.0f, 0.0f));
    mModelMatrix.setToIdentity();
}

void MirrorRenderer::updateTextureInfo(int width, int height, int format) {

    if (format == AV_PIX_FMT_YUV420P) {
        // yuv420p
        mTexY->setSize(width, height);
        mTexY->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt8);

        mTexU->setSize(width / 2, height / 2);
        mTexU->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt8);

        mTexV->setSize(width / 2, height / 2);
        mTexV->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt8);
    } else {
        // Process according to yuv444p first
        mTexY->setSize(width, height);
        mTexY->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt8);

        mTexU->setSize(width, height);
        mTexU->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt8);

        mTexV->setSize(width, height);
        mTexV->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt8);
    }
    mTextureAlloced = true;
}

void MirrorRenderer::updateTextureData(const YUVData &data) {
    if (data.Y.size() <= 0) {
        qWarning() << "y array is empty";
        return;
    }
    if (data.U.size() <= 0) {
        qWarning() << "u array is empty";
        return;
    }
    if (data.V.size() <= 0) {
        qWarning() << "v array is empty";
        return;
    }
    QOpenGLPixelTransferOptions options;
    options.setImageHeight(data.height);
    options.setRowLength(data.yLineSize);
    mTexY->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, data.Y.data(), &options);
    options.setRowLength(data.uLineSize);
    mTexU->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, data.U.data(), &options);
    options.setRowLength(data.vLineSize);
    mTexV->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, data.V.data(), &options);
}

void MirrorRenderer::paint() {

    glDepthMask(true);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!mTextureAlloced) {
        return;
    }
    mProgram.bind();

    mModelMatHandle = mProgram.uniformLocation("u_modelMatrix");
    mViewMatHandle = mProgram.uniformLocation("u_viewMatrix");
    mProjectMatHandle = mProgram.uniformLocation("u_projectMatrix");
    mVerticesHandle = mProgram.attributeLocation("qt_Vertex");
    mTexCoordHandle = mProgram.attributeLocation("texCoord");
    //vertex
    mProgram.enableAttributeArray(mVerticesHandle);
    mProgram.setAttributeArray(mVerticesHandle, mVertices.constData());

    //texture coordinates
    mProgram.enableAttributeArray(mTexCoordHandle);
    mProgram.setAttributeArray(mTexCoordHandle, mTexcoords.constData());

    // MVP Matrix
    mProgram.setUniformValue(mModelMatHandle, mModelMatrix);
    mProgram.setUniformValue(mViewMatHandle, mViewMatrix);
    mProgram.setUniformValue(mProjectMatHandle, mProjectionMatrix);

    // pixFmt
    mProgram.setUniformValue("pixFmt", mPixFmt);

    //texture
    // Y
    glActiveTexture(GL_TEXTURE0);
    mTexY->bind();

    // U
    glActiveTexture(GL_TEXTURE1);
    mTexU->bind();

    // V
    glActiveTexture(GL_TEXTURE2);
    mTexV->bind();

    mProgram.setUniformValue("tex_y", 0);
    mProgram.setUniformValue("tex_u", 1);
    mProgram.setUniformValue("tex_v", 2);

    glDrawArrays(GL_TRIANGLE_FAN, 0, mVertices.size());

    mProgram.disableAttributeArray(mVerticesHandle);
    mProgram.disableAttributeArray(mTexCoordHandle);
    mProgram.release();
}

void MirrorRenderer::safeDeleteTexture(QOpenGLTexture *texture) {
    if (texture) {
        if (texture->isBound()) {
            texture->release();
        }
        if (texture->isCreated()) {
            texture->destroy();
        }
        delete texture;
        texture = nullptr;
    }
}