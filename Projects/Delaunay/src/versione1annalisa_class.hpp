#ifndef __EMPTY_H
#define __EMPTY_H

#include <iostream>
#include <cmath>


namespace ProjectLibrary
{
struct Point
{
    double x;
    double y;
};

struct Triangle
{
    //un vettore di 3 elementi della struct Point (= i 3 vertici del triangolo considerato)
    Point vertices[3];

    //un array di 3 elementi: si tratta di 3 puntatori a oggetti della struct Triangle (= i 3 triangoli adiacenti al triangolo considerato)
    Triangle* adjacent_triangles[3];

    //il costruttore
    Triangle(double x1, double y1, double x2, double y2, double x3, double y3)
    {
        vertices[0] = {x1, y1};
        vertices[1] = {x2, y2};
        vertices[2] = {x3, y3};
        adjacent_triangles[0] = nullptr;
        adjacent_triangles[1] = nullptr;
        adjacent_triangles[2] = nullptr;
        //Inizializziamo i puntatori a 'nullptr', non avendo ancora deinito gli oggetti Triangle adiacenti.
    }

    /* CONSIDERAZIONI SU const posto dopo un metodo:
    La parola "const" scritta alla fine della dichiarazione del metodo "area()" e "containsPoint()" indica che questo metodo non modifica
    lo stato interno dell'oggetto Triangle su cui viene chiamato. In altre parole, il metodo non modifica i valori dei vertici
    del triangolo o qualsiasi altra informazione all'interno dell'oggetto Triangle.
    L'utilizzo della parola "const" consente anche di chiamare il metodo su oggetti "const Triangle", che sono oggetti che non possono essere modificati.
    */

    //funzione "area" calcola l'area del triangolo
    double area() const {
        Point a = vertices[0];
        Point b = vertices[1];
        Point c = vertices[2];
        //calcolo l'area mediante il prodotto vettoriale: area(abc) = (ab x ac)/2
        double ab_x = a.x - a.x;
        double ab_y = b.y - a.y;
        double ac_x = c.x - a.x;
        double ac_y = c.y - a.y;
        double area = std::abs(ab_x * ac_y - ab_y * ac_x)*0.5; //(svolgo il prodotto vettoriale con il determinante)

        return area;
    }

    //funzione "containsPoint" prende in input le coordinate di un nuovo punto e stabilisce se questo sia interno o meno al triangolo su cui viene richiamata.
    bool containsPoint(double x, double y) const
    {
        //chiamiamo a,b,c i vertici del triangolo e d il nuovo punto.
        double ax = vertices[0].x, ay = vertices[0].y;
        double bx = vertices[1].x, by = vertices[1].y;
        double cx = vertices[2].x, cy = vertices[2].y;
        double dx = x, dy = y;

        double det = (ax - dx) * (by - cy) * (cx - dx) * (ay - cy) - (ax - dx) * (by - dy) * (cx - dx) * (ay - cy) -
                     (ax - dx) * (by - cy) * (cx - dx) * (ay - dy) + (ax - dx) * (by - dy) * (cx - dx) * (ay - dy) +
                     (ax - bx) * (by - cy) * (cx - dx) * (ay - cy) - (ax - bx) * (by - dy) * (cx - dx) * (ay - cy) -
                     (ax - bx) * (by - cy) * (cx - dx) * (ay - dy) + (ax - bx) * (by - dy) * (cx - dx) * (ay - dy) +
                     (ax - bx) * (by - cy) * (cx - cx) * (ay - cy) - (ax - bx) * (by - dy) * (cx - cx) * (ay - cy) -
                     (ax - bx) * (by - cy) * (cx - cx) * (ay - dy) + (ax - bx) * (by - dy) * (cx - cx) * (ay - dy);

        if (det > 0)
        {return 0;}
        else
        {return 1;}
    }

};

};


#endif // __EMPTY_H
