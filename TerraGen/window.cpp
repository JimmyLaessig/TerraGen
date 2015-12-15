#include "window.h"
#include "ui_window.h"
#include <QString>
#include <QStringBuilder>

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{   
    ui->setupUi(this);
    ui->fpsLabel->setStyleSheet("QLabel { color:rgb(0, 255, 0); }");
}

Window::~Window()
{
    delete ui;
}


void Window::setNoiseImage(QImage* image)
{
    ui->noiseImageLabel->setPixmap(QPixmap::fromImage(*image));
    ui->noiseImageLabel->show();
}

void Window::setFPSLabel(double fps)
{

    if(fps < 1000)
        ui->fpsLabel->setText(QString("FPS: %1").arg(fps));
    else
        ui->fpsLabel->setText(QString("FPS: > 1000"));

    ui->fpsLabel->show();
}
