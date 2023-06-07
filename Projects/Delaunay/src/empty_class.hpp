
#ifndef __EMPTY_H
#define __EMPTY_H
#include "Eigen/Eigen"
#include <unordered_set>

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

    void reorderPointsCounterclockwiseTr(Point& p1, Point& p2, Point& p3);
    void reorderPointsCounterclockwiseQ(Point& p1, Point& p2, Point& p3, Point& p4);
    double determinante(double& a11, double& a12, double& a13, double& a21, double& a22, double& a23, double& a31, double& a32, double& a33);



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
        int adiacentTriangles[3];
        vector<Triangle> PointedTriangles;
    public:
        Triangle() = default;
        Triangle(Point& a, Point& b, Point& c) {vertices[0]=a;vertices[1]=b;vertices[2]=c;}
        void Show() const { cout<< "TRIANGLE\n  a: "<<vertices[0].toString()<< "\n  b: "<<vertices[1].toString()<< "\n  c: "<<vertices[2].toString()<< endl; }
        int ContainsPoint(Point& point);

    };

    bool CheckConvex(Triangle& Triangle1, Triangle& Triangle2);
    bool DelunayProperty(Triangle& Triangle1, Triangle& Triangle2);

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
        Square SquareOf(Point& point);
        void Show();
    };


    class Mesh
    {
    public:  //Da capire
        //ConvexHull da capire, aggiungere triangoli guida (?)
        vector<Triangle> meshTriangles;
        list<Point> convexHull;
        list<Triangle> hullTriangles;
        list<Triangle> lastMesh;
    public:
        Mesh() = default;
        Mesh(Triangle& triangle) {meshTriangles.push_back(triangle);}  //Aggiungere Convex Hull al costruttore (capire come)
        void DelunayPropagation(Triangle& startTriangle);
        void Delunay(vector<Triangle>& new_triangles);
        void Flip(Triangle& Triangle1, Triangle& Triangle2);
        void OperationEdges(Triangle& Triangle1, Triangle& Triangle2, Triangle& Triangle3, Triangle& Triangle4);
    };
}

#endif // __EMPTY_H
