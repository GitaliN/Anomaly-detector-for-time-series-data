/*
 * anomaly_detection_util.cpp
 *
 * Author: Gitali Naim
 */

#include <math.h>
#include "anomaly_detection_util.h"

// returns the average of X
float avg(float *x, int size)
{
    float xArr = 0;
    for (int i = 0; i < size; i++)
    {
        xArr = xArr + x[i];
    }
    float avg = (1 / (float)size) * xArr;
    return avg;
}

// returns the variance of X and Y
float var(float *x, int size)
{
    float xArrExp = 0;
    for (int i = 0; i < size; i++)
    {
        xArrExp = xArrExp + pow((x[i]), 2);
    }
    float var = (1 / (float)size) * (xArrExp)-pow(avg(x, size), 2);
    return var;
}

// returns the covariance of X and Y
float cov(float *x, float *y, int size)
{
    float avgX = avg(x, size);
    float avgY = avg(y, size);
    float arr[size];
    for (int i = 0; i < size; i++)
    {
        arr[i] = (x[i] - avgX) * (y[i] - avgY);
    }
    float cov = avg(arr, size);
    return cov;
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float *x, float *y, int size)
{
    float pearson = cov(x, y, size) / (sqrt(var(x, size) * var(y, size)));
    return pearson;
}

// performs a linear regression and returns the line equation
Line linear_reg(Point **points, int size)
{
    float xPoint[size], yPoint[size];
    for (int i = 0; i < size; i++)
    {
        xPoint[i] = points[i]->x;
        yPoint[i] = points[i]->y;
    }
    float a = cov(xPoint, yPoint, size) / var(xPoint, size);
    float b = avg(yPoint, size) - a * avg(xPoint, size);
    return Line(a, b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size)
{
    Line line = linear_reg(points, size);
    float devPoints = dev(p, line);
    return devPoints;
}

// returns the deviation between point p and the line
float dev(Point p, Line l)
{
    float devLine = l.f(p.x) - p.y;
    if (devLine < 0)
    {
        devLine = -1 * (devLine);
    }
    return devLine;
}
