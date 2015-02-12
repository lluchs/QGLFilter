#ifndef GLFILTER_H
#define GLFILTER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

class GLFilter : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    GLFilter(QWidget *parent) : QOpenGLWidget(parent) { }
    ~GLFilter();

    void setImage(QImage &img);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    QOpenGLTexture *texture;
    QOpenGLShaderProgram *program;
    QOpenGLBuffer vbo;

    // Shader parameter positions
    const int SHADER_POSITION_ATTRIBUTE = 0;
};

#endif // GLFILTER_H
