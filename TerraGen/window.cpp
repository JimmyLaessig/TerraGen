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

    ui->biasSpinner->setValue(HeightmapGenerator::Bias);
    ui->frequency1Spinner->setValue(HeightmapGenerator::Frequency1);
    ui->frequency2Spinner->setValue(HeightmapGenerator::Frequency2);

    ui->persistenceSpinner->setValue(HeightmapGenerator::Persistence);
    ui->scaleSpinner->setValue(HeightmapGenerator::Scale);
}

Window::~Window()
{
    delete ui;
}


void Window::setNoiseImage(QImage* image)
{
    ui->noiseImageLabel->setPixmap(QPixmap::fromImage(*image));
    ui->noiseImageLabel->setScaledContents(true);
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
