#include "empty_class.hpp"
#include <iostream>
#include <chrono>

using namespace DelaunayLibrary;

int main()
{
    int testNum = 25;
    array<double,testNum> times;
    for (int i=0; i<testNum; i++)
    {
        string inputFile = "Test"+to_string(i)+".csv";
        Delaunay delaunay = Delaunay(inputFile);
        //delaunay.Show();
        chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        delaunay.ExecuteDelaunay();
        chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        double elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        times[i] = elapsedTime;
    }
    return 0;
}

