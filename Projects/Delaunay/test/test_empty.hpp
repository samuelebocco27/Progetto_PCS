#ifndef __TEST_EMPTY_H
#define __TEST_EMPTY_H

#include <gtest/gtest.h>

#include "empty_class.hpp"

using namespace testing;
using namespace std;
using namespace ProjectLibrary;

// Test per verificare Triangle::OrderTrianglePoints
TEST(TestOrderPoints, TestOrderTrianglePoints)
{
    Triangle t = Triangle(0, 0, 0, 1, 1, 0);
    Point sortedVertices[3] = {{0,0}, {1,0}, {0,1}};
    EXPECT_EQ(t.OrderTrianglePoints(t.vertices), sortedVertices);  // non riesco a levare questo errorino uffi
}

// Test per verificare Triangle::area
TEST(TestTriangleArea, TestComputeTriangleArea)
{
    Triangle t = Triangle(0, 0, 1, 0, 0, 1);
    double areaT = 0.5;
    EXPECT_EQ(t.area(), areaT);
}


// Set di test (di nome TestCircoContainsPoint) per verificare Triangle::circoContainsPoints

TEST(TestCircoContainsPoint, TestPointInternal)
{
    Triangle t = Triangle(0, 0, 1, 0, 0, 1);
    // Point point = {0.5, 0.6}   è il punto che voglio verificare che sia interno
    EXPECT_EQ(t.circoContainsPoint(0.5, 0.6), true);
}

TEST(TestCircoContainsPoint, TestPointBorder)
{
    Triangle t = Triangle(0, 0, 1, 0, 0, 1);
    // Point point = {0.0, 0.0}   è il punto che voglio verificare che sia di bordo
    EXPECT_EQ(t.circoContainsPoint(0.0, 0.0), false);
}

TEST(TestCircoContainsPoint, TestPointExternal)
{
    Triangle t = Triangle(0, 0, 1, 0, 0, 1);
    // Point point = {1.5, 0.5}   è il punto che voglio verificare che sia esterno
    EXPECT_EQ(t.circoContainsPoint(1.5, 0.5), false);
}


// Set di test (di nome TestTriangleContainsPoint) per verificare Triangle::triangleContainsPoints

TEST(TestTriangleContainsPoint, TestPointInternal)
{
    Triangle t = Triangle(0, 0, 1, 0, 0, 1);
    // Point point = {0.5, 0.1}   è il punto che voglio verificare che sia interno
    EXPECT_EQ(t.circoContainsPoint(0.5, 0.1), true);
}

TEST(TestTriangleContainsPoint, TestPointBorder)
{
    Triangle t = Triangle(0, 0, 1, 0, 0, 1);
    // Point point = {0.5, 0.5}   è il punto che voglio verificare che sia di bordo
    EXPECT_EQ(t.circoContainsPoint(0.5, 0.5), true);
}

TEST(TestTriangleContainsPoint, TestPointExternal)
{
    Triangle t = Triangle(0, 0, 1, 0, 0, 1);
    // Point point = {0.5, 0.6}   è il punto che voglio verificare che sia esterno
    EXPECT_EQ(t.circoContainsPoint(0.5, 0.6), false);
}



#endif // __TEST_EMPTY_H
