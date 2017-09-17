/***********************************************
*         RaspMx - console lumière             *
*               Nicolas Perraut                *
*                                              *
* Fichier  : main.cpp                          *
* Date     : 27/12/15                          *
*                                              *
* Fichier principal de l'application :         *
* lancement et instaciation de l'IHM           *
************************************************/

#include "mainwindow.h"          // Inclusion fichier d'entête fenêtre principale
#include <QApplication>          // Inclusion du composant Qt
#include <QFile>
#include <QSettings>
#include <QDateTime>
#include <QTextStream>
#include "stdio.h"               // Bibliothèque d'entrée-sortie standards
#include "spimanager.h"

// Fonction principale. accpete les arguments au lancement (passés à Qt)
int main(int argc, char *argv[])
{
   QApplication a(argc, argv);         // Création d'une nouvelle application Qt
   MainWindow w;                       // Création d'une nouvelle fenêtre Qt

   SpiManager::getInstance();

   QFile f(":qdarkstyle/style.qss");
   if (!f.exists())
   {
       printf("Unable to set stylesheet, file not found\n");
   }
   else
   {
       f.open(QFile::ReadOnly | QFile::Text);
       QTextStream ts(&f);
       qApp->setStyleSheet(ts.readAll());
   }

   w.showMaximized();
   w.show();                           // Affichage de la fenêtre

   return a.exec();                    // Execution de l'application Qt notre
                                       // sortie y est subordonnée
}
