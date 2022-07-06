#include <iostream>
#include "timeseries.h"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <utility>


using namespace std;


// Constructor
TimeSeries::TimeSeries(const char *CSVfileName) {
    resultVec = readCsvFile(CSVfileName);
    featursVector = returnFeat(CSVfileName);
    rowsNum = rowsNumber();
}

// Read a given CSV file and entered the data into vector of pairs <string, vector<float>>
// string is the first row - features, vector<float> is the values in each column.
vector<pair<string, vector<float>>> TimeSeries::readCsvFile(const char* CSVfileName){
    string line, typeFeature, ssValues, strValue;

    // Create a filestream of from the CSV file
    ifstream inputFile(CSVfileName);

    // Read the first line in the CSV file
    getline(inputFile, line);
    // Create a string stream to the first line, this line represent the features
    stringstream features(line);

    // Enter each feature to different cell in the first side (string) of the result vector
    while (getline(features, typeFeature, ',')) {
        resultVec.push_back({typeFeature, vector<float>{}});
    }

    // Continue to read all the lines in the file
    while(getline(inputFile,line)){
        int count = 0;
        // Create a string stream to the next line - each line represent the values
        stringstream ssValues(line);
        // Enter each value to different cell in the second side (vector<float>) of the result vector
        while (getline(ssValues, strValue, ',')) {
            // Casting from string to float
            float value = stof(strValue);
            resultVec[count].second.push_back(value);
            count++;
        }
    }
    inputFile.close();
    return resultVec;
};

// Return the number of rows in the table
int TimeSeries::rowsNumber(){
    return resultVec[0].second.size();
}

// Return a vector of the features - the first row in the table
vector<string> TimeSeries::returnFeat(const char* CSVfileName) {
    string line, typeFeature;

    // Create a filestream of from the CSV file
    ifstream inputFile(CSVfileName);

    getline(inputFile, line);
    stringstream features(line);

    // Enter each feature to different cell in the first side (string) of the result vector
    while (getline(features, typeFeature, ',')) {
        featursVector.push_back(typeFeature);
    }
    inputFile.close();
    return featursVector;
}
