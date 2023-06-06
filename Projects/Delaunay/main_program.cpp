
//MAIN CHE SERVE A NOI PER FARE DELLE PROVE, ES. STAMPARE A SCHERMO
#include "empty_class.hpp"
using namespace DelaunayLibrary;

int main()
{

//_______________________________________________________________________________________________________________________
//DA FILE A ARRAY DI PUNTI

    string inputFile = "test1.csv";
    Delaunay delaunay = Delaunay(inputFile);
    delaunay.Show();


//_______________________________________________________________________________________________________________________
//CREAZIONE E STAMPA GRIGLIA

    Grid grid = Grid(delaunay.pointsVector);
    grid.Show();


//_______________________________________________________________________________________________________________________
//ESEMPIO MESH

    //Punti
    Point point0 = Point(4,0);
    Point point1 = Point(4,5);
    Point point2 = Point(7,4);
    Point point3 = Point(3,3);
    Point point4 = Point(3,0);
    Point point5 = Point(5,7);
    Point point6 = Point(7,2);

    Point externalPoint = Point(0,3);

    //Triangoli
    Triangle triangle0 = Triangle(point0, point1, point2);
    Triangle triangle1 = Triangle(point0, point1, point3);
    Triangle triangle2 = Triangle(point0, point3, point4);
    Triangle triangle3 = Triangle(point1, point2, point5);
    Triangle triangle4 = Triangle(point0, point2, point6);

    //Aggiunta triangoli
    Mesh mesh = Mesh(triangle0);
    mesh.meshTriangles.push_back(triangle1);
    mesh.meshTriangles.push_back(triangle2);
    mesh.meshTriangles.push_back(triangle3);
    mesh.meshTriangles.push_back(triangle4);

    mesh.guideTriangles.push_back(triangle1);
    mesh.guideTriangles.push_back(triangle2);
    mesh.guideTriangles.push_back(triangle3);
    mesh.guideTriangles.push_back(triangle4);

    //Elementi convex hull
    convexHullElem* elem0 = mesh.convexHull;
    convexHullElem* elem1 = new convexHullElem(point6, triangle4);
    convexHullElem* elem2 = elem0->next;  //new convexHullElem(point2, triangle4);
    convexHullElem* elem3 = new convexHullElem(point5, triangle3);
    convexHullElem* elem4 = elem2->next;  //new convexHullElem(point1, triangle3);
    convexHullElem* elem5 = new convexHullElem(point3, triangle1);
    convexHullElem* elem6 = new convexHullElem(point4, triangle2);

    //Connessioni convex hull
    elem0->SetNext(elem1);
    elem1->SetNext(elem2);
    elem2->SetNext(elem3);
    elem3->SetNext(elem4);
    elem4->SetNext(elem5);
    elem5->SetNext(elem6);
    elem6->SetNext(elem0);

    elem0->SetPrev(elem6);
    elem1->SetPrev(elem0);
    elem2->SetPrev(elem1);
    elem3->SetPrev(elem2);
    elem4->SetPrev(elem3);
    elem5->SetPrev(elem4);
    elem6->SetPrev(elem5);

//_______________________________________________________________________________________________________________________
//AGGIUNTA PUNTO ESTERNO

    //Stampa punti convex hull
    cout<<"PUNTI CONVEX HULL INIZIALE"<<endl;
    convexHullElem* currentElem1 = elem0;
    for (int i=0; i<7; i++)
    {
        currentElem1->hullPoint->Show();
        currentElem1 = currentElem1->next;
    }
    cout<<endl;

    //Aggiunta di punto esterno
    mesh.AddExternalPoint(externalPoint);

    //Stampa punti convex hull
    cout<<"PUNTI NUOVO CONVEX HULL"<<endl;
    convexHullElem* currentElem2 = elem0;
    for (int i=0; i<6; i++)
    {
        currentElem2->hullPoint->Show();
        currentElem2 = currentElem2->next;
    }
    cout<<endl;


    return 0;
}
