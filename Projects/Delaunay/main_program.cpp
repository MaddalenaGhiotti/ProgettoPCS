
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

    //Vector di punti per prove
    vector<Point> points;

    points.push_back(Point(7.0, 4.0));
    points.push_back(Point(1.0, 0.0));
    points.push_back(Point(3.0, 6.0));
    points.push_back(Point(3.0, 8.0));
    points.push_back(Point(0.0, 4.0));
    points.push_back(Point(1.0, 6.0));
    points.push_back(Point(7.0, 2.0));
    points.push_back(Point(1.0, 1.0));
    points.push_back(Point(4.0, 5.0));
    points.push_back(Point(4.0, 0.0));

    Grid grid = Grid(points);
    grid.Show();

    //DelaunayLibrary::Triangle triangle;
    //triangle = DelaunayLibrary::Triangle(pointTry1, pointTry2, pointTry3);

    //int est = triangle.ContainsPoint(externalPoint);
    //int inte = triangle.ContainsPoint(internalPoint);
    //int bord = triangle.ContainsPoint(borderPoint);
    //cout<<"Punto esterno: "<<est<<"\nPunto interno: "<<inte<<"\nPunto sul lato: "<<bord<<endl;


    return 0;
}
