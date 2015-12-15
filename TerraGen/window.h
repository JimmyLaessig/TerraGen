#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QTime>

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

    void setNoiseImage(QImage* image);
    void setFPSLabel(double fps);

private:
    Ui::Window *ui;

    QTime startTime;
};

#endif // WINDOW_H
