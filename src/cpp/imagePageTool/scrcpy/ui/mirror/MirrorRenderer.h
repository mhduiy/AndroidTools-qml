#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QQuickFramebufferObject>
#include <memory>

#include "core/include/QtScrcpyCore.h"
#include "service/ResourceService.h"

const static int cBufferSize = 1920 * 768;

struct YUVData
{
    YUVData()
    {
        Y.reserve(cBufferSize);
        U.reserve(cBufferSize);
        V.reserve(cBufferSize);
    }
    QByteArray Y;
    QByteArray U;
    QByteArray V;
    int yLineSize;
    int uLineSize;
    int vLineSize;
    int height;
};
Q_DECLARE_METATYPE(YUVData)

class MirrorRenderer : public  QObject, public QOpenGLFunctions
{
public:
    MirrorRenderer(QObject *parent = nullptr);
    MirrorRenderer();
    ~MirrorRenderer();
    void init();
    void paint();
    void resize(int width, int height);
    void updateTextureInfo(int width, int height, int format);
    void updateTextureData(const YUVData& data);

protected:
    void initTexture();
    void initShader();
    void initGeometry();

public slots:
    void onFrame(int width, int height, uint8_t* dataY, uint8_t* dataU, uint8_t* dataV, int linesizeY, int linesizeU, int linesizeV);

private:
    static void safeDeleteTexture(QOpenGLTexture *texture) ;

private:
    QOpenGLShaderProgram mProgram;
    QOpenGLTexture* mTexY = nullptr;
    QOpenGLTexture* mTexU = nullptr;
    QOpenGLTexture* mTexV = nullptr;
    QVector<QVector3D> mVertices;
    QVector<QVector2D> mTexcoords;
    int mModelMatHandle, mViewMatHandle, mProjectMatHandle;
    int mVerticesHandle;
    int mTexCoordHandle;

    QMatrix4x4 mModelMatrix;
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mProjectionMatrix;       
    GLint mPixFmt = 0;
    bool mTextureAlloced = false;

    int m_videoWidth = 864;
    int m_videoHeight = 1920;

    ResourceService* m_resourceService = nullptr;
};