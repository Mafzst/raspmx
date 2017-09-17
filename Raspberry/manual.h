/***********************************************
*         RaspMx - console lumière             *
*               Nicolas Perraut                *
*                                              *
* Fichier  : manual.h                          *
* Date     : 27/12/15                          *
*                                              *
* Fichier d'entête mode manuel                 *
************************************************/

#ifndef MANUAL_H
#define MANUAL_H

#include <QWidget>      // Inclusion de l'objet Qt

namespace Ui            // Déclaration du namespace
{
    class manual;       // Déclaration de la classe
}

// Déclaration des prototypes de la fenêtre
class manual : public QWidget
{
    Q_OBJECT

public:
    explicit manual(QWidget *parent = 0);   // Constrcuteur
    ~manual();                              // Destrcuteur

private:
    Ui::manual *ui;     // Variable pour l'IHM générée

// Déclaration des slots
public slots:
    void changeFocus(void);     // Perte de focus des spinbox
    void handleDialer(void);    // Clic sur un bouton du clavier
    void handleCancel(void);    // Clic sur Annuler
    void handleSubmit(void);    // Clic sur OK
};

#endif // MANUAL_H
