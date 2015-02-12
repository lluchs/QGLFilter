#include "glfilter.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

GLFilter::~GLFilter()
{
    makeCurrent();
    vbo.destroy();
    delete texture;
    delete program;
    doneCurrent();
}

void GLFilter::setImage(QImage &img)
{
    delete texture;
    texture = new QOpenGLTexture(img);
}

const char *vertexShader = R"(
in vec2 position;
out varying vec2 texpos;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    texpos = (position + vec2(1.0)) / vec2(2.0);
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
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shader/filter.frag");
    program->bindAttributeLocation("position", SHADER_POSITION_ATTRIBUTE);
    program->link();
    program->bind();
    program->setUniformValue("image", 0);
}

void GLFilter::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void GLFilter::paintGL()
{
    program->enableAttributeArray(SHADER_POSITION_ATTRIBUTE);
    program->setAttributeBuffer(SHADER_POSITION_ATTRIBUTE, GL_FLOAT, 0, 2);
    texture->bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
