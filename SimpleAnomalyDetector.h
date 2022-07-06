#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"

#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>


struct correlatedFeatures{
    string feature1,feature2 = {}; // names of the correlated features
    float corrlation = 0;
    Line lin_reg = Line(0,0);
    float threshold = 0;
    Point center = Point(0,0);
    float radius = 0;
};

class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
    vector<correlatedFeatures> cf={};
    int rowsNum = 0;
public:
    SimpleAnomalyDetector();

    virtual ~SimpleAnomalyDetector();

    virtual void learnNormal(const TimeSeries& ts);

    virtual vector<AnomalyReport> detect(const TimeSeries& ts);

    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }

    virtual void initCircle(int cfSize, Point **ps, int sizeRow);

    virtual vector<AnomalyReport> anomaly(Point *p, int j, int i, vector<AnomalyReport> anomRep);

    void setRowsNum(int rowsNum);

    int getRowsNum();
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
