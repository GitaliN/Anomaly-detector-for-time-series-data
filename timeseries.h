#include <vector>
#include <string>

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;

class TimeSeries{

public:
    vector<pair<string, vector<float>>> resultVec;
    vector<string> featursVector;
    int rowsNum;
    TimeSeries(const char* CSVfileName);
    vector<pair<string, vector<float>>> readCsvFile(const char* CSVfileName);
    vector<string> returnFeat(const char* CSVfileName);
    int rowsNumber();
};

#endif /* TIMESERIES_H_ */
