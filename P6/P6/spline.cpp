#include "spline.h"
//Implementation ...
//TODO...

void Spline::addPoint(Vector3 point)
{
	this->controlPoints.push_back(point);

	int size = this->controlPoints.size();

	if ((size-1 != 0) && ((size-1)%3 == 0)) 
	{
		//Agregamos el punto reflejado para asegurar C1
		Vector3 v = this->controlPoints[size-1] + (this->controlPoints[size-1] - this->controlPoints[size-2]);
		
		this->controlPoints.push_back(v);
	}

}

Vector3 Spline::getInterpolatedPoint(float t, int index)
{
	Vector1x4 TM;
	TM.calculateBernstein(t);
	VectorPoints4x1 G(this->controlPoints[index-3],this->controlPoints[index-2], this->controlPoints[index-1], this->controlPoints[index]);

	return TM*G;
}

void Spline::draw(float increments)
{
	for (int i = 3; i < this->controlPoints.size(); i+= 3)
	{
		glColor3f(1,0,0);

		glBegin(GL_LINE_STRIP);

		Vector3 currentincrement;

		for(float t=0.0f; t <= 1.0f; t+=increments)
		{
			currentincrement = this->getInterpolatedPoint(t, i);

			glVertex3f(currentincrement.x, currentincrement.y, currentincrement.z);

		}

		glEnd();
	}
}

bool Spline::isFirstControlPoint(int point_number)
{
	return (point_number%3 == 1);
}

bool Spline::isSecondControlPoint(int point_number)
{
	return (point_number%3 == 2);
}

Vector3 Spline::getNormalizedInterpolatedPoint(float t)
{
	Vector3 v;

	float numcurves = this->getNumberOfCurves();

	int pointedcurve;

	if (numcurves == 1)
	{
		pointedcurve = 1;
	}
	else
	{
		if (t != 1)
		{
			pointedcurve = (t * numcurves) + 1;
		}
		else
		{
			pointedcurve = (t*numcurves);
		}
	}

	float localt = (t * numcurves) - (pointedcurve - 1);

	v = this->getInterpolatedPoint(localt,3*pointedcurve);

	return v;
}

int Spline::getNumberOfCurves()
{
	if (this->controlPoints.size() >= 4)
	{
		return ((this->controlPoints.size()-1)/3);
	}

	return 0;
}

void Spline::closeCurve()
{
	if (this->getNumberOfCurves() > 0)
	{
		int size = this->controlPoints.size();

		if (this->isFirstControlPoint(size-1))
		{
			Vector3 v = this->controlPoints[size-1] + (this->controlPoints[size-1] - this->controlPoints[size-2]);
			this->controlPoints.push_back(v);
		}

		this->controlPoints.push_back(this->controlPoints[0]);
	}
}

void Spline::updateCurve(int point_position)
{
	if (this->getNumberOfCurves()>0)
	{
		if(this->isFirstControlPoint(point_position) && point_position > 1)
		{
			this->controlPoints[point_position-2] = this->controlPoints[point_position-1] + (this->controlPoints[point_position-1] - this->controlPoints[point_position]);
		}
		else if(this->isSecondControlPoint(point_position) && (this->controlPoints.size()-1 >= point_position+2))
		{
			this->controlPoints[point_position+2] = this->controlPoints[point_position+1] + (this->controlPoints[point_position+1] - this->controlPoints[point_position]);
		}
	}
}