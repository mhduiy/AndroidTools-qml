
#pragma once


#include "ui/util/config.h"

#include <QDebug>
#include <QCoreApplication>

#include "ui/sceneprovider/SceneProviderRenderer.h"

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QQuickFramebufferObject>
#include <QQuickWindow>

#include "service/ServiceManager.h"
#include "service/ResourceService.h"

#include "core/include/QtScrcpyCore.h"
//!
//! SceneRenderer will live on RenderThread
//!
class SceneRenderer : public QQuickFramebufferObject::Renderer, public QObject, public QOpenGLFunctions {

public:
    SceneRenderer();
    SceneRenderer(QObject *parent);

    virtual void render() override;
    virtual QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override;
    virtual void synchronize(QQuickFramebufferObject*) override;

    void setFrameSize(const QSize &frameSize);
    const QSize &frameSize();


private slots:
    void onFrame(int width, int height, uint8_t* dataY, uint8_t* dataU, uint8_t* dataV, int linesizeY, int linesizeU, int linesizeV);

private:
    void initialize();
    void initShader();
    void initTextures();
    void deInitTextures();

    void updateTextures(quint8 *dataY, quint8 *dataU, quint8 *dataV, quint32 linesizeY, quint32 linesizeU, quint32 linesizeV);
    void updateTexture(GLuint texture, quint32 textureType, quint8 *pixels, quint32 stride);

private:
    QQuickWindow* m_window = nullptr;

    ResourceService* m_resourceService = nullptr;

    // Video frame size
    QSize m_frameSize = { -1, -1 };
    bool m_needUpdate = false;
    bool m_textureInited = false;

    QOpenGLVertexArrayObject *m_vao;

    // Vertex Buffer Objects (VBO): the default is VertexBuffer (GL_ARRAY_BUFFER) type
    QOpenGLBuffer m_vertexBuffer;

    // Shader program: compile linked shaders
    QOpenGLShaderProgram m_program;

    // YUV textures for generating texture maps
    GLuint m_texture[3] = { 0 };

};
