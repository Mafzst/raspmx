#include "spimanager.h"
#include <QtDebug>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static const char *device = "/dev/spidev0.0";
static uint32_t mode;
static uint8_t bits = 8;
static uint32_t speed = 50000;
static uint16_t delay;

int fd;

uint8_t tx[] = {0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0A};
uint8_t rx[ARRAY_SIZE(tx) * 2] = {0, };

static void pabort(const char *s)
{
    qCritical(s);
    //abort();
}

static int transfer(int fd, uint8_t const *tx, uint8_t const *rx, size_t len)
{
    int ret;

    struct spi_ioc_transfer tr = {
        (unsigned long)tx,
        (unsigned long)rx,
        len,
        speed,
        delay,
        bits,
    };

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
        pabort("can't send spi message");

    //ret = ioctl(fd, SPI_IOC_MESSAGE(2), &rr);

    return ret;
}

void init ()
{
    int ret = 0;

    fd = open(device, O_RDWR);
    if (fd < 0)
        pabort("can't open device");
    /*
     * spi mode
     */
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1)
        pabort("can't set spi mode");

    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (ret == -1)
        pabort("can't get spi mode");

    /*
     * bits per word
     */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't set bits per word");

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't get bits per word");

    /*
     * max speed hz
     */
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        pabort("can't set max speed hz");

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        pabort("can't get max speed hz");

    printf("spi mode: 0x%x\n", mode);
    printf("bits per word: %d\n", bits);
    printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
}

SpiManager *SpiManager::_singleton = NULL;

SpiManager::SpiManager() {
    qDebug() << "Initialisation du gestionnaire SPI";

    init();

    //transfer(fd, tx, rx, sizeof(tx));
}

SpiManager *SpiManager::getInstance() {
    if(NULL == SpiManager::_singleton) {
        SpiManager::_singleton = new SpiManager;
    }

    return SpiManager::_singleton;
}

void SpiManager::kill() {
    if(NULL != SpiManager::_singleton) {
        delete SpiManager::_singleton;
        SpiManager::_singleton = NULL;
    }
}

void SpiManager::sendMessage(int channel, int value) {
    qDebug() << "Envoi DMX sur SPI. Canal : " << channel << " Valeur : " << value;
    tx[3] = channel;
    tx[4] = value;
    unsigned int i;
    char buff;

    int retour = transfer(fd, tx, rx, sizeof(tx) + 2);

//    for(i = 0; i < sizeof(tx) + 2; i++)
//    {
//        qDebug() << rx[i];
//    }

    //read(fd,&buff,1);

    qDebug() << "Retour : " << buff;

    //qDebug() << retour;
}
