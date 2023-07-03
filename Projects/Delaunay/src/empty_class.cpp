
//IMPLEMENTAZIONE DEI METODI
#include "empty_class.hpp"
#include <fstream>
#include <vector>


namespace DelaunayLibrary
{
//COME FOSSE IL NOSTRO MAIN
//    void Delaunay::ExecuteDelaunay()
//    {
//        Grid grid = Grid(pointsVector); //Costruzione griglia
//        grid.PointsInRectangle(pointsVector); //Aggiunge ad ogni punto il rettangolo a cui è interno e ad ogni rettangolo il vector dei punti a cui è interno. (Valutare se fondere con costruttore griglia)
//        Point firstPoints[4] = grid.Snake(); //Restituisce 4 punti di cui i primi 3 sono il triangolone iniziale (Attenzione a punti allineati, stesso punto, ecc.)
//        firstTriangle = Triangle(firstPoints[0],firstPoints[1],firstPoints[2]);
//        Mesh mesh = Mesh(firstTriangle); //Decidere come gestire Convex Hull
//        //Delaunay con quarto punto
//        int position = ContainsPoint(firstPoints[4]); //Controllare se è interno o esterno
//        if (position == -1){mesh.AddExternalPoint(firstPoints[4]);}
//        else if (position == 0) {mesh.AddInternalPoint(firstPoints[4], firstTriangle);}
//        else {} //Da capire
//        for (Point point : pointsVector)
//        {
//            // (crossing Triangle) Controllare se il punto è esterno o interno (e in tal caso identificare il triangolo a cui è interno)
//            // If interno AddInternalPoint, if external AddExternalPoint.
//        }
//        mesh.MeshToEdges();
//        ELIMINARE OGGETTI DALLA MEMORIA!!
//    }

        //void Mesh::AddInternalPoint(Point point)
        //{

        //}


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

    void Triangle::SetAdiacentTriangle(Triangle& existingTriangle, Triangle& addingTriangle, Point& tail, Point& head)
    {
        //Settare triangolo adiacente a triangolo già esistente
        if (existingTriangle.vertices[0] == tail){existingTriangle.adiacentTriangles[0] = &addingTriangle;}
        else if (existingTriangle.vertices[1] == tail){existingTriangle.adiacentTriangles[1] = &addingTriangle;}
        else {existingTriangle.adiacentTriangles[2] = &addingTriangle;}
        //Settare triangolo adiacente a triangolo aggiunto
        if (addingTriangle.vertices[0] == head){addingTriangle.adiacentTriangles[0] = &existingTriangle;}
        else if (addingTriangle.vertices[1] == head){addingTriangle.adiacentTriangles[1] = &existingTriangle;}
        else {addingTriangle.adiacentTriangles[2] = &existingTriangle;}
    }

    Delaunay::Delaunay(const string& inputFileName)
    {
        fileName = inputFileName;
        ifstream file;
        file.open(fileName);
        if (file.fail()){
            cerr << "Error while opening file" << endl;
            //return pointsVector;
        }
        string line;
        getline(file, line);
        while (!file.eof()){
            getline(file, line);
            int useless;
            double x;
            double y;
            istringstream ss(line);
            ss >> useless >> x >> y;
            Point point = Point(x, y);
            pointsVector.push_back(point);
        }
    }


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
        guideTriangles.push_back(triangle);
        //array<Point,3> orderedPoints = triangle.OrderVertices();
        //Creazione dei tre oggetti Convex Hull
        convexHull = new convexHullElem(triangle.vertices[0], triangle);
        //convexHullElem* firstElem = new convexHullElem(triangle.vertices[0], triangle);
        convexHullElem* secondElem = new convexHullElem(triangle.vertices[1], triangle);
        convexHullElem* thirdElem = new convexHullElem(triangle.vertices[2], triangle);
        //Assegnazione degli indirizzi ai puntatori
        convexHull->SetNext(secondElem);
        secondElem->SetNext(thirdElem);
        thirdElem->SetNext(convexHull);

        secondElem->SetPrev(convexHull);
        thirdElem->SetPrev(secondElem);
        convexHull->SetPrev(thirdElem);

        //convexHull = firstElem;
    }


    void Mesh::AddExternalPoint(Point& point)
    {
        cout<<"Appena entrati nella funzione:\n"<<&point<<endl;
        vector<Triangle*> newTriangles;
        convexHullElem* newElem;

        cout<<"PRIMO PUNTO CONVEX HULL"<<endl;
        convexHullElem* elemHead = convexHull->next;
        convexHullElem* elemTail = convexHull;
        Point* head = elemHead->hullPoint;
        Point* tail = elemTail->hullPoint;
        cout<<*tail;
        cout<<*head;

        double d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
        cout<<d<<endl;
        cout<<endl;

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
                cout<<*tail;
                cout<<*head;
                d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
                cout<<d<<endl<<endl;
            }
            //Aggiunta di un nuovo triangolo
            Triangle* newGuideTriangle = new Triangle(point, *head, *tail);
//            newTriangles.push_back(newGuideTriangle);
//            meshTriangles.push_back(newGuideTriangle);
//            guideTriangles.push_back(newGuideTriangle);
            cout<<*newGuideTriangle;
            cout<<endl;

            //Aggiunta di un nuovo elemento nel convex hull
            newElem = new convexHullElem(point, *newGuideTriangle);
            cout<<"Indirizzo interno alla funzione\n"<<&point<<endl;
            cout<<"Indirizzo interno all'elemento\n"<<newElem->hullPoint<<endl;
            newElem->SetPrev(elemTail);
            elemTail->SetNext(newElem);

            //...DA ELIMINARE...
            //newElem->SetNext(firstElem);
            Point* firstRight = tail;
            cout<<"Primo punto da collegare: ";
            cout<<*firstRight;
            cout<<endl;
            //..................

            elemTail = elemHead;
            elemHead = elemHead->next;
            head = elemHead->hullPoint;
            tail = elemTail->hullPoint;
            cout<<*tail;
            cout<<*head;
            d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
            cout<<d<<endl<<endl;

            //Continuo a girare in senso antiorario
            while (d<0)
            {
                //Aggiunta del nuovo triangolo
                Triangle* newGuideTriangle = new Triangle(point, *head, *tail);
                newTriangles.push_back(newGuideTriangle);
//                meshTriangles.push_back(newGuideTriangle);
//                guideTriangles.push_back(newGuideTriangle);
                cout<<*newGuideTriangle;
                cout<<endl;

                //Eliminazione dal convex hull dell'elemento in coda al vettore
                //delete (elemTail->hullPoint);
                //delete elemTail;

                //Spostamento al vettore successivo
                elemTail = elemHead;
                elemHead = elemHead->next;
                head = elemHead->hullPoint;
                tail = elemTail->hullPoint;
                cout<<*tail;
                cout<<*head;
                d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
                cout<<d<<endl<<endl;
            }

            //Aggiunta di un nuovo legame nel convex hull
            elemTail->SetPrev(newElem);
            newElem->SetNext(elemTail);
            elemTail->SetTriangle(newTriangles.back());


            //...DA ELIMINARE...
            convexHullElem* successivoNewElem = newElem->next;
            cout<<"Ultimo punto da collegare: ";
            cout<<*(successivoNewElem->hullPoint);
            cout<<endl;
            //..................
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
//                guideTriangles.push_back(newGuideTriangle);
                cout<<*newGuideTriangle;
                cout<<endl;

                //Eliminazione dal convex hull dell'elemento in coda al vettore
                if (i!=0)
                {
                    //delete (elemTail->hullPoint);
                    //delete elemTail;
                }

                //Spostamento al vettore successivo
                elemTail = elemHead;
                elemHead = elemHead->next;
                head = elemHead->hullPoint;
                tail = elemTail->hullPoint;
                cout<<*tail;
                cout<<*head;
                d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
                cout<<d<<endl;
                cout<<endl;
                i++;
            }

            //Aggiunta di un nuovo elemento nel convex hull
            newElem = new convexHullElem(point, *newTriangles.back());
            cout<<*(newElem->hullPoint)<<endl;
            cout<<*(newElem->externalTriangle)<<endl;
            elemTail->SetPrev(newElem);
            newElem->SetNext(elemTail);
            elemTail->SetTriangle(newTriangles.back());

            //...DA ELIMINARE...
            convexHullElem* successivoNewElem = newElem->next;
            cout<<"Primo punto da collegare: ";
            cout<<*(successivoNewElem->hullPoint);
            cout<<endl;
            //..................

            //Torno al punto iniziale
            elemHead = convexHull;
            elemTail = convexHull->prev;
            head = elemHead->hullPoint;
            tail = elemTail->hullPoint;
            cout<<*tail;
            cout<<*head;
            d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
            cout<<d<<endl<<endl;

            //Inizio a girare in senso orario
            while (d<0)
            {
                //Aggiunta del nuovo triangolo
                Triangle* newGuideTriangle = new Triangle(point, *head, *tail);
                newTriangles.push_back(newGuideTriangle);
//                meshTriangles.push_back(newGuideTriangle);
//                guideTriangles.push_back(newGuideTriangle);
                cout<<*newGuideTriangle;
                cout<<endl;

                //Eliminazione dal convex hull dell'elemento in coda al vettore
                //delete (elemHead->hullPoint);
                //delete elemHead;

                //Spostamento al vettore successivo
                elemHead = elemTail;
                elemTail = elemTail->prev;
                head = elemHead->hullPoint;
                tail = elemTail->hullPoint;
                cout<<*tail;
                cout<<*head;

                d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
                cout<<d<<endl<<endl;
            }

            cout<<*(newElem->hullPoint)<<endl;
            cout<<*(newElem->externalTriangle)<<endl;

            //Aggiunta di un nuovo legame nel convex hull
            elemHead->SetNext(newElem);
            newElem->SetPrev(elemHead);
            //newElem->SetTriangle(&newTriangles.back());
            SetConvexHull(newElem);
            newElem->SetTriangle(newTriangles.back());

            //...DA ELIMINARE...
            convexHullElem* precedenteNewElem = newElem->prev;
            cout<<"Ultimo punto da collegare: ";
            cout<<*(precedenteNewElem->hullPoint);
            cout<<endl;
            //..................
        }
        cout<<"Indirizzo interno all'elemento (dopo l'if)\n"<<newElem->hullPoint<<endl;
    }

//    void Mesh::AddInternalPoint(Point point)
//    {

//    }

    void Delaunay::Show()
    {
        cout<<"PUNTI DELAUNAY"<<endl;
        string str = "";
        for (Point point : pointsVector){
            cout<<point;
        }
        cout<<endl;
    }


    Grid::Grid(vector<Point>& points)
    {

        int n = points.size();
        intNum = (int)(sqrt(points.size()));
        squares.resize(intNum, intNum);
        //Eigen::Matrix<Square, intNum, intNum> squaresLocal;
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
                squares(i,j) = Square(startX, startY);
            }
        }
        //squares = squaresLocal;
    }


    void Grid::Show()
    {
        cout<<"GRIGLIA"<<endl;
        for (int i=0; i<intNum; i++)
        {
            for (int j=0; j<intNum; j++){cout<<"("+squares(i,j).toString()+")"<<"    ";}
            cout<<endl;
        }
        cout<<endl;
    }
}
