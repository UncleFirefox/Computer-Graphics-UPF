#ifndef SPLINE_H
#define SPLINE_H

#include "framework.h"
#include <vector>

class Spline
{
public:
	std::vector<Vector3> controlPoints;
	void addPoint( Vector3 point);
	Vector3 Spline::getInterpolatedPoint(float t, int index);
	void draw(float increments);
	bool isFirstControlPoint(int point_number);
	bool isSecondControlPoint(int point_number);
	Vector3 getNormalizedInterpolatedPoint(float t);
	int getNumberOfCurves();
	void closeCurve();
	void updateCurve(int point_position);
};


#endif