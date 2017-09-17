/***********************************************
*         RaspMx - console lumière             *
*                                              *
* Fichier  : arduino.ino                       *
* Date     : 25/03/16                          *
*                                              *
* Fichier code pour l'Arduino :                *
* réception des                                *
* trames SPI et traitement                     *
* Modification des valeur de paramètres de la  *
* console en fonction des valuer DMX           *
* Actualisation des valeurs du bus DMX en      *        
* réaction à un changement de curseur console  *
* Emission acquitement à faire                 *
************************************************/

#include <SPI.h>        // Bibliothèque de gestion SPI
#include <DmxSimple.h>  // Bibliothèque de gestion DMX

// Addresse de l'esclave
const int SLAVE_ADDR = 0x01;

// Masques d'adresses des composants
const word MASK_GPIO = 0xFFF0;
const word MASK_DMX = 0xFE00;

//Premières adresse de catégories de composants
const word ADDR_GPIO = 0xFFD0;
const word ADDR_DMX = 0x0000;




byte buf [100], tbuf[100];    // Buffers
// Structure de trame console
byte buffco[] = {0xf0, 0x43, 0x10, 0x3d, 0x08, 0x01, 0x00, 0x00, 0xf7};
volatile byte pos;        // Curseur de lecture
volatile boolean process_it;      // Etat de l'IT SPI
volatile boolean console_process; // Etat IT console

struct spi_frame {            // Strcuture trame SPI
  byte addr;                  // Adresse destinataire
  byte oppcode;               // Code opération
  word param;                 // Id composant à affecter
  byte len;                   // Longeur trame
  byte args[];                // Arguements de l'opération            
} rx_frame, tx_frame;         // Trame reçue, trame à émettre

char channel, value;          // Variables bus DMX

// Fonction de paramétrage
void setup (void)
{
  Serial.begin (38400);   // Liaison série console

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // SPI en mode esclave
  SPCR |= _BV(SPE);
  
  pos = 0;                 // Buffer de recpetion vide
  process_it = false;      // Pas d'IT SPI
  console_process = false; // Pas d'IT console

  // Ajout d'une IT sur SPI
  SPI.attachInterrupt();
  
  DmxSimple.usePin(3);        // DMX sur la broche 3
  DmxSimple.maxChannel(512);  // 512 canaux

}  // fin setup


// Routine d'interruption SPI
ISR (SPI_STC_vect)
{
  byte c = SPDR;  // Récupération de l'octet reçu 
  
  // Ajout de l'octet dans le buffer (si place dispo)
  if (pos < sizeof buf)
    {
    buf [pos++] = c;
    //Serial.println(c);
    
    // 0x00 suivi de 0x0A marque la fin de la trame
    if (c == '\n' && buf[pos - 2] == 0)
      process_it = true;  // Demande du traitement de l'IT
      
    } 
}  // fin routine IT

// Boucle principale. Attente d'un traitement d'IT
void loop (void)
{
  if (process_it)         // IT SPI
    {    
      process_received(); // Appel de la fonction de traitement
      
      process_it = false; // Acquitement IT
      pos = 0;            // Buffer vidé
    } // fin IT SPI

   if (console_process) {            // IT console
    DmxSimple.write(channel, value); // Mise à jour DMX
    console_process = false;         // Acquitement
   } // Fin IT console
} // fin boucle principale

// Fonction de traitement de l'IT SPI
int process_received (void)
{
  // Vérification de l'adresse
  if(buf[0] != SLAVE_ADDR) {
    return 0;  // Autre esclave
  }
  
  // Récupération de l'adresse du composant sur 2 octets
  word param = buf[2] << 8;
  param += buf[3];
  
  // Création de la strcuture trame
  rx_frame.addr = buf[0];    // Adresse esclave
  rx_frame.oppcode = buf[1]; // Code oppération
  rx_frame.param = param;    // Adresse du composant
  
  // Recupération des arguments
  for(int i = 4; i < pos - 2; i++) {
    rx_frame.args[i - 4] = buf[i];
  }
  
  // Fonction de traitement des données
  resolve_fctn();
  
  return 1;
} // Fin traitement IT SPI

// Fonction de traitement des données
int resolve_fctn(void) {
  boolean error_unknown = false; // Pas d'erreur pour l'instant
  
   
   // Gestion du bus DMX direct
   if (rx_frame.param <= 512 && rx_frame.oppcode == 1) {
     byte value = rx_frame.args[0];            // Récupération de la valeur
     DmxSimple.write(rx_frame.param, value);   // Modification du bus (canal, valeur)
     buffco[6] = rx_frame.param + 44;          // Actualisation trame console (offset AUX4)
     buffco[7] = value / 2;                    // Actualisation console (valeur max 127)
     int i;

     for(i = 0; i < 9; i++)
     {
       Serial.write(buffco[i]);                // Envoi de la trame console
     }
   }
   
   // A FAIRE : Aquiter au Raspberry
           
 } // fin traitement données SPI

// Fonction appellée par une IT interne sur Série
void serialEvent() {
  // Lecture en continu du heartbeat
  char oct = (char)Serial.read();

  // Si pas octer d'heartbeat
  if(oct != 0xFFFFFFFE && !console_process) {
    pos++;                  // On compte les octets
    if(pos == 7) {          // Le 7e est le canal
      channel = oct - 44;   // Offset AUX4 à déclarer globalement
    }
    if(pos == 8) {          // Le 8e est la valeur
      value = oct * 2;      // Valuer max 127 donc *2
    }
  }

  // On est arrivé à la fin de la trame
  if(oct == 0xFFFFFFF7 && !console_process) {
      console_process = true;  // On demande le traitement
      pos = 0;                 // On repart à 0 pour le comptage suivant
  }
}
