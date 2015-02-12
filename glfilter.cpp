#include "glfilter.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLTexture>
#include <QOpenGLFramebufferObject>

GLFilter::~GLFilter()
{
    makeCurrent();
    vbo.destroy();
    delete texture;
    delete program;
    delete fbo;
    doneCurrent();
}

void GLFilter::setImage(QImage &img)
{
    makeCurrent();
    delete texture;
    texture = new QOpenGLTexture(img);

    if (fbo) delete fbo;
    fbo = new QOpenGLFramebufferObject(img.size());
    doneCurrent();
}

bool GLFilter::setShader(QString &fileName)
{
    auto shader = new QOpenGLShader(QOpenGLShader::Fragment);
    if (shader->compileSourceFile(fileName)) {
        makeCurrent();
        program->removeShader(fragmentShader);
        delete fragmentShader;
        fragmentShader = shader;
        program->addShader(fragmentShader);
        program->link();
        doneCurrent();
        return true;
    }
    return false;
}

QImage GLFilter::grabImage()
{
    makeCurrent();
    fbo->bind();
    auto size = fbo->size();
    resizeGL(size.width(), size.height());
    paintGL();
    auto img = fbo->toImage();
    fbo->release();
    doneCurrent();
    return img;
}

const char *vertexShader = R"(
in vec2 position;
out varying vec2 texpos;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    texpos = (position + vec2(1.0)) / vec2(2.0);
    texpos.y *= -1;
}
)";

const char *idShader = R"(
uniform sampler2D image;
in vec2 texpos;

void main()
{
    gl_FragColor = texture(image, texpos);
}
)";

void GLFilter::initializeGL()
{
    initializeOpenGLFunctions();

    static GLfloat coords[] = {
        -1, -1,
        -1,  1,
         1,  1,
         1, -1
    };
    vbo.create();
    vbo.bind();
    vbo.allocate(coords, sizeof(coords) * sizeof(GLfloat));

    texture = new QOpenGLTexture(QImage());

    program = new QOpenGLShaderProgram;
    program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader);
    fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fragmentShader->compileSourceCode(idShader);
    program->addShader(fragmentShader);
    program->bindAttributeLocation("position", SHADER_POSITION_ATTRIBUTE);
    program->link();
    program->bind();
    program->setUniformValue("image", 0);
}

void GLFilter::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void GLFilter::paintGL()
{
    program->enableAttributeArray(SHADER_POSITION_ATTRIBUTE);
    program->setAttributeBuffer(SHADER_POSITION_ATTRIBUTE, GL_FLOAT, 0, 2);
    texture->bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
