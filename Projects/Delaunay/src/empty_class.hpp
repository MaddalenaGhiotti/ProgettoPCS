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
    public:
        Point() = default;
        Point(double x, double y): x(x), y(y) {}
        static array<Point,2> OrederSide(Point& point1, Point& point2);
        friend bool operator==(const Point& point1, const Point& point2){return (point1.x==point2.x && point1.y==point2.y);}
        friend bool operator!=(const Point& point1, const Point& point2){return (point1.x!=point2.x || point1.y!=point2.y);}
        friend bool operator<(const Point& point1, const Point& point2){if (point1.x!=point2.x){return point1.x<point2.x;} else {return point1.y<point2.y;}}
        static array<Point,2> OrderSide(Point& point1, Point& point2);
    };

    inline ostream& operator<<(ostream& os, const Point& point)
    {os<<"x="<<to_string(point.x)<<"  y="<<to_string(point.y)<<endl;
        return os;
    }

    void reorderPointsCounterclockwiseTr(Point& p1, Point& p2, Point& p3);
    void reorderPointsCounterclockwiseQ(Point& p1, Point& p2, Point& p3, Point& p4);
    double determinante(double& a11, double& a12, double& a13, double& a21, double& a22, double& a23, double& a31, double& a32, double& a33);

    class Triangle
    {
    public: //Da capire
        Point vertices[3];
        vector<Triangle*> pointedTriangles;
        array<Triangle*,3> adiacentTriangles;
    public:
        Triangle() = default;
        Triangle(Point& a, Point& b, Point& c);
        array<Point,3> OrderVertices();
        int ContainsPoint(Point& point);
        Triangle* FromRootToLeaf(Point& point);
        static void SetAdiacentTriangle(Triangle& triangle1,Triangle* triangle2, Point& tail, Point& head);
        static void SetAdiacentTriangleMod(Triangle* triangle1,Triangle* triangle2, Point* Punto1, Point* Punto2);
        static array<Point*,4> FindCommonEdge(Triangle& triangle1, Triangle& triangle2);
        // i primi due punti sono i punti del lato in comune, gli altri due sono i punti "esterni"
        static array<Triangle*, 2> Flip(Triangle& Triangle1, Triangle& Triangle2);
        // per flippare due triangoli
        static void adjourn(Triangle* triangle_new_1, Triangle* triangolo1, Triangle* triangolo2);
        static array<Point*, 2> findOrderedEdge(Triangle* triangle1, Point* Punto1, Point* Punto2);
        friend bool operator==(const Triangle& triangle1, const Triangle& triangle2){return (triangle1.vertices[0]==triangle2.vertices[0] && triangle1.vertices[1]==triangle2.vertices[1] && triangle1.vertices[2]==triangle2.vertices[2]);}
    };

    inline ostream& operator<<(ostream& os, const Triangle& triangle)
    {os<<"TRIANGLE\n  a: "<<triangle.vertices[0]<< "  b: "<<triangle.vertices[1]<< "  c: "<<triangle.vertices[2];
        return os;
    }

    bool CheckConvex(Triangle& Triangle1, Triangle& Triangle2);
    bool DelunayProperty(Triangle& Triangle1, Triangle& Triangle2);


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
        vector<Point*> getPointsVector(){return pointsVector;}
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
        string toString() const {return "startX="+to_string(startX)+"  startY="+to_string(startY);}
        friend bool operator==(const Rectangle& rectangle1, const Rectangle& rectangle2){return (rectangle1.startX==rectangle2.startX && rectangle1.startY==rectangle2.startY);}
    };


    class Grid
    {
    public:
        //Altezza rettangolo e larghezza rettangolo
        int intNum;
        Eigen::Matrix<Rectangle, Eigen::Dynamic, Eigen::Dynamic> rectangles;
        vector<Point*> PointsGrid;
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


    class convexHullElem
    {
    public:
        Point *hullPoint;
        Triangle *externalTriangle; //Triangolo adiacente al lato esterno precedente il punto (andando in senso antiorario)
        convexHullElem *prev;
        convexHullElem *next;
    public:
        convexHullElem() = default;
        //convexHullElem(Point& point, Triangle& triangle): hullPoint(new Point(point)), externalTriangle(new Triangle(triangle)) {}
        convexHullElem(Point& point, Triangle& triangle): hullPoint(&point), externalTriangle(&triangle) {}
        //convexHullElem(Point* point, Triangle& triangle): hullPoint(point), externalTriangle(&triangle) {}
        void SetPrev(convexHullElem* previous){prev=previous;}
        void SetNext(convexHullElem* nextOne){next=nextOne;}
        void SetTriangle(Triangle* triangle){externalTriangle=triangle;}
    };

    class Mesh
    {
    public:  //Da capire
        //ConvexHull da capire, aggiungere triangoli guida (?)
        vector<Triangle> meshTriangles;
        vector<Triangle*> guideTriangles;
        convexHullElem *convexHull;
        list<Triangle> lastMesh;
    public:
        Mesh() = default;
        Mesh(Triangle& triangle);  //Aggiungere Convex Hull al costruttore (capire come)
        void DelunayPropagation(Triangle* startTriangle);
        void Delunay(vector<Triangle*> new_triangles);
        void OperationEdges(Triangle& Triangle1, Triangle& Triangle2, Triangle& Triangle3, Triangle& Triangle4);
        void AddExternalPoint(Point& point);
        void AddInternalPoint(Point& point, Triangle* rootTriangle);
        void AddSidePoint(Point& point, Triangle* bigTriangle, int side);
        void SetConvexHull(convexHullElem* elem) {convexHull=elem;}
        int CheckInside(Point point);
    };
}

#endif // __EMPTY_H
