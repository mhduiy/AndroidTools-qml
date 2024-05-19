#include "SceneProviderRenderer.h"

#include "service/ServiceManager.h"
#include "ui/util/config.h"

#include <QDebug>
#include <QCoreApplication>

struct VertexData3D {
    QVector3D vertex;
    QVector2D texCoord;
    QVector3D normal;
};

Q_DECLARE_TYPEINFO(VertexData3D, Q_PRIMITIVE_TYPE);

namespace {
    constexpr float FOV = 45.f;
    constexpr float NearPlane = 0.1f;
    constexpr float FarPlane = 100.f;
} // namespace


SceneProviderRenderer::SceneProviderRenderer(QObject *parent) : QObject{parent} {
    initializeOpenGLFunctions();
    connect(&qsc::IDeviceManage::getInstance(), &qsc::IDeviceManage::onNewFrame, this, &SceneProviderRenderer::onFrame);
}

SceneProviderRenderer::~SceneProviderRenderer() {
    cleanup();
    m_vertexBuffer->destroy();
}

QOpenGLFramebufferObject *SceneProviderRenderer::scene() const {
    return m_scene;
}

void SceneProviderRenderer::init(QQuickWindow *window, const QSize &resolution) {
    m_window = window;

    m_resourceService = ServiceManager::getInstance().resourceService();
    m_resourceService->setFrameSize(QSize(480, 900));

    setupRendering();

    initShader();

    if (m_scene) {
        if (m_scene->isBound()) {
            m_scene->release();
        }
        delete m_scene;
        m_scene = nullptr;
    }

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::Attachment::Depth);
    format.setMipmap(true);
    m_scene = new QOpenGLFramebufferObject(resolution, format);

    m_program->bind();
    m_matrixProjection.setToIdentity();
    m_matrixProjection.perspective(FOV,
                                   static_cast<float>(resolution.width()) / static_cast<float>(resolution.height()),
                                   NearPlane,
                                   FarPlane);
    m_program->setUniformValue("projection", m_matrixProjection);
    m_program->release();

    ServiceManager::getInstance().addRenderer(this);
}

void SceneProviderRenderer::setupRendering() {

    m_texture_photo = new QOpenGLTexture(QImage("/home/mahdi/CLionProjects/QtWs2022/opengl/2.jpg"));

    m_vao = new QOpenGLVertexArrayObject();

    m_program = new QOpenGLShaderProgram(this);

    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
    if (!m_vertexBuffer->create()) {
        qFatal("Couldn't create Vertex Buffer");
    }

    const QVector<VertexData3D> cubeVertices{
            {{-1, -1, -1}, {0, 0}, {0,  0,  -1}}, // VertexCoord(3), TextureCoord(2), Normal(3)
            {{1,  -1, -1}, {1, 0}, {0,  0,  -1}},
            {{1,  1,  -1}, {1, 1}, {0,  0,  -1}},
            {{-1, 1,  -1}, {0, 1}, {0,  0,  -1}},
            //
            {{-1, -1, 1},  {0, 0}, {0,  0,  1}},
            {{1,  -1, 1},  {1, 0}, {0,  0,  1}},
            {{1,  1,  1},  {1, 1}, {0,  0,  1}},
            {{-1, 1,  1},  {0, 1}, {0,  0,  1}},
            //
            {{-1, 1,  1},  {1, 0}, {-1, 0,  0}},
            {{-1, 1,  -1}, {1, 1}, {-1, 0,  0}},
            {{-1, -1, -1}, {0, 1}, {-1, 0,  0}},
            {{-1, -1, 1},  {0, 0}, {-1, 0,  0}},
            //
            {{1,  1,  1},  {1, 0}, {1,  0,  0}},
            {{1,  1,  -1}, {1, 1}, {1,  0,  0}},
            {{1,  -1, -1}, {0, 1}, {1,  0,  0}},
            {{1,  -1, 1},  {0, 0}, {1,  0,  0}},
            //
            {{-1, -1, -1}, {0, 1}, {0,  -1, 0}},
            {{1,  -1, -1}, {1, 1}, {0,  -1, 0}},
            {{1,  -1, 1},  {1, 0}, {0,  -1, 0}},
            {{-1, -1, 1},  {0, 0}, {0,  -1, 0}},
            //
            {{-1, 1,  -1}, {0, 1}, {0,  1,  0}},
            {{1,  1,  -1}, {1, 1}, {0,  1,  0}},
            {{1,  1,  1},  {1, 0}, {0,  1,  0}},
            {{-1, 1,  1},  {0, 0}, {0,  1,  0}},
    };

    m_vertexBuffer->bind();
    m_vertexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexBuffer->allocate(cubeVertices.constData(), cubeVertices.size() * static_cast<int>(sizeof(VertexData3D)));

    m_elementBuffer = new QOpenGLBuffer(QOpenGLBuffer::Type::IndexBuffer);
    if (!m_elementBuffer->create()) {
        qFatal("Couldn't create Element Buffer");
    }
    const QVector<unsigned int> quadIndices = QVector<unsigned int>{
            0, 1, 2, 2, 3, 0,  //
            4, 5, 6, 6, 7, 4,  //
            8, 9, 10, 10, 11, 8,  //
            12, 13, 14, 14, 15, 12, //
            16, 17, 18, 18, 19, 16, //
            20, 21, 22, 22, 23, 20, //
    };
    m_elementBuffer->bind();
    m_elementBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_elementBuffer->allocate(quadIndices.constData(), quadIndices.size() * static_cast<int>(sizeof(unsigned int)));

    if (!m_program->create()) {
        qFatal("Couldn't create Shader Program");
    }
    if (!m_program->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "/home/mahdi/CLionProjects/QtWs2022/opengl/cube.vsh")) {
        qFatal("Vertex shaders compilation failed");
    }
    if (!m_program->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "/home/mahdi/CLionProjects/QtWs2022/opengl/cube.fsh")) {
        qFatal("Fragment shaders compilation failed");
    }

    if (!m_program->link()) {
        qFatal("Couldn't link shaders program");
    }

    m_program->bind();

    m_program->setUniformValue("texture", 0);
    m_matrixProjection.perspective(FOV, 1.0f, NearPlane, FarPlane);

    constexpr float viewZ = -4.5;
    m_matrixView.lookAt({0, 0, viewZ}, {0, 0, 0}, {0, 1, 0});

    m_program->setUniformValue("objectColor", 1, 1, 1);
    m_program->setUniformValue("lightColor", 1, 1, 1);
    m_program->setUniformValue("lightPos", 0, 0, viewZ * 4);
    m_program->setUniformValue("viewPos", 0, 0, viewZ);

    m_program->setUniformValue("projection", m_matrixProjection);
    m_program->setUniformValue("model", m_matrixModel);
    m_program->setUniformValue("view", m_matrixView);
    m_program->setUniformValue("trans_inv_model", m_matrixModel.normalMatrix());

    QOpenGLVertexArrayObject::Binder binder(m_vao);
    m_vertexBuffer->bind();
    m_program->enableAttributeArray("verCoord");
    m_program->setAttributeBuffer("verCoord", GL_FLOAT, 0, 3, sizeof(VertexData3D));

    m_program->enableAttributeArray("texCoord");
    m_program->setAttributeBuffer("texCoord", GL_FLOAT, sizeof(QVector3D), 2, sizeof(VertexData3D));

    m_program->enableAttributeArray("normal");
    m_program->setAttributeBuffer("normal", GL_FLOAT, sizeof(QVector2D) + sizeof(QVector3D), 3, sizeof(VertexData3D));

    m_elementBuffer->bind();

    m_program->release();
}

void SceneProviderRenderer::synchronize() {
    m_program->bind();

    m_matrixModel.setToIdentity();
    QQuaternion quaternion = QQuaternion::fromEulerAngles(static_cast<float>(0),
                                                          static_cast<float>(0),
                                                          static_cast<float>(0));
    quaternion.normalize();
    m_matrixModel.rotate(quaternion);
    m_program->setUniformValue("model", m_matrixModel);
    m_program->setUniformValue("trans_inv_model", m_matrixModel.normalMatrix());

    m_program->release();
}

void SceneProviderRenderer::render() {

    if (m_resourceService->getMirror() == false) {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return;
    }

    if (!m_scene || !m_program) {
        return;
    }

    m_scene->bind();
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        QOpenGLVertexArrayObject::Binder binder(m_vao);

        glViewport(0, 0, 400, 900);

        if (m_needUpdate) {
            deInitTextures();
            initTextures();
            m_needUpdate = false;
        }

        m_program->bind();

        if (m_textureInited) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_texture[0]);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_texture[1]);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, m_texture[2]);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //m_texture_photo->bind();
        //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 512, 512, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_texture_photo);
        m_program->release();

    }
    m_scene->release();
}

void SceneProviderRenderer::setFrameSize(const QSize &frameSize) {
    if (m_frameSize != frameSize) {
        m_frameSize = frameSize;
        m_needUpdate = true;
        // init texture immediately
//        render();
    }
}

void SceneProviderRenderer::onFrame(int width, int height, uint8_t *dataY, uint8_t *dataU, uint8_t *dataV, int linesizeY, int linesizeU, int linesizeV) {
    if (m_resourceService->getMirror() == false) {
        return;
    }

    setFrameSize(QSize(width, height));

    if (m_resourceService->frameSize().width() != width) {
        m_resourceService->setFrameSize(QSize(width, height));
    }

    updateTextures(dataY, dataU, dataV, linesizeY, linesizeU, linesizeV);
}

void SceneProviderRenderer::updateTextures(quint8 *dataY, quint8 *dataU, quint8 *dataV, quint32 linesizeY, quint32 linesizeU, quint32 linesizeV) {
    if (m_textureInited) {
        updateTexture(m_texture[0], 0, dataY, linesizeY);
        updateTexture(m_texture[1], 1, dataU, linesizeU);
        updateTexture(m_texture[2], 2, dataV, linesizeV);
    }
}

void SceneProviderRenderer::updateTexture(GLuint texture, quint32 textureType, quint8 *pixels, quint32 stride) {
    if (!pixels)
        return;

    QSize size = (textureType == 0 ? m_frameSize : m_frameSize);

    m_scene->bind();
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, static_cast<GLint>(stride));
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.width(), size.height(), GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
    }
}

void SceneProviderRenderer::initShader() {
    m_program = new QOpenGLShaderProgram(this);

    if (!m_program->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, Config::getInstance().getProjectPath() + "/res/opengl/squircle.vsh")) {
        qDebug() << "Error: " << typeid(this).name() << "Vertex shaders compilation failed";
        return;
    }
    if (!m_program->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, Config::getInstance().getProjectPath() + "/res/opengl/squircle.fsh")) {
        qDebug() << "Error: " << typeid(this).name() << "Fragment shaders compilation failed";
        return;
    }

    m_program->link();
    m_program->bind();

    // Specify the access method of vertex coordinates in vba

    QOpenGLVertexArrayObject::Binder binder(m_vao);
    m_vertexBuffer->bind();
    m_program->enableAttributeArray("verCoord");
    m_program->setAttributeBuffer("verCoord", GL_FLOAT, 0, 3, sizeof(VertexData3D));

    m_program->enableAttributeArray("texCoord");
    m_program->setAttributeBuffer("texCoord", GL_FLOAT, sizeof(QVector3D), 2, sizeof(VertexData3D));

    m_program->enableAttributeArray("normal");
    m_program->setAttributeBuffer("normal", GL_FLOAT, sizeof(QVector2D) + sizeof(QVector3D), 3, sizeof(VertexData3D));


    // Parameter explanation: the parameter name of the vertex coordinates in the shaders, the vertex coordinates are float, the starting offset is 0,
    // the vertex coordinate type is vec3, and the stride is 3 floats
    m_program->setAttributeBuffer("vertexIn", GL_FLOAT, 0, 3, 3 * sizeof(float));
    // Enable vertex attributes
    m_program->enableAttributeArray("vertexIn");


    // Specify how texture coordinates are accessed in vbo
    // Parameter explanation: the parameter name of the texture coordinate in the shaders, the texture coordinate is float,
    // the starting offset is 12 float (skip the 12 vertex coordinates stored earlier),
    // the texture coordinate type is vec2, and the stride is 2 float
    m_program->setAttributeBuffer("textureIn", GL_FLOAT, 12 * sizeof(float), 2, 2 * sizeof(float));
    m_program->enableAttributeArray("textureIn");

    //Associate the texture unit in the fragment shaders with the texture unit in opengl (opengl generally provides 16 texture units)
    m_program->setUniformValue("textureY", 0);
    m_program->setUniformValue("textureU", 1);
    m_program->setUniformValue("textureV", 2);

    m_program->release();
}

void SceneProviderRenderer::initTextures() {

    m_scene->bind();
    {
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

    }
    m_textureInited = true;
}

void SceneProviderRenderer::deInitTextures() {

    if (QOpenGLFunctions::isInitialized(QOpenGLFunctions::d_ptr)) {
        glDeleteTextures(3, m_texture);
    }

    memset(m_texture, 0, sizeof(m_texture));
    m_textureInited = false;
}

void SceneProviderRenderer::cleanup() {
    if (m_program) {
        m_program->release();
        m_program->deleteLater();
        m_program = nullptr;
    }

    if (m_texture_photo) {
        if (m_texture_photo->isBound()) {
            m_texture_photo->release();
        }
        if (m_texture_photo->isCreated()) {
            m_texture_photo->destroy();
        }
        delete m_texture_photo;
        m_texture_photo = nullptr;
    }

    if (m_vao) {
        if (m_vao->isCreated()) {
            m_vao->release();
            m_vao->destroy();
        }
        delete m_vao;
        m_vao = nullptr;
    }

    if (m_elementBuffer) {
        if (m_elementBuffer->isCreated()) {
            m_elementBuffer->release();
            m_elementBuffer->destroy();
        }
        delete m_elementBuffer;
        m_elementBuffer = nullptr;
    }

    if (m_vertexBuffer) {
        if (m_vertexBuffer->isCreated()) {
            m_vertexBuffer->release();
            m_vertexBuffer->destroy();
        }
        delete m_vertexBuffer;
        m_vertexBuffer = nullptr;
    }
}




