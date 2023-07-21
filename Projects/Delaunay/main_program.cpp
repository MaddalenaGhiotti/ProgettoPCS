//Versione più aggiornata
//MAIN CHE SERVE A NOI PER FARE DELLE PROVE, ES. STAMPARE A SCHERMO
#include "empty_class.hpp"
#include <iostream>

using namespace DelaunayLibrary;

int main()
{

//_______________________________________________________________________________________________________________________
//GENERALE

    string inputFile = "Test1.csv";
    Delaunay delaunay = Delaunay(inputFile);
    delaunay.Show();
    delaunay.ExecuteDelaunay();
    for (Point* pointPtr:delaunay.pointsVector){delete pointPtr;}


//_______________________________________________________________________________________________________________________
////ESEMPIO MESH

    // Punti
//    Point point0 = Point(4,2);
//    Point point1 = Point(2,4);
//    Point point2 = Point(6,4);
//    Point point3 = Point(4,6);
//    Point point4 = Point(8,8);
//    Point point5 = Point(8,6);

//    //Triangoli
//    Triangle triangle0 = Triangle(point0, point1, point2);
//    Triangle triangle1 = Triangle(point1, point2, point3);
//    Triangle triangle2 = Triangle(point2, point3, point4);
//    Triangle triangle3 = Triangle(point2, point4, point5);

//    //Aggiunta triangoli
//    Mesh mesh = Mesh(triangle0);
//    // cout<<"Stampa riga di verifica"<<endl;
//    // cout<<*(mesh.convexHull->hullPoint)<<endl;

//    mesh.meshTriangles.push_back(triangle1);
//    mesh.meshTriangles.push_back(triangle2);
//    mesh.meshTriangles.push_back(triangle3);

//    mesh.guideTriangles.push_back(&triangle1);
//    mesh.guideTriangles.push_back(&triangle2);
//    mesh.guideTriangles.push_back(&triangle3);

//////    //Elementi convex hull
//////    convexHullElem* elem0 = mesh.convexHull;
//////    //cout<<*(mesh.convexHull->hullPoint)<<endl;
//////    //cout<<*(elem0->hullPoint)<<endl;
//////    convexHullElem* elem1 = new convexHullElem(point6, triangle4);
//////    convexHullElem* elem2 = new convexHullElem(point2, triangle4);
//////    convexHullElem* elem3 = new convexHullElem(point5, triangle3);
//////    convexHullElem* elem4 = new convexHullElem(point1, triangle3);
//////    convexHullElem* elem5 = new convexHullElem(point3, triangle1);
//////    convexHullElem* elem6 = new convexHullElem(point4, triangle2);

//////    //Aggiornamento triangoli convex hull
//////    elem0->SetTriangle(&triangle2);
//////    elem2->SetTriangle(&triangle4);
//////    elem4->SetTriangle(&triangle3);

//////    //Connessioni convex hull
//////    elem0->SetNext(elem1);
//////    elem1->SetNext(elem2);
//////    elem2->SetNext(elem3);
//////    elem3->SetNext(elem4);
//////    elem4->SetNext(elem5);
//////    elem5->SetNext(elem6);
//////    elem6->SetNext(elem0);

//////    elem0->SetPrev(elem6);
//////    elem1->SetPrev(elem0);
//////    elem2->SetPrev(elem1);
//////    elem3->SetPrev(elem2);
//////    elem4->SetPrev(elem3);
//////    elem5->SetPrev(elem4);
//////    elem6->SetPrev(elem5);

//    //Settaggio triangoli adiacenti
//    Triangle::SetAdiacentTriangleMod(triangle1, &triangle2, &point2, &point3);
//    Triangle::SetAdiacentTriangleMod(triangle0, &triangle1, &point1, &point2);
//    Triangle::SetAdiacentTriangleMod(triangle3, &triangle2, &point3, &point4);

//////___________________________________________________________________________________________________________________//_______________________________________________________________________________________________________________________
////Stampo un tentativo di Delaunay
//    vector<Triangle*> new_triangles;
//    new_triangles.push_back(&triangle3);
//    mesh.Delunay(new_triangles);

//    //OUTPUT
//    delaunay.MeshToEdges(mesh.guideTriangles);
//    for (array<Point,2> side:delaunay.finalEdges){cout<<"Lato\n"<<side[0]<<side[1]<<endl;}
//    delaunay.OutputEdges();
//    cout<<"Numero totale di lati: "<<delaunay.finalEdges.size()<<endl;
//_______________________________________________________________________________________________________________________
//CHIUSURA FUNZIONE
    return 0;
}




