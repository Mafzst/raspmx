#ifndef FILM_H
#define FILM_H

#include <QWidget>

namespace Ui {
class Film;
}

class Film : public QWidget
{
    Q_OBJECT

public:
    explicit Film(QWidget *parent = 0);
    ~Film();

private:
    Ui::Film *ui;
};

#endif // FILM_H
