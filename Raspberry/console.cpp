/***********************************************
*         RaspMx - console lumière             *
*               Nicolas Perraut                *
*                                              *
* Fichier  : console.cpp                       *
* Date     : 27/12/15                          *
*                                              *
* Fichier de code de la console virtuelle      *
************************************************/

#include "console.h"        // Inclusion des entêtes propres
#include "ui_console.h"     // Inclusion de l'IHM (compilation)
#include <QtDebug>          // Inclusion des fonction de debug
#include "spimanager.h"

// Constructeur de l'objet Qt représentant la fenêtre
Console::Console(QWidget *parent) :
    QWidget(parent),        // Appel de la fonction Qt créant la fenêtre
    ui(new Ui::Console)     // Génération de l'IHM
{
    ui->setupUi(this);      // Ajout de l'IHM à la fenêtre

    // Création des liens entres les composants et des fonctions
    // Appel de le fonction 'changeDmx' quand un curseur change de valeur
    connect(ui->vsCh1, SIGNAL(valueChanged(int)), this, SLOT(changeDmx()));
    connect(ui->vsCh2, SIGNAL(valueChanged(int)), this, SLOT(changeDmx()));
    connect(ui->vsCh3, SIGNAL(valueChanged(int)), this, SLOT(changeDmx()));
    connect(ui->vsCh4, SIGNAL(valueChanged(int)), this, SLOT(changeDmx()));
    connect(ui->vsCh5, SIGNAL(valueChanged(int)), this, SLOT(changeDmx()));
}

// Destructeur
Console::~Console()
{
    delete ui;              // Destrcution de l'IHM
}

// Fonction appelée pour modifier la valeur du canal DMX
void Console::changeDmx()
{
    QSlider* slider;        // Création d'une nouvelle variable slider
    int value;
    char name;

    // Récupération de l'object ayant déclenché l'appel de la fonction
    slider = qobject_cast<QSlider *>(QObject::sender());

    if(slider)  // S'il l'appelant existe réellement
    {
        // On récupère sa valeur et son numéro (dernier caractère du nom)
        value = slider->sliderPosition();
        name = slider->objectName()[slider->objectName().length() - 1].toLatin1();

        qDebug() << name << " : " << value; // Debug : affichage du numéro et de la valeur
        // A FAIRE : Appeler un fonction pour demander le changement à l'Arduino

        int channel  = name - 48;
        channel += ui->sbOffset->value();

        SpiManager::sendMessage(channel, value);
    }

}
