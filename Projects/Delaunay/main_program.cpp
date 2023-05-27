
//MAIN CHE SERVE A NOI PER FARE DELLE PROVE, ES. STAMPARE A SCHERMO (?)
#include "empty_class.hpp"
using namespace DelaunayLibrary;

int main()
{
    //Funzione da file a array di punti
    //Costruzione T1
    void Costr_T1(&Grid, &array)
    {

    }
    //Costruzione T2



    ////// PROVE

    //DelaunayLibrary::Point pointTry1, pointTry2, pointTry3, externalPoint, internalPoint, borderPoint;
    Point points[10];

    points[0] = Point(7, 4);
    points[1] = Point(1, 0);
    points[2] = Point(3, 6);
    points[3] = Point(3, 8);
    points[4] = Point(0, 4);
    points[5] = Point(1, 6);
    points[6] = Point(7, 2);
    points[7] = Point(1, 1);
    points[8] = Point(4, 5);
    points[9] = Point(4, 0);

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
