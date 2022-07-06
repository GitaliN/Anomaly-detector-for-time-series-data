#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

// this class represents a default io
class DefaultIO{
public:
    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    virtual ~DefaultIO(){}
};

// this class represents a command
class Command{
protected:
    DefaultIO* dio;
public:
    Command(DefaultIO* dio):dio(dio){}
    string description ={};
    virtual void execute()=0;
    virtual ~Command(){}
};

// this class represents the number 1 command - upload a time series csv file
class Command1: public Command{
public:
    Command1(DefaultIO* dio):Command(dio){
        description = "1.upload a time series csv file\n";
    }
    void execute() override {
        // upload the local train csv file
        dio->write("Please upload your local train CSV file.\n");
        ofstream trainOut("anomalyTrain.csv");
        // read the first line from the input
        string line = dio->read();
        do {
            // save each line in a csv file
            trainOut << line << endl;
            line = dio->read();
            // stop when "done" is written in a line
        } while (line != "done");
        trainOut.close();
        dio->write("Upload complete.\n");

        // upload the local test csv file
        dio->write("Please upload your local test CSV file.\n");
        ofstream testOut("anomalyTest.csv");
        line = dio->read();
        do {
            testOut << line << endl;
            line = dio->read();
        } while (line != "done");
        testOut.close();
        dio->write("Upload complete.\n");
    }
};

// this class represents the number 2 command - algorithm settings
class Command2: public Command{
    HybridAnomalyDetector* hybridAD;
public:
    Command2(DefaultIO* dio, HybridAnomalyDetector* hybridAD):Command(dio){
        description = "2.algorithm settings\n";
        this->hybridAD = hybridAD;
    }
    void execute() override {
        dio->write("The current correlation threshold is ");
        // get correletion threshold and print it
        dio->write(hybridAD->getTreshold());
        dio->write("\nType a new threshold\n");
        // read a new treshold
        float checkTreshold = stof(dio->read());
        // if the new treshold is smaller than 0 or bigger than 1, choose another
        while (checkTreshold < 0 || checkTreshold > 1) {
            dio->write("please choose a value between 0 and 1.");
            checkTreshold = stof(dio->read());
        }
        // change the treshold if its value is between 0 and 1
        hybridAD->setTreshold(checkTreshold);
    }
};

// this class represents the command number 3 - detect anomalies
class Command3: public Command{
    HybridAnomalyDetector* hybridAD;
    vector<AnomalyReport>* anomRep;
public:
    Command3(DefaultIO* dio, HybridAnomalyDetector* hybridAD, vector<AnomalyReport>* anomRep):Command(dio){
        description = "3.detect anomalies\n";
        this->hybridAD = hybridAD;
        this->anomRep = anomRep;
    }
    void execute() override{
        TimeSeries anomalyTrain("anomalyTrain.csv");
        TimeSeries anomalyTest("anomalyTest.csv");
        // activate the learn normal function on the train file
        this->hybridAD->learnNormal(anomalyTrain);
        // activate the detect function on the test file
        *this->anomRep = this->hybridAD->detect(anomalyTest);
        dio->write("anomaly detection complete.\n");
    }
};

// this class represents the number 4 command - display results
class Command4: public Command{
    vector<AnomalyReport>* anomRep;
public:
    Command4(DefaultIO* dio, vector<AnomalyReport>* anomRep):Command(dio){
        description = "4.display results\n";
        this->anomRep = anomRep;
    }
    void execute() override{
        // pass over the vector of the anomalies reports
        for (int i = 0; i < anomRep->size(); ++i) {
            // print the time step of the anomalies reports
            dio->write(anomRep->at(i).timeStep);
            dio->write("\t");
            // print the description of the anomalies report
            dio->write(anomRep->at(i).description);
            dio->write("\n");
        }
        dio->write("Done.\n");
    }
};

// this class represents the command number 5 - upload anomalies and analyze results
class Command5: public Command {
    HybridAnomalyDetector* hybridAD;
    vector<AnomalyReport> *anomRep;
public:
    Command5(DefaultIO *dio,HybridAnomalyDetector* hybridAD,vector<AnomalyReport> *anomRep):Command(dio) {
        description = "5.upload anomalies and analyze results\n";
        this->anomRep = anomRep;
        this->hybridAD = hybridAD;
    }

    void execute() override {
        dio->write("Please upload your local anomalies file.\n");
        vector<float> locAnom;
        float P = 0;
        float TP = 0;
        float N = (float)this->hybridAD->getRowsNum();
        string strLine;
        string line = dio->read();

        // read the file line by line and stop when the line is "done"
        while (line!="done"){
            // Create a string stream to the line
            stringstream ssLine (line);
            // Enter each value into a vector
            while (getline(ssLine, strLine, ',')) {
                // Casting from string to float
                float sVal = stof(strLine);
                locAnom.push_back(sVal);
            }

            bool checkIfFind = false;
            // calculate N - the amount of time steps in which there was no anomaly
            N -= locAnom.at(1)+1 - locAnom.at(0);
            // pass over the vector in which the anomalies actually occurred
            for (int j = locAnom.at(0); j < locAnom.at(1)+1; j++) {
                // pass over the vector in which the expected anomalies
                for (int k = 0; k < anomRep->size(); k++) {
                    // check if find an intersection between the time steps of these vectors
                    if (j == anomRep->at(k).timeStep) {
                        checkIfFind = true;
                        TP++;
                        break;
                    }
                }
                if (checkIfFind) {
                    break;
                }
            }
            // counter of the number of anomalies that occurred
            P++;
            // read the next line
            line = dio->read();
            // clear the vector
            locAnom.clear();
        }

        float diffAnom = 1;
        // check how many anomalies there are in the vector of the expected anomalies
        for (int i = 1; i < anomRep->size(); ++i) {
            if (anomRep->at(i).description != anomRep->at(i-1).description){
                diffAnom++;
            }
        }

        // calculate FP
        float FP = diffAnom - TP;
        dio->write("Upload complete.\n");
        dio->write("True Positive Rate: ");
        // print the TP rate
        dio->write(floor((TP/P)*1000)/1000);
        dio->write("\n");
        // print the FP rate
        dio->write("False Positive Rate: ");
        dio->write(floor((FP/N)*1000)/1000);
        dio->write("\n");
    }
};

// this class represents the number 6 command - exit
class Command6: public Command{
public:
    Command6(DefaultIO* dio):Command(dio){
        description = "6.exit\n";
    }
    void execute() override{
    }
};

#endif /* COMMANDS_H_ */