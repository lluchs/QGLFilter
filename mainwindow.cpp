#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glfilter.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    glfilter = new GLFilter(this);
    ui->container->addWidget(glfilter);

    connect(ui->selectImageButton, SIGNAL(clicked()), this, SLOT(selectImage()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveResult()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectImage()
{
    auto fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.png *.jpg *.bmp)");

    QImage image(fileName);
    if (image.isNull())
        QMessageBox::critical(this, "Loading Error", "Could not load image file.");
    else
        ui->statusBar->showMessage(fileName);

    glfilter->setImage(image);
}

void MainWindow::saveResult()
{
    QImage result = glfilter->grabFramebuffer();
    auto fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "Image Files (*.png *.jpg *.bmp)");
    if (result.save(fileName)) {
        ui->statusBar->showMessage(QString("Saved to ") + fileName);
    } else {
        QMessageBox::critical(this, "Saving Error", "Could not save image file.");
    }
}
