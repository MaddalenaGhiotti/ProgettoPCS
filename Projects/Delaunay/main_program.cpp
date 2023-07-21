#include "empty_class.hpp"
#include <iostream>

using namespace DelaunayLibrary;

int main()
{
    string inputFile = "Test1.csv";
    Delaunay delaunay = Delaunay(inputFile);
    delaunay.Show();
    delaunay.ExecuteDelaunay();
    return 0;
}




