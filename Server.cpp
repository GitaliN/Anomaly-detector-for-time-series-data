#include "Server.h"
#include <unistd.h>
#include "thread"
#include "iostream"

using namespace std;

static bool running = true;

// constructor
Server::Server(int port)throw (const char*) {
    fd = socket(AF_INET,SOCK_STREAM,0);
    if (fd<0){
        throw "socket failed";
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if(bind(fd, (struct sockaddr*)&server,sizeof(server))<0){
        throw "bind failure";
    }

    if(listen(fd,3)<0){
        throw "listen failure";
    }
}

// activate the server
void Server::start(ClientHandler& ch)throw(const char*){
    t = new thread([&ch, this](){
        while (running){
            cout << "waiting for a client" << "\n";
            socklen_t sizeOfClient = sizeof(client);
            int firstClient = accept(fd,(struct sockaddr*)&client,&sizeOfClient);
            if (firstClient<0){
                throw "accept failure";
            }
            cout << "client connected" << "\n";
            ch.handle(firstClient);
            close(firstClient);
            this_thread::sleep_for (chrono::seconds(1));
        }
        close(fd);
    });
}

// stop the activation of the server
void Server::stop(){
    running = false;
    t->join(); // do not delete this!
}

Server::~Server() {
}