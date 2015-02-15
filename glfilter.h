#ifndef GLFILTER_H
#define GLFILTER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLShader)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)
QT_FORWARD_DECLARE_CLASS(QOpenGLFramebufferObject)

class GLFilter : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    GLFilter(QWidget *parent) : QOpenGLWidget(parent) { }
    ~GLFilter();

    void setImage(QImage &img);
    bool setShader(QString &fileName);
    QImage grabImage();
    void setUniforms(QMap<QString, float> &uniforms);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    QOpenGLTexture *texture;
    QOpenGLShaderProgram *program;
    QOpenGLShader *fragmentShader;
    QOpenGLFramebufferObject *fbo;
    QOpenGLBuffer vbo;

    QMap<QString, float> uniforms;

    // Shader parameter positions
    const int SHADER_POSITION_ATTRIBUTE = 0;
};

#endif // GLFILTER_H
