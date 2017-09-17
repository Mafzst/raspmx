#include <QComboBox>
#include <QDebug>
#include <unistd.h>

#include "prepa.h"
#include "ui_prepa.h"
#include "spimanager.h"

int pan, tilt, color, shape;

Prepa::Prepa(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Prepa)
{
    ui->setupUi(this);
    SpiManager::sendMessage(5, 255);

    connect(ui->btnSend, SIGNAL(clicked()), this, SLOT(send()));

    connect(ui->cbPan, SIGNAL(currentIndexChanged(int)), this, SLOT(changePan()));
    connect(ui->cbTilt, SIGNAL(currentIndexChanged(int)), this, SLOT(changeTilt()));
    connect(ui->cbColor, SIGNAL(currentIndexChanged(int)), this, SLOT(changeColor()));
    connect(ui->cbShape, SIGNAL(currentIndexChanged(int)), this, SLOT(changeShape()));

}

Prepa::~Prepa()
{
    delete ui;
}

void Prepa::changePan()
{
    QComboBox* cb;

    cb = qobject_cast<QComboBox*>(QObject::sender());

    int index = cb->currentIndex();

    pan = index * 100;
}

void Prepa::changeTilt()
{
    QComboBox* cb;

    cb = qobject_cast<QComboBox*>(QObject::sender());

    int index = cb->currentIndex();

    tilt = index * 100;
}

void Prepa::changeColor()
{
    QComboBox* cb;

    cb = qobject_cast<QComboBox*>(QObject::sender());

    int index = cb->currentIndex();

    color = index * 11;
}

void Prepa::changeShape()
{
    QComboBox* cb;

    cb = qobject_cast<QComboBox*>(QObject::sender());

    int index = cb->currentIndex();

    shape = index * 12;
}

void Prepa::send()
{
    SpiManager::sendMessage(1, pan);
    usleep(1000);
    SpiManager::sendMessage(2, tilt);
    usleep(1000);
    SpiManager::sendMessage(3, color);
    usleep(1000);
    SpiManager::sendMessage(4, shape);
    usleep(1000);
}
