/***********************************************
*         RaspMx - console lumière             *
*               Nicolas Perraut                *
*                                              *
* Fichier  : mainwindow.h                      *
* Date     : 27/12/15                          *
*                                              *
* Fichie d'entête fenêtre principale           *
************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>  // Inclusion des entêtes Qt
#include "console.h"    // Inclusion des fichiers d'entêtes pour la console virtuelle
#include "manual.h"     // Inclusion des fichiers d'entêtes pour le mode manuel
#include "prepa.h"

namespace Ui            // déclaration du namespace
{
    class MainWindow;   // Déclaration de la classe
}

// Déclaration des prototypes des fonctions de la fenêtre principale
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0); // Constrcuteur
    ~MainWindow();

private:
    Ui::MainWindow *ui;     // Variable contenant l'IHM genérée
    Console *consoleFrame;  // Fenêtre console virtuelle
    Prepa   *prepaFrame;
    manual  *manualFrame;    // Fenêtre mode manuel

// Déclaration des slots
public slots:
    void openConsole();     // Ouverture de la console virtuelle
    void openManual();      // Ouverture du mode manuel
    void openPrepa();
    void startDemoFadeIO();
    void startDemoScan();
};

#endif // MAINWINDOW_H
