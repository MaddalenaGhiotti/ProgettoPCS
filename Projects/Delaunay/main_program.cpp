
//MAIN CHE SERVE A NOI PER FARE DELLE PROVE, ES. STAMPARE A SCHERMO
#include "empty_class.hpp"
#include <iostream>

using namespace DelaunayLibrary;

int main()
{

//_______________________________________________________________________________________________________________________
//GENERALE

    string inputFile = "Test2.csv";
    Delaunay delaunay = Delaunay(inputFile);
<<<<<<< HEAD
    delaunay.Show();
    delaunay.ExecuteDelaunay();
//    delaunay.MeshToEdges(mesh.guideTriangles);
//    //Stampa output e caricameno su file
//    for (array<Point,2> side:delaunay.finalEdges){cout<<"Lato\n"<<side[0]<<side[1]<<endl;}
//    delaunay.OutputEdges();
//    cout<<"Numero totale di lati: "<<delaunay.finalEdges.size()<<endl;
=======
    // delaunay.Show();

>>>>>>> EmilioZOrzi

//_______________________________________________________________________________________________________________________
//CREAZIONE E STAMPA GRIGLIA

<<<<<<< HEAD
//    Grid grid = Grid(delaunay.pointsVector);
//    grid.PointsInRectangle(delaunay.pointsVector);
//    array<Point, 4> firstPoints = grid.Snake();
//    cout << firstPoints[0];
//    cout << firstPoints[1];
//    cout << firstPoints[2];
//    cout << firstPoints[3];


////_______________________________________________________________________________________________________________________
////CHECK DELUNAY
//    Point a = Point(0, 2);
//    Point b = Point(4, 2);
//    Point c = Point(2, 1);
//    Point d = Point(2, 3);
//    Triangle Tri1 = Triangle(a, b, c);
//    Triangle Tri2 = Triangle(a, b, d);
//    if (CheckConvex(Tri1, Tri2) == 1)
//    {
//       cout << "True" << endl;
//    }
//    if (DelunayProperty(Tri1, Tri2) == 0)
//    {
//       cout << "da flippare" << endl;
//    }


////_______________________________________________________________________________________________________________________
////ESEMPIO MESH

//    //Punti
//    Point point0 = Point(4,0);
//    Point point1 = Point(4,5);
//    Point point2 = Point(7,4);
//    Point point3 = Point(3,3);
//    Point point4 = Point(3,0);
//    Point point5 = Point(5,7);
//    Point point6 = Point(7,2);

//    //Triangoli
//    Triangle triangle0 = Triangle(point0, point1, point2);
//    Triangle triangle1 = Triangle(point0, point1, point3);
//    Triangle triangle2 = Triangle(point0, point3, point4);
//    Triangle triangle3 = Triangle(point1, point2, point5);
//    Triangle triangle4 = Triangle(point0, point2, point6);

//    //Aggiunta triangoli
//    Mesh mesh = Mesh(triangle0);
//    cout<<"Stampa riga di verifica"<<endl;
//    cout<<*(mesh.convexHull->hullPoint)<<endl;

//    mesh.meshTriangles.push_back(triangle1);
//    mesh.meshTriangles.push_back(triangle2);
//    mesh.meshTriangles.push_back(triangle3);
//    mesh.meshTriangles.push_back(triangle4);

//    mesh.guideTriangles.push_back(triangle1);
//    mesh.guideTriangles.push_back(triangle2);
//    mesh.guideTriangles.push_back(triangle3);
//    mesh.guideTriangles.push_back(triangle4);

//    //Elementi convex hull
//    convexHullElem* elem0 = mesh.convexHull;
//    //cout<<*(mesh.convexHull->hullPoint)<<endl;
//    //cout<<*(elem0->hullPoint)<<endl;
//    convexHullElem* elem1 = new convexHullElem(point6, triangle4);
//    convexHullElem* elem2 = new convexHullElem(point2, triangle4);
//    convexHullElem* elem3 = new convexHullElem(point5, triangle3);
//    convexHullElem* elem4 = new convexHullElem(point1, triangle3);
//    convexHullElem* elem5 = new convexHullElem(point3, triangle1);
//    convexHullElem* elem6 = new convexHullElem(point4, triangle2);

//    //Aggiornamento triangoli convex hull
//    elem0->SetTriangle(&triangle2);
//    elem2->SetTriangle(&triangle4);
//    elem4->SetTriangle(&triangle3);

//    //Connessioni convex hull
//    elem0->SetNext(elem1);
//    elem1->SetNext(elem2);
//    elem2->SetNext(elem3);
//    elem3->SetNext(elem4);
//    elem4->SetNext(elem5);
//    elem5->SetNext(elem6);
//    elem6->SetNext(elem0);

//    elem0->SetPrev(elem6);
//    elem1->SetPrev(elem0);
//    elem2->SetPrev(elem1);
//    elem3->SetPrev(elem2);
//    elem4->SetPrev(elem3);
//    elem5->SetPrev(elem4);
//    elem6->SetPrev(elem5);

//    //Settaggio triangoli adiacenti
//    Triangle::SetAdiacentTriangle(triangle1, &triangle2, point3, point0);
//    Triangle::SetAdiacentTriangle(triangle0, &triangle1, point1, point0);
//    Triangle::SetAdiacentTriangle(triangle0, &triangle3, point2, point1);
//    Triangle::SetAdiacentTriangle(triangle0, &triangle4, point0, point2);
=======
    Grid grid = Grid(delaunay.pointsVector);
    // grid.Show();

//_______________________________________________________________________________________________________________________
//CHECK DELUNAY
//    Point a = Point(0, 2);
//    Point b = Point(4, 2);
//    Point c = Point(2, 1);
//    Point d = Point(2, 3);
//    Point e = Point(5, 4);
//    Triangle Tri1 = Triangle(a, b, c);
//    Triangle Tri2 = Triangle(a, b, d);
//    array<Point*,4> Output = Triangle::FindCommonEdge(Tri1, Tri2);
//    cout << "Prova" << endl;
//    for (int i = 0; i < 4; i++)
//    {
//        cout<<*Output[i]<<endl;
//    }
//    Triangle::Flip(Tri1, Tri2);

//_______________________________________________________________________________________________________________________
////ESEMPIO MESH

    // Punti
    Point point0 = Point(4,2);
    Point point1 = Point(2,4);
    Point point2 = Point(6,4);
    Point point3 = Point(4,6);
    Point point4 = Point(8,8);
    Point point5 = Point(8,6);

    //Triangoli
    Triangle triangle0 = Triangle(point0, point1, point2);
    Triangle triangle1 = Triangle(point1, point2, point3);
    Triangle triangle2 = Triangle(point2, point3, point4);
    Triangle triangle3 = Triangle(point2, point4, point5);

    //Aggiunta triangoli
    Mesh mesh = Mesh(triangle0);
    // cout<<"Stampa riga di verifica"<<endl;
    // cout<<*(mesh.convexHull->hullPoint)<<endl;

    mesh.meshTriangles.push_back(triangle1);
    mesh.meshTriangles.push_back(triangle2);
    mesh.meshTriangles.push_back(triangle3);

    mesh.guideTriangles.push_back(&triangle1);
    mesh.guideTriangles.push_back(&triangle2);
    mesh.guideTriangles.push_back(&triangle3);

//    //Elementi convex hull
//    convexHullElem* elem0 = mesh.convexHull;
//    //cout<<*(mesh.convexHull->hullPoint)<<endl;
//    //cout<<*(elem0->hullPoint)<<endl;
//    convexHullElem* elem1 = new convexHullElem(point6, triangle4);
//    convexHullElem* elem2 = new convexHullElem(point2, triangle4);
//    convexHullElem* elem3 = new convexHullElem(point5, triangle3);
//    convexHullElem* elem4 = new convexHullElem(point1, triangle3);
//    convexHullElem* elem5 = new convexHullElem(point3, triangle1);
//    convexHullElem* elem6 = new convexHullElem(point4, triangle2);

//    //Aggiornamento triangoli convex hull
//    elem0->SetTriangle(&triangle2);
//    elem2->SetTriangle(&triangle4);
//    elem4->SetTriangle(&triangle3);

//    //Connessioni convex hull
//    elem0->SetNext(elem1);
//    elem1->SetNext(elem2);
//    elem2->SetNext(elem3);
//    elem3->SetNext(elem4);
//    elem4->SetNext(elem5);
//    elem5->SetNext(elem6);
//    elem6->SetNext(elem0);

//    elem0->SetPrev(elem6);
//    elem1->SetPrev(elem0);
//    elem2->SetPrev(elem1);
//    elem3->SetPrev(elem2);
//    elem4->SetPrev(elem3);
//    elem5->SetPrev(elem4);
//    elem6->SetPrev(elem5);

    //Settaggio triangoli adiacenti
    Triangle::SetAdiacentTriangle(triangle1, &triangle2, &point2, &point3);
    Triangle::SetAdiacentTriangle(triangle0, &triangle1, &point1, &point2);
    Triangle::SetAdiacentTriangle(triangle3, &triangle2, &point3, &point4);
>>>>>>> EmilioZOrzi

////_______________________________________________________________________________________________________________________
////AGGIUNTA PUNTO ESTERNO

<<<<<<< HEAD
//    Point externalPoint = Point(0,4);
=======
//    Point externalPoint = Point(8,0);
>>>>>>> EmilioZOrzi

//    //Stampa punti convex hull
//    cout<<"\nELEMENTI CONVEX HULL INIZIALE"<<endl;
//    cout<<endl;
//    convexHullElem* currentElem1 = mesh.convexHull;
//    for (int i=0; i<7; i++)
//    {
//        cout<<*(currentElem1->hullPoint);
//        cout<<*(currentElem1->externalTriangle);
//        currentElem1 = currentElem1->next;
//    }
//    cout<<endl;

//    //Aggiunta di punto esterno
//    cout<<"Nuovo punto esterno da aggiungere: "<<externalPoint<<endl;
//    mesh.AddExternalPoint(externalPoint);

//    //Stampa punti convex hull
//    cout<<"\nELEMENTI NUOVO CONVEX HULL"<<endl;
//    cout<<endl;
//    convexHullElem* currentElem2 = mesh.convexHull;
//    for (int i=0; i<8; i++)
//    {
//        cout<<*(currentElem2->hullPoint);
//        cout<<*(currentElem2->externalTriangle);
//        currentElem2 = currentElem2->next;
//    }
//    cout<<endl;

////_______________________________________________________________________________________________________________________
////AGGIUNTA PUNTO INTERNO

//    Mesh meshInternal = Mesh(triangle0);

//    Point internalPoint1 = Point(5,3);
//    Point internalPoint2 = Point(5,4);
//    Point internalPoint3 = Point(6,3);

//    //Aggiunta di punto interno
//    cout<<"Nuovi punti interni da aggiungere:\n"<<internalPoint1<<internalPoint2<<internalPoint3<<endl;
//    meshInternal.AddInternalPoint(internalPoint1, triangle0);
//    meshInternal.AddInternalPoint(internalPoint2, triangle0);
<<<<<<< HEAD
//    meshInternal.AddInternalPoint(internalPoint3, triangle0);

//    cout<<"TUTTI I TRIANGOLI GENERATI"<<endl;
//    Triangle* lastTriangle;
//    for (Triangle tr:meshInternal.lastMesh){lastTriangle = &tr; cout<<tr<<endl;}
//    cout<<"Triangolo prova\n"<<*lastTriangle<<endl;

//    cout<<"Triangoli adiacenti al triangolo prova"<<endl;
//    for (Triangle* adTr:lastTriangle->adiacentTriangles){cout<<*adTr<<endl;}

//    cout<<"Figli del grosso"<<endl;
//    for (Triangle* trPtr:triangle0.pointedTriangles){cout<<*trPtr<<endl;}

//_______________________________________________________________________________________________________________________
//ESTERNI PIU' INTERNI

//    Point internalPoint1 = Point(5,5.5);
//    Point externalPoint = Point(0,4);

//    //Aggiunta di punto interno
//    cout<<"Nuovo punto interno da aggiungere: "<<internalPoint1<<endl;
//    mesh.AddInternalPoint(internalPoint1, triangle3);

//    //Aggiunta di punto esterno
//    cout<<"Nuovo punto esterno da aggiungere: "<<externalPoint<<endl;
//    mesh.AddExternalPoint(externalPoint);

//    cout<<"TUTTI I TRIANGOLI GENERATI"<<endl;
//    Triangle* lastTriangle;
//    for (Triangle tr:mesh.meshTriangles){cout<<tr<<endl;}

//    cout<<"Triangolo di verifica:\n"<<mesh.meshTriangles[8];

//    cout<<"Triangolo adiacente"<<endl;
//    cout<<*(mesh.meshTriangles[8].adiacentTriangles[1])<<endl;

//    cout<<"Triangoli in array"<<endl;
//    for (Triangle tr:trArray){cout<<tr<<endl;}

//_______________________________________________________________________________________________________________________
//ALTRO

    //Prova punto interno o esterno
//    Point point = Point(6,6);
//    cout<<mesh.CheckInside(point);
    //Aggiunta punto interno
//    Point internalPoint = Point(5,5.5);
//    mesh.AddInternalPoint(internalPoint, triangle3);

//    //OUTPUT
//    delaunay.MeshToEdges(mesh.guideTriangles);
//    for (array<Point,2> side:delaunay.finalEdges){cout<<"Lato\n"<<side[0]<<side[1]<<endl;}
//    delaunay.OutputEdges();
//    cout<<"Numero totale di lati: "<<delaunay.finalEdges.size()<<endl;

//_______________________________________________________________________________________________________________________
//CHIUSURA FUNZIONE
//    Point point1 = Point(0,1);
//    Point point2 = Point(1,1);
//    Point point3 = Point(0,0);
//    Point extPoint = Point(2,2);
//    Triangle triangleProva = Triangle(point1, point2, point3);
//    int result = triangleProva.ContainsPoint(extPoint);
//    cout << result << endl;
=======
//    //meshInternal.AddInternalPoint(internalPoint3, triangle0);

//    for (Triangle tr:meshInternal.lastMesh){cout<<tr<<endl;}

//    cout<<"Figli del grosso"<<endl;
//    for (Triangle* trPtr:triangle0.pointedTriangles){cout<<*trPtr<<endl;}

//    cout<<"Indirizzo di memoria di triangle0   "<<&triangle0<<endl;

//_______________________________________________________________________________________________________________________
//CHIUSURA FUNZIONE
    vector<Triangle*> new_triangles;
    new_triangles.push_back(&triangle3);
    mesh.Delunay(new_triangles);

    //OUTPUT
    delaunay.MeshToEdges(mesh.guideTriangles);
    for (array<Point,2> side:delaunay.finalEdges){cout<<"Lato\n"<<side[0]<<side[1]<<endl;}
    delaunay.OutputEdges();
    cout<<"Numero totale di lati: "<<delaunay.finalEdges.size()<<endl;
>>>>>>> EmilioZOrzi
    return 0;
}




