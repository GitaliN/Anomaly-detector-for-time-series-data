#include <iostream>
#include "SimpleAnomalyDetector.h"

using namespace std;

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

/* Before I implemented these functions, I sat down to understand the exercise with
 * my friend Avital Abergel and we thought together about the solutions. of course,
 * each one of us wrote and implemented the code by herself, without looking at the other code.*/


// This function is the offline learning step
// it receives a ts object - the file after being inserted into the data structure
// the function initializes the fields of cf which is a vector of the correlative features

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    int sizeRow = ts.rowsNum;
    setRowsNum(sizeRow);
    int sizeCol = ts.featursVector.size();
    float x[sizeRow], y[sizeRow];
    vector<float> pears;

    // This loop pass over all the combinations of two columns
    for (int i = 0; i < sizeCol-1; i++) {
        // convert vector to array
        copy(ts.resultVec[i].second.begin(), ts.resultVec[i].second.end(), x);
        for (int j = i + 1; j < sizeCol; j++) {
            copy(ts.resultVec[j].second.begin(), ts.resultVec[j].second.end(), y);
            // Check for each feature which of the other features is most correlative to it
            pears.push_back(pearson(x, y, sizeRow));
        }

        // Return the maximum pearson
        float maxPears = *max_element(pears.begin(), pears.end());

        int maxIndex;
        // Return the index of the maximum pearson
        for(int j = 0;j < sizeRow;j++){
            if(pears[j] == maxPears) {
                maxIndex = j;
                break;
            }
        }
        maxIndex += i + 1;

        int cfSize = cf.size();
        // check if the maximum pearson greater than 0.5
        if (abs(maxPears) > 0.5) {
            copy(ts.resultVec[maxIndex].second.begin(), ts.resultVec[maxIndex].second.end(), y);
            cf.push_back(correlatedFeatures());

            // Initialize some of the the fields of cf
            cf[cfSize].corrlation = maxPears;
            cf[cfSize].feature1 = ts.featursVector[i];
            cf[cfSize].feature2 = ts.featursVector[maxIndex];

            Point *ps[sizeRow];
            // Create an array of points, each point is from pair of different columns
            for (int j = 0; j < sizeRow; j++) {
                ps[j] = new Point(x[j], y[j]);
            }

            // Calculate the regression line for each correlative pair
            Line linearLine = linear_reg(ps, sizeRow);
            cf[cfSize].lin_reg = linearLine;

            //Create an array of deviation between each point and the regression line
            float deviation[sizeRow];
            for (int j = 0; j < sizeRow; j++) {
                deviation[j] = dev(*ps[j], linearLine);
            }

            // Return the maximum deviation
            float maxDev = *max_element(deviation, deviation + sizeRow)*1.1;
            cf[cfSize].threshold = maxDev;

            initCircle(cfSize, ps, sizeRow);

            // destructor
            for (int j = 0; j < sizeRow; j++) {
                delete ps[j];
            }

        }
        pears.clear();
    }
}

// This function initialize the circle fields of correlated features with default values.
// The real value, will be needed for a minimal radius circle algorithm,
// they will be initialized in HybridAnomalyDetector
void SimpleAnomalyDetector::initCircle(int cfSize, Point **ps, int sizeRow){
    cf[cfSize].center = Point(0,0);
    cf[cfSize].radius = 0;
}

// This function enter an anomaly to a vector of anomalyReport and return the vector
vector<AnomalyReport> SimpleAnomalyDetector::anomaly(Point *p, int j, int i, vector<AnomalyReport> anomRep) {
    // Calculate the deviation between the point and the regressive line (calculated in learnNormal)
    float devi = dev(*p, cf[j].lin_reg);
    // Check if the deviation larger than the threshold in order to find anomaly
    if (devi > cf[j].threshold) {
        // Create an object of anomalyReport to report on anomaly
        AnomalyReport *ar = new AnomalyReport(cf[j].feature1 + "-" + cf[j].feature2, i + 1);
        anomRep.push_back(*ar);
        delete ar;
    }
    delete p;
    return anomRep;
}

// This function is the online anomaly detection
// it receives a ts object - the file after being inserted into the data structure
// the function returns a vector of anomalyReport objects, the vector contain the anomalies
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
    int sizeRowTs = ts.rowsNum;
    int sizeColTs = ts.resultVec.size();
    int sizeColCf = cf.size();
    vector<AnomalyReport> anomRep;

    // The first loop pass over the rows of ts in order to read the input line by line
    for (int j = 0; j < sizeColCf; j++) {
        for (int i = 0; i < sizeRowTs; i++) { // Pass over the columns of cf
            for (int k = 0; k < sizeColTs - 1; k++) { // Pass over the columns of ts
                for (int l = k + 1; l < sizeColTs; ++l) { // Pass over the columns of ts
                    // Compare between the features in cf and the features in ts
                    int compareFirstFeat = ts.featursVector[k].compare(cf[j].feature1);
                    int compareSecFeat = ts.featursVector[l].compare(cf[j].feature2);
                    // If the strings are equal
                    if (compareFirstFeat == 0 && compareSecFeat == 0) {
                        // Create a point from the values of ts that are in the correlated features
                        float x = ts.resultVec[k].second[i];
                        float y = ts.resultVec[l].second[i];
                        Point *p = new Point(x, y);
                        anomRep = anomaly(p, j, i, anomRep);
                    }
                }
            }
        }
    }
    return anomRep;
}

void SimpleAnomalyDetector::setRowsNum(int rowsNum){
    this->rowsNum = rowsNum;
}

int SimpleAnomalyDetector::getRowsNum(){
    return this->rowsNum;
}