#ifndef __EMPTY_H
#define __EMPTY_H
#include "Eigen/Eigen"

// DEFINIZIONE DI TUTTE LE CLASSI
#include <iostream>

using namespace std;

namespace DelaunayLibrary
{

    class Point
    {
    public: //Da capire
        double x, y;
        //inRectangle Tipo rettangolo a cui il punto è interno
    public:
        Point() = default;
        Point(double x, double y): x(x), y(y) {}
        string toString() const {return "x="+to_string(x)+"  y="+to_string(y);}
        void Show() const {cout<<toString()<<endl;}
    };


    class Delaunay
    {
    public:
        string fileName;
        vector<Point> pointsVector;
    public:
        Delaunay() = default;
        Delaunay(const string& inputFileName);
        void ExecuteDelaunay();
        void Show();
    };


    class Triangle
    {
    public: //Da capire
        Point vertices[3];
        vector<Triangle*> adiacentTriangles;
    public:
        Triangle() = default;
        Triangle(Point& a, Point& b, Point& c) {vertices[0]=a;vertices[1]=b;vertices[2]=c;}
        array<Point,3> OrderVertices();
        void Show() const { cout<< "TRIANGLE\n  a: "<<vertices[0].toString()<< "\n  b: "<<vertices[1].toString()<< "\n  c: "<<vertices[2].toString()<< endl; }
        int ContainsPoint(Point& point);
    };


    class Square //Cambiare in Rectangle
    {
    public:
        double startX;
        double startY;
        vector<Triangle> crossingTriangles;
        vector<Point> crossingPoints; //Cambiare crossing con un altro nome
    public:
        Square() = default;
        Square(double& startX, double& startY): startX(startX), startY(startY) {}
        string toString() const {return "startX="+to_string(startX)+"  startY="+to_string(startY);}
    };


    class Grid
    {
    public:
        //Altezza rettangolo e larghezza rettangolo
        int intNum;
        Eigen::Matrix<Square, Eigen::Dynamic, Eigen::Dynamic> squares;
        double x_min;
        double y_min;
    public:
        Grid() = default;
        Grid(vector<Point>& points);
        //Square SquareOf(Point& point){}
        void Show();
    };


    class convexHullElem
    {
    public:
        Point *hullPoint;
        Triangle *externalTriangle;
        convexHullElem *prev;
        convexHullElem *next;
    public:
        convexHullElem() = default;
        convexHullElem(Point& point, Triangle& triangle): hullPoint(new Point(point)), externalTriangle(&triangle) {}
        //convexHullElem(Point& point, Triangle& triangle): hullPoint(&point), externalTriangle(&triangle) {}
        void SetPrev(convexHullElem* previous){prev=previous;}
        void SetNext(convexHullElem* nextOne){next=nextOne;}
        void SetTriangle(Triangle* triangle){externalTriangle=triangle;}
    };

    class Mesh
    {
    public:  //Da capire
        //ConvexHull da capire, aggiungere triangoli guida (?)
        vector<Triangle> meshTriangles;
        vector<Triangle> guideTriangles;
        convexHullElem *convexHull;
    public:
        Mesh() = default;
        Mesh(Triangle& triangle);
        void AddExternalPoint(Point point);
    };
}

#endif // __EMPTY_H
