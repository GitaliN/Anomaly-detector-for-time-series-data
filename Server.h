/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_
#include "sys/socket.h"
#include "netinet/in.h"
#include "pthread.h"
#include "thread"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include <iostream>
#include "commands.h"
#include "CLI.h"


using namespace std;

// this class represents a SocketIO
class SocketIO: public DefaultIO{
    int clientID;
public:
    SocketIO(int clientID){
        this->clientID = clientID;
    }

    void write(string input){
        ::write(clientID,input.c_str(),input.length());
    }

    string read(){
        string clientInput="";
        char c=0;
        ::read(clientID,&c,sizeof(char));
        while(c!='\n'){
            clientInput+=c;
            ::read(clientID,&c,sizeof(char));
        }
        return clientInput;
    }

    void write(float f) {
        string sf = to_string(f);
        sf.erase(sf.find_last_not_of('0') + 1, string::npos);
        ::write(clientID, sf.c_str(), sf.length());
    }

    void read(float* f){
    }
};

// ClientHandler interface
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

// this class represents an AnomalyDetectionHandler class
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
        SocketIO s(clientID);
        CLI cli(&s);
        cli.start();
    }
};

// this class represents a Server
class Server {
	thread* t; // the thread to run the start() method in
	int fd;
	sockaddr_in server;
	sockaddr_in client;

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */