/***********************************************
*         RaspMx - console lumière             *
*                                              *
* Fichier  : arduino.ino                       *
* Date     : 27/12/15                          *
*                                              *
* Fichier code pour l'Arduino : réception des  *
* trames SPI et traitement                     *
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
volatile byte pos;            // Curseur de lecture
volatile boolean process_it;  // Etat de l'IT SPI

struct spi_frame {            // Strcuture trame SPI
  byte addr;                  // Adresse destinataire
  byte oppcode;               // Code opération
  word param;                 // Id composant à affecter
  byte len;                   // Longeur trame
  byte args[];                // Arguements de l'opération            
} rx_frame, tx_frame;         // Trame reçue, trame à émettre

int PIN = 2;

// Fonction de paramétrage
void setup (void)
{
  Serial.begin (9600);   // debugging
  Serial.write("Setup OK");

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  pinMode(PIN, OUTPUT);
  
  // SPI en mode esclave
  SPCR |= _BV(SPE);
  
  pos = 0;             // Buffer de recpetion vide
  process_it = false;  //P Pas d'IT en cours

  // Ajout d'une IT sur SPI
  SPI.attachInterrupt();
  SPI.setDataMode(SPI_MODE3);
  
  DmxSimple.usePin(3);        // DMX sur la broche 3
  DmxSimple.maxChannel(512);  // 512 canaux

}  // fin setup


// Routine d'interruption SPI
ISR (SPI_STC_vect)
{
  byte c = SPDR;  // Récupération de l'octet reçu 
  //Serial.print(c);
  
  // Ajout de l'octet dans le buffer (si place dispo)
  if (pos < sizeof buf)
    {
    buf [pos++] = c;
    Serial.println(c);
    
    // 0x00 suivi de 0x0A marque la fin de la trame
    if (c == '\n' && buf[pos - 2] == 0)
      process_it = true;  // Demande du traitement de l'IT
      //digitalWrite(PIN, HIGH);
      
    } 
}  // fin routine IT

// Boucle principale. Attente d'un traitement d'IT
void loop (void)
{
  if (process_it)
    {    
      process_received(); // Appel de la fonction de traitement
      
      process_it = false; // Acquitement IT
      pos = 0;            // Buffer vidé
    }
    
}

// Fonction de traitement de l'IT
int process_received (void)
{
  Serial.println("Recu !"); // Debug
  
  // Vérification de l'adresse
  if(buf[0] != SLAVE_ADDR) {
    Serial.print("Pas pour moi ;) ");
    Serial.println(buf[0]);
    return 0;  // Autre esclave
  }
  
  // Récupération de l'adresse du composant sur 2 octets
  word param = buf[2] << 8;
  param += buf[3];
  
  // Création de la strcuture trame
  rx_frame.addr = buf[0];    // Adresse esclave
  rx_frame.oppcode = buf[1]; // Code oppération
  rx_frame.param = param;    // Adresse du composant
  Serial.println(param);     // Debug
  
  // Recupération des arguments
  for(int i = 4; i < pos - 2; i++) {
    rx_frame.args[i - 4] = buf[i];
    //rx_frame.len++;
  }
  
  // Fonction de traitement des données
  resolve_fctn();
  
  return 1;
}

// Fonction de traitement des données
int resolve_fctn(void) {
  boolean error_unknown = false; // Pas d'erreur pour l'instant
  
  // Pour test : Gestion des GPIO (allumer/etteindre)
   if(rx_frame.param & MASK_GPIO == ADDR_GPIO && rx_frame.oppcode == 1) {
     //byte pin = byte(rx_frame.param) & 0x0F; // Récupération du numéro de pin
     
     // Sélection de l'action à effectuer (allumer/eteindre/inconnu)
     switch(rx_frame.args[0]) {
       case 0:
         digitalWrite(PIN, LOW);
         break;
       case 1:
         digitalWrite(PIN, HIGH);
         break;
       default:
         error_unknown = true;  // Il y a une erreur
         break;
     }
   }
   
   // Gestion du bus DMX direct
   else if (rx_frame.param <= 512 && rx_frame.oppcode == 1) {
     Serial.println("DMX process");            // Debug
     byte value = rx_frame.args[0];            // Récupération de la valeur
     Serial.println(value);                    // Debug
     DmxSimple.write(rx_frame.param, value);   // Modification du bus (canal, valeur)
   }
   
   // A FAIRE : Gestion des adresse console
     
   //Renvoi d'une trame
   if(!error_unknown) {
     tx_frame.addr = rx_frame.addr;
     tx_frame.oppcode = rx_frame.oppcode + 0x70;
     tx_frame.param = rx_frame.param;
     tx_frame.args[0] = 0x00;
     tx_frame.len = 7;         
 }
 
 SPDR = 'U';
 
 //send_response();  
}

void send_response(void) {
  //SPI.begin();
  Serial.println("========================");
  Serial.println(SPI.transfer('U'));
  Serial.println("========================");
  
  /*tbuf[0] = tx_frame.addr;
  tbuf[1] = tx_frame.oppcode;
  tbuf[2] = (tx_frame.param & 0xFF00) >> 8;
  tbuf[3] = tx_frame.param & 0x00FF;
  tbuf[4] = tx_frame.args[0];
  tbuf[5] = 0;
  tbuf[6] = 10;
  
  for(byte i = 0; i < tx_frame.len; i++) {
    Serial.println(tbuf[i], HEX);
    SPDR = byte(tbuf[i]);
  }*/
  
  //SPI.end();
}
