#include "CLI.h"

// constructor
CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
}

void CLI::start() {
    // create an object of each command and enter to an array of commands
    Command *com1 = new Command1(dio);
    Command *com2 = new Command2(dio, &hybridAD);
    Command *com3 = new Command3(dio, &hybridAD, &anomRep);
    Command *com4 = new Command4(dio, &anomRep);
    Command *com5 = new Command5(dio, &hybridAD, &anomRep);
    Command *com6 = new Command6(dio);
    Command **commands[] = {&com1, &com2, &com3, &com4, &com5, &com6};
    Command *pointer;
    int option = 0;

    while (option != 6) {
        // print the menu
        dio->write("Welcome to the Anomaly Detection Server.\n"
                   "Please choose an option:\n");
        // print the description of each command
        for (auto & command : commands) {
            pointer = *command;
            dio->write(pointer->description);
        }
        option = stoi(dio->read());
        // pointer to the requested command
        pointer = *commands[option-1];
        // execute the requested command
        pointer->execute();
    }
};

CLI::~CLI() {
}