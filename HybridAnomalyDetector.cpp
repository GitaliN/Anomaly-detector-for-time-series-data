
#include "HybridAnomalyDetector.h"


HybridAnomalyDetector::HybridAnomalyDetector() {

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

// This function initialize the circle fields of correlated features,
// the circle fields return from the minimal radius circle algorithm.
void HybridAnomalyDetector::initCircle(int cfSize, Point **ps, int sizeRow) {
    cf[cfSize].center = findMinCircle(ps,sizeRow).center;
    cf[cfSize].radius = findMinCircle(ps,sizeRow).radius * 1.1;
}


void HybridAnomalyDetector::setTreshold(float t) {
    this->changeTreshold = t;
}
float HybridAnomalyDetector::getTreshold() {
    return changeTreshold;
}

// This function enter an anomaly to a vector of anomalyReport and return the vector.
// The anomaly detection algorithm is hybrid - combining the simple detector
// with the minimum radius algorithm.
vector<AnomalyReport> HybridAnomalyDetector::anomaly(Point *p, int j, int i, vector<AnomalyReport> anomRep) {
    //If the correlation threshold is greater or equal to the set threshold
    // - runs the simple algorithm (implementation in the father)
    if (cf[j].corrlation >= getTreshold()){
        anomRep = SimpleAnomalyDetector::anomaly(p,j,i,anomRep);
        return anomRep;
        //If there is a feature whose maximum correlation with another feature is greater than 0.5 but
        // smaller than the correlation threshold - so for these two features the minimum radius
        // that encompasses the entire training points will be determined.
    }else{
        // calculate the distance between the point and the center point of circle
        float devi = dist(*p, cf[j].center);
        // Check if the distance larger than the radius in order to find anomaly
        if (devi > cf[j].radius) {
            // Create an object of anomalyReport to report on anomaly
            // this object include first field of string - the anomaly features,
            // and second field of timeStep - the row's number
            AnomalyReport *ar = new AnomalyReport(cf[j].feature1 + "-" + cf[j].feature2, i + 1);
            anomRep.push_back(*ar);
            delete ar;
        }
        delete p;
        return anomRep;
    }
}