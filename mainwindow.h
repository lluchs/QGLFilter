#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class GLFilter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void selectImage();
    void saveResult();

private:
    Ui::MainWindow *ui;
    GLFilter *glfilter;
};

#endif // MAINWINDOW_H
