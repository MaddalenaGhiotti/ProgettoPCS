
//MAIN CHE SERVE A NOI PER FARE DELLE PROVE, ES. STAMPARE A SCHERMO (?)
#include "empty_class.hpp"

int main()
{


    DelaunayLibrary::Point pointTry1, pointTry2, pointTry3, externalPoint, internalPoint, borderPoint;
    pointTry1 = DelaunayLibrary::Point(0, 0);
    pointTry2 = DelaunayLibrary::Point(0, 1);
    pointTry3 = DelaunayLibrary::Point(1, 0);

    externalPoint = DelaunayLibrary::Point(1, 1);
    internalPoint = DelaunayLibrary::Point(0.3, 0.3);
    borderPoint = DelaunayLibrary::Point(0.5, 0.0);

    DelaunayLibrary::Triangle triangle;
    triangle = DelaunayLibrary::Triangle(pointTry1, pointTry2, pointTry3);

    int est = triangle.ContainsPoint(externalPoint);
    int inte = triangle.ContainsPoint(internalPoint);
    int bord = triangle.ContainsPoint(borderPoint);
    cout<<"Punto esterno: "<<est<<"\nPunto interno: "<<inte<<"\nPunto sul lato: "<<bord<<endl;


    return 0;
}
