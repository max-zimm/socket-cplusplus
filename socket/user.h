#ifndef USER_H
#define USER_H
#include <vector>
#include "settings.h"

int userCount = 0;
struct User{


    int user_id;
    SOCKET userSocket;
};

std::vector<User> userList;

class Messenger{

    public:
        Messenger();
        User CreateUser();
        void SendMessage(User sender, char* message);
        char * GetMessage();


};


#endif