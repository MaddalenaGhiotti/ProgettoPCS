#ifndef __TEST_EMPTY_H
#define __TEST_EMPTY_H

#include <gtest/gtest.h>

#include "empty_class.hpp"
#include "Eigen/Eigen"

using namespace testing;
using namespace DelaunayLibrary;

TEST(TestDelaunay, PuntoSulBordo1)
{
    //creo una serie di punti
    Point* p0 = new Point(0.5, 2);
    Point* p2 = new Point(5, 3);
    Point* p4 = new Point(5, 2);
    Point* p6 = new Point(5, 1);
    Grid grid = Grid({p0, p2, p4, p6});
    grid.PointsInRectangle({p0, p2, p4, p6});
    // check punto sul bordo
    EXPECT_EQ(grid.rectangles(1, 0).containedPoints[0], *p0);
}

TEST(TestDelaunay, PuntoSulBordo2)
{
    //creo una serie di punti
    Point* p0 = new Point(0.5, 2);
    Point* p2 = new Point(5, 3);
    Point* p4 = new Point(5, 2);
    Point* p6 = new Point(5, 1);
    Grid grid = Grid({p0, p2, p4, p6});
    grid.PointsInRectangle({p0, p2, p4, p6});
    // check punto sul bordo
    EXPECT_EQ(grid.rectangles(0, 1).containedPoints[0], *p2);
}

TEST(TestDelaunay, PiùPuntiSulBordo)
{
    //creo una serie di punti
    Point* p0 = new Point(0.5, 2);
    Point* p2 = new Point(5, 3);
    Point* p4 = new Point(5, 2);
    Point* p6 = new Point(5, 1);
    Grid grid = Grid({p0, p2, p4, p6});
    grid.PointsInRectangle({p0, p2, p4, p6});
    // check più di un punto sul bordo
    EXPECT_TRUE(grid.rectangles(1, 1).containedPoints.size() >= 2);
}

TEST(TestDelaunay, CheckSnakeNonAllineati)
{
    //creo una serie di punti
    Point* p0 = new Point(0.5, 2);
    Point* p2 = new Point(5, 3);
    Point* p4 = new Point(5, 2);
    Point* p6 = new Point(5, 1);
    Grid grid = Grid({p0, p2, p4, p6});
    grid.PointsInRectangle({p0, p2, p4, p6});
    // check non allineati
    array<Point, 4> output = grid.Snake();
    EXPECT_FALSE(output[0] == *p2 && output[1] == *p4 && output[2] == *p6);
}

TEST(TestDelaunay, CheckSnakeUrtoDiRettangoli)
{
    //creo una serie di punti
    Point* p0 = new Point(0.5, 2);
    Point* p1 = new Point(5, 1);
    Point* p2 = new Point(5, 1.5);
    Point* p3 = new Point(5, 2);
    Point* p4 = new Point(5, 2.5);
    Point* p5 = new Point(5, 3);
    Point* p6 = new Point(3, 1.5);
    Point* p7 = new Point(3, 2.5);
    Point* p8 = new Point(4, 1.5);
    Point* p9 = new Point(4, 2.5);
    Grid grid = Grid({p0, p1, p2, p3, p4, p5, p6, p7, p8, p9});
    grid.PointsInRectangle({p0, p1, p2, p3, p4, p5, p6, p7, p8, p9});

    // check urto di rettangoli
    testing::internal::CaptureStderr();
    grid.Snake();
    string capturedOutput = testing::internal::GetCapturedStderr();
    // Ossia, ci aspettiamo che non sia vero che la serpetina non ha printato il seguente messaggio d'errore
    EXPECT_NE(capturedOutput.find("Il metodo della serpentina ha reggiunto due volte lo stesso rettangolo! Saranno dunque selezionati 4 punti casualmente!"), std::string::npos);
}

// testo la verifica della proprietà di Delaunay
TEST(TestDelaunayProperty, TestTriangle)
{
  // creo una serie di punti
  Point p_2 = Point(6, 4);
  Point p_3 = Point(4, 6);
  Point p_4 = Point(8, 8);
  Point p_5 = Point(8, 6);
  // creo due triangoli a partire da quei punti
  Triangle Tr1 = Triangle(p_2, p_3, p_4);
  Triangle Tr2 = Triangle(p_3, p_4, p_5);
  bool i = Triangle::DelaunayProperty(Tr1, Tr2);
  EXPECT_EQ(i, 0);
}

TEST(TestDelaunayProperty2, TestTriangle)
{
  // creo una serie di punti
  Point p_0 = Point(4, 2);
  Point p_1 = Point(2, 4);
  Point p_2 = Point(6, 4);
  Point p_3 = Point(4, 6);
  // creo due triangoli a partire da quei punti
  Triangle Tr1 = Triangle(p_0, p_1, p_2);
  Triangle Tr2 = Triangle(p_1, p_2, p_3);
  bool i = Triangle::DelaunayProperty(Tr1, Tr2);
  EXPECT_EQ(i, 1);
}

// Testo che il flip venga eseguito nel modo corretto
TEST(TestDelaunayFlip, TestTriangle)
{
  // creo una serie di punti
  Point p_2 = Point(6, 4);
  Point p_3 = Point(4, 6);
  Point p_4 = Point(8, 8);
  Point p_5 = Point(8, 6);
  // creo due triangoli a partire da quei punti
  Triangle Tr1 = Triangle(p_2, p_4, p_3);
  Triangle Tr2 = Triangle(p_2, p_4, p_5);
  array<Triangle*, 2> FlippedTriangles = Triangle::Flip(Tr1, Tr2);
  Triangle TrFl1 = Triangle(p_3, p_2, p_5);
  Triangle TrFl2 = Triangle(p_3, p_4, p_5);
  Triangle FirstFlipped = *FlippedTriangles[0];
  Triangle SecondFlipped = *FlippedTriangles[1];
  EXPECT_EQ(FirstFlipped, TrFl1);
  EXPECT_EQ(SecondFlipped, TrFl2);
}

// testo che il lato in comune tra due triangoli sia individuato nel modo corretto
TEST(FindCommonEdge, TestTriangle)
{
    // creo una serie di punti
    Point p_2 = Point(6, 4);
    Point p_3 = Point(4, 6);
    Point p_4 = Point(8, 8);
    Point p_5 = Point(8, 6);
    // creo due triangoli a partire da quei punti
    Triangle Tr1 = Triangle(p_2, p_4, p_3);
    Triangle Tr2 = Triangle(p_2, p_4, p_5);
    array<Point*, 4> Output = Triangle::FindCommonEdge(Tr1, Tr2);
    EXPECT_EQ(*Output[0], p_2);
    EXPECT_EQ(*Output[1], p_4);
}

// altro test sul lato in comune
TEST(FindCommonEdge2, TestTriangle)
{
    // creo una serie di punti
    Point p_0 = Point(4, 2);
    Point p_1 = Point(2, 4);
    Point p_2 = Point(6, 4);
    Point p_3 = Point(4, 6);
    // creo due triangoli a partire da quei punti
    Triangle Tr1 = Triangle(p_0, p_1, p_2);
    Triangle Tr2 = Triangle(p_1, p_2, p_3);
    array<Point*, 4> Output = Triangle::FindCommonEdge(Tr1, Tr2);
    EXPECT_EQ(*Output[0], p_2);
    EXPECT_EQ(*Output[1], p_1);
}

//testo la funzione che mi indica se il punto da aggiungere è interno o meno alla triangolazione
TEST(CheckInside, ConvexHull)
{
    // creo una serie di punti
    Point p_4 = Point(8, 8);
    Point p_5 = Point(8, 6);
    Point p_0 = Point(4, 2);
    Point p_1 = Point(2, 4);
    Point p_2 = Point(6, 4);
    Point p_3 = Point(4, 6);
    // creo un triangolo a partire da quei punti
    Triangle Tr3 = Triangle(p_0, p_2, p_1);
    Mesh mesh = Mesh(Tr3);
    mesh.AddExternalPoint(p_3);
    mesh.AddExternalPoint(p_4);
    int i = mesh.CheckInside(p_5);
    EXPECT_EQ(i, 0);
}

//testo una seconda la funzione che mi indica se il punto da aggiungere è interno o meno alla triangolazione
TEST(CheckInside2, ConvexHull)
{
    // creo una serie di punti
    Point p_4 = Point(8, 8);
    Point p_5 = Point(8, 6);
    Point p_0 = Point(4, 2);
    Point p_1 = Point(2, 4);
    Point p_2 = Point(6, 4);
    Point p_3 = Point(4, 6);
    Point p_7 = Point(4, 4);
    // creo un triangolo a partire da quei punti
    Triangle Tr3 = Triangle(p_0, p_2, p_1);
    Mesh mesh = Mesh(Tr3);
    mesh.AddExternalPoint(p_3);
    mesh.AddExternalPoint(p_4);
    mesh.AddExternalPoint(p_5);
    int i = mesh.CheckInside(p_7);
    EXPECT_EQ(i, 1);
}

// testo il meccanismo di aggiunta di un punto interno alla triangolazione (e non su un lato)
TEST(AddInternalPoint, ConvexHull)
{
    // creo una serie di punti
    Point p_0 = Point(4, 2);
    Point p_1 = Point(2, 4);
    Point p_2 = Point(6, 4);
    Point p_3 = Point(4, 3);
    // creo un triangolo a partire da quei punti
    Triangle Tr1 = Triangle(p_0, p_2, p_1);
    Triangle Tr2 = Triangle(p_1, p_0, p_3);
    Triangle Tr3 = Triangle(p_0, p_2, p_3);
    Triangle Tr4 = Triangle(p_2, p_1, p_3);
    Mesh mesh = Mesh(Tr1);
    mesh.AddInternalPoint(p_3, &Tr1);
    EXPECT_EQ(*mesh.guideTriangles[0], Tr1);
    EXPECT_EQ(*(mesh.guideTriangles[0]->pointedTriangles[0]), Tr3);
    EXPECT_EQ(*(mesh.guideTriangles[0]->pointedTriangles[1]), Tr4);
    EXPECT_EQ(*(mesh.guideTriangles[0]->pointedTriangles[2]), Tr2);
}

// testo il meccanismo di aggiunta di un punto interno alla triangolazione e su un lato di un triangolo
TEST(AddSidePoint, ConvexHull)
{
    // creo una serie di punti
    Point p_0 = Point(4, 2);
    Point p_1 = Point(2, 4);
    Point p_2 = Point(6, 4);
    Point p_3 = Point(4, 4);
    // creo un triangolo a partire da quei punti
    Triangle Tr1 = Triangle(p_0, p_2, p_1);
    Triangle Tr2 = Triangle(p_1, p_0, p_3);
    Triangle Tr3 = Triangle(p_2, p_3, p_0);
    Mesh mesh = Mesh(Tr1);
    mesh.AddSidePoint(p_3, &Tr1, 2);
    EXPECT_EQ(*mesh.guideTriangles[0], Tr1);
    EXPECT_EQ(*(mesh.guideTriangles[0]->pointedTriangles[0]), Tr3);
    EXPECT_EQ(*(mesh.guideTriangles[0]->pointedTriangles[1]), Tr2);
}


#endif // __TEST_EMPTY_H
