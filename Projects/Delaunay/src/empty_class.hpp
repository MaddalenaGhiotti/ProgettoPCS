#ifndef __EMPTY_H
#define __EMPTY_H

// DEFINIZIONE DI TUTTE LE CLASSI
#include <iostream>

using namespace std;

namespace DelaunayLibrary
{
    class Point
    {
    public: //Da capire
        float x, y;
    public:
        Point() = default;
        Point(float x, float y): x(x), y(y) {}
        string toString() const {return "x="+to_string(x)+"  y="+to_string(y);}
        void Show() const {cout<<toString()<<endl;}
    };

    class Triangle
    {
    public: //Da capire
        Point vertices[3];
        Triangle* adiacentTriangles[3];
    public:
        Triangle() = default;
        Triangle(Point a, Point b, Point c) {vertices[0]=a;vertices[1]=b;vertices[2]=c;}
        void Show() const { cout<< "TRIANGLE\n  a: "<<vertices[0].toString()<< "\n  b: "<<vertices[1].toString()<< "\n  c: "<<vertices[2].toString()<< endl; }
        int ContainsPoint(Point point);
    };
}

#endif // __EMPTY_H
