/***********************************************
*         RaspMx - console lumière             *
*               Nicolas Perraut                *
*                                              *
* Fichier  : manual.cpp                        *
* Date     : 27/12/15                          *
*                                              *
* Code de le fenêtre du mode manuel            *
************************************************/

#include "manual.h"     // Inclusion des entêtes propres
#include "ui_manual.h"  // Inclusion de l'IHM générée (compilation)
#include "spimanager.h"
#include <QtDebug>      // Inclusion des fonctions de debug

QSpinBox* currentReciever;      // Spibox recevant le clic du clavier
bool dirtyReciever = false;     // Saisie en cours dans le spinbox
int lastChannel, lastValue = 0; // Dernières valeurs sauvegardées

// Constrcuteur de l'objet Qt représentant la fenêtre
manual::manual(QWidget *parent) :
    QWidget(parent),            // Appel de le fonction Qt créant la fenêtre
    ui(new Ui::manual)          // Génération de l'IHM
    {
        ui->setupUi(this);      // Installation de l'IHM dans la fenêtre

        currentReciever = ui->sbChannel;    // Sélection du spinbox canal par défaut
        currentReciever->setFocus();        // Focus sur le spinbox canal

        // Création des connexion entre evt IHM et fonctions
        // Sélection du dernier spinbox sélectionné comme recepteur clavier (perte de focus)
        connect(ui->sbChannel, SIGNAL(editingFinished()), this, SLOT(changeFocus()));
        connect(ui->sbValue, SIGNAL(editingFinished()), this, SLOT(changeFocus()));
        // Réaction à l'appui sur une touche du clavier numérique
        connect(ui->pb1, SIGNAL(clicked()), this, SLOT(handleDialer()));
        connect(ui->pb2, SIGNAL(clicked()), this, SLOT(handleDialer()));
        connect(ui->pb3, SIGNAL(clicked()), this, SLOT(handleDialer()));
        connect(ui->pb4, SIGNAL(clicked()), this, SLOT(handleDialer()));
        connect(ui->pb5, SIGNAL(clicked()), this, SLOT(handleDialer()));
        connect(ui->pb6, SIGNAL(clicked()), this, SLOT(handleDialer()));
        connect(ui->pb7, SIGNAL(clicked()), this, SLOT(handleDialer()));
        connect(ui->pb8, SIGNAL(clicked()), this, SLOT(handleDialer()));
        connect(ui->pb9, SIGNAL(clicked()), this, SLOT(handleDialer()));
        connect(ui->pb0, SIGNAL(clicked()), this, SLOT(handleDialer()));
        // Réaction à l'appui sur le bouton Annuler
        connect(ui->pbCancel, SIGNAL(clicked()), this, SLOT(handleSubmit()));
        // Réaction à l'appui sur le bouton Ok
        connect(ui->pb0K, SIGNAL(clicked()), this, SLOT(handleSubmit()));

    }

// Destructeur
manual::~manual()
{
    delete ui;  // Destruction de l'IHM
}

// Fonction appelée à la perte de focus d'un spinbox
void manual::changeFocus()
{
    // Le recpeteur clavier est l'emetteur de l'evt. Il est propre
    currentReciever =  qobject_cast<QSpinBox *>(QObject::sender());
    dirtyReciever = false;

}

// Fonction appelé à l'appui sur une touche du clavier digital
void manual::handleDialer()
{
    QPushButton* sender;
    int value = 0;

    // On révcupère l'émetteur
    sender = qobject_cast<QPushButton *>(QObject::sender());

    qDebug() << dirtyReciever;  // Débug : affichage de la propreté

    // Si le récepteur est propre
    if(!dirtyReciever)
    {
        dirtyReciever = true;   // On le rend sale

        // On sauvegarde la valeur actuelle (dernière valeur appliquée)
        if(currentReciever->objectName() == ui->sbChannel->objectName())
        {
            lastChannel = currentReciever->value(); // Svg de la valeur canal
        }
        else if (currentReciever->objectName() == ui->sbValue->objectName())
        {
            lastValue = currentReciever->value();   // Svg de la valeur valeur
        }
    }
    else    // Il est sale
    {
        // On multiplie sa valeur par dix
        value = currentReciever->value();
        value *= 10;
        currentReciever->setValue(value);
    }
    // Dans tous les cas on additionne la valeur du bouton à la valeur actuelle
    currentReciever->setValue(value + sender->text().toInt());
}

// Fonction appelée lors de l'appui sur le bouton annuler
void manual::handleCancel()
{
    // On restaure la dernière valeur suavegardée
    if(currentReciever->objectName() == ui->sbChannel->objectName())
    {
        currentReciever->setValue(lastChannel); // Valeur canal
    }
    else if (currentReciever->objectName() == ui->sbValue->objectName())
    {
        currentReciever->setValue(lastValue);   // Valeur valeur
    }
}

// Fonction appelée lors de l'appui sur le bouton valider
void manual::handleSubmit()
{
    // Dans tous les cas on sauvegarde la valeur du canal
    lastChannel = ui->sbChannel->value();

    // Si on était en train d'éditer le canal...
    if(currentReciever->objectName() == ui->sbChannel->objectName())
    {
        ui->sbValue->setFocus(); //... on passe à l'édition de la valeur
    }
    else // Si on édiatit la valeur...
    {
        lastValue = ui->sbValue->value();           //... On sauvegarde cette valeur

        qDebug() << lastChannel << ":" << lastValue; // Pour débug : on affiche canal/valeur
        // TODO : Envoyer l'odre de modification à l'Arduino

        SpiManager::sendMessage(lastChannel, lastValue);
    }
}
