#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:

    float changeTreshold = 0.9;

    HybridAnomalyDetector();

    virtual ~HybridAnomalyDetector();

    void initCircle(int cfSize, Point **ps, int sizeRow);

    vector<AnomalyReport> anomaly(Point *p, int j, int i, vector<AnomalyReport> anomRep);

    void setTreshold(float t);

    float getTreshold();
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */