#include "empty_class.hpp"
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

namespace DelaunayLibrary
{

    double TOL = 0.000000001;

////////////////////// COSTRUTTORI ///////////////////////


    //Costruttore triangolo da vertici
    Triangle::Triangle(Point& p1, Point& p2, Point& p3)
    {
        double crossProduct = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
        if (crossProduct < 0){vertices[0]=p1; vertices[1]=p3; vertices[2]=p2;}
        else {vertices[0]=p1; vertices[1]=p2; vertices[2]=p3;}
        adjacentTriangles = {nullptr, nullptr, nullptr};
    }


    //Costruttore Delaunay da nome file
    Delaunay::Delaunay(const string& inputFileName)
    {
        fileName = inputFileName;
        vector<Point> randomVector;
        ifstream file;
        file.open(fileName);
        if (file.fail()){
            cerr << "Errore nell'apertura del file" << endl;
        }
        string line;
        getline(file, line);
        while (getline(file, line)){
            int useless;
            double x;
            double y;
            istringstream ss(line);
            ss >> useless >> x >> y;
//            Point point = Point(x*100.0, y*100.0);
            Point* point = new Point(x*100.0, y*100.0);
            pointsVector.push_back(point);
//            randomVector.push_back(point);
        }
//        sort(randomVector.begin(), randomVector.end());
//        for (Point pt:randomVector)
//        {
//            Point* point = new Point(pt.x, pt.y);
//            pointsVector.push_back(point);
//        }
        file.close();
    }


    //Costruttore griglia
    Grid::Grid(vector<Point*> points)
    {
        int n = points.size();
        intNum = (int)(sqrt(points.size()));
        rectangles.resize(intNum, intNum);
        //Ricerca estremi
        x_min = points[0]->x;
        double x_max = points[0]->x;
        y_min = points[0]->y;
        double y_max = points[0]->y;
        for (int i=1; i<n; i++)
        {
            if (points[i]->x<x_min){x_min = points[i]->x;}
            if (points[i]->x>x_max){x_max = points[i]->x;}
            if (points[i]->y<y_min){y_min = points[i]->y;}
            if (points[i]->y>y_max){y_max = points[i]->y;}
        }
        //Definizione ampiezza intervalli
        intervalX = (x_max-x_min)/intNum;
        intervalY = (y_max-y_min)/intNum;
        //Creazione rettangoli e inserimento in matrice
        for (int i=0; i<intNum; i++){
            for (int j=0; j<intNum; j++){
                double startX = x_min+intervalX*j;
                double startY = y_min+intervalY*i;
                rectangles(i,j) = Rectangle(startX, startY);
            }
        }
        //Inserimento in vettore punti
        for (unsigned int i = 0; i < points.size(); i++)
        {
            pointsGrid.push_back(points[i]);
        }
    }


    //Costruttore mesh
    Mesh::Mesh (Triangle& triangle)
    {
        guideTriangles.push_back(&triangle);
        //Creazione dei tre oggetti Convex Hull
        ConvexHullElem* firstElem = new ConvexHullElem(triangle.vertices[0], triangle);
        ConvexHullElem* secondElem = new ConvexHullElem(triangle.vertices[1], triangle);
        ConvexHullElem* thirdElem = new ConvexHullElem(triangle.vertices[2], triangle);
        //Assegnazione degli indirizzi ai puntatori
        firstElem->SetNext(secondElem);
        secondElem->SetNext(thirdElem);
        thirdElem->SetNext(firstElem);

        secondElem->SetPrev(firstElem);
        thirdElem->SetPrev(secondElem);
        firstElem->SetPrev(thirdElem);

        convexHull = firstElem;
    }


////////////////////// METODO PRINCIPALE ///////////////////////


    //Metodo che, a partire dal set di punti restituisce i lati della triangolazione di Delaunay
    void Delaunay::ExecuteDelaunay()
    {
        //Costruzione griglia
        Grid grid = Grid(pointsVector);
        //grid.Show();
        grid.PointsInRectangle(pointsVector);
        //Identificazione 4 punti iniziali
        array<Point, 4> firstPoints = grid.Snake();
        cout<<"Primi 4 punti"<<endl;
        for (Point point:firstPoints){cout<<point;}
        //Costruzione della mesh
        Triangle* firstTriangle = new Triangle(firstPoints[0],firstPoints[1],firstPoints[2]);
        //Triangle* firstTriangle = new Triangle(*pointsVector[0],*pointsVector[1],*pointsVector[2]);
        Mesh mesh = Mesh(*firstTriangle);
        //Mescolamento dell'ordine dei punti, ad eccezione dei primi 4
        //random_shuffle(pointsVector.begin(), pointsVector.end());
        pointsVector.insert(pointsVector.begin(),&firstPoints[3]);

        //AGGIUNTA PUNTI ALLA MESH
        bool flag=false;
        for (Point* point : pointsVector)
        {
            //Considero tutti i punti che non siano i primi 4 identificati
            //if (*point!=*pointsVector[0] && *point!=*pointsVector[1] && *point!=*pointsVector[2])
            if (*point!=firstPoints[0] && *point!=firstPoints[1] && *point!=firstPoints[2] && (*point!=firstPoints[3] || flag == false))
            {
                flag = true;
                //Controllo se il nuovo punto è interno o esterno
                int pos = mesh.CheckInside(*point);
                //Caso punto esterno
                if (pos==0){mesh.AddExternalPoint(*point);}
                //Caso punto interno o sul bordo
                else
                {
                    //Identifico il triangolo radice dentro cui si trova il punto
                    for (Triangle* trPtr:mesh.guideTriangles)
                    {
                        if ((trPtr->ContainsPoint(*point))!=-1)
                        {
                            //Identifico il triangolo foglia dentro cui si trova il punto
                            Triangle* bigTrianglePtr = trPtr->FromRootToLeaf(*point);
                            int posTr = bigTrianglePtr->ContainsPoint(*point);
                            //Caso in cui il punto sia interno al triangolo
                            if (posTr==0){mesh.AddInternalPoint(*point, bigTrianglePtr);}
                            //Caso in cui il punto sia sul bordo del triangolo
                            else {mesh.AddSidePoint(*point, bigTrianglePtr, posTr);}
                            break;
                        }
                    }

                }
                cout<<"Punto aggiunto: "<<*point;
            }
        }
        cout<<endl;
//        //Identificazione dei lati della triangolazione e stampa a schermo
        MeshToEdges(mesh.guideTriangles);
        cout<<endl;
        cout<<"LATI DELLA TRIANGOLAZIONE\n"<<endl;
        for (array<Point,2> side:finalEdges){cout<<"Lato\n"<<side[0]<<side[1]<<endl;}
        cout<<"Numero totale di lati: "<<finalEdges.size()<<endl;
//        //Caricamento dei risultati su file
        OutputEdges();

        //Eliminazione oggetti in memoria
        mesh.DeleteConvexHull();
        FromGraphToTree(mesh.guideTriangles);
        DeleteTriangles(mesh.guideTriangles);
        DeletePoints();
    }


////////////////////// IDENTIFICAZIONE TRIANGOLI INIZIALI ///////////////////////


    //Assegnazione dei punti ai rettangoli a cui sono interni
    void Grid::PointsInRectangle(vector<Point*> points)
    {
        for (Point* pt : points)
        {
            int col = floor((pt->x - x_min) / intervalX);
            if (col == intNum){col = intNum - 1;}
            int row = floor(intNum - ((pt->y - y_min) / intervalY));
            if (row == intNum){row = intNum - 1;}
            rectangles(row, col).containedPoints.push_back(*pt);
        }
    }


    //Selezione di 4 punti iniziali casuali
    array<Point, 4> Grid::PickFourRandomPoints(vector<Point*> points)
    {
        array<Point, 4> result;
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(points.begin(), points.end(), g);
        double prod1 = 0;
        while (abs(prod1) <= TOL)
        {
            std::shuffle(points.begin(), points.end(), g);
            prod1 = (points[2]->x - points[1]->x) * (points[0]->y - points[1]->y) - (points[0]->x - points[1]->x) * (points[2]->y - points[1]->y);  //Formula che mi restituisce zero se il point è allineato ai primi due punti
        }
        result[0] = *points[0];
        result[1] = *points[1];
        result[2] = *points[2];
        result[3] = *points[3];
        return result;
    }


    //Selezione dei quattro puntiiniziali tramite la serpentina
    array<Point, 4> Grid::Snake()
    {
        array<Point, 4> firstPoints;
        array<Rectangle, 3> chosenRectangles;
        bool foundRectangle = false; // Variable to track, for each corner, if a rectangle has been found
        bool rectanglesCollapsed = false; // Variable to track if the method reached an already found rectangle
        bool alignedPoints = false; // Variable to track if the method selected 3 aligned points as the first 3 points
        // primo punto (in alto a sx)
        for (int sum = 0; sum < intNum; sum++)
        {
            int i = sum;
            for (int j = 0; j <= sum; j++)
            {
                if ((rectangles(i, j).containedPoints).empty() == false)
                {
                    chosenRectangles[0] = rectangles(i, j);
                    firstPoints[0] = rectangles(i, j).containedPoints[0];
                    foundRectangle = true;
                    break; // Breaks out of the inner loop
                }
                if (i > 0){i--;}
            }
            if (foundRectangle){break;} // Breaks out of the outer loop as well
        }

        // primo punto (in basso a sx)
        foundRectangle = false; // Variable to track if the condition is satisfied
        for (int sum = intNum - 1; sum >= 0; sum--)
        {
            int i = sum;
            for (int j = 0; j <= intNum - 1 - sum; j++)
            {
                if (rectangles(i,j) == chosenRectangles[0])
                {
                    rectanglesCollapsed = true;
                    break;
                }
                if ((rectangles(i, j).containedPoints).empty() == false)
                {
                    chosenRectangles[1] = rectangles(i, j);
                    firstPoints[1] = rectangles(i, j).containedPoints[0];
                    foundRectangle = true;
                    break; // Breaks out of the inner loop
                }
                if (i < intNum - 1){i++;}
            }

            if (foundRectangle || rectanglesCollapsed == true){break;}  // Breaks out of the outer loop as well
        }

        // primo punto (in alto a dx)
        if (rectanglesCollapsed == false)
        {
            foundRectangle = false; // Variable to track if the condition is satisfied
            for (int sum = 0; sum < intNum; sum++)
            {
                int i = sum;
                for (int j = intNum - 1; j >= intNum - 1 - sum; j--)
                {
                    if (rectangles(i,j) == chosenRectangles[0] || rectangles(i,j) == chosenRectangles[1])
                    {
                        rectanglesCollapsed = true;
                        break;
                    }
                    if ((rectangles(i, j).containedPoints).empty() == false)
                    {
                        chosenRectangles[2] = rectangles(i, j);
                        Point point = rectangles(i, j).containedPoints[0];
                        double prod1 = (point.x - firstPoints[1].x) * (firstPoints[0].y - firstPoints[1].y) - (firstPoints[0].x - firstPoints[1].x) * (point.y - firstPoints[1].y);  //Formula che mi restituisce zero se il point è allineato ai primi due punti
                        if (abs(prod1) <= TOL)
                        {
                            alignedPoints = true;
                            break;
                        }
                        else
                        {
                            firstPoints[2] = point;
                            foundRectangle = true;
                            break; // Breaks out of the inner loop
                        }
                    }
                    if (i > 0){i--;}
                }
                if (foundRectangle || rectanglesCollapsed == true || alignedPoints == true){break;} // Breaks out of the outer loop as well
            }
        }

        // primo punto (in basso a dx)
        if (rectanglesCollapsed == false && alignedPoints == false)
        {
            foundRectangle = false; // Variable to track if the condition is satisfied
            for (int sum = intNum - 1; sum >= 0; sum--)
            {
                int i = sum;
                for (int j = intNum - 1; j >= sum; j--)
                {
                    if (rectangles(i,j) == chosenRectangles[0] || rectangles(i,j) == chosenRectangles[1] || rectangles(i,j) == chosenRectangles[2])
                    {
                       rectanglesCollapsed = true;
                       break;
                    }
                    if ((rectangles(i, j).containedPoints).empty() == false)
                    {
                        firstPoints[3] = rectangles(i, j).containedPoints[0];
                        foundRectangle = true;
                        break; // Breaks out of the inner loop
                    }
                    if (i < intNum - 1){i++;}
                }
                if (foundRectangle || rectanglesCollapsed == true){break;} // Breaks out of the outer loop as well
            }
        }
        if (rectanglesCollapsed == true)
        {
            cerr << "Il metodo della serpentina ha reggiunto due volte lo stesso rettangolo! Saranno dunque selezionati 4 punti casualmente!"<<endl;;
            return PickFourRandomPoints(pointsGrid);
        }
        else if (alignedPoints == true)
        {
            cerr << "Il metodo della serpentina ha selezionato 3 punti allineati! Saranno dunque selezionati 4 punti casualmente!"<<endl;
            return PickFourRandomPoints(pointsGrid);
        }
        return firstPoints;
    }


////////////////////// AGGIUNTA DI PUNTI ALLA MESH ///////////////////////


    //Metodo che restituisce 0 se il punto in input è interno al triangolo, -1 se è esterno,
    //mentre se giace su un lato del triangolo restituisce 1, 2 o 3 rispettivamente se giace sul lato tra i primi due vertivi, tra i secondi due o tra il primo e l'ultimo.
    int Triangle::ContainsPoint(Point& point)
    {
        double prod1 = (point.x - vertices[1].x) * (vertices[0].y - vertices[1].y) - (vertices[0].x - vertices[1].x) * (point.y - vertices[1].y);
        double prod2 = (point.x - vertices[2].x) * (vertices[1].y - vertices[2].y) - (vertices[1].x - vertices[2].x) * (point.y - vertices[2].y);
        double prod3 = (point.x - vertices[0].x) * (vertices[2].y - vertices[0].y) - (vertices[2].x - vertices[0].x) * (point.y - vertices[0].y);
        //Il punto si trova a sinistra di tutti i lati (interno)
        if (prod1>TOL && prod2>TOL && prod3>TOL){return 0;}
        //Il punto si trova a destra di almeno un lato (esterno)
        if (prod1<-TOL || prod2<-TOL || prod3<-TOL){return -1;}
        //Il punto si trova sul primo lato (assumiamo non possano esserci punti sovrapposti)
        if (abs(prod1)<TOL){return 1;}
        //Il punto si trova sul secondo lato
        if (abs(prod2)<TOL){return 2;}
        //Per esclusione il punto si trova sul terzo lato
        return 3;
    }


    //Controllo della posizione del punto rispetto alla mesh già esistente. Restituisce 1 se il punto è interno alla mesh o 0 se è esterno
    int Mesh::CheckInside(Point point)
    {
        ConvexHullElem* elemHead = convexHull->next;
        ConvexHullElem* elemTail = convexHull;
        Point* head = elemHead->hullPoint;
        Point* tail = elemTail->hullPoint;
        double d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
        while (elemHead!=convexHull && d>=-TOL)
        {
            elemTail = elemHead;
            elemHead = elemHead->next;
            head = elemHead->hullPoint;
            tail = elemTail->hullPoint;
            d = ((point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y));
        }
        if (d>=-TOL){return 1;}
        return 0;
    }


    //Aggiunta di un punto esterno
    void Mesh::AddExternalPoint(Point& point)
    {
        vector<Triangle*> newTriangles; //Vettore di tutti i nuovi triangoli aggiunti
        ConvexHullElem* newElem; //Nuovo elemento del ConvexHull creato
        //Inizializzazione primo vettore
        ConvexHullElem* elemHead = convexHull->next;
        ConvexHullElem* elemTail = convexHull;
        Point* head = elemHead->hullPoint;
        Point* tail = elemTail->hullPoint;
        double d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.

        //SE IL PRIMO LATO NON E' DA COLLEGARE
        if (d>=-TOL)
        {
            //Inizio a girare in senso antiorario
            while (d>=-TOL)
            {
                elemTail = elemHead;
                elemHead = elemHead->next;
                head = elemHead->hullPoint;
                tail = elemTail->hullPoint;
                d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
            }
            //Aggiunta di un nuovo triangolo
            Triangle* newGuideTriangle = new Triangle(point, *head, *tail);
            newTriangles.push_back(newGuideTriangle);
            guideTriangles.push_back(newGuideTriangle);
            //Aggiornamento adiacenze
            Point* midPoint = new Point(((tail->x)+(head->x))/2,((tail->y)+(head->y))/2);
            Triangle* ExternalTriangle = elemHead->externalTriangle;
            Triangle* lastTrPtr = ExternalTriangle->FromRootToLeaf(*midPoint);
            array<Point*,4> check = Triangle::FindCommonEdge(*newGuideTriangle, *lastTrPtr);
            Triangle::SetAdjacentTriangle(*lastTrPtr, newGuideTriangle, *check[1], *check[0]);
            delete midPoint;
            // Aggiunta di un nuovo elemento nel convex hull
            newElem = new ConvexHullElem(point, *newGuideTriangle);
            newElem->SetPrev(elemTail);
            elemTail->SetNext(newElem);
            //Passaggio al vettore successivo
            elemTail = elemHead;
            elemHead = elemHead->next;
            head = elemHead->hullPoint;
            tail = elemTail->hullPoint;
            d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.

            //Continuo a girare in senso antiorario
            while (d<-TOL)
            {
                //Aggiunta del nuovo triangolo
                Triangle* newGuideTriangle = new Triangle(point, *head, *tail);
                //Aggiornamento adiacenze tra nuovi triangoli
                array<Point*, 4> Check = Triangle::FindCommonEdge(*newGuideTriangle, *(newTriangles.back()));
                Triangle::SetAdjacentTriangle(*newGuideTriangle, newTriangles.back(), *Check[0], *Check[1]);
                //Aggiornamento adiacenze con triangoli esistenti
                Point* midPoint = new Point(abs((tail->x)+(head->x))/2,abs((tail->y)+(head->y))/2);
                Triangle* lastTrPtr = (elemHead->externalTriangle)->FromRootToLeaf(*midPoint);
                array<Point*, 4> Prova2 = Triangle::FindCommonEdge(*lastTrPtr, *newGuideTriangle);
                Triangle::SetAdjacentTriangle(*lastTrPtr, newGuideTriangle, *Prova2[0], *Prova2[1]);
                delete midPoint;
                //Aggiunta triangolo a vettori
                newTriangles.push_back(newGuideTriangle);
                guideTriangles.push_back(newGuideTriangle);
                //Eliminazione dal convex hull dell'elemento in coda al vettore
                delete elemTail;
                //Passaggio al vettore successivo
                elemTail = elemHead;
                elemHead = elemHead->next;
                head = elemHead->hullPoint;
                tail = elemTail->hullPoint;
                d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
            }
            //Aggiunta di un nuovo legame nel convex hull
            elemTail->SetPrev(newElem);
            newElem->SetNext(elemTail);
            elemTail->SetTriangle(newTriangles.back());
        }

        //SE IL PRIMO LATO E' DA COLLEGARE
        else
        {
            int i=0;
            //Inizio a girare in senso antiorario
            while (d<-TOL)
            {
                //Aggiunta del nuovo triangolo
                Triangle* newGuideTriangle = new Triangle(point, *head, *tail);
                //Aggiornamento adiacenze
                if (i != 0)
                {
                    array<Point*, 4> Output_giusto_1 = Triangle::FindCommonEdge(*newGuideTriangle, *(newTriangles.back()));
                    Triangle::SetAdjacentTriangle(*newGuideTriangle, newTriangles.back(), *(Output_giusto_1[0]), *(Output_giusto_1[1]));
                }
                Point* midPoint = new Point(((tail->x)+(head->x))/2,((tail->y)+(head->y))/2);
                Triangle* lastTrPtr = (elemHead->externalTriangle)->FromRootToLeaf(*midPoint);
                array<Point*, 4> Output_giusto = Triangle::FindCommonEdge(*lastTrPtr, *newGuideTriangle);
                Triangle::SetAdjacentTriangle(*lastTrPtr, newGuideTriangle, *Output_giusto[0], *Output_giusto[1]);
                delete midPoint;
                //Aggiunta triangolo a vettori
                newTriangles.push_back(newGuideTriangle);
                guideTriangles.push_back(newGuideTriangle);
                //Eliminazione dal convex hull dell'elemento in coda al vettore
                if (i!=0){delete elemTail;}
                //Passaggio al vettore successivo
                elemTail = elemHead;
                elemHead = elemHead->next;
                head = elemHead->hullPoint;
                tail = elemTail->hullPoint;
                d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
                i++;
            }
            //Aggiunta di un nuovo elemento nel convex hull
            newElem = new ConvexHullElem(point, *newTriangles[0]);
            elemTail->SetPrev(newElem);
            newElem->SetNext(elemTail);
            elemTail->SetTriangle(newTriangles.back());
            //Torno al punto iniziale
            elemHead = convexHull;
            elemTail = convexHull->prev;
            head = elemHead->hullPoint;
            tail = elemTail->hullPoint;
            d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.

            //Inizio a girare in senso orario
            int j = 0;
            while (d<-TOL)
            {
                //Aggiunta del nuovo triangolo
                Triangle* newGuideTriangle = new Triangle(point, *head, *tail);
                //Aggiornamento adiacenze
                if (j == 0)
                {
                    array<Point*, 4> Output_2 = Triangle::FindCommonEdge(*newGuideTriangle, *(newTriangles[0]));
                    Triangle::SetAdjacentTriangle(*newGuideTriangle, newTriangles[0], *Output_2[0], *Output_2[1]);
                }
                else
                {
                    array<Point*, 4> Output_3 = Triangle::FindCommonEdge(*newGuideTriangle, *(newTriangles.back()));
                    Triangle::SetAdjacentTriangle(*newGuideTriangle, newTriangles.back(), *Output_3[0], *Output_3[1]);
                }
                Point* midPoint = new Point(((tail->x)+(head->x))/2,((tail->y)+(head->y))/2);
                Triangle* lastTrPtr = (elemHead->externalTriangle)->FromRootToLeaf(*midPoint);
                array<Point*, 4> Output_giusto = Triangle::FindCommonEdge(*lastTrPtr, *newGuideTriangle);
                Triangle::SetAdjacentTriangle(*lastTrPtr, newGuideTriangle, *Output_giusto[0], *Output_giusto[1]);
                delete midPoint;
                //Aggiunta triangolo a vettori
                newTriangles.push_back(newGuideTriangle);
                guideTriangles.push_back(newGuideTriangle);
                //Passaggio al vettore successivo
                elemHead = elemTail;
                elemTail = elemTail->prev;
                head = elemHead->hullPoint;
                tail = elemTail->hullPoint;
                d = (point.x - head->x) * (tail->y - head->y) - (tail->x - head->x) * (point.y - head->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
                j++;
            }
            //Aggiunta di un nuovo legame nel convex hull
            elemHead->SetNext(newElem);
            newElem->SetPrev(elemHead);
            convexHull = newElem;
            if (j!=0){newElem->SetTriangle(newTriangles.back());}
        }
        //Applicazione algoritmo per ipotesi di Delaunay
        DelaunayNewTriangles(newTriangles);
    }


    //Ricerca del triangolo foglia dentro cui si trova il punto, partendo dal triangolo radice
    Triangle* Triangle::FromRootToLeaf(Point& point)
    {
        if (pointedTriangles.empty()){return this;}
        for (Triangle* tr:pointedTriangles)
        {
            if (tr->ContainsPoint(point)!=-1){return tr->FromRootToLeaf(point);}
        }
        cerr << "Problemi di riconoscimento del triangolo a cui il punto è interno" << endl;
        exit(1);
    }


    //Aggiunta punto interno ad un triangolo pre-esistente
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
        //Aggiornamento adiacenze
        Triangle::SetAdjacentTriangle(*triangle1, bigTriangle->adjacentTriangles[0], (bigTriangle->vertices[0]), (bigTriangle->vertices[1]));
        Triangle::SetAdjacentTriangle(*triangle2, bigTriangle->adjacentTriangles[1], bigTriangle->vertices[1], bigTriangle->vertices[2]);
        Triangle::SetAdjacentTriangle(*triangle3, bigTriangle->adjacentTriangles[2], bigTriangle->vertices[2], bigTriangle->vertices[0]);
        Triangle::SetAdjacentTriangle(*triangle1, triangle2, bigTriangle->vertices[1], point);
        Triangle::SetAdjacentTriangle(*triangle2, triangle3, bigTriangle->vertices[2], point);
        Triangle::SetAdjacentTriangle(*triangle3, triangle1, bigTriangle->vertices[0], point);
        //Aggiunta triangoli a vettore
        vector<Triangle*> newTriangles;
        newTriangles.push_back(triangle3);
        newTriangles.push_back(triangle1);
        newTriangles.push_back(triangle2);
        //Applicazione algoritmo per ipotesi di Delaunay
        DelaunayNewTriangles(newTriangles);
    }


    //Aggiunta punto su un lato pre-esistente
    void Mesh::AddSidePoint(Point& point, Triangle* bigTriangle, int side)
    {
        //Se il punto è su un lato tra due triangoli interni alla mesh
        vector<Triangle*> newTriangles;
        if (bigTriangle->adjacentTriangles[side-1]!=nullptr)
        {
            Point nullPoint = Point(0,0);
            Triangle* adjacentTrPtr = bigTriangle->adjacentTriangles[side-1];
            Point* oppositPointPtr = &nullPoint;
            int commonSidePos = 0;
            int i=0;
            for (Point pt:(adjacentTrPtr->vertices)){
                if (pt!=bigTriangle->vertices[side-1] && pt!=bigTriangle->vertices[side%3]){oppositPointPtr=&pt; commonSidePos=(i+1)%3; break;}
                i++;
            }
            //Creazione dei nuovi triangoli
            Triangle* triangle1 = new Triangle(bigTriangle->vertices[side-1], bigTriangle->vertices[(side+1)%3], point);
            Triangle* triangle2 = new Triangle(bigTriangle->vertices[side%3], bigTriangle->vertices[(side+1)%3], point);
            Triangle* triangle3 = new Triangle(bigTriangle->vertices[side-1], *oppositPointPtr, point);
            Triangle* triangle4 = new Triangle(bigTriangle->vertices[side%3], *oppositPointPtr, point);
            //Inserimento dei nuovi triangoli nel vettore dei puntati del padre
            bigTriangle->pointedTriangles.push_back(triangle1);
            bigTriangle->pointedTriangles.push_back(triangle2);
            adjacentTrPtr->pointedTriangles.push_back(triangle3);
            adjacentTrPtr->pointedTriangles.push_back(triangle4);
            //Aggiornamento adiacenze con triangoli pre-esistenti
            Triangle::SetAdjacentTriangle(*triangle1, bigTriangle->adjacentTriangles[(side+1)%3], bigTriangle->vertices[(side+1)%3], bigTriangle->vertices[side-1]);
            Triangle::SetAdjacentTriangle(*triangle2, bigTriangle->adjacentTriangles[side%3], bigTriangle->vertices[side%3], bigTriangle->vertices[(side+1)%3]);
            Triangle::SetAdjacentTriangle(*triangle3, adjacentTrPtr->adjacentTriangles[(commonSidePos+1)%3], bigTriangle->vertices[side-1], *oppositPointPtr);
            Triangle::SetAdjacentTriangle(*triangle4, adjacentTrPtr->adjacentTriangles[(commonSidePos+2)%3], *oppositPointPtr, bigTriangle->vertices[side%3]);
            //Aggiornamento adiacenze tra nuovi triangoli aggiunti
            Triangle::SetAdjacentTriangle(*triangle1, triangle2, point, bigTriangle->vertices[(side+1)%3]);
            Triangle::SetAdjacentTriangle(*triangle3, triangle4, *oppositPointPtr, point);
            Triangle::SetAdjacentTriangle(*triangle1, triangle3, bigTriangle->vertices[side-1], point);
            Triangle::SetAdjacentTriangle(*triangle2, triangle4, point, bigTriangle->vertices[side%3]);
            //Aggiunta triangoli a vettore
            newTriangles.push_back(triangle1);
            newTriangles.push_back(triangle2);
            newTriangles.push_back(triangle3);
            newTriangles.push_back(triangle4);
        }

        //Se il punto è su un lato del ConvexHull
        else
        {
            //Creazione dei nuovi triangoli
            Triangle* triangle1 = new Triangle(bigTriangle->vertices[side-1], bigTriangle->vertices[(side+1)%3], point);
            Triangle* triangle2 = new Triangle(bigTriangle->vertices[side%3], bigTriangle->vertices[(side+1)%3], point);
            //Inserimento dei nuovi triangoli nel vettore dei puntati del padre
            bigTriangle->pointedTriangles.push_back(triangle1);
            bigTriangle->pointedTriangles.push_back(triangle2);
            //Aggiornamento adiacenze
            Triangle::SetAdjacentTriangle(*triangle1, bigTriangle->adjacentTriangles[(side+1)%3], bigTriangle->vertices[(side+1)%3], bigTriangle->vertices[side-1]);
            Triangle::SetAdjacentTriangle(*triangle2, bigTriangle->adjacentTriangles[side%3], bigTriangle->vertices[side%3], bigTriangle->vertices[(side+1)%3]);
            Triangle::SetAdjacentTriangle(*triangle1, triangle2, point, bigTriangle->vertices[(side+1)%3]);
            //Aggiornamento convexHull
            ConvexHullElem* elemHead = convexHull->next;
            ConvexHullElem* elemTail = convexHull;
            Point* tail = elemTail->hullPoint;
            while (*tail!=(bigTriangle->vertices[side-1]))
            {
                elemTail = elemHead;
                elemHead = elemHead->next;
                tail = elemTail->hullPoint;
            }
            ConvexHullElem* newElem = new ConvexHullElem(point, *triangle1);
            newElem->SetPrev(elemTail);
            newElem->SetNext(elemHead);
            elemHead->SetPrev(newElem);
            elemTail->SetNext(newElem);
            elemHead->SetTriangle(triangle2);
            //Aggiunta triangoli a vettore
            newTriangles.push_back(triangle1);
            newTriangles.push_back(triangle2);
        }
        //Applicazione algoritmo per ipotesi di Delaunay
        DelaunayNewTriangles(newTriangles);
    }


////////////////////// AGGIORNAMENTO ADIACENZE ///////////////////////


    //Dati due triangoli, restituisce un vettore output che presenta al proprio interno, in ordine:
    // - i due punti del lato in comune ai due triangoli ordinati in senso antiorario rispetto al primo triangolo;
    // - i due punti non comuni dei triangoli;
    //Se i due triangoli non presentano punti in comune, restituisce semplicemente un array di nullptr
    array<Point*,4> Triangle::FindCommonEdge(Triangle& triangle1, Triangle& triangle2)
    {
        array<int, 2> posOpp = {-1,-1};  //Posizione 0: Opposto in tr1; Posizione 1: Opposto in tr2

        for (int i=0; i<3; i++)
        {
           bool found = true;
           for (int j=0; j<3; j++)
           {
                if (triangle1.vertices[i] == triangle2.vertices[j]){found =false;}
           }
           if (found == true){posOpp[0]=i; break;}
        }
        for (int i=0; i<3; i++)
        {
           bool found = true;
           for (int j=0; j<3; j++)
           {
                if (triangle2.vertices[i] == triangle1.vertices[j]){found =false;}
           }
           if (found == true){posOpp[1]=i; break;}
        }
        int opp1 = posOpp[0];
        int opp2 = posOpp[1];
        array<Point*,4> output = {nullptr,nullptr,nullptr,nullptr};
        if ((triangle1.vertices[(opp1+1)%3] == triangle2.vertices[(opp2+2)%3]) && (triangle1.vertices[(opp1+2)%3] == triangle2.vertices[(opp2+1)%3]))
        {
            output[0] = &(triangle1.vertices[(opp1+1)%3]);
            output[1] = &(triangle1.vertices[(opp1+2)%3]);
            output[2] = &(triangle1.vertices[opp1]);
            output[3] = &(triangle2.vertices[opp2]);
        }
        return output;
    }


    //Settaggio delle adiacenze tra i due triangoli passati come argomenti
    void Triangle::SetAdjacentTriangle(Triangle& existingTriangle, Triangle* addingTriangle, Point& tail, Point& head) //head e tail sono la testa e la coda del vettore visto come lato del triangolo già esistente ordiato in senso antiorario.
    {
        //Settare triangolo adiacente a triangolo già esistente
        if (existingTriangle.vertices[0] == tail){existingTriangle.adjacentTriangles[0] = addingTriangle;}
        else if (existingTriangle.vertices[1] == tail){existingTriangle.adjacentTriangles[1] = addingTriangle;}
        else {existingTriangle.adjacentTriangles[2] = addingTriangle;}
        //Settare triangolo adiacente a triangolo aggiunto
        if (addingTriangle!=nullptr){
            if (addingTriangle->vertices[0] == head){addingTriangle->adjacentTriangles[0] = &existingTriangle;}
            else if (addingTriangle->vertices[1] == head){addingTriangle->adjacentTriangles[1] = &existingTriangle;}
            else {addingTriangle->adjacentTriangles[2] = &existingTriangle;}}
    }


    //Aggiornamento delle adiacenze del nuovo triangolo formatosi in seguito al flip
    //grazie alle liste di adiacenza dei due vecchi triangoli: Triangle1 e Triangle2.
    void Triangle::Adjourn(Triangle* Triangle_new_1, Triangle* Triangle1, Triangle* Triangle2)
    {
        if (Triangle1->adjacentTriangles[0] == nullptr && Triangle1->adjacentTriangles[1] == nullptr && Triangle1->adjacentTriangles[2] == nullptr){return;}
        for (Triangle* triangolo : Triangle1->adjacentTriangles)
        {
            if ((triangolo == nullptr) or (*triangolo == *Triangle2)){continue;}
            array<Point*,4> Output = Triangle::FindCommonEdge(*Triangle_new_1, *triangolo);
            if (Output[0] != nullptr)
            {
                array<Point*, 4> OutputTriangle = FindCommonEdge(*Triangle_new_1, *triangolo);
                Triangle::SetAdjacentTriangle(*Triangle_new_1, triangolo, *OutputTriangle[0], *OutputTriangle[1]);
            }
        }
    }


////////////////////// IPOTESI DI DELAUNAY ///////////////////////


    //Calcolo del determinante di una matrice 3x3
    double determinante(double& a11, double& a12, double& a13, double& a21, double& a22, double& a23, double& a31, double& a32, double& a33)
    {
        double det = (a11 * a22 * a33) + (a12 * a23 * a31) + (a13 * a21 * a32) - (a13 * a22 * a31) - (a12 * a21 * a33) - (a11 * a23 * a32);
        return det;
    }


    // verifica la proprietà di Delaunay con il metodo del determinante
    bool Triangle::DelaunayProperty(Triangle& Triangle1, Triangle& Triangle2)
    {
        Point A = Triangle1.vertices[0];
        Point B = Triangle1.vertices[1];
        Point C = Triangle1.vertices[2];
        Point D;
        for (int i = 0; i < 3; i++)
        {
            int k = 0;
            for (int j = 0; j < 3; j++)
            {
                if (Triangle2.vertices[i].x == Triangle1.vertices[j].x and Triangle1.vertices[j].y == Triangle2.vertices[i].y){k = 1;}
            }
            if (k == 0){D = Triangle2.vertices[i];}
        }
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
        if (a > 0){return 0;}
        else{return 1;}
    }


    //Controllo della convessità del quadrilatero per verifica ipotei Delaunay
//    bool CheckConvex(Triangle& Triangle1, Triangle& Triangle2)
//    {
//        int i = 0;
//        int sign = 0;
//        Point* esterno1;
//        Point* esterno2;
//        array<Point*,2> Lato[2];
//        for (auto& punto1 : Triangle1.vertices)
//        {
//           for (auto& punto2 : Triangle2.vertices)
//           {
//            if (punto1 == punto2)
//            {
//              Lato[i] = punto1;
//              i += 1;
//              sign = 1;
//            }
//           }
//           if (sign == 0){esterno1 = punto1;}
//        }
//        for (auto& punto : Triangle2.vertices)
//        {
//            if ((punto.x == Lato[0].x and punto.y == Lato[0].y) or (punto.x == Lato[1].x and punto.y == Lato[1].y)){}
//            else{esterno2 = punto;}
//        }
//        Point Quadril[4];
//        Quadril[0] = esterno1;
//        Quadril[1] = Lato[0];
//        Quadril[2] = esterno2;
//        Quadril[3] = Lato[1];
//        bool segno[4];
//        for (int i = 0; i < 4; i++)
//        {
//            // double a = ((Quadril[(i+1)%4].x - Quadril[i].x) * (Quadril[(i+2)%4].y - Quadril[(i+1)%4].y) - (Quadril[(i+1)%4].y - Quadril[i].y) * (Quadril[(i+2)%4].x - Quadril[(i+1)%4].x));
//            if (( Quadril[(i+1)%4].x - Quadril[i].x) * (Quadril[(i+2)%4].y - Quadril[(i+1)%4].y) - (Quadril[(i+1)%4].y - Quadril[i].y) * (Quadril[(i+2)%4].x - Quadril[(i+1)%4].x) > 0)
//            {
//                segno [i] = 1;
//            }
//            else
//            {
//                segno[i] = 0;
//            }
//        }
//        if ((segno[0] == segno[1]) and (segno[1] == segno[2]) and (segno[2] == segno [3])){return 1;}
//        else{return 0;}
//    }


    // Operazione di flip vera e propria, considero due triangoli con un lato adiacente, lo trovo, costruisco due nuovi triangoli
    // a partire da quelli già esistenti e aggiorno tutte le liste di adiacenza, oltre che l'array pointedTriangles per ciascuno
    // dei due triangoli a partire dai quali si è originato il flip;
    array<Triangle*, 2> Triangle::Flip(Triangle& Triangle1, Triangle& Triangle2)
    {
        Point* esterno1;
        Point* esterno2;
        Point* Lato[2];
        array<Point*, 4> Output = Triangle::FindCommonEdge(Triangle1, Triangle2);
        esterno1 = Output[2];
        esterno2 = Output[3];
        Lato[0] = Output[0];
        Lato[1] = Output[1];
        //Creazione dei nuovi triangoli
        Triangle* Triangle_new_1 = new Triangle(*esterno1, *esterno2, *Lato[0]);
        Triangle* Triangle_new_2 = new Triangle(*esterno1, *esterno2, *Lato[1]);
        //Aggiornamento adiacenze
        array<Point*, 4> Output_new = Triangle::FindCommonEdge(*Triangle_new_1, *Triangle_new_2);
        Triangle::SetAdjacentTriangle(*Triangle_new_1, Triangle_new_2, *Output_new[0], *Output_new[1]);
        Triangle::Adjourn(Triangle_new_1, &Triangle1, &Triangle2);
        Triangle::Adjourn(Triangle_new_1, &Triangle2, &Triangle1);
        Triangle::Adjourn(Triangle_new_2, &Triangle2, &Triangle1);
        Triangle::Adjourn(Triangle_new_2, &Triangle1, &Triangle2);
        //Inserimento dei nuovi triangoli nel vettore dei puntati del padre
        Triangle1.pointedTriangles.push_back(Triangle_new_1);
        Triangle1.pointedTriangles.push_back(Triangle_new_2);
        Triangle2.pointedTriangles.push_back(Triangle_new_1);
        Triangle2.pointedTriangles.push_back(Triangle_new_2);
        //Restituzione output
        array<Triangle*, 2> Tri_prop;
        Tri_prop[0] = Triangle_new_1;
        Tri_prop[1] = Triangle_new_2;
        return Tri_prop;
    }


    // Propagazione dell'algoritmo di Delaunay a partire da un triangolo. Abbiamo già verificato che l'input sia papabile, ossia che
    // non punti a nulla (siamo dunque nella mesh "più aggiornata" per intenderci). Parto dal presupposto che i triangoli adiacenti
    // facciano tutti parte della mesh più aggiornata (da verificare se in adjourn l'operazione porta effettivamente a una situazione
    // di questo tipo, in cui l'adiacenza a quello da flippare viene sostituita con l'adiacenza a quello flippato;
    void Mesh::DelaunayPropagation(Triangle* StartTriangle)
    {
        for (int i = 0; i < 3; i++)
        {
            Triangle* TriangoloAdj =StartTriangle->adjacentTriangles[i];
            if (TriangoloAdj == nullptr){continue;}
            if (Triangle::DelaunayProperty(*StartTriangle, *TriangoloAdj) == 0)
                {
                array<Triangle*, 2> Tri_prop = Triangle::Flip(*StartTriangle, *TriangoloAdj);
                Triangle* Triangolo_flip_1  = Tri_prop[0];
                Triangle* Triangolo_flip_2  = Tri_prop[1];
                Mesh::DelaunayPropagation(Triangolo_flip_1);
                if (Triangolo_flip_2->pointedTriangles.empty())
                {
                    Mesh::DelaunayPropagation(Triangolo_flip_2);
                }
            }
        }
    }


    // Inizializzazione dell'operazione di flip secondo l'algoritmo di Delaunay dati i nuovi triangoli presenti nel vettore new_triangles
    void Mesh::DelaunayNewTriangles(vector<Triangle*> new_triangles)
    {
        for (auto& element : new_triangles)
        {
            if (element->pointedTriangles.empty()){DelaunayPropagation(element);}
        }
    }


////////////////////// OPERAZIONI DI DELETE ///////////////////////


    //Eliminazione degli elementi dinamici del ConvexHull
    void Mesh::DeleteConvexHull()
    {
        ConvexHullElem* elemHead = convexHull->next->next;
        ConvexHullElem* elemTail = convexHull->next;
        while (elemHead != convexHull)
        {
            delete elemTail;
            elemTail = elemHead;
            elemHead = elemHead->next;
        }
        delete elemTail;
        delete convexHull;
        elemTail = nullptr;
        convexHull = nullptr;
    }


    //Modifica di tutti i triangoli in triangoli degeneri e settaggio di puntatori a nullptr se triangoli sono puntati da piùtriangoli padri.
    void Delaunay::FromGraphToTree(vector<Triangle*>& trPtrVec)
    {
        for(unsigned int i=0; i<trPtrVec.size(); i++)
        {
            if (trPtrVec[i] != nullptr)
            {
                Point nullPoint = Point(0,0);
                Triangle trivialTriangle = Triangle(nullPoint,nullPoint,nullPoint);
                if (*(trPtrVec[i]) == trivialTriangle){trPtrVec[i] = nullptr;}
                else
                {
                    (trPtrVec[i])->vertices[0] = Point(0,0);
                    (trPtrVec[i])->vertices[1] = Point(0,0);
                    (trPtrVec[i])->vertices[2] = Point(0,0);
                }
                if (trPtrVec[i] != nullptr && !((trPtrVec[i])->pointedTriangles.empty())){FromGraphToTree((trPtrVec[i])->pointedTriangles);}
            }
        }
    }


    //Eliminazione dei triangoli allocati dinamicamente
    void Delaunay::DeleteTriangles(vector<Triangle*>& trPtrVec)
    {
        for (unsigned int i=0; i<trPtrVec.size(); i++)
        {
            if (trPtrVec[i] != nullptr)
            {
                if (!(trPtrVec[i]->pointedTriangles.empty()))
                {
                    DeleteTriangles(trPtrVec[i]->pointedTriangles);
                }
                delete trPtrVec[i];
                trPtrVec[i] = nullptr;
            }
        }
    }


    //Eliminazione dei punti
    void Delaunay::DeletePoints()
    {
        pointsVector[0] = nullptr;
        for (unsigned int i = 1; i < pointsVector.size(); i++)
        {
            delete pointsVector[i];
            pointsVector[i] = nullptr;
        }
    }


////////////////////// OUTPUT ///////////////////////


    //Ordinamento dei punti del lato dal minore al maggiore
    array<Point,2> Point::OrderSide(Point& point1, Point& point2)
    {
        if (point1<point2){return {point1,point2};}
        else {return {point2,point1};}
    }


    //Ricavare ricorsivamente i lati a partire dai triangoli
    void Delaunay::MeshToEdges(vector<Triangle*> trPtrVec)
    {
        for (Triangle* tr:trPtrVec)
        {
            if (tr->pointedTriangles.empty())
            {
                array<Point,2> side1 = Point::OrderSide(tr->vertices[0], tr->vertices[1]);
                array<Point,2> side2 = Point::OrderSide(tr->vertices[1], tr->vertices[2]);
                array<Point,2> side3 = Point::OrderSide(tr->vertices[2], tr->vertices[0]);
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


    //Caricamento output su file
    void Delaunay::OutputEdges()
    {
        string fileNameG = "outputEdges_Geogebra";
        ofstream fileG;
        fileG.open(fileNameG);
        if (fileG.fail()){
            cerr << "Errore nell'apertura del file per Geogebra" << endl;
        }
//        string outputFileName = "outputEdges";
//        ofstream outputFile;
//        outputFile.open(outputFileName);
//        if (outputFile.fail()){
//            cerr << "Errore nell'apertura del file di output" << endl;
//        }
        //Stampa dei punti sul file Geogebra
        fileG<<"PUNTI"<<endl;
        string fileStringPoints = "{";
        for (Point* point:pointsVector){fileStringPoints+=("("+to_string((point->x)/100.0)+","+to_string((point->y)/100.0)+"),");}
        fileStringPoints = fileStringPoints.substr(0,fileStringPoints.length()-1)+"}";
        fileG<<fileStringPoints<<endl;
        fileG<<endl;
        //STAMPA LATI SU FILE GEOGEBRA E FILE OUTPUT
        //Intestazione lati File Geogebra
        fileG<<"LATI"<<endl;
        //Intestazione lati File Output
//        outputFile<<"LATI TRIANGOLAZIONE DI DELAUNAY"<<endl;
//        outputFile<<endl;
        //Stampa lati
        string fileString = "{";
        int i=0;
        for (array<Point,2> side:finalEdges)
        {
            fileString+=("Segmento(("+to_string((side[0].x)/100.0)+","+to_string((side[0].y)/100.0)+"),("+to_string((side[1].x)/100.0)+","+to_string((side[1].y)/100.0)+")),");
//            outputFile<<to_string(i)+". ("+to_string((side[0].x)/100.0)+","+to_string((side[0].y)/100.0)+")  ("+to_string((side[1].x)/100.0)+","+to_string((side[1].y)/100.0)+")"<<endl;
            i++;
        }
        fileString = fileString.substr(0,fileString.length()-1)+"}";
        fileG<<fileString<<endl;
        fileG.close();
//        outputFile.close();
    }


    //Stampa a schermo di tutti i punti
    void Delaunay::Show()
    {
        cout<<"PUNTI DELAUNAY"<<endl;
        string str = "";
        for (Point* point : pointsVector){
            cout<<*point;
        }
        cout<<endl;
    }


    //Stampa a schermo degli estremi dei rettangoli della griglia
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


}
