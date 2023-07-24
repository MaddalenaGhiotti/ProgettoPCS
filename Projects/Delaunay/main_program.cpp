#include "empty_class.hpp"
#include <iostream>
#include <chrono>

using namespace DelaunayLibrary;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
      cerr<< "Necessario passare al programma il nome del file"<< endl;
      return -1;
    }
    string inputFile = argv[1];
    Delaunay delaunay = Delaunay(inputFile);
    //delaunay.Show();
    delaunay.ExecuteDelaunay();

    return 0;
}


//int main()
//{
//    int testNum = 130;
//    vector<double> times;
//    for (int i=121; i<=testNum; i++)
//    {
//        string inputFile = "Test"+to_string(i)+".csv";
//        Delaunay delaunay = Delaunay(inputFile);
//        //delaunay.Show();
//        chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//        delaunay.ExecuteDelaunay();
//        chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//        double elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
//        cout<<elapsedTime<<endl;
//        times.push_back(elapsedTime);
//    }
//    //for (double time:times){cout<<time<<endl;}
//    return 0;
//}

