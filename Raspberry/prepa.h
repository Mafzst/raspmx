#ifndef PREPA_H
#define PREPA_H

#include <QWidget>

namespace Ui {
class Prepa;
}

class Prepa : public QWidget
{
    Q_OBJECT

public:
    explicit Prepa(QWidget *parent = 0);
    ~Prepa();

private:
    Ui::Prepa *ui;

public slots:
    void changePan();
    void changeTilt();
    void changeColor();
    void changeShape();
    void send();
};

#endif // PREPA_H
