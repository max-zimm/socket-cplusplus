#ifndef SERVER_H
#define SERVER_H
#include "settings.h"


class Server {

        public:
            int port;
            Server();

        private:
            int start();
            
        
    };

#endif