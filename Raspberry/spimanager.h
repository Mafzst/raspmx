#ifndef SPIMANAGER_H
#define SPIMANAGER_H

#endif // SPIMANAGER_H

class SpiManager
{

public:
    // Constructeur Destrcuteur
    SpiManager();
    ~SpiManager() {}

    static SpiManager *getInstance();

    static void kill();

    //=============================//

    static void sendMessage(int channel, int value);
    static SpiManager *_singleton;
};
