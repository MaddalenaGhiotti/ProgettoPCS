
//MAIN CHE SERVE A NOI PER FARE DELLE PROVE, ES. STAMPARE A SCHERMO (?)
#include "empty_class.hpp"
using namespace DelaunayLibrary;

int main()
{
    //Test da file a array di punti
//    string inputFile = "../Dataset/test1.csv";
//    Delaunay delaunay = Delaunay(inputFile);
//    delaunay.ImportPoints();
//    delaunay.Show();

    //Costruzione T1
//    void Costr_T1(&Grid, &array)
//    {

//    }
    //Costruzione T2



    ////// PROVE

    //DelaunayLibrary::Point pointTry1, pointTry2, pointTry3, externalPoint, internalPoint, borderPoint;
    vector<Point> points;

    double x1=7.0, y1=4.0;
    double x2=1.0, y2=0.0;
    double x3=3.0, y3=6.0;
    double x4=3.0, y4=8.0;
    double x5=0.0, y5=4.0;
    double x6=1.0, y6=6.0;
    double x7=7.0, y7=2.0;
    double x8=1.0, y8=1.0;
    double x9=4.0, y9=5.0;
    double x10=4.0, y10=0.0;



    points[0] = Point(x1, y1);
    points[1] = Point(x2, y2);
    points[2] = Point(x3, y3);
    points[3] = Point(x4, y4);
    points[4] = Point(x5, y5);
    points[5] = Point(x6, y6);
    points[6] = Point(x7, y7);
    points[7] = Point(x8, y8);
    points[8] = Point(x9, y9);
    points[9] = Point(x10, y10);


//    points[1] = Point(1, 0);
//    points[2] = Point(3, 6);
//    points[3] = Point(3, 8);
//    points[4] = Point(0, 4);
//    points[5] = Point(1, 6);
//    points[6] = Point(7, 2);
//    points[7] = Point(1, 1);
//    points[8] = Point(4, 5);
//    points[9] = Point(4, 0);

    Grid(points).Show();

    //externalPoint = DelaunayLibrary::Point(5, 5);
    //internalPoint = DelaunayLibrary::Point(4,6);
    //borderPoint = DelaunayLibrary::Point(3,2);

    //DelaunayLibrary::Triangle triangle;
    //triangle = DelaunayLibrary::Triangle(pointTry1, pointTry2, pointTry3);

    //int est = triangle.ContainsPoint(externalPoint);
    //int inte = triangle.ContainsPoint(internalPoint);
    //int bord = triangle.ContainsPoint(borderPoint);
    //cout<<"Punto esterno: "<<est<<"\nPunto interno: "<<inte<<"\nPunto sul lato: "<<bord<<endl;


    return 0;
}
