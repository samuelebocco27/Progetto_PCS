#ifndef __TEST_EMPTY_H
#define __TEST_EMPTY_H

#include <gtest/gtest.h>

#include "empty_class.hpp"

using namespace testing;
using namespace std;
using namespace DelaunayTriangle;

//TEST DI TRIANGLE

// Test per verificare Triangle::OrderTrianglePoints
TEST(TestOrderPoints, TestOrderTrianglePoints)
{
    Mesh mesh;
    Point a, b, c;
    a.x = 0.0; a.y = 0.0; a.id = 0;
    b.x = 0.0; b.y = 1.0; b.id = 1;
    c.x = 1.0; c.y = 0.0; c.id = 2;
    mesh.points.push_back(a);
    mesh.points.push_back(b);
    mesh.points.push_back(c);
    Triangle t = Triangle(a.id, b.id, c.id, &mesh);

    int sortedVertices[3] = {a.id, c.id, b.id};
    int actualVertices[3] = {t.idVertices[0], t.idVertices[1], t.idVertices[2]};
    for (int i = 0; i < 3; i++)
    {
        EXPECT_EQ(actualVertices[i], sortedVertices[i]);
    }
}

// Test per verificare Triangle::Area
TEST(TestTriangleArea, TestComputeTriangleArea)
{
    Mesh mesh;
    Point p1, p2, p3;
    p1.id=0; p1.x=0.0; p1.y=0.0;
    p2.id=1; p2.x=1.0; p2.y=0.0;
    p3.id=2; p3.x=0.0; p3.y=1.0;
    mesh.points.push_back(p1);
    mesh.points.push_back(p2);
    mesh.points.push_back(p3);

    Triangle t = Triangle(p1.id, p2.id, p3.id, &mesh);
    double areaT = 0.5;
    EXPECT_EQ(areaT, t.Area(&mesh));
}

//Set di test (di nome TestCircoContainsPoint) per verificare Triangle::CircoContainsPoint

TEST(TestCircoContainsPoint, TestPointInternalCirco)
{
    Mesh mesh;
    Point p1, p2, p3;
    p1.id=0; p1.x=0.0; p1.y=0.0;
    p2.id=1; p2.x=1.0; p2.y=0.0;
    p3.id=2; p3.x=0.0; p3.y=1.0;
    Point pinterno;
    pinterno.id=3; pinterno.x=0.5; pinterno.y=0.5;
    mesh.points.push_back(p1);
    mesh.points.push_back(p2);
    mesh.points.push_back(p3);
    mesh.points.push_back(pinterno);

    Triangle t = Triangle(p1.id, p2.id, p3.id, &mesh);

    EXPECT_TRUE(t.CircoContainsPoint(pinterno.id, mesh));
}

TEST(TestCircoContainsPoint, TestPointBorderCirco)
{
    Mesh mesh;
    Point p1, p2, p3;
    p1.id=0; p1.x=0.0; p1.y=0.0;
    p2.id=1; p2.x=1.0; p2.y=0.0;
    p3.id=2; p3.x=0.0; p3.y=1.0;
    Point pbordo;
    pbordo.id=3; pbordo.x=0.0; pbordo.y=0.0;
    mesh.points.push_back(p1);
    mesh.points.push_back(p2);
    mesh.points.push_back(p3);
    mesh.points.push_back(pbordo);

    Triangle t = Triangle(p1.id, p2.id, p3.id, &mesh);

    EXPECT_FALSE(t.CircoContainsPoint(pbordo.id, mesh));
}

TEST(TestCircoContainsPoint, TestPointExternalCirco)
{
    Mesh mesh;
    Point p1, p2, p3;
    p1.id=0; p1.x=0.0; p1.y=0.0;
    p2.id=1; p2.x=1.0; p2.y=0.0;
    p3.id=2; p3.x=0.0; p3.y=1.0;
    Point pesterno;
    pesterno.id=3; pesterno.x=10.0; pesterno.y=10.0;
    mesh.points.push_back(p1);
    mesh.points.push_back(p2);
    mesh.points.push_back(p3);
    mesh.points.push_back(pesterno);

    Triangle t = Triangle(p1.id, p2.id, p3.id, &mesh);

    EXPECT_FALSE(t.CircoContainsPoint(pesterno.id, mesh));
}

//
//Set di test (di nome TestTriangleContainsPoint) per verificare Triangle::TriangleContainsPoint

TEST(TestTriangleContainsPoint, TestPointBorder)
{
    Mesh mesh;
    Point p1, p2, p3;
    p1.id=0; p1.x=0.0; p1.y=0.0;
    p2.id=1; p2.x=1.0; p2.y=0.0;
    p3.id=2; p3.x=0.0; p3.y=1.0;
    Point pbordo;
    pbordo.id=3; pbordo.x=0.5; pbordo.y=0.5;
    mesh.points.push_back(p1);
    mesh.points.push_back(p2);
    mesh.points.push_back(p3);
    mesh.points.push_back(pbordo);

    Triangle t = Triangle(p1.id, p2.id, p3.id, &mesh);
    EXPECT_EQ(2, t.TriangleContainsPoint(pbordo.id, mesh));
}

TEST(TestTriangleContainsPoint, TestPointExternal)
{
    Mesh mesh;
    Point p1, p2, p3;
    p1.id=0; p1.x=0.0; p1.y=0.0;
    p2.id=1; p2.x=1.0; p2.y=0.0;
    p3.id=2; p3.x=0.0; p3.y=1.0;
    Point pesterno;
    pesterno.id=3; pesterno.x=1; pesterno.y=1;
    mesh.points.push_back(p1);
    mesh.points.push_back(p2);
    mesh.points.push_back(p3);
    mesh.points.push_back(pesterno);

    Triangle t = Triangle(p1.id, p2.id, p3.id, &mesh);
    EXPECT_EQ(0, t.TriangleContainsPoint(pesterno.id, mesh));
}

TEST(TestTriangleContainsPoint, TestPointInternal)
{
    Mesh mesh;
    Point p1, p2, p3;
    p1.id=0; p1.x=0.0; p1.y=0.0;
    p2.id=1; p2.x=1.0; p2.y=0.0;
    p3.id=2; p3.x=0.0; p3.y=1.0;
    Point pinterno;
    pinterno.id=3; pinterno.x=0.25; pinterno.y=0.25;
    mesh.points.push_back(p1);
    mesh.points.push_back(p2);
    mesh.points.push_back(p3);
    mesh.points.push_back(pinterno);

    Triangle t = Triangle(p1.id, p2.id, p3.id, &mesh);
    EXPECT_EQ(1, t.TriangleContainsPoint(pinterno.id, mesh));
}

//
//

//TEST DI MESH
//
//Test per verificare Mesh::GetMaxAreaTriangle. N.B. NEL CASO IN CUI VOGLIAMO UTILIZZARLO DOBBIAMO MODIFICARE GetMaxAreaTriangle per farla lavorare con gli id
/*TEST(TestMaxAreaTriangle, TestMaxArea)
{
    Mesh mesh;
    Point a, b, c, d, e, f;
    a.id=0; a.x=0.0; a.y=0.0;
    b.id=1; b.x=0.0; b.y=0.5;
    c.id=2; c.x=0.0; c.y=1.0;
    d.id=3; d.x=0.5; d.y=0.0;
    e.id=4; e.x=0.5; e.y=0.5;
    f.id=5; f.x=1.0; f.y=0.0;
    mesh.points.push_back(a);mesh.points.push_back(b);mesh.points.push_back(c);
    mesh.points.push_back(d);mesh.points.push_back(e);mesh.points.push_back(f);

    Triangle max = Triangle(a.id, c.id, f.id, &mesh);

    Triangle max_area = mesh.GetMaxAreaTriangle(mesh.points, 0, mesh.points.size());
    EXPECT_EQ(max.Area(&mesh), max_area.Area(&mesh));
}
*/
//

//Test per verificare Mesh::FakeTriangleCover()
TEST(TestFakeTriangleCover, TestFakeTriangleCover)
{
    Mesh mesh;

    Point p1, p2, p3, p4, p5, p6;
    p1.id=0; p1.x=0.0; p1.y=0.0; p2.id=1; p2.x=0.5; p2.y=0.0; p3.id=2; p3.x=0.75; p3.y=0.0;
    p4.id=3; p4.x=0.5; p4.y=0.5; p5.id=4; p5.x=0.0; p5.y=0.5; p6.id=5; p6.x=0.0; p6.y=0.75;


    mesh.points.push_back(p1); mesh.points.push_back(p2); mesh.points.push_back(p3);
    mesh.points.push_back(p4); mesh.points.push_back(p5); mesh.points.push_back(p6);

    Triangle t_fake_triangle_cover = mesh.FakeTriangleCover();
    double p1x = mesh.points[t_fake_triangle_cover.idVertices[0]].x;double p1y = mesh.points[t_fake_triangle_cover.idVertices[0]].y;
    double p2x = mesh.points[t_fake_triangle_cover.idVertices[1]].x;double p2y = mesh.points[t_fake_triangle_cover.idVertices[1]].y;
    double p3x = mesh.points[t_fake_triangle_cover.idVertices[2]].x;double p3y = mesh.points[t_fake_triangle_cover.idVertices[2]].y;
    EXPECT_EQ(0.0,p1x);EXPECT_EQ(0.0,p1y);
    EXPECT_EQ(1.5,p2x);EXPECT_EQ(0.0,p2y);
    EXPECT_EQ(0.0,p3x);EXPECT_EQ(1.5,p3y);

}

//
//Test per verificare Mesh::DelunayCondition()
TEST(TestDelaunayCondition, TestDelunay)
{
    Mesh mesh;

    Point p1, p2, p3, p4;
    p1.id=0; p1.x=0.0; p1.y=0.0;
    p2.id=1; p2.x=3.0; p2.y=0.0;
    p3.id=2; p3.x=0.0; p3.y=2.0;
    p4.id=3; p4.x=1.0; p4.y=2.0;
    mesh.points.push_back(p1);
    mesh.points.push_back(p2);
    mesh.points.push_back(p3);
    mesh.points.push_back(p4);

    Triangle p123 = Triangle(p1.id, p2.id, p3.id, &mesh);
    Triangle p234 = Triangle(p2.id, p4.id, p3.id, &mesh);
    mesh.AddTriangle(p123);
    mesh.AddTriangle(p234);
    int flippedT1 = -1;
    int flippedT2 = -1;
    EXPECT_TRUE(mesh.DelaunayCondition(p123.id,p234.id, flippedT1, flippedT2));
}

//
//Test per verificare Mesh::GenerateMesh()
TEST(TestGenerateMesh, TestGenerateMesh)
{
    Mesh mesh;
    Point p1, p2, p3, p4, p5, p6, p7;
    p1.id=0; p1.x=0.0; p1.y=0.0; p2.id=1; p2.x=3.0; p2.y=0.0; p3.id=2; p3.x=2.0; p3.y=0.6;
    p4.id=3; p4.x=1.0; p4.y=0.8; p5.id=4; p5.x=0.5; p5.y=1.0; p6.id=5; p6.x=0.0; p6.y=2.0; p7.id=6; p7.x=1.0; p7.y=2.0;
    mesh.points.push_back(p1); mesh.points.push_back(p2); mesh.points.push_back(p3); mesh.points.push_back(p4);
    mesh.points.push_back(p5); mesh.points.push_back(p6); mesh.points.push_back(p7);

    Triangle maxTriangle = mesh.FakeTriangleCover();
    mesh.AddTriangle(maxTriangle);
    mesh.GenerateMesh();
    mesh.DeactivateFakeTriangles();

    int cont=0;
    for (unsigned int i = 0; i < mesh.edges.size(); i++) {
        Edge e = mesh.edges[i];
        if(e.active)
        {
            cont++;
        }

    }
    EXPECT_EQ(14, cont);
}
//
//TEST FUNZIONI ESTERNE ALLE STRUCT


TEST(TestDistance, TestDistanza)
{
    Point p1, p2;
    p1.x = 0.0;
    p1.y = 0.0;
    p2.x = 2.0;
    p2.y = 0.0;
    p1.id = 1;
    p2.id = 2;
    double distanza_punti = 2.0;
    EXPECT_EQ(distanza_punti, Distance(p1,p2));
}

TEST(TestCalculateAngle, TestAngolo)
{
    Point p1, p2, p3;
    p1.x=0.0; p1.y=0.0;
    p2.x=1.0; p2.y=0.0;
    p3.x=0.0; p3.y=1.0;
    p1.id = 1; p2.id = 2; p3.id = 3;
    double d1,d2,d3; // d1 = distance(p2-p3) d2 = distance(p3-p1) d3 = distance(p2-p1)
    d1 = sqrt(2);
    d2 = 1.0;
    d3 = 1.0;

    double angolo = acos((d2 * d2 + d3 * d3 - d1 * d1) / (2 * d2 * d3));

    EXPECT_EQ(angolo, CalculateAngle(p1,p2,p3));
}

#endif // __TEST_EMPTY_H

