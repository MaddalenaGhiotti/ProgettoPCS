
#ifndef __EMPTY_H
#define __EMPTY_H
#include "Eigen/Eigen"
#include <unordered_set>

// DEFINIZIONE DI TUTTE LE CLASSI
#include <iostream>
#include <string>
//#include <sstream>

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
        friend bool operator==(const Point& point1, const Point& point2){return (point1.x==point2.x && point1.y==point2.y);}
    };

    inline ostream& operator<<(ostream& os, const Point& point)
    {os<<"x="<<to_string(point.x)<<"  y="<<to_string(point.y)<<endl;
        return os;
    }

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
        vector<Triangle> PointedTriangles;
        vector<Triangle*> adiacentTriangles;
    public:
        Triangle() = default;
        Triangle(Point& a, Point& b, Point& c);
        array<Point,3> OrderVertices();
<<<<<<< HEAD
        //void Show() const { cout<< "TRIANGLE\n  a: "<<vertices[0].toString()<< "\n  b: "<<vertices[1].toString()<< "\n  c: "<<vertices[2].toString()<< endl; }
=======
        //void Show() const {cout<< "TRIANGLE\n  a: "<<vertices[0].toString()<< "\n  b: "<<vertices[1].toString()<< "\n  c: "<<vertices[2].toString()<< endl;}
>>>>>>> 0c3536f388ad430e607a42202c7054cc5248ab02
        int ContainsPoint(Point& point);
        static void SetAdiacentTriangle(Triangle& triangle1,Triangle& triangle2, Point& tail, Point& head);

    };

    inline ostream& operator<<(ostream& os, const Triangle& triangle)
    {os<<"TRIANGLE\n  a: "<<triangle.vertices[0]<< "  b: "<<triangle.vertices[1]<< "  c: "<<triangle.vertices[2];
        return os;
    }

    bool CheckConvex(Triangle& Triangle1, Triangle& Triangle2);
    bool DelunayProperty(Triangle& Triangle1, Triangle& Triangle2);

    class Rectangle
    {
    public:
        double startX;
        double startY;
        vector<Triangle> crossingTriangles;
        vector<Point> containedPoints;
    public:
        Rectangle() = default;
        Rectangle(double& startX, double& startY): startX(startX), startY(startY) {}
        string toString() const {return "startX="+to_string(startX)+"  startY="+to_string(startY);}
    };


    class Grid
    {
    public:
        //Altezza rettangolo e larghezza rettangolo
        int intNum;
        Eigen::Matrix<Rectangle, Eigen::Dynamic, Eigen::Dynamic> rectangles;
        double x_min;
        double y_min;
    public:
        Grid() = default;
        Grid(vector<Point>& points);
        Rectangle RectangleOf(Point& point);
        void Show();
        double intervalX;
        double intervalY;
        void pointsInRectangle(vector<Point> &points);
        array<Point, 4> PickFourRandomPoints(vector<Point>& points);
        array<Point, 4> Snake();
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
        //convexHullElem(Point& point, Triangle& triangle): hullPoint(new Point(point)), externalTriangle(new Triangle(triangle)) {}
        convexHullElem(Point& point, Triangle& triangle): hullPoint(&point), externalTriangle(&triangle) {}
        convexHullElem(Point* point, Triangle& triangle): hullPoint(point), externalTriangle(&triangle) {}
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
        list<Triangle> lastMesh;
    public:
        Mesh() = default;
        Mesh(Triangle& triangle);  //Aggiungere Convex Hull al costruttore (capire come)
        void DelunayPropagation(Triangle& startTriangle);
        void Delunay(vector<Triangle>& new_triangles);
        void Flip(Triangle& Triangle1, Triangle& Triangle2);
        void OperationEdges(Triangle& Triangle1, Triangle& Triangle2, Triangle& Triangle3, Triangle& Triangle4);
        void AddExternalPoint(Point& point);
        void SetConvexHull(convexHullElem* elem) {convexHull=elem;}
    };
}

#endif // __EMPTY_H
