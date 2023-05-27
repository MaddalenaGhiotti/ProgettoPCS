#ifndef __EMPTY_H
#define __EMPTY_H
#include "Eigen/Eigen"

// DEFINIZIONE DI TUTTE LE CLASSI
#include <iostream>

using namespace std;

namespace DelaunayLibrary
{
    class Delaunay
    {

    };

    class Point
    {
    public: //Da capire
        double x, y;
    public:
        Point() = default;
        Point(double x, double y): x(x), y(y) {}
        string toString() const {return "x="+to_string(x)+"  y="+to_string(y);}
        void Show() const {cout<<toString()<<endl;}
    };

    class Triangle
    {
    public: //Da capire
        Point vertices[3];
        int adiacentTriangles[3];
    public:
        Triangle() = default;
        Triangle(Point a, Point b, Point c) {vertices[0]=a;vertices[1]=b;vertices[2]=c;}
        void Show() const { cout<< "TRIANGLE\n  a: "<<vertices[0].toString()<< "\n  b: "<<vertices[1].toString()<< "\n  c: "<<vertices[2].toString()<< endl; }
        int ContainsPoint(Point point);
    };

    class Mesh
    {
    public:  //Da capire
        vector<Triangle> meshTriangles;
        list<Point> convexHull;
        list<Triangle> hullTriangles;
    public:
        Mesh() = default;
        Mesh(Triangle triangle) {meshTriangles.push_back(triangle);}


    };

    class Square
    {
    public:
        double startX;
        double startY;
        vector<Triangle> crossingTriangles;
        vector<Point> crossingPoints;
    public:
        Square() = default;
        Square(double startX, double startY): startX(startX), startY(startY) {}
        string toString() const {return "startX="+to_string(startX)+"  startY="+to_string(startY);}
    };

    class Grid
    {
    public:
        int n;
        int intNum;
        Matrix<Square*,intNum,intNum> squares;
        double x_min;
        double y_min;
    public:
        Grid() = default;
        Grid(Point points[n]) {}
        Square SquareOf(Point point){}
        void Show() const {cout<<squares;}
    };
}

#endif // __EMPTY_H
