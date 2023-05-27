
//MAIN CHE SERVE A NOI PER FARE DELLE PROVE, ES. STAMPARE A SCHERMO (?)
#include "empty_class.hpp"

int main()
{
    //Funzione da file a array di punti
    //Costruzione T1
    void Costr_T1(&Grid, &array)
    {

    }
    //Costruzione T2



    //////

    DelaunayLibrary::Point pointTry1, pointTry2, pointTry3, externalPoint, internalPoint, borderPoint;
    pointTry1 = DelaunayLibrary::Point(7, 4);
    pointTry2 = DelaunayLibrary::Point(1, 0);
    pointTry3 = DelaunayLibrary::Point(3, 6);

    externalPoint = DelaunayLibrary::Point(5, 5);
    internalPoint = DelaunayLibrary::Point(4,6);
    borderPoint = DelaunayLibrary::Point(3,2);

    DelaunayLibrary::Triangle triangle;
    triangle = DelaunayLibrary::Triangle(pointTry1, pointTry2, pointTry3);

    int est = triangle.ContainsPoint(externalPoint);
    int inte = triangle.ContainsPoint(internalPoint);
    int bord = triangle.ContainsPoint(borderPoint);
    cout<<"Punto esterno: "<<est<<"\nPunto interno: "<<inte<<"\nPunto sul lato: "<<bord<<endl;


    return 0;
}
