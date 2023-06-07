
//IMPLEMENTAZIONE DEI METODI
#include "empty_class.hpp"
#include <fstream>
#include <vector>


namespace DelaunayLibrary
{
    //Metodo che restituisce 0 se il punto in input è interno al triangolo, -1 se è esterno,
    //mentre se giace su un lato del triangolo restituisce 1, 2 o 3 rispettivamente se giace sul lato tra i primi due vertivi, tra i secondi due o tra il primo e l'ultimo.
    void Mesh::OperationEdges(Triangle& TriangleNew1, Triangle& TriangleNew2, Triangle& TriangleOld1, Triangle& TriangleOld2)
    {
      for (int i; i < 3; i++)
      {

      }
    }

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
    //void Delaunay::ExecuteDelaunay()
    //{
        //Grid grid = Grid(pointsVector); //Costruzione griglia
        //grid.PointsInRectangle(pointsVector); //Aggiunge ad ogni punto il rettangolo a cui è interno e ad ogni rettangolo il vector dei punti a cui è interno. (Valutare se fondere con costruttore griglia)
        //Point firstPoints[4] = grid.Snake(); //Restituisce 4 punti di cui i primi 3 sono il triangolone iniziale (Attenzione a punti allineati, stesso punto, ecc.)
        //firstTriangle = Triangle(firstPoints[0],firstPoints[1],firstPoints[2]);
        //Mesh mesh = Mesh(firstTriangle); //Decidere come gestire Convex Hull
        //Delaunay con quarto punto
        //int position = ContainsPoint(firstPoints[4]); //Controllare se è interno o esterno
        //if (position == -1){mesh.AddExternalPoint(firstPoints[4]);}
        //else if (position == 0) {mesh.AddInternalPoint(firstPoints[4], firstTriangle);}
        //else {} //Da capire
        //for (Point point : pointsVector)
        //{
            // (crossing Triangle) Controllare se il punto è esterno o interno (e in tal caso identificare il triangolo a cui è interno)
            // If interno AddInternalPoint, if external AddExternalPoint.
        //}
        //mesh.MeshToEdges();
    //}

    //void Mesh::AddExternalPoint(Point point)
    //{

    //}

    //void Mesh::AddInternalPoint(Point point)
    //{

    //}
    double determinante(double& a11, double& a12, double& a13, double& a21, double& a22, double& a23, double& a31, double& a32, double& a33)
    {
        double det = (a11 * a22 * a33) + (a12 * a23 * a31) + (a13 * a21 * a32) - (a13 * a22 * a31) - (a12 * a21 * a33) - (a11 * a23 * a32);
        return det;
    }

    void reorderPointsCounterclockwiseTr(Point& p1, Point& p2, Point& p3)
    {
        double crossProduct = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
        if (crossProduct < 0) {
            Point temp = p2;
            p2 = p3;
            p3 = temp;
        }
    }

    void reorderPointsCounterclockwiseQ(vector<Point> &quadruplet)
    {
        for (int i = 0; i < 3; i++)
        {
            if (( quadruplet[(i+1)%3].x - quadruplet[i].x) * (quadruplet[3].y - quadruplet[i].y) - (quadruplet[(i+1)%3].y - quadruplet[i].y) * (quadruplet[3].x - quadruplet[i].x) < 0)
            {
                vector<Point> Copia = quadruplet;
                quadruplet[0] = Copia[i];
                quadruplet[1] = Copia[3];
                quadruplet[2] = Copia[(i + 1) %3];
                quadruplet[3] = Copia[(i + 2) %3];
            }
        }
    }

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
            double a = ((Quadril[(i+1)%4].x - Quadril[i].x) * (Quadril[(i+2)%4].y - Quadril[(i+1)%4].y) - (Quadril[(i+1)%4].y - Quadril[i].y) * (Quadril[(i+2)%4].x - Quadril[(i+1)%4].x));
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
        Triangle TrNew1 = Triangle(esterno1, Lato[0], esterno2);
        TrNew1.adiacentTriangles[0] = meshTriangles.size();
        operationEdges(TrNew1);
        Triangle TrNew2 = Triangle(esterno1, Lato[1], esterno2);
        TrNew1.adiacentTriangles[0] = meshTriangles.size() + 1 ;
        operationEdges(TrNew1);
        meshTriangles.push_back(TrNew1);
        meshTriangles.push_back(TrNew1);
        Triangle1.PointedTriangles.push_back(TrNew1);
        Triangle1.PointedTriangles.push_back(TrNew2);
        Triangle2.PointedTriangles.push_back(TrNew1);
        Triangle2.PointedTriangles.push_back(TrNew2);
    }

    void Mesh::DelunayPropagation(Triangle& StartTriangle)
    {
       for (int i = 0; i < 3; i++)
       {
        Triangle TriangoloAdj = meshTriangles[StartTriangle.adiacentTriangles[i]];
        if (CheckConvex(StartTriangle, TriangoloAdj) == 0)
        {
            if (DelunayProperty(StartTriangle, TriangoloAdj) == 0 or (DelunayProperty(StartTriangle, TriangoloAdj) == 0) )
            {
                Flip(StartTriangle, TriangoloAdj);
            }
        }

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
