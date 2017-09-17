/***********************************************
*         RaspMx - console lumière             *
*               Nicolas Perraut                *
*                                              *
* Fichier  : mainwindow                        *
* Date     : 27/12/15                          *
*                                              *
* Fenêtre principale de l'application :        *
* Choix du mode d'affichage + accès au régalges*
************************************************/

#include "mainwindow.h"         // Inclusion des entêtes
#include "ui_mainwindow.h"      // Inclusion de l'IHM générée (compilation)
#include "spimanager.h"
#include <unistd.h>


// Constrcuteur de l'objet Qt représentant la fenêtre principale
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),        // Appel de la fonction Qt créant la fenêtre
    ui(new Ui::MainWindow)      // Génération de l'IHM
{
    ui->setupUi(this);          // Ajout de l'IHM dans le fenêtre

    // Création des liens entres les composants et des fonctions
    // Ouvertiure de la console virtuelle au clic sur le bouton 'btnConsole'
    connect(ui->btnConsole, SIGNAL(clicked()), this, SLOT(openConsole()));
    // Ouverture du mode manuel au clic sur le bouton 'btnManual'
    connect(ui->btnManual, SIGNAL(clicked()), this, SLOT(openManual()));

    connect(ui->btnPrepa, SIGNAL(clicked()), this, SLOT(openPrepa()));

    connect(ui->actionFade_in_Fade_Out, SIGNAL(triggered()), this, SLOT(startDemoFadeIO()));
    connect(ui->actionDemo_Scan, SIGNAL(triggered()), this, SLOT(startDemoScan()));
}

// Destructeur de l'objet Qt représentant la fenêtre principale
MainWindow::~MainWindow()
{
    delete ui;                  // Destuction de l'IHM
}


// Fonction d'ouverture de la console virtuelle
void MainWindow::openConsole()
{
    consoleFrame = new Console();   // Création d'un nouvel objet 'Console'
    consoleFrame->showMaximized();
    consoleFrame->show();           // Affichage de la nouvelle fenêtre
}


// Fonction d'ouverture du mode manuel
void MainWindow::openManual()
{
    manualFrame = new manual();     //Création d'un nouvel objet 'manual'
    manualFrame->showMaximized();
    manualFrame->show();            // Affichage de la nouvelle fenêtre
}

void MainWindow::openPrepa()
{
    prepaFrame = new Prepa();
    prepaFrame->showMaximized();
    prepaFrame->show();
}

void MainWindow::startDemoFadeIO()
{
    int i;

    SpiManager::sendMessage(5, 255);
    SpiManager::sendMessage(4, 0);
    SpiManager::sendMessage(3, 0);

    for(i = 0; i < 256; i++)
    {
        SpiManager::sendMessage(1, i);
        SpiManager::sendMessage(2, i);
        usleep(3000000);
    }

    for(i = 255; i >= 0; i--)
    {
        SpiManager::sendMessage(1, i);
        SpiManager::sendMessage(2, i);
        usleep(3000000);
    }
}

void MainWindow::startDemoScan()
{
    prepaFrame = new Prepa();
    prepaFrame->show();

    consoleFrame = new Console();
    consoleFrame->show();
}
