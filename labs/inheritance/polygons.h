#ifndef __POLYGONS_H_
#define __POLYGONS_H_

#include <vector>
#include <string>
#include <algorithm>

#include "utilities.h"

using namespace std;

class Polygon {
private:
	string name;
	vector<Point> vertices;
public:
	Polygon(const string& name, const vector<Point>& vertices) {
		if (vertices.size() < 3) throw 0;
		this->name = name;
		this->vertices = vertices;
	}

	virtual ~Polygon() {}

	unsigned int numEqualSides() const {
		vector<double> lengths;
		for (unsigned i = 0; i < vertices.size(); ++i){
			lengths.push_back(DistanceBetween(vertices[i], vertices[(i+1) % vertices.size()]));
		}
		sort(lengths.begin(), lengths.end());
		unsigned max = 0, curr_count = 1;
		double prev_val = lengths[0];

		for (unsigned i = 1; i < lengths.size(); ++i) {
			if (EqualSides(lengths[i], prev_val)) {
				++curr_count;
				if (curr_count > max) max = curr_count;
			} else curr_count = 1;
			prev_val = lengths[i];
		}
		return max;
	}

	bool HasAllEqualAngles() const {
		double angle = Angle(vertices[0], vertices[1], vertices[2]);
		for (unsigned i = 0; i < vertices.size(); ++i ) {
			if (Angle( vertices[i], vertices[(i+1) % vertices.size()], vertices[(i+2) % vertices.size()] ) != angle){
				return false;
			}
		}
		return true;
	}

	bool HasARightAngle() const {
		for (unsigned i = 0; i < vertices.size(); ++i) {
			if (RightAngle( Angle(vertices[i], vertices[(i+1) % vertices.size()], vertices[(i+2) % vertices.size()]) )){
				return true;
			}
		}
		return false;
	}

	bool HasAllEqualSides() const {
		return vertices.size() == numEqualSides();
	}

	virtual const string& getName() const { return this->name; }
};



class Triangle : public Polygon {
private:
	string name;
	vector<Point> vertices;
public:
	Triangle(const string& name, const vector<Point> vertices) : Polygon(name, vertices) {
		if (vertices.size() != 3) throw 0;
		
		this->name = name;
		this->vertices = vertices; 
	}
};




class IsoscelesTriangle : virtual public Triangle {
private:
	string name;
	vector<Point> vertices;
public:
	IsoscelesTriangle(const string& name, const vector<Point>& vertices) : Triangle(name, vertices) {
		if (numEqualSides() < 2) throw 0;

		this->name = name;
		this->vertices = vertices; 
	}
};



class RightTriangle : virtual public Triangle {
private:
	string name;
	vector<Point> vertices;
public:
	RightTriangle(const string& name, const vector<Point>& vertices) : Triangle(name, vertices) {
		if (!HasARightAngle()) throw 0;

		this->name = name;
		this->vertices = vertices; 
	}
};


class EquilateralTriangle : public IsoscelesTriangle {
private:
	string name;
	vector<Point> vertices;
public:
	EquilateralTriangle(const string& name, const vector<Point>& vertices) : 
	Triangle(name, vertices), IsoscelesTriangle(name, vertices) {
		if (!HasAllEqualSides()) throw 0; 
		//if(!HasAllEqualAngles()) throw 0;

		this->name = name;
		this->vertices = vertices; 
	}
};


class IsoscelesRightTriangle : public IsoscelesTriangle, public RightTriangle {
private:
	string name;
	vector<Point> vertices;
public:
	IsoscelesRightTriangle(const string& name, const vector<Point>& vertices) 
	: Triangle(name, vertices), IsoscelesTriangle(name, vertices), RightTriangle(name, vertices) {
		this->name = name;
		this->vertices = vertices;	
	}
};


class Quadrilateral : public Polygon {
private:
	string name;
	vector<Point> vertices;
public:
	Quadrilateral(const string& name, const vector<Point>& vertices) : Polygon(name, vertices) {
		if (vertices.size() != 4) throw 0;
		this->name = name;
		this->vertices = vertices; 
	}
};


class Rectangle : public Quadrilateral {
private:
	string name;
	vector<Point> vertices;
public:
	Rectangle(const string& name, const vector<Point>& vertices) : Quadrilateral(name, vertices) {
		if (!HasAllEqualAngles()) throw 0;

		this->name = name;
		this->vertices = vertices; 
	}
};


class Square : public Rectangle {
private:
	string name;
	vector<Point> vertices;
public:
	Square(const string& name, const vector<Point>& vertices) : Rectangle(name, vertices) {
		if (!HasAllEqualSides()) throw 0; 

		this->name = name;
		this->vertices = vertices;  
	}
};

// write a class for Rhombus
class Rhombus : public Quadrilateral {
private:
	string name;
	vector<Point> vertices;
public:
	Rhombus(const string& name, const vector<Point>& vertices) : Quadrilateral(name, vertices) {
		if (!HasAllEqualSides()) throw 0; 

		this->name = name;
		this->vertices = vertices;  
	}
};

// write a class for Parallelogram
class Parallelogram : public Quadrilateral {
private:
	string name;
	vector<Point> vertices;
public:
	Parallelogram(const string& name, const vector<Point>& vertices) : Quadrilateral(name, vertices) {
		if (!HasAllEqualAngles()) throw 0; 

		this->name = name;
		this->vertices = vertices;  
	}
};

// write a class for Kite
class Kite : public Quadrilateral {
private:
	string name;
	vector<Point> vertices;
public:
	Kite(const string& name, const vector<Point>& vertices) : Quadrilateral(name, vertices) {
		if (numEqualSides() != 2) throw 0; 

		this->name = name;
		this->vertices = vertices;  
	}
};

// write a class for Trapezoid
class Trapezoid : public Quadrilateral {
private:
	string name;
	vector<Point> vertices;
public:
	Trapezoid(const string& name, const vector<Point>& vertices) : Quadrilateral(name, vertices) {
		if (numEqualSides() != 0) throw 0; 

		this->name = name;
		this->vertices = vertices;  
	}
};



#endif