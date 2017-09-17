/***********************************************
*         RaspMx - console lumière             *
*               Nicolas Perraut                *
*                                              *
* Fichier  : console.h                         *
* Date     : 27/12/15                          *
*                                              *
* Fichier d'entête console virtuelle           *
************************************************/

#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>  // Inclusion de l'objet Qt

namespace Ui        // Déclaration du namesapce
{
    class Console;  // Déclaration de la classe
}

// Déclaration des prototypes de la fenêtre principale
class Console : public QWidget
{
    Q_OBJECT

public:
    explicit Console(QWidget *parent = 0);  // Constructeur
    ~Console();                             // Destrcuteur

private:
    Ui::Console *ui;        // Variable pour l'IHM générée

public slots :
    void changeDmx();       // Slot pour changement valeur curseur
};

#endif // CONSOLE_H
