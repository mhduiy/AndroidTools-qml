#include "SceneRenderer.h"

#include "service/ServiceManager.h"
#include "ui/util/config.h"

#include <QDebug>
#include <QCoreApplication>

#include <QDebug>
#include <QOpenGLFramebufferObjectFormat>
#include <QVector2D>

static const GLfloat coordinate[] = {
        // Vertex coordinates, store 4 xyz coordinates
        // The coordinate range is [-1,1], and the center point is 0,0
        // 2D image z is always 0
        // How GL_TRIANGLE_STRIP is drawn：
        // Draw a triangle using the first 3 coordinates and a triangle using the last 3 coordinates, exactly a rectangle
        // x     y     z
        -1.0f,
        -1.0f,
        0.0f,
        1.0f,
        -1.0f,
        0.0f,
        -1.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,

        // Texture coordinates, store 4 xy coordinates
        // The coordinate range is [0,1], and the lower left corner is 0,0
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f
};

SceneRenderer::SceneRenderer(QObject *parent) : QObject{parent} {

}

SceneRenderer::SceneRenderer() {
    m_resourceService = ServiceManager::getInstance().resourceService();

    //connect(&qsc::IDeviceManage::getInstance(), &qsc::IDeviceManage::onNewFrame, this, &SceneRenderer::onFrame);

    initializeOpenGLFunctions();
    initialize();
    initShader();
}

void SceneRenderer::initialize() {
    initializeOpenGLFunctions();

    glDisable(GL_DEPTH_TEST);

    m_vao = new QOpenGLVertexArrayObject;
    m_vao->create();
    QOpenGLVertexArrayObject::Binder vaoBinder(m_vao);

    // Vertex buffer object initialization
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(coordinate, sizeof(coordinate));

    initShader();

    // Set background cleanup color to black
    glClearColor(0.0, 0.0, 0.0, 0.0);
    // clean up color background
    glClear(GL_COLOR_BUFFER_BIT);
}

QOpenGLFramebufferObject *SceneRenderer::createFramebufferObject(const QSize &size) {
    Q_UNUSED(size);
    QOpenGLFramebufferObjectFormat format;
    return new QOpenGLFramebufferObject(QSize(40, 90), format);
}

void SceneRenderer::synchronize(QQuickFramebufferObject *object) {
    m_window = object->window();
    //m_window->setClearBeforeRendering(false);
    m_window->setTextRenderType(QQuickWindow::NativeTextRendering);

//    if (!m_sceneProviderRenderer) {
//        m_sceneProviderRenderer = ServiceManager::getInstance().renderer();
//    }
}

void SceneRenderer::setFrameSize(const QSize &frameSize) {
    if (m_frameSize != frameSize) {
        m_frameSize = frameSize;
        m_needUpdate = true;
        // init texture immediately
        //requestUpdate();
        //update();
    }
}

void SceneRenderer::onFrame(int width, int height, uint8_t *dataY, uint8_t *dataU, uint8_t *dataV, int linesizeY, int linesizeU, int linesizeV) {
    setFrameSize(QSize(width, height));
    updateTextures(dataY, dataU, dataV, linesizeY, linesizeU, linesizeV);
}

void SceneRenderer::updateTextures(quint8 *dataY, quint8 *dataU, quint8 *dataV, quint32 linesizeY, quint32 linesizeU, quint32 linesizeV) {
    if (m_textureInited) {
        updateTexture(m_texture[0], 0, dataY, linesizeY);
        updateTexture(m_texture[1], 1, dataU, linesizeU);
        updateTexture(m_texture[2], 2, dataV, linesizeV);
        //requestUpdate();
        //update();
    }
}

void SceneRenderer::updateTexture(GLuint texture, quint32 textureType, quint8 *pixels, quint32 stride) {
    if (!pixels)
        return;

    QSize size = 0 == textureType ? m_frameSize : m_frameSize / 2;

    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, static_cast<GLint>(stride));
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.width(), size.height(), GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
}

void SceneRenderer::render() {
//    glViewport(0, 0, 100, 200);
//
//    QOpenGLVertexArrayObject::Binder vaoBinder(m_vao);
//    //m_program.bind();
//    {
//        if (m_needUpdate) {
//            deInitTextures();
//            initTextures();
//            m_needUpdate = false;
//        }
//
//        if (m_textureInited) {
//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_2D, m_texture[0]);
//
//            glActiveTexture(GL_TEXTURE1);
//            glBindTexture(GL_TEXTURE_2D, m_texture[1]);
//
//            glActiveTexture(GL_TEXTURE2);
//            glBindTexture(GL_TEXTURE_2D, m_texture[2]);
//
//            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//        }
//    }
    //m_program.release();

}

void SceneRenderer::initShader() {

    if (!m_program.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, Config::getInstance().getProjectPath() + "/res/opengl/squircle.vsh")) {
        qDebug() << "Error: " << typeid(this).name() << "Vertex shaders compilation failed";
        return;
    }
    if (!m_program.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, Config::getInstance().getProjectPath() + "/res/opengl/squircle.fsh")) {
        qDebug() << "Error: " << typeid(this).name() << "Fragment shaders compilation failed";
        return;
    }
    m_program.link();
    //m_program.bind();

    m_vertexBuffer.bind();
    // Specify the access method of vertex coordinates in vba

    // Parameter explanation: the parameter name of the vertex coordinates in the shaders, the vertex coordinates are float, the starting offset is 0,
    // the vertex coordinate type is vec3, and the stride is 3 floats
    m_program.setAttributeBuffer("vertexIn", GL_FLOAT, 0, 3, 3 * sizeof(float));
    // Enable vertex attributes
    m_program.enableAttributeArray("vertexIn");

    // Specify how texture coordinates are accessed in vbo
    // Parameter explanation: the parameter name of the texture coordinate in the shaders, the texture coordinate is float,
    // the starting offset is 12 float (skip the 12 vertex coordinates stored earlier),
    // the texture coordinate type is vec2, and the stride is 2 float
    m_program.setAttributeBuffer("textureIn", GL_FLOAT, 12 * sizeof(float), 2, 2 * sizeof(float));
    m_program.enableAttributeArray("textureIn");

    m_vertexBuffer.release();

    //Associate the texture unit in the fragment shaders with the texture unit in opengl (opengl generally provides 16 texture units)
    m_program.setUniformValue("textureY", 0);
    m_program.setUniformValue("textureU", 1);
    m_program.setUniformValue("textureV", 2);

   ;

}

void SceneRenderer::initTextures() {

    // create texture
    glGenTextures(1, &m_texture[0]);
    glBindTexture(GL_TEXTURE_2D, m_texture[0]);
    // Strategy when setting texture scaling
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Set the display strategy when the texture exceeds the coordinates in the st direction
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_frameSize.width(), m_frameSize.height(), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, nullptr);

    glGenTextures(1, &m_texture[1]);
    glBindTexture(GL_TEXTURE_2D, m_texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_frameSize.width() / 2, m_frameSize.height() / 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, nullptr);

    glGenTextures(1, &m_texture[2]);
    glBindTexture(GL_TEXTURE_2D, m_texture[2]);
    // Strategy when setting texture scaling
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Set the display strategy when the texture exceeds the coordinates in the st direction
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_frameSize.width() / 2, m_frameSize.height() / 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, nullptr);

    m_textureInited = true;
}

void SceneRenderer::deInitTextures() {

    if (QOpenGLFunctions::isInitialized(QOpenGLFunctions::d_ptr)) {
        glDeleteTextures(3, m_texture);
    }

    memset(m_texture, 0, sizeof(m_texture));
    m_textureInited = false;
}

