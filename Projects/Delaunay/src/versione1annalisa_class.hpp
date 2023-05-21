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
    //un array di 3 elementi della struct Point (= i 3 vertici del triangolo considerato)
    Point vertices[3];

    //un array di 3 elementi: si tratta di 3 puntatori a oggetti della struct Triangle (= i 3 triangoli adiacenti al triangolo considerato)
    Triangle* adjacent_triangles[3];

    //il costruttore
    Triangle(double x1 = 0.0, double y1 = 0.0, double x2 = 0.0, double y2 = 0.0, double x3 = 0.0, double y3 = 0.0)
    {
        vertices[0] = {x1, y1};
        vertices[1] = {x2, y2};
        vertices[2] = {x3, y3};
        OrderTrianglePoints(vertices);  // Ordino i vertici del triangolo in senso antiorario
        adjacent_triangles[0] = nullptr;
        adjacent_triangles[1] = nullptr;
        adjacent_triangles[2] = nullptr;
        //Inizializziamo i puntatori a 'nullptr', non avendo ancora definito gli oggetti Triangle adiacenti.
    }

    ///\brief ordina i vertici del triangolo in senso antiorario. Il controllo avviene con il prodotto vettoriale
    ///\param vertices: array di tre elementi di tipo Point, che sono i vertici del triangolo
    void OrderTrianglePoints( Point (&vertices)[3] )
    {
        Point a = vertices[0];
        Point b = vertices[1];
        Point c = vertices[2];

        Point vectorAB = {b.x - a.x, b.y - a.y};  // "Vettore" B - A
        Point vectorAC = {c.x - a.x, c.y - a.y};  // "Vettore" C - A

        // Prodotto vettoriale: vectorAB x vectorAC = (B-A) x (C-A)
        double crossProduct = vectorAB.x * vectorAC.y - vectorAB.y * vectorAC.x;

        if (crossProduct < 0)
        {
            // Scambio il punto B e il punto C
            vertices[1] = vertices[2];
            vertices[2] = b;
        }
    }

    /* CONSIDERAZIONI SU const posto dopo un metodo:
    La parola "const" scritta alla fine della dichiarazione del metodo "area()" e "circoContainsPoint()" e "triangleContainsPoint()"
    indica che questo metodo non modifica lo stato interno dell'oggetto Triangle su cui viene chiamato.
    In altre parole, il metodo non modifica i valori dei vertici del triangolo o qualsiasi altra informazione all'interno dell'oggetto Triangle.
    L'utilizzo della parola "const" consente anche di chiamare il metodo su oggetti "const Triangle", che sono oggetti che non possono essere modificati.*/

    //funzione "area" calcola l'area del triangolo
    double area() const
    {
        Point a = vertices[0];
        Point b = vertices[1];
        Point c = vertices[2];
        //calcolo l'area mediante il prodotto vettoriale: area(abc) = (ab x ac)/2
        double ab_x = b.x - a.x;
        double ab_y = b.y - a.y;
        double ac_x = c.x - a.x;
        double ac_y = c.y - a.y;
        double area = std::abs(ab_x * ac_y - ab_y * ac_x)*0.5; //(svolgo il prodotto vettoriale con il determinante)

        return area;
    }


    //funzione "circoContainsPoint" prende in input le coordinate di un nuovo punto e stabilisce se questo sia interno o meno alla circonferenza circoscritta al triangolo su cui viene richiamata.
    bool circoContainsPoint(const double x, const double y) const //NOTA BENE: i vertici a,b,c vanno passati in senso antiorario!
    {
        //chiamiamo a,b,c i vertici del triangolo e d il nuovo punto.
        //creo queste variabili ausiliarie dal nome "corto" per semplificare la scrittura del determinante.
        double ax = vertices[0].x; double ay = vertices[0].y;
        double bx = vertices[1].x; double by = vertices[1].y;
        double cx = vertices[2].x; double cy = vertices[2].y;
        double dx = x; double dy = y;

        double det = (ax - dx) * (by - cy) * (cx - dx) * (ay - cy) - (ax - dx) * (by - dy) * (cx - dx) * (ay - cy) -
                     (ax - dx) * (by - cy) * (cx - dx) * (ay - dy) + (ax - dx) * (by - dy) * (cx - dx) * (ay - dy) +
                     (ax - bx) * (by - cy) * (cx - dx) * (ay - cy) - (ax - bx) * (by - dy) * (cx - dx) * (ay - cy) -
                     (ax - bx) * (by - cy) * (cx - dx) * (ay - dy) + (ax - bx) * (by - dy) * (cx - dx) * (ay - dy) +
                     (ax - bx) * (by - cy) * (cx - cx) * (ay - cy) - (ax - bx) * (by - dy) * (cx - cx) * (ay - cy) -
                     (ax - bx) * (by - cy) * (cx - cx) * (ay - dy) + (ax - bx) * (by - dy) * (cx - cx) * (ay - dy);

        if (det > 0)
        {return true;} //il punto è interno alla circonferenza
        else
        {return false;} //il punto è esterno alla circonferenza
    }


    //funzione "triangleContainsPoint" prende in input le coordinate di un nuovo punto e stabilisce se questo sia interno o meno al triangolo su cui viene richiamata
    bool triangleContainsPoint(const double x, const double y) const
    {
        //faccio un primo check verificando se il punto d sia esterno o meno alla circonferenza circoscritta al triangolo
        if (!circoContainsPoint(x, y))
        {return false;}  //il punto è sicuramente esterno al triangolo

        //chiamiamo a,b,c i vertici del triangolo e d il nuovo punto
        //creo queste variabili ausiliarie dal nome "corto" per semplificare la scrittura dei determinanti
        double ax = vertices[0].x; double ay = vertices[0].y;
        double bx = vertices[1].x; double by = vertices[1].y;
        double cx = vertices[2].x; double cy = vertices[2].y;
        double dx = x; double dy = y;

        //Questo calcolo può essere interpretato come la terza componente
        //del prodotto vettoriale tra i vettori DA e DB (con origine in D) (ovvero DA x DB)
        double detABD = (ax - dx) * (by - dy) - (bx - dx) * (ay - dy);
        //Osservo: avendo preso A,B,C in senso antiorario,
        //il segno del detABD sarà positivo se D si trova a sinistra del vettore diretto da A a B,
        //e negativo se si trova invece alla sua a destra. (interpreto destra e sinistra "volgendo gli occhi a B e dando le spalle ad A")
        double detBCD = (bx - dx) * (cy - dy) - (cx - dx) * (by - dy);
        double detCAD = (cx - dx) * (ay - dy) - (ax - dx) * (cy - dy);

        //se tutti i determinanti hanno lo stesso segno, il punto d è interno al triangolo, altrimenti è esterno
        bool result = (detABD >= 0 && detBCD >= 0 && detCAD >= 0) || (detABD <= 0 && detBCD <= 0 && detCAD <= 0);
        //NOTO: dal momento che i vertici a,b,c del triangolo sono ordinati in senso orario,
        //allora il segno dei determinanti sarà sempre positivo (qualora d fosse interno)

        return result;
    }
};
// FINE STRUCT TRIANGLE

///\brief Calcola la distanza tra due punti
///\param Due punti
///\return La distanza tra i due punti
double Distance(const Point& p1,
                const Point& p2)
{
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}


///\brief Calcola l'angolo tra tre punti, corrispondente all'angolo al vertice nel primo dei punti passati (p1)
///\param Tre punti (corrispondenti ai vertici del triangolo): p1, p2, p3
///\return Il valore dell'angolo al vertice p1.
double calculateAngle(const Point& p1,
                      const Point& p2,
                      const Point& p3)
{
    double d1 = Distance(p2, p3);
    double d2 = Distance(p1, p3);
    double d3 = Distance(p1, p2);
    return acos((d2 * d2 + d3 * d3 - d1 * d1) / (2 * d2 * d3));
}


///\brief Verifica se i due triangoli in input verificano l'ipotesi di Delaunay o meno
///\param Due triangoli t1 e t2, adiacenti
///\return True se l'ipotesi è rispettata, False altrimenti
void delaunayCondition( Triangle& t1,
                        Triangle& t2 )
{
    Point& a1 = t1.vertices[0];
    Point& b1 = t1.vertices[1];
    Point& c1 = t1.vertices[2];
    Point& a2 = t2.vertices[0];
    Point& b2 = t2.vertices[1];
    Point& c2 = t2.vertices[2];

    Point* sharedVertex1 = nullptr;
    Point* sharedVertex2 = nullptr;
    Point* notShared1 = &b1;          // Puntatore al vertice di t1 che non è condiviso con t2
    Point* notShared2 = nullptr;      // Puntatore al vertice di t2 che non è condiviso con t1

    // Verifico quali vertici del triangolo t1 siano in comune con quelli del triangolo t2 (sono necessariamente 2).
    // Contemporaneamente, salvo anche il vertice di t1 non in comune a t2.
    if (&a1 == &a2 || &a1 == &b2 || &a1 == &c2)
        {sharedVertex1 = &a1;}

    if (&b1 == &a2 || &b1 == &b2 || &b1 == &c2)
    {
        if (sharedVertex1 == nullptr)
        {
            sharedVertex1 = &b1;
            notShared1 = &a1;
        }
        else
        {
            sharedVertex2 = &b1;
            notShared1 = &c1;
        }
    }

    if (&c1 == &a2 || &c1 == &b2 || &c1 == &c2)
        {sharedVertex2 = &c1;}

    // Trovo il vertice di t2 non comune a t1
    if (&a2 != sharedVertex1 && &a2 != sharedVertex2)
        {notShared2 = &a2;}
    else if (&b2 != sharedVertex1 && &b2 != sharedVertex2)
        {notShared2 = &b2;}
    else
        {notShared2 = &c2;}

    // Calcolo gli angoli al vertice per notShared1 e notShared2.
    double angle1 = calculateAngle(*notShared1, *sharedVertex1, *sharedVertex2);  // gli input sono di tipo Point
    double angle2 = calculateAngle(*notShared2, *sharedVertex1, *sharedVertex2);  // gli input sono di tipo Point

    // Verifico se l'ipotesi di Delaunay è soddisfatta.
    // Caso in cui l'ipotesi non è soddisfatta --> Eseguire l'operazione di flip.
    if (angle1 + angle2 > M_PI)
    {
        // Elimino il lato (sharedVertex1, sharedVertex2) e creo il lato (notShared1, notShared2)
        t1 = Triangle(notShared1->x, notShared1->y, notShared2->x, notShared2->y, sharedVertex1->x, sharedVertex1->y);
        t2 = Triangle(notShared1->x, notShared1->y, notShared2->x, notShared2->y, sharedVertex2->x, sharedVertex2->y);
    }
}


};
#endif // __EMPTY_H
