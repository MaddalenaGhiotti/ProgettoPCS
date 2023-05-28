
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


    vector<Point> Delaunay::ImportPoints()
    {
        cout<<"Pippo";
        cout<<fileName;
        ifstream file;
        file.open(fileName);
        if (file.fail()){
            cerr << "Error while opening file" << endl;
            return pointsVector;
        }
        string line;
        getline(file, line);
        while (!file.eof()){
            getline(file, line);
            cout<<line<<endl;
            int useless1;
            char useless2;
            double x;
            double y;
            istringstream ss(line);
            ss >> useless1 >> useless2 >> x >> useless2 >> y;
            cout<<x<<y<<endl;
            Point point = Point(x, y);
            point.Show();
            pointsVector.push_back(point);

        }
    }


    void Delaunay::Show()
    {
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
    }


    void Grid::Show()
    {
        for (int i=0; i<intNum; i++)
        {
            for (int j=0; j<intNum; j++){cout<<squares(i,j).toString()<<"    ";}
            cout<<endl;
        }
    }
}
