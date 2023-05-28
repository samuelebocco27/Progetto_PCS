#ifndef __TEST_EMPTY_H
#define __TEST_EMPTY_H

#include <gtest/gtest.h>

#include "empty_class.hpp"

using namespace testing;
using namespace std;
using namespace DelaunayTriangle;

//// Test per verificare Triangle::OrderTrianglePoints
//TEST(TestOrderPoints, TestOrderTrianglePoints)
//{
//    Point a, b, c;
//    a.x = 0.0; a.y = 0.0;
//    b.x = 0.0; b.y = 1.0;
//    c.x = 1.0; c.y = 0.0;
//    Triangle t = Triangle(a, b, c);
//    Point sortedVertices[3] = {a, c, b};
//    Point actualVertices[3] = {*t.vertices[0], *t.vertices[1], *t.vertices[2]};
//    for (int i = 0; i < 3; i++)
//    {
//        EXPECT_EQ(actualVertices[i].x, sortedVertices[i].x);
//        EXPECT_EQ(actualVertices[i].y, sortedVertices[i].y);
//    }
//}
//
//// Test per verificare Triangle::area
//TEST(TestTriangleArea, TestComputeTriangleArea)
//{
//    Triangle t = Triangle(0.0, 0.0, 1.0, 0.0, 0.0, 1.0);
//    double areaT = 0.5;
//    EXPECT_EQ(t.area(), areaT);
//}
//
//// Set di test (di nome TestCircoContainsPoint) per verificare Triangle::circoContainsPoints
//
//TEST(TestCircoContainsPoint, TestPointInternal)
//{
//    Triangle t = Triangle(0.0, 0.0, 1.0, 0.0, 0.0, 1.0);
//    // Point point = {0.5, 0.6}   è il punto che voglio verificare che sia interno
//    EXPECT_TRUE(t.circoContainsPoint(0.5, 0.6));
//}
//
//TEST(TestCircoContainsPoint, TestPointBorder)
//{
//    Triangle t = Triangle(0.0, 0.0, 1.0, 0.0, 0.0, 1.0);
//    // Point point = {0.0, 0.0}   è il punto che voglio verificare che sia di bordo
//    EXPECT_FALSE(t.circoContainsPoint(0.0, 0.0));
//}
//
//TEST(TestCircoContainsPoint, TestPointExternal)
//{
//    Triangle t = Triangle(0.0, 0.0, 1.0, 0.0, 0.0, 1.0);
//    // Point point = {1.5, 0.5}   è il punto che voglio verificare che sia esterno
//    EXPECT_FALSE(t.circoContainsPoint(1.5, 0.5));
//}
//
//
//// Set di test (di nome TestTriangleContainsPoint) per verificare Triangle::triangleContainsPoints
//
//TEST(TestTriangleContainsPoint, TestPointInternal)
//{
//    Triangle t = Triangle(0.0, 0.0, 1.0, 0.0, 0.0, 1.0);
//    // Point point = {0.5, 0.1}   è il punto che voglio verificare che sia interno
//    EXPECT_TRUE(t.triangleContainsPoint(0.5, 0.1));
//}
//
//TEST(TestTriangleContainsPoint, TestPointBorder)
//{
//    Triangle t = Triangle(0.0, 0.0, 1.0, 0.0, 0.0, 1.0);
//    // Point point = {0.5, 0.5}   è il punto che voglio verificare che sia di bordo
//    EXPECT_FALSE(t.triangleContainsPoint(0.5, 0.5));
//}
//
//TEST(TestTriangleContainsPoint, TestPointExternal)
//{
//    Triangle t = Triangle(0.0, 0.0, 1.0, 0.0, 0.0, 1.0);
//    // Point point = {0.5, 0.6}   è il punto che voglio verificare che sia esterno
//    EXPECT_FALSE(t.triangleContainsPoint(0.5, 0.6));
//}


#endif // __TEST_EMPTY_H
