#include "networking/ConnectionHandler.h"

class Application {
    public:
        Application(Network::ConnectionHandler* connectionHandler);
        void setup();
        void loop();

    private:
        Network::ConnectionHandler* _connectionHandler;
};