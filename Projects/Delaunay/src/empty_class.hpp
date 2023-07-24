#ifndef __EMPTY_H
#define __EMPTY_H
#include "Eigen/Eigen"
#include <array>
#include <unordered_set>
#include <iostream>
#include <string>

using namespace std;

namespace DelaunayLibrary
{

    class Point
    {
    public:
        double x, y;
    public:
        Point() = default;
        Point(double x, double y): x(x), y(y) {}
        static array<Point,2> OrderSide(Point& point1, Point& point2);
        friend bool operator==(const Point& point1, const Point& point2){return (point1.x==point2.x && point1.y==point2.y);}
        friend bool operator!=(const Point& point1, const Point& point2){return (point1.x!=point2.x || point1.y!=point2.y);}
        friend bool operator<(const Point& point1, const Point& point2){if (point1.x!=point2.x){return point1.x<point2.x;} else {return point1.y<point2.y;}}
        friend ostream& operator<<(ostream& os, const Point& point){os<<"x="<<to_string(point.x)<<"  y="<<to_string(point.y)<<endl; return os;}
    };


    double determinante(double& a11, double& a12, double& a13, double& a21, double& a22, double& a23, double& a31, double& a32, double& a33);

    class Triangle
    {
    public:
        array<Point,3> vertices;
        vector<Triangle*> pointedTriangles;
        array<Triangle*,3> adjacentTriangles;
    public:
        Triangle() = default;
        Triangle(Point& a, Point& b, Point& c);
        int ContainsPoint(Point& point);
        Triangle* FromRootToLeaf(Point& point);
        static void SetAdjacentTriangle(Triangle& triangle1,Triangle* triangle2, Point& tail, Point& head);
        static array<Point*,4> FindCommonEdge(Triangle& triangle1, Triangle& triangle2);
        static array<Triangle*, 2> Flip(Triangle& Triangle1, Triangle& Triangle2);
        static void Adjourn(Triangle* triangle_new_1, Triangle* triangolo1, Triangle* triangolo2);
        static bool DelaunayProperty(Triangle& Triangle1, Triangle& Triangle2);
        friend bool operator==(const Triangle& triangle1, const Triangle& triangle2){return (triangle1.vertices[0]==triangle2.vertices[0] && triangle1.vertices[1]==triangle2.vertices[1] && triangle1.vertices[2]==triangle2.vertices[2]);}
        friend ostream& operator<<(ostream& os, const Triangle& triangle)
        {os<<"TRIANGLE\n  a: "<<triangle.vertices[0]<< "  b: "<<triangle.vertices[1]<< "  c: "<<triangle.vertices[2];
            return os;
        }
    };


    class Delaunay
    {
    public:
        string fileName;
        vector<Point*> pointsVector;
        vector<array<Point,2>> finalEdges;
    public:
        Delaunay() = default;
        Delaunay(const string& inputFileName);
        void ExecuteDelaunay();
        void Show();
        void MeshToEdges(vector<Triangle*> guideTriangles);
        void OutputEdges();
        void FromGraphToTree(vector<Triangle*>& trPtrVec);
        void DeleteTriangles(vector<Triangle*>& trPtrVec);
        void DeletePoints();
    };

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
        string toString() const {return "startX="+to_string(startX/100.0)+"  startY="+to_string(startY/100.0);}
        friend bool operator==(const Rectangle& rectangle1, const Rectangle& rectangle2){return (rectangle1.startX==rectangle2.startX && rectangle1.startY==rectangle2.startY);}
    };


    class Grid
    {
    public:
        int intNum;
        Eigen::Matrix<Rectangle, Eigen::Dynamic, Eigen::Dynamic> rectangles;
        vector<Point*> pointsGrid;
        double x_min;
        double y_min;
        double intervalX;
        double intervalY;
    public:
        Grid() = default;
        Grid(vector<Point*> points);
        Rectangle RectangleOf(Point& point);
        void Show();
        void PointsInRectangle(vector<Point*> points);
        array<Point, 4> PickFourRandomPoints(vector<Point*> points);
        array<Point, 4> Snake();
    };


    class ConvexHullElem
    {
    public:
        Point *hullPoint;
        Triangle *externalTriangle; //Triangolo adiacente al lato esterno precedente il punto (andando in senso antiorario)
        ConvexHullElem *prev;
        ConvexHullElem *next;
    public:
        ConvexHullElem() = default;
        ConvexHullElem(Point& point, Triangle& triangle): hullPoint(&point), externalTriangle(&triangle) {}
        void SetPrev(ConvexHullElem* previous){prev=previous;}
        void SetNext(ConvexHullElem* nextOne){next=nextOne;}
        void SetTriangle(Triangle* triangle){externalTriangle=triangle;}
    };

    class Mesh
    {
    public:
        vector<Triangle*> guideTriangles;
        ConvexHullElem* convexHull;
    public:
        Mesh() = default;
        Mesh(Triangle& triangle);
        void DelaunayPropagation(Triangle* startTriangle);
        void DelaunayNewTriangles(vector<Triangle*> new_triangles);
        void AddExternalPoint(Point& point);
        void AddInternalPoint(Point& point, Triangle* rootTriangle);
        void AddSidePoint(Point& point, Triangle* bigTriangle, int side);
        int CheckInside(Point point);
        void DeleteConvexHull();
    };
}

#endif // __EMPTY_H
