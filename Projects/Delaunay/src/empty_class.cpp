
//IMPLEMENTAZIONE DEI METODI
#include "empty_class.hpp"
#include <fstream>


namespace DelaunayLibrary
{
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
//    }

    array<Point,3> Triangle::OrderVertices()
    {
        double d = (vertices[2].x - vertices[1].x) * (vertices[0].y - vertices[1].y) - (vertices[0].x - vertices[1].x) * (vertices[2].y - vertices[1].y);  //Formula che restituisce un numero positivo se il punto si trova a destra del primo lato, negativo se si trova a sinistra.
        if (d<0){return {vertices[0], vertices[2], vertices[1]};}
        return {vertices[0], vertices[1], vertices[2]};
    }

    Mesh::Mesh (Triangle& triangle)
    {
        meshTriangles.push_back(triangle);
        guideTriangles.push_back(triangle);
        array<Point,3> orderedPoints = triangle.OrderVertices();
        //Creazione dei tre oggetti
        convexHullElem* firstElem = new convexHullElem(orderedPoints[0], triangle);
        convexHullElem* secondElem = new convexHullElem(orderedPoints[1], triangle);
        convexHullElem* thirdElem = new convexHullElem(orderedPoints[2], triangle);
        //Assegnazione degli indirizzi ai puntatori
        firstElem->SetNext(secondElem);
        secondElem->SetNext(thirdElem);
        thirdElem->SetNext(firstElem);

        secondElem->SetPrev(firstElem);
        thirdElem->SetPrev(secondElem);
        firstElem->SetPrev(thirdElem);

        convexHull = firstElem;
    }

    void Mesh::AddExternalPoint(Point point)
    {
        cout<<"PRIMO PUNTO CONVEX HULL"<<endl;
        convexHullElem* elemHeadCCW = convexHull->next;
        convexHullElem* elemTailCCW = convexHull;
        Point* headCCW = elemHeadCCW->hullPoint;
        Point* tailCCW = elemTailCCW->hullPoint;
        tailCCW->Show();
        headCCW->Show();

        double d = (point.x - headCCW->x) * (tailCCW->y - headCCW->y) - (tailCCW->x - headCCW->x) * (point.y - headCCW->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
        cout<<d<<endl;
        cout<<endl;

        //Se il primo lato non è da collegare
        if (d>0)
        {
            //Inizio a girare in senso antiorario
            while (d>0)
            //for(int i=0; i<2; i++)
            {
                elemTailCCW = elemHeadCCW;
                elemHeadCCW = elemHeadCCW->next;
                headCCW = elemHeadCCW->hullPoint;
                tailCCW = elemTailCCW->hullPoint;
                tailCCW->Show();
                headCCW->Show();
                d = (point.x - headCCW->x) * (tailCCW->y - headCCW->y) - (tailCCW->x - headCCW->x) * (point.y - headCCW->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
                cout<<d<<endl<<endl;
            }
            //Aggiunta di un nuovo triangolo
            Triangle newGuideTriangle = Triangle(point, *headCCW, *tailCCW);
            Triangle lastTriangle = newGuideTriangle;
            meshTriangles.push_back(newGuideTriangle);
            guideTriangles.push_back(newGuideTriangle);
            newGuideTriangle.Show();
            cout<<endl;

            //Aggiunta di un nuovo elemento nel convex hull
            convexHullElem* newElem = new convexHullElem(point, newGuideTriangle);
            newElem->SetPrev(elemTailCCW);
            elemTailCCW->SetNext(newElem);

            //...DA ELIMINARE...
            //newElem->SetNext(firstElem);
            Point* firstRight = tailCCW;
            cout<<"Primo punto da collegare: ";
            firstRight->Show();
            cout<<endl;
            //..................

            elemTailCCW = elemHeadCCW;
            elemHeadCCW = elemHeadCCW->next;
            headCCW = elemHeadCCW->hullPoint;
            tailCCW = elemTailCCW->hullPoint;
            tailCCW->Show();
            headCCW->Show();
            d = (point.x - headCCW->x) * (tailCCW->y - headCCW->y) - (tailCCW->x - headCCW->x) * (point.y - headCCW->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
            cout<<d<<endl<<endl;

            //Continuo a girare in senso antiorario
            while (d<0)
            {
                //Aggiunta del nuovo triangolo
                Triangle newGuideTriangle = Triangle(point, *headCCW, *tailCCW);
                lastTriangle = newGuideTriangle;
                meshTriangles.push_back(newGuideTriangle);
                guideTriangles.push_back(newGuideTriangle);
                newGuideTriangle.Show();
                cout<<endl;

                //Eliminazione dal convex hull dell'elemento in coda al vettore
                delete elemTailCCW->hullPoint;
                delete elemTailCCW;

                //Spostamento al vettore successivo
                elemTailCCW = elemHeadCCW;
                elemHeadCCW = elemHeadCCW->next;
                headCCW = elemHeadCCW->hullPoint;
                tailCCW = elemTailCCW->hullPoint;
                tailCCW->Show();
                headCCW->Show();
                d = (point.x - headCCW->x) * (tailCCW->y - headCCW->y) - (tailCCW->x - headCCW->x) * (point.y - headCCW->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
                cout<<d<<endl<<endl;
            }

            //Aggiunta di un nuovo legame nel convex hull
            elemTailCCW->SetPrev(newElem);
            newElem->SetNext(elemTailCCW);
            elemTailCCW->SetTriangle(&lastTriangle);


            //...DA ELIMINARE...
            convexHullElem* successivoNewElem = newElem->next;
            cout<<"Ultimo punto da collegare: ";
            successivoNewElem->hullPoint->Show();
            cout<<endl;
            //..................
        }

        //Se il primo lato è da collegare
//        else if (d<0)
//        {
//            //Inizio a girare in senso antiorario
//            while (d<0)
//            {
//                //Aggiunta del nuovo triangolo
//                Triangle newGuideTriangle = Triangle(point, *headCCW, *tailCCW);
//                //Triangle lastTriangle = newGuideTriangle;
//                meshTriangles.push_back(newGuideTriangle);
//                guideTriangles.push_back(newGuideTriangle);
//                newGuideTriangle.Show();

//                elemTailCCW = elemHeadCCW;
//                elemHeadCCW = elemHeadCCW->next;
//                headCCW = elemHeadCCW->hullPoint;
//                tailCCW = elemTailCCW->hullPoint;
//                tailCCW->Show();
//                headCCW->Show();
//                d = (point.x - headCCW->x) * (tailCCW->y - headCCW->y) - (tailCCW->x - headCCW->x) * (point.y - headCCW->y);  //Formula che restituisce un numero positivo se il punto si trova a sinistra del lato, negativo se si trova a destra.
//                cout<<d<<endl;
//            }
//        }


//        convexHullElem* elemHeadCW = convexHull;
//        convexHullElem* elemTailCW = convexHull->prev;
//        Point* headCW = elemHeadCW->hullPoint;
//        headCW->Show();
//        Point* tailCW = elemTailCW->hullPoint;
//        tailCW->Show();
        //Inizio a girare in senso orario
        //while (tailCCW != headCW && headCCW !=headCW)
       //        {
       //            convexHullElem* elemHeadCW = elemTailCW;
       //            convexHullElem* elemTailCW = elemTailCW->prev;
       //            Point* headCW = elemHeadCW->hullPoint;
       //            headCW->Show();
       //            Point* tailCW = elemTailCW->hullPoint;
       //            tailCW->Show();
    }

//    void Mesh::AddInternalPoint(Point point)
//    {

//    }

    void Delaunay::Show()
    {
        cout<<"PUNTI DELAUNAY"<<endl;
        string str = "";
        for (Point point : pointsVector){
            str += point.toString() + '\n';
        }
        cout<< str << endl;
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
