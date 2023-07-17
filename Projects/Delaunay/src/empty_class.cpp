
//IMPLEMENTAZIONE DEI METODI
#include "empty_class.hpp"
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_set>


namespace DelaunayLibrary
{
//COME FOSSE IL NOSTRO MAIN
    void Delaunay::ExecuteDelaunay()
    {
        cout<<"Indirizzi di memoria"<<endl;
        cout<<pointsVector[0]<<endl;
        cout<<pointsVector[1]<<endl;
        cout<<pointsVector[2]<<endl;
//        Grid grid = Grid(pointsVector); //Costruzione griglia
//        grid.PointsInRectangle(pointsVector); //Aggiunge ad ogni punto il rettangolo a cui è interno e ad ogni rettangolo il vector dei punti a cui è interno. (Valutare se fondere con costruttore griglia)
//        Point firstPoints[4] = grid.Snake(); //Restituisce 4 punti di cui i primi 3 sono il triangolone iniziale (Attenzione a punti allineati, stesso punto, ecc.)
//        Triangle firstTriangle = Triangle(firstPoints[0],firstPoints[1],firstPoints[2]);

        //Da eliminare -------------
        Triangle firstTriangle = Triangle(*pointsVector[0],*pointsVector[1],*pointsVector[2]);
        //--------------------------

        Mesh mesh = Mesh(firstTriangle);
//        //Delaunay con quarto punto
//        int position = firstTriangle.ContainsPoint(firstPoints[4]); //Controllare se è interno o esterno
//        if (position == -1){mesh.AddExternalPoint(firstPoints[4]);}
//        else if (position == 0) {mesh.AddInternalPoint(firstPoints[4], firstTriangle);}
//        else {} //Da capire
        cout<<"ConvexHull inizialissimo"<<endl;
        cout<<*(mesh.convexHull->hullPoint);
        cout<<*(mesh.convexHull->next->hullPoint);
        cout<<*(mesh.convexHull->next->next->hullPoint);
        cout<<endl;
        for (Point* point : pointsVector)
        {
            cout<<point<<endl;
            cout<<endl;
            cout<<"Punto da aggiungere: "<<*point;
            cout<<"ConvexHull iniziale"<<endl;
            cout<<*(mesh.convexHull->hullPoint);
            cout<<*(mesh.convexHull->next->hullPoint);
            cout<<*(mesh.convexHull->next->next->hullPoint);
            //cout<<*(mesh.convexHull->next->next->next->hullPoint);
            cout<<endl;
            if (point!=pointsVector[0] && point!=pointsVector[1] && point!=pointsVector[2])
            {
                int pos = mesh.CheckInside(*point);
                if (pos==0) //Punto esterno
                {
                    cout<<"ext"<<endl;
                    mesh.AddExternalPoint(*point);
                }
                else  //Punto interno o sul bordo
                {
                    cout<<"int"<<endl;
                    //DA ELIMINARE (non tutto) --------
                    for (Triangle* trPtr:mesh.guideTriangles)
                    {
                        if ((trPtr->ContainsPoint(*point))!=-1)
                        {
                            Triangle* bigTrianglePtr = trPtr->FromRootToLeaf(*point);
                            int posTr = bigTrianglePtr->ContainsPoint(*point);
                            if (posTr==0){mesh.AddInternalPoint(*point, bigTrianglePtr);}
                            else {mesh.AddSidePoint(*point, *bigTrianglePtr, posTr);}
                        }
                    }
                    //---------------------------------

                }
                cout<<"Point added"<<endl;
                cout<<endl;
            cout<<"ConvexHull"<<endl;
            cout<<*(mesh.convexHull->hullPoint);
            cout<<*(mesh.convexHull->next->hullPoint);
            cout<<*(mesh.convexHull->next->next->hullPoint);
            //cout<<*(mesh.convexHull->next->next->next->hullPoint);
            cout<<endl;
            }
            // (crossing Triangle) Controllare se il punto è esterno o interno (e in tal caso identificare il triangolo guida a cui è interno)
            // If interno AddInternalPoint, if external AddExternalPoint.
        }
        //Da capire dentro o fuori!! ------------------
        MeshToEdges(mesh.guideTriangles);
        //Stampa output e caricameno su file
        for (array<Point,2> side:finalEdges){cout<<"Lato\n"<<side[0]<<side[1]<<endl;}
        OutputEdges();
        cout<<"Numero totale di lati: "<<finalEdges.size()<<endl;
        //ELIMINARE OGGETTI DALLA MEMORIA!!
        //---------------------------------------------
    }

    //Restituisce 1 se il punto è interno alla mesh o 0 se è esterno
    int Mesh::CheckInside(Point point)
    {
        cout<<"Primo punto convexHull"<<*(convexHull->hullPoint);
        convexHullElem* elemHead = convexHull->next;
        convexHullElem* elemTail = convexHull;
        Point* head = elemHead->hullPoint;
        Point* tail = elemTail->hullPoint;
        cout<<"Coda: "<<*tail<<"Testa: "<<*head;
        double d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
        while (elemHead!=convexHull && d>0)
        {
            elemTail = elemHead;
            elemHead = elemHead->next;
            head = elemHead->hullPoint;
            tail = elemTail->hullPoint;
            cout<<"Coda: "<<*tail<<"Testa: "<<*head;
            d = d*((point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y));
        }
        if (d>0){return 1;}
        return 0;
    }


//    void Mesh::OperationEdges(Triangle& TriangleNew1, Triangle& TriangleNew2, Triangle& TriangleOld1, Triangle& TriangleOld2)
//    {
//      for (int i; i < 3; i++)
//      {

//      }
//    }

    Triangle::Triangle(Point& p1, Point& p2, Point& p3)
    {
        double crossProduct = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
        if (crossProduct < 0){vertices[0]=p1; vertices[1]=p3; vertices[2]=p2;}
        else {vertices[0]=p1; vertices[1]=p2; vertices[2]=p3;}
        adiacentTriangles = {nullptr, nullptr, nullptr};
    }

//Metodo che restituisce 0 se il punto in input è interno al triangolo, -1 se è esterno,
//mentre se giace su un lato del triangolo restituisce 1, 2 o 3 rispettivamente se giace sul lato tra i primi due vertivi, tra i secondi due o tra il primo e l'ultimo.
    int Triangle::ContainsPoint(Point& point)
    {
        double d = (point.x - vertices[1].x) * (vertices[0].y - vertices[1].y) - (vertices[0].x - vertices[1].x) * (point.y - vertices[1].y);  //Formula che restituisce un numero positivo se il punto si trova a destra del primo lato, negativo se si trova a sinistra.
        double prod1 = ((point.x - vertices[2].x) * (vertices[1].y - vertices[2].y) - (vertices[1].x - vertices[2].x) * (point.y - vertices[2].y))*d; //Prodotto positivo se il punto si trova a destra di entrambi primo e secondo lato o a sinistra di entrambi. E' negativo se si trova a destra di uno e a sinistra dell'altro (quindi esterno al triangolo)
        double prod2 = ((point.x - vertices[0].x) * (vertices[2].y - vertices[0].y) - (vertices[2].x - vertices[0].x) * (point.y - vertices[0].y))*d; //Prodotto positivo se il punto si trova a destra di entrambi primo e terzo lato o a sinistra di entrambi. E' negativo se si trova a destra di uno e a sinistra dell'altro (quindi esterno al triangolo)

        if (prod1>0 && prod2>0) //Il punto si trova a destra di tutti i lati o a sinistra di tutti i lati (interno)
            return 0;
        if (prod1!=0 && prod2!=0) //Il punto si trova a destra di almeno un lato e a sinistra di almeno un lato (esterno)
            return -1;
        if (d==0) //Il punto si trova sul primo lato (assumiamo non possano esserci punti sovrapposti)
            return 1;
        if (prod1==0) //Il punto si trova sul secondo lato
            return 2;
        return 3; //Per esclusione il punto si trova sul terzo lato
    }

    void Triangle::SetAdiacentTriangle(Triangle& existingTriangle, Triangle* addingTriangle, Point& tail, Point& head) //head e tail sono la testa e la coda del vettore visto come lato del triangolo già esistente ordiato in senso antiorario.
    {
        //Settare triangolo adiacente a triangolo già esistente
        if (existingTriangle.vertices[0] == tail){existingTriangle.adiacentTriangles[0] = addingTriangle;}
        else if (existingTriangle.vertices[1] == tail){existingTriangle.adiacentTriangles[1] = addingTriangle;}
        else {existingTriangle.adiacentTriangles[2] = addingTriangle;}
        //Settare triangolo adiacente a triangolo aggiunto
        if (addingTriangle!=nullptr){
            if (addingTriangle->vertices[0] == head){addingTriangle->adiacentTriangles[0] = &existingTriangle;}
            else if (addingTriangle->vertices[1] == head){addingTriangle->adiacentTriangles[1] = &existingTriangle;}
            else {addingTriangle->adiacentTriangles[2] = &existingTriangle;}}
    }

    Delaunay::Delaunay(const string& inputFileName)
    {
        fileName = inputFileName;
        ifstream file;
        file.open(fileName);
        if (file.fail()){
            cerr << "Error while opening file" << endl;
        }
        string line;
        getline(file, line);
        //for(int i=0; i<80; i++){
        while (!file.eof()){
            getline(file, line);
            int useless;
            double x;
            double y;
            istringstream ss(line);
            ss >> useless >> x >> y;
            Point* point = new Point(x, y);
            pointsVector.push_back(point);
            //i++;
        }
    }

    array<Point,2> Point::OrederSide(Point& point1, Point& point2)
    {
        if (point1<point2){return {point1,point2};}
        else {return {point2,point1};}
    }


    //vector<Point[2]> finalEdges;
    void Delaunay::MeshToEdges(vector<Triangle*> trPtrVec)
    {
        for (Triangle* tr:trPtrVec)
        {
            if (tr->pointedTriangles.empty())
            {
                array<Point,2> side1 = Point::OrederSide(tr->vertices[0], tr->vertices[1]);
                array<Point,2> side2 = Point::OrederSide(tr->vertices[1], tr->vertices[2]);
                array<Point,2> side3 = Point::OrederSide(tr->vertices[2], tr->vertices[0]);
                //Aggiungere lati al vector finale
                finalEdges.push_back(side1);
                finalEdges.push_back(side2);
                finalEdges.push_back(side3);
                sort(finalEdges.begin(), finalEdges.end());
                finalEdges.erase(unique(finalEdges.begin(), finalEdges.end()), finalEdges.end());
            }
            else {MeshToEdges(tr->pointedTriangles);}
        }
    }

    void Delaunay::OutputEdges()
    {
        fileName = "outputEdges";
        ofstream file;
        file.open(fileName);
        if (file.fail()){
            cerr << "Error while opening file" << endl;
        }
        string fileString = "{";
        for (array<Point,2> side:finalEdges){fileString+=("Segmento(("+to_string(side[0].x)+","+to_string(side[0].y)+"),("+to_string(side[1].x)+","+to_string(side[1].y)+")),");}
        fileString = fileString.substr(0,fileString.length()-1)+"}";
        file<<fileString<<endl;
        string fileStringPoints = "{";
        for (Point* point:pointsVector){fileStringPoints+=("("+to_string(point->x)+","+to_string(point->y)+"),");}
        fileStringPoints = fileStringPoints.substr(0,fileStringPoints.length()-1)+"}";
        file<<fileStringPoints;
    }


    void Delaunay::Show()
    {
        cout<<"PUNTI DELAUNAY"<<endl;
        string str = "";
        for (Point* point : pointsVector){
            cout<<*point;
        }
        cout<<endl;
    }


////////////////////// EMI ///////////////////////


    double determinante(double& a11, double& a12, double& a13, double& a21, double& a22, double& a23, double& a31, double& a32, double& a33)
    {
        double det = (a11 * a22 * a33) + (a12 * a23 * a31) + (a13 * a21 * a32) - (a13 * a22 * a31) - (a12 * a21 * a33) - (a11 * a23 * a32);
        return det;
    }


//______________________________________________________________________________________________
//ORDINARE I VERTICI

    void reorderPointsCounterclockwiseTr(Point& p1, Point& p2, Point& p3)
    {
        double crossProduct = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
        if (crossProduct < 0) {
            Point temp = p2;
            p2 = p3;
            p3 = temp;
        }
    }
//    void reorderPointsCounterclockwiseQ(vector<Point> &quadruplet)
//    {
//        for (int i = 0; i < 3; i++)
//        {
//            if (( quadruplet[(i+1)%3].x - quadruplet[i].x) * (quadruplet[3].y - quadruplet[i].y) - (quadruplet[(i+1)%3].y - quadruplet[i].y) * (quadruplet[3].x - quadruplet[i].x) < 0)
//            {
//                vector<Point> Copia = quadruplet;
//                quadruplet[0] = Copia[i];
//                quadruplet[1] = Copia[3];
//                quadruplet[2] = Copia[(i + 1) %3];
//                quadruplet[3] = Copia[(i + 2) %3];
//            }
//        }
//    }

//    array<Point,3> Triangle::OrderVertices()
//    {
//        double d = (vertices[2].x - vertices[1].x) * (vertices[0].y - vertices[1].y) - (vertices[0].x - vertices[1].x) * (vertices[2].y - vertices[1].y);  //Formula che restituisce un numero positivo se il punto si trova a destra del primo lato, negativo se si trova a sinistra.
//        if (d<0){return {vertices[0], vertices[2], vertices[1]};}
//        return {vertices[0], vertices[1], vertices[2]};
//    }

//______________________________________________________________________________________________


    bool DelunayProperty(Triangle& Triangle1, Triangle& Triangle2)
    {
        Point A = Triangle1.vertices[0];
        Point B = Triangle1.vertices[1];
        Point C = Triangle1.vertices[2];
        reorderPointsCounterclockwiseTr(A, B, C);
        Point D;
        //vector<Point> quadruplet = {A, B, C, D};
        for (int i = 0; i < 3; i++)
        {
            int k = 0;
            for (int j = 0; j < 3; j++)
            {
                if (Triangle2.vertices[i].x == Triangle1.vertices[j].x and Triangle1.vertices[j].y == Triangle2.vertices[i].y)
                {
                    k = 1;
                }
            }
            if (k == 0)
            {
               D = Triangle2.vertices[i];
            }
        }
        //reorderPointsCounterclockwiseQ(quadruplet);
        double a11 = A.x - D.x;
        double a12 = A.y - D.y;
        double a13 = (A.x - D.x)*(A.x - D.x) + (A.y - D.y)*(A.y - D.y);
        double a21 = B.x - D.x;
        double a22 = B.y - D.y;
        double a23 = (B.x - D.x)*(B.x - D.x) + (B.y - D.y)*(B.y - D.y);
        double a31 = C.x - D.x;
        double a32 = C.y - D.y;
        double a33 = (C.x - D.x)*(C.x - D.x) + (C.y - D.y)*(C.y - D.y);
        double a = determinante(a11, a12, a13, a21, a22, a23, a31, a32, a33);
        if (a > 0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    bool CheckConvex(Triangle& Triangle1, Triangle& Triangle2)
    {
        int i = 0;
        int sign = 0;
        Point esterno1;
        Point esterno2;
        Point Lato[2];
        for (auto& punto1 : Triangle1.vertices)
        {
           for (auto& punto2 : Triangle2.vertices)
           {
            if (punto1.x == punto2.x and punto1.y == punto2.y)
            {
              Lato[i] = punto1;
              i += 1;
              sign = 1;
            }
           }
           if (sign == 0)
           {
            esterno1 = punto1;
           }
        }
        for (auto& punto : Triangle2.vertices)
        {
            if ((punto.x == Lato[0].x and punto.y == Lato[0].y) or (punto.x == Lato[1].x and punto.y == Lato[1].y))
            {
                // do nothing
            }
            else
            {
                esterno2 = punto;
                // cout << esterno2.x << endl;
            }
        }
        Point Quadril[4];
        Quadril[0] = esterno1;
        Quadril[1] = Lato[0];
        //cout << Lato[0].x << Lato[0].y << endl;
        //cout << Lato[1].x << Lato[1].y << endl;
        Quadril[2] = esterno2;
        Quadril[3] = Lato[1];
        bool segno[4];
        for (int i = 0; i < 4; i++)
        {
            // double a = ((Quadril[(i+1)%4].x - Quadril[i].x) * (Quadril[(i+2)%4].y - Quadril[(i+1)%4].y) - (Quadril[(i+1)%4].y - Quadril[i].y) * (Quadril[(i+2)%4].x - Quadril[(i+1)%4].x));
            if (( Quadril[(i+1)%4].x - Quadril[i].x) * (Quadril[(i+2)%4].y - Quadril[(i+1)%4].y) - (Quadril[(i+1)%4].y - Quadril[i].y) * (Quadril[(i+2)%4].x - Quadril[(i+1)%4].x) > 0)
            {
                segno [i] = 1;
            }
            else
            {
                segno[i] = 0;
            }
            //cout << a << endl;
        }
        for (int i = 0; i < 4; i++)
        {
            //cout << segno[i] << endl;
            //cout << Quadril[i].x << Quadril[i].y << endl;
        }

        if ((segno[0] == segno[1]) and (segno[1] == segno[2]) and (segno[2] == segno [3]))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    void Mesh::Flip(Triangle& Triangle1, Triangle& Triangle2)
    {
        int i = 0;
        int sign = 0;
        Point esterno1;
        Point esterno2;
        Point Lato[2];
        for (auto& punto1 : Triangle1.vertices)
        {
           for (auto& punto2 : Triangle2.vertices)
           {
            if (punto1.x == punto2.x and punto1.y == punto2.y)
            {
              Lato[i] = punto1;
              i += 1;
              sign = 1;
            }
           }
           if (sign == 0)
           {
            esterno1 = punto1;
           }
        }
        for (auto& punto : Triangle2.vertices)
        {
            if ((punto.x == Lato[0].x and punto.y == Lato[0].y) or (punto.x == Lato[1].x and punto.y == Lato[1].y))
            {
                // do nothing
            }
            else
            {
                esterno2 = punto;
                // cout << esterno2.x << endl;
            }
        }
        Point Quadril[4];
        Quadril[0] = esterno1;
        Quadril[1] = Lato[0];
        //cout << Lato[0].x << Lato[0].y << endl;
        //cout << Lato[1].x << Lato[1].y << endl;
        Quadril[2] = esterno2;
        Quadril[3] = Lato[1];
        //Triangle TrNew1 = Triangle(esterno1, Lato[0], esterno2);
//        TrNew1.adiacentTriangles[0] = meshTriangles.size();
//        operationEdges(TrNew1);
//        Triangle TrNew2 = Triangle(esterno1, Lato[1], esterno2);
//        TrNew1.adiacentTriangles[0] = meshTriangles.size() + 1 ;
//        operationEdges(TrNew1);
//        meshTriangles.push_back(TrNew1);
//        meshTriangles.push_back(TrNew1);
//        Triangle1.PointedTriangles.push_back(TrNew1);
//        Triangle1.PointedTriangles.push_back(TrNew2);
//        Triangle2.PointedTriangles.push_back(TrNew1);
//        Triangle2.PointedTriangles.push_back(TrNew2);
    }

    void Mesh::DelunayPropagation(Triangle& StartTriangle)
    {
       for (int i = 0; i < 3; i++)
       {
//            Triangle TriangoloAdj = meshTriangles[StartTriangle.adiacentTriangles[i]];
//            if (CheckConvex(StartTriangle, TriangoloAdj) == 0)
//            {
//                if (DelunayProperty(StartTriangle, TriangoloAdj) == 0 or (DelunayProperty(StartTriangle, TriangoloAdj) == 0) )
//                {
//                    Flip(StartTriangle, TriangoloAdj);
//                }
//            }

       }
    }

    void Mesh::Delunay(vector<Triangle>& new_triangles)
    {
        for (auto& element : new_triangles)
        {
            meshTriangles.push_back(element);
            lastMesh.push_back(element);
            DelunayPropagation(element);
        }
    }

////////////////////// MADDI ///////////////////////

    Mesh::Mesh (Triangle& triangle)
    {
        meshTriangles.push_back(triangle);
        guideTriangles.push_back(&triangle);
        //Creazione dei tre oggetti Convex Hull
        convexHullElem* firstElem = new convexHullElem(triangle.vertices[0], triangle);
        convexHullElem* secondElem = new convexHullElem(triangle.vertices[1], triangle);
        convexHullElem* thirdElem = new convexHullElem(triangle.vertices[2], triangle);
        //Assegnazione degli indirizzi ai puntatori
        firstElem->SetNext(secondElem);
        secondElem->SetNext(thirdElem);
        thirdElem->SetNext(firstElem);

        secondElem->SetPrev(firstElem);
        thirdElem->SetPrev(secondElem);
        firstElem->SetPrev(thirdElem);

        convexHull = firstElem;
    }


    void Mesh::AddExternalPoint(Point& point)
    {
        vector<Triangle*> newTriangles;
        convexHullElem* newElem;

        //cout<<"PRIMO PUNTO CONVEX HULL"<<endl;
        convexHullElem* elemHead = convexHull->next;
        convexHullElem* elemTail = convexHull;
        Point* head = elemHead->hullPoint;
        Point* tail = elemTail->hullPoint;
        //cout<<*tail;
        //cout<<*head;

        double d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.

        //SE IL PRIMO LATO NON E' DA COLLEGARE
        if (d>=0)
        {
            //Inizio a girare in senso antiorario
            while (d>=0)
            //for(int i=0; i<2; i++)
            {
                elemTail = elemHead;
                elemHead = elemHead->next;
                head = elemHead->hullPoint;
                tail = elemTail->hullPoint;
                //cout<<*tail;
                //cout<<*head;
                d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
                //cout<<d<<endl<<endl;
            }
            //Aggiunta di un nuovo triangolo
            Triangle* newGuideTriangle = new Triangle(point, *head, *tail);
            newTriangles.push_back(newGuideTriangle);
            meshTriangles.push_back(*newGuideTriangle);
            guideTriangles.push_back(newGuideTriangle);
            //Aggiornamento adiacenze
//            cout<<"Testa\n"<<*head<<"\nCoda\n"<<*tail<<endl;
//            Point* midPoint = new Point(abs((tail->x)+(head->x))/2,abs((tail->y)+(head->y))/2);
//            cout<<"Punto medio\n"<<*midPoint<<endl;
//            Triangle* lastTrPtr = (elemHead->externalTriangle)->FromRootToLeaf(*midPoint);
//            Triangle::SetAdiacentTriangle(*lastTrPtr, newGuideTriangle, *tail, *head);
//            delete midPoint;

//            cout<<"Nuovo triangolo radice:\n"<<*newGuideTriangle;
//            cout<<endl;

            //Aggiunta di un nuovo elemento nel convex hull
            newElem = new convexHullElem(point, *newGuideTriangle);
            newElem->SetPrev(elemTail);
            elemTail->SetNext(newElem);

//            //...DA ELIMINARE...
//            //newElem->SetNext(firstElem);
//            Point* firstRight = tail;
//            cout<<"Primo punto da collegare: ";
//            cout<<*firstRight;
//            cout<<endl;
//            //..................

            elemTail = elemHead;
            elemHead = elemHead->next;
            head = elemHead->hullPoint;
            tail = elemTail->hullPoint;
            //cout<<*tail;
            //cout<<*head;
            d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
            //cout<<d<<endl<<endl;

            //Continuo a girare in senso antiorario
            while (d<0)
            {
                //Aggiunta del nuovo triangolo
                Triangle* newGuideTriangle = new Triangle(point, *head, *tail);
                newTriangles.push_back(newGuideTriangle);
                meshTriangles.push_back(*newGuideTriangle);
                guideTriangles.push_back(newGuideTriangle);
                Triangle::SetAdiacentTriangle(*(elemHead->externalTriangle), newGuideTriangle, *tail, *head);
//                cout<<"Nuovo triangolo radice:\n"<<*newGuideTriangle;
//                cout<<endl;

                //Eliminazione dal convex hull dell'elemento in coda al vettore
                //delete (elemTail->hullPoint);
                delete elemTail;

                //Spostamento al vettore successivo
                elemTail = elemHead;
                elemHead = elemHead->next;
                head = elemHead->hullPoint;
                tail = elemTail->hullPoint;
                //cout<<*tail;
                //cout<<*head;
                d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
                //cout<<d<<endl<<endl;
            }

            //Aggiunta di un nuovo legame nel convex hull
            elemTail->SetPrev(newElem);
            newElem->SetNext(elemTail);
            elemTail->SetTriangle(newTriangles.back());


//            //...DA ELIMINARE...
//            convexHullElem* successivoNewElem = newElem->next;
//            cout<<"Ultimo punto da collegare: ";
//            cout<<*(successivoNewElem->hullPoint);
//            cout<<endl;
//            //..................
        }

        //SE IL PRIMO LATO E' DA COLLEGARE
        else
        {
            int i=0;
            //Inizio a girare in senso antiorario
            while (d<0)
            {
                //Aggiunta del nuovo triangolo
                Triangle* newGuideTriangle = new Triangle(point, *head, *tail);
                newTriangles.push_back(newGuideTriangle);
//                meshTriangles.push_back(newGuideTriangle);
                guideTriangles.push_back(newGuideTriangle);
                Triangle::SetAdiacentTriangle(*(elemHead->externalTriangle), newGuideTriangle, *tail, *head);
//                cout<<"Nuovo triangolo radice:\n"<<*newGuideTriangle;
//                cout<<endl;

                //Eliminazione dal convex hull dell'elemento in coda al vettore
                if (i!=0)
                {
                    //delete (elemTail->hullPoint);
                    delete elemTail;
                }

                //Spostamento al vettore successivo
                elemTail = elemHead;
                elemHead = elemHead->next;
                head = elemHead->hullPoint;
                tail = elemTail->hullPoint;
                d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
                i++;
            }

            //Aggiunta di un nuovo elemento nel convex hull
            newElem = new convexHullElem(point, *newTriangles.back());
            elemTail->SetPrev(newElem);
            newElem->SetNext(elemTail);
            elemTail->SetTriangle(newTriangles.back());

//            //...DA ELIMINARE...
//            convexHullElem* successivoNewElem = newElem->next;
//            cout<<"Primo punto da collegare: ";
//            cout<<*(successivoNewElem->hullPoint);
//            cout<<endl;
//            //..................

            //Torno al punto iniziale
            elemHead = convexHull;
            elemTail = convexHull->prev;
            head = elemHead->hullPoint;
            tail = elemTail->hullPoint;
            d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.

            //Inizio a girare in senso orario
            while (d<0)
            {
                //Aggiunta del nuovo triangolo
                Triangle* newGuideTriangle = new Triangle(point, *head, *tail);
                newTriangles.push_back(newGuideTriangle);
//                meshTriangles.push_back(newGuideTriangle);
                guideTriangles.push_back(newGuideTriangle);
                Triangle::SetAdiacentTriangle(*(elemHead->externalTriangle), newGuideTriangle, *tail, *head);
//                cout<<"Nuovo triangolo radice:\n"<<*newGuideTriangle;
//                cout<<endl;

                //Eliminazione dal convex hull dell'elemento in coda al vettore
                //delete (elemHead->hullPoint);
                //delete elemHead;                            !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                //Spostamento al vettore successivo
                elemHead = elemTail;
                elemTail = elemTail->prev;
                head = elemHead->hullPoint;
                tail = elemTail->hullPoint;
                d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
            }

            //Aggiunta di un nuovo legame nel convex hull
            elemHead->SetNext(newElem);
            newElem->SetPrev(elemHead);
            //newElem->SetTriangle(&newTriangles.back());
            SetConvexHull(newElem);
            newElem->SetTriangle(newTriangles.back());

//            //...DA ELIMINARE...
//            convexHullElem* precedenteNewElem = newElem->prev;
//            cout<<"Ultimo punto da collegare: ";
//            cout<<*(precedenteNewElem->hullPoint);
//            cout<<endl;
//            //..................
        }
    }

    Triangle* Triangle::FromRootToLeaf(Point& point)
    {
        if (pointedTriangles.empty()){return this;}
        for (Triangle* tr:pointedTriangles)
        {
            if (tr->ContainsPoint(point)!=-1){return tr->FromRootToLeaf(point);}
        }
    }

    void Mesh::AddInternalPoint(Point& point, Triangle* bigTriangle)
    {
        //Creazione dei nuovi triangoli
        Triangle* triangle1 = new Triangle(bigTriangle->vertices[0], bigTriangle->vertices[1], point);
        Triangle* triangle2 = new Triangle(bigTriangle->vertices[1], bigTriangle->vertices[2], point);
        Triangle* triangle3 = new Triangle(bigTriangle->vertices[2], bigTriangle->vertices[0], point);
        //Inserimento dei nuovi triangoli nel vettore dei puntati del padre
        bigTriangle->pointedTriangles.push_back(triangle1);
        bigTriangle->pointedTriangles.push_back(triangle2);
        bigTriangle->pointedTriangles.push_back(triangle3);
        //cout<<"Stampa pointed triangles dentro metodo"<<endl;
        //for (Triangle* trPtr:bigTrianglePtr->pointedTriangles){cout<<*trPtr<<endl;}
        //cout<<"Fine stampa dentro metodo"<<endl;
        //Aggiornamento adiacenze
        Triangle::SetAdiacentTriangle(*triangle1, bigTriangle->adiacentTriangles[0], (bigTriangle->vertices[0]), (bigTriangle->vertices[1]));
        Triangle::SetAdiacentTriangle(*triangle2, bigTriangle->adiacentTriangles[1], bigTriangle->vertices[1], bigTriangle->vertices[2]);
        Triangle::SetAdiacentTriangle(*triangle3, bigTriangle->adiacentTriangles[2], bigTriangle->vertices[2], bigTriangle->vertices[0]);
        Triangle::SetAdiacentTriangle(*triangle1, triangle2, bigTriangle->vertices[1], point);
        Triangle::SetAdiacentTriangle(*triangle2, triangle3, bigTriangle->vertices[2], point);
        Triangle::SetAdiacentTriangle(*triangle3, triangle1, bigTriangle->vertices[0], point);
        //Aggiunta triangoli a leaf mesh (?)
        meshTriangles.push_back(*triangle3);
        meshTriangles.push_back(*triangle1);
        meshTriangles.push_back(*triangle2);

        //Rimozione triangolo radice da leaf mesh (?)
        //Aggiornamento triangoli adiacenti al convex hull (?)
        //Verifica Delaunay
    }

    void Mesh::AddSidePoint(Point& point, Triangle& bigTriangle, int side)
    {
        //Se il punto è su un lato tra due triangoli
        if (bigTriangle.adiacentTriangles[side-1]!=nullptr)
        {
            Triangle* adiacentTrPtr = bigTriangle.adiacentTriangles[side-1];
            Point* oppositPointPtr;
            int commonSidePos;
            int i=0;
            for (Point pt:adiacentTrPtr->vertices){
                if (pt!=bigTriangle.vertices[side-1] && pt!=bigTriangle.vertices[side%3]){oppositPointPtr=&pt; commonSidePos=(i+1)%3;}
                i++;
            }
            cout<<*oppositPointPtr<<endl;
            cout<<i<<endl;
            //Creazione dei nuovi triangoli
            Triangle* triangle1 = new Triangle(bigTriangle.vertices[side-1], bigTriangle.vertices[(side+1)%3], point);
            Triangle* triangle2 = new Triangle(bigTriangle.vertices[side%3], bigTriangle.vertices[(side+1)%3], point);
            Triangle* triangle3 = new Triangle(bigTriangle.vertices[side-1], *oppositPointPtr, point);
            Triangle* triangle4 = new Triangle(bigTriangle.vertices[side%3], *oppositPointPtr, point);
            //Inserimento dei nuovi triangoli nel vettore dei puntati del padre
            bigTriangle.pointedTriangles.push_back(triangle1);
            bigTriangle.pointedTriangles.push_back(triangle2);
            adiacentTrPtr->pointedTriangles.push_back(triangle3);
            adiacentTrPtr->pointedTriangles.push_back(triangle4);
            //Aggiornamento adiacenze
            Triangle::SetAdiacentTriangle(*triangle1, bigTriangle.adiacentTriangles[(side+1)%3], bigTriangle.vertices[(side+1)%3], bigTriangle.vertices[side-1]);
            Triangle::SetAdiacentTriangle(*triangle2, bigTriangle.adiacentTriangles[side%3], bigTriangle.vertices[side%3], bigTriangle.vertices[(side+1)%3]);
            Triangle::SetAdiacentTriangle(*triangle3, adiacentTrPtr->adiacentTriangles[(commonSidePos+1)%3], bigTriangle.vertices[side-1], *oppositPointPtr);
            Triangle::SetAdiacentTriangle(*triangle4, adiacentTrPtr->adiacentTriangles[(commonSidePos+2)%3], *oppositPointPtr, bigTriangle.vertices[side%3]);
            Triangle::SetAdiacentTriangle(*triangle1, triangle2, point, bigTriangle.vertices[(side+1)%3]);
            Triangle::SetAdiacentTriangle(*triangle3, triangle4, *oppositPointPtr, point);
            Triangle::SetAdiacentTriangle(*triangle1, triangle3, bigTriangle.vertices[side-1], point);
            Triangle::SetAdiacentTriangle(*triangle2, triangle4, point, bigTriangle.vertices[side%3]);
            //Aggiunta triangoli a leaf mesh (?)
            meshTriangles.push_back(*triangle1);
            meshTriangles.push_back(*triangle2);
            meshTriangles.push_back(*triangle3);
            meshTriangles.push_back(*triangle4);
            //Rimozione triangoli radice da leaf mesh (?)
            //Aggiornamento triangoli adiacenti al convex hull (?)
            //Verifica Delaunay
        }
        //Se il punto è su un lato del ConvexHull
        else
        {
            //Creazione dei nuovi triangoli
            Triangle triangle1 = Triangle(bigTriangle.vertices[side-1], bigTriangle.vertices[(side+1)%3], point);
            Triangle triangle2 = Triangle(bigTriangle.vertices[side%3], bigTriangle.vertices[(side+1)%3], point);
            //Inserimento dei nuovi triangoli nel vettore dei puntati del padre
            bigTriangle.pointedTriangles.push_back(&triangle1);
            bigTriangle.pointedTriangles.push_back(&triangle2);
            //Aggiornamento adiacenze
            Triangle::SetAdiacentTriangle(triangle1, bigTriangle.adiacentTriangles[(side+1)%3], bigTriangle.vertices[(side+1)%3], bigTriangle.vertices[side-1]);
            Triangle::SetAdiacentTriangle(triangle2, bigTriangle.adiacentTriangles[side%3], bigTriangle.vertices[side%3], bigTriangle.vertices[(side+1)%3]);
            Triangle::SetAdiacentTriangle(triangle1, &triangle2, point, bigTriangle.vertices[(side+1)%3]);
            //Aggiornamento convexHull

            //Aggiunta triangoli a leaf mesh (?)
            meshTriangles.push_back(triangle1);
            meshTriangles.push_back(triangle2);
            //Rimozione triangoli radice da leaf mesh (?)
            //Verifica Delaunay
        }
    }


    Grid::Grid(vector<Point>& points)
    {

        int n = points.size();
        intNum = (int)(sqrt(points.size()));
        rectangles.resize(intNum, intNum);
        //Eigen::Matrix<Rectangle, intNum, intNum> rectanglesLocal;
        double x_min = points[0].x;
        double x_max = points[0].x;
        double y_min = points[0].y;
        double y_max = points[0].y;
        for (int i=1; i<n; i++)
        {
            if (points[i].x<x_min){x_min = points[i].x;}
            if (points[i].x>x_max){x_max = points[i].x;}
            if (points[i].y<y_min){y_min = points[i].y;}
            if (points[i].y>y_max){y_max = points[i].y;}
        }
        double intervalX = (x_max-x_min)/intNum;
        double intervalY = (y_max-y_min)/intNum;
        for (int i=0; i<intNum; i++){
            for (int j=0; j<intNum; j++){
                double startX = x_min+intervalX*j;
                double startY = y_min+intervalY*i;
                rectangles(i,j) = Rectangle(startX, startY);
            }
        }
        //rectangles = rectanglesLocal;
    }


    void Grid::Show()
    {
        cout<<"GRIGLIA"<<endl;
        for (int i=0; i<intNum; i++)
        {
            for (int j=0; j<intNum; j++){cout<<"("+rectangles(i,j).toString()+")"<<"    ";}
            cout<<endl;
        }
        cout<<endl;
    }

    void MeshToEdges()
    {

    }

////////////////////// ALDO ///////////////////////

//    void Grid::pointsInRectangle(vector<Point> &points)
//    {
//        for (Point pt : points){
//            int col = fmod((pt.x - x_min), intervalX);
//            int row = fmod((pt.y - y_min), intervalY);
//            rectangles(row, col).containedPoints.push_back(pt);
//        }
//    }


//    array<Point, 4> Grid::PickFourRandomPoints(vector<Point>& points){
//       array<Point, 4> result;
//       vector<Point> shuffledPoints = points;
//       random_shuffle(shuffledPoints.begin(), shuffledPoints.end());
//       for (int i = 0; i < 4; ++i) {
//            result[i] = shuffledPoints[i];
//       }

//       return result;
//    }


//    array<Point, 4> Grid::Snake()
//    {
//       array<Point, 4> fourPoints;
//       array<Rectangle, 3> chosenRectangles;

//       string flag = "well";

//       // primo punto (in alto a sx)

////       int i, j = 0;
////       for (int sum = 0; sum < intNum; sum++){
////            i = sum;
////            if (!(rectangles(i, j).containedPoints).empty()){
////                chosenRectangles[0] = rectangles(i, j);
////                fourPoints[0] = rectangles(i, j).containedPoints[0];
////            }
////            while (i > 0){
////                i = i - 1;
////                j = j + 1;
////                if (!(rectangles(i, j).containedPoints).empty()){
////                    chosenRectangles[0] = rectangles(i, j);
////                    fourPoints[0] = rectangles(i, j).containedPoints[0];
////                }
////            }
////            j = 0;
////       }

//       for (int sum = 0; sum < intNum; sum++)
//       {

//   int i = sum;
//           for (int j = 0; j <= sum; j++)
//           {
//               if (!(rectangles(i, j).containedPoints).empty())
//               {
//                chosenRectangles[0] = rectangles(i, j);
//                fourPoints[0] = rectangles(i, j).containedPoints[0];
//               }
//           }
//           if (i > 0)
//           {
//               i--;
//           }
//       }


//       // secondo punto (in basso a sx)
//       for (int i = intNum - 1; i >= 0; i--)
//       {
//            int k = i;
//            for (int j = 0; j <= (intNum - 1 - k); j++)
//            {
//                if (!(rectangles(i, j).containedPoints).empty())
//                {
//                 if ()
//                 chosenRectangles[0] = rectangles(i, j);
//                 fourPoints[0] = rectangles(i, j).containedPoints[0];
//                }
//                else
//                {
//                 cerr << "Rectangle already found by a previous zig-zagging." << endl;
//                 flag = "bad";
//                }

//                if (i < intNum - 1)
//                {
//                    i++;
//                }
//            }
//            k = 0;
//       }

//       // terzo punto (in alto a dx)
//       if (flag != "bad"){
//            for (int i = 0; i < m; i++){
//                for (int j = m; j >= m - i; j--){
//                    Rectangle rectangle = rectangles(i, j);
//                    vector<Point> points = rectangles.containedPoints();
//                    if (!points.empty()){
//                        if (rectangle != chosenRectangles[0] && rectangle != chosenRectangles[1]){
//                            chosenRectangles[2] = rectangle;
//                            fourPoints[2] = points[0];
//                        }
//                        else{
//                            cerr << "Rectangle already found by a previous zig-zagging." << endl;
//                            flag = "bad";
//                        }
//                    }
//                    if (i > 0){
//                        i--;
//                    }
//                }
//            }
//       }


//       // secondo punto (in basso a dx)
//       if (flag != "bad"){
//            for (int i = m; i >= 0; i--){
//                for (int j = m; j >= i; j--){
//                    Rectangle rectangle = rectangles(i, j);
//                    vector<Point> points = rectangles.containedPoints();
//                    if (!points.empty()){
//                        if (rectangle != chosenRectangles[0] && rectangle != chosenRectangles[1] && rectangle != chosenRectangles[2]){
//                            fourPoints[3] = points[0];
//                        }
//                        else{
//                            cerr << "Rectangle already found by a previous zig-zagging." << endl;
//                            flag = "bad";
//                        }
//                    }
//                    if (i < m){
//                        i++;
//                    }
//                }
//            }
//       }


//       if (flag == "true"){
//           cout << "Zig-zagging algorithm worked well! The four points obtained by zig-zagging are: " << endl;
//           return fourPoints;
//       }
//       else{
//           cout << "Zig-zagging algorithm didn't work well! Four random points are: " << endl;
//           return PickFourRandomPoints(Delaunay.pointsVector);
//       }
//    }
}
