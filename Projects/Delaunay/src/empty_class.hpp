#ifndef __EMPTY_H
#define __EMPTY_H

#include <iostream>
#include "Eigen/Eigen"


using namespace std;

// COMMENTO GUIDA: In questo file (e nel suo corrispettivo .cpp) sono state inseriti i contenuti delle struct Point e Triangle del file "versione1annalisa_class.hpp".
// Il file "versione1annalisa_class.hpp" contiene ancora tutto al suo interno.
// Le funzioni del file "versione1annalisa_class.hpp" che stanno sotto alla struct Triangle NON sono ancora state scritte da nessun'altra parte.

namespace DelaunayTriangle
{
    struct Triangle;


    struct Point
    {
        int id;
        double x, y;
    };

    struct Edge
    {
        int id;
        int idOrigin, idEnd;
        //vector<Triangle*> idTriangles;  // id dei triangoli che contribuisce a comporre
    };


    struct Triangle
    {
        int id;

        //un array di 3 puntatori a elementi della struct Point (= i 3 vertici del triangolo considerato)
        array<Point*, 3> vertices;

        //un array di 3 puntatori a elementi della struct Edge (= i 3 lati del triangolo considerato)
        //array<Edge*, 3> edges;

        //un array di 3 elementi: si tratta di 3 puntatori a oggetti della struct Triangle (= i 3 triangoli adiacenti al triangolo considerato)
        //Triangle* adjacent_triangles[3];

        bool active = false;

        //il costruttore
        Triangle() = default;
        Triangle(Point& a, Point& b, Point& c);

        ///\brief ordina i vertici del triangolo in senso antiorario. Il controllo avviene con il prodotto vettoriale
        ///\param vertices: array di tre elementi di tipo Point, che sono i vertici del triangolo
        void OrderTrianglePoints( array<Point*, 3>& vertices );

        /* CONSIDERAZIONI SU const posto dopo un metodo:
        La parola "const" scritta alla fine della dichiarazione del metodo "area()" e "circoContainsPoint()" e "triangleContainsPoint()"
        indica che questo metodo non modifica lo stato interno dell'oggetto Triangle su cui viene chiamato.
        In altre parole, il metodo non modifica i valori dei vertici del triangolo o qualsiasi altra informazione all'interno dell'oggetto Triangle.
        L'utilizzo della parola "const" consente anche di chiamare il metodo su oggetti "const Triangle", che sono oggetti che non possono essere modificati.*/

        ///\brief calcola l'area del triangolo
        ///\return area del triangolo
        double Area() const;

        ///\brief stabilisce se il punto in input sia interno o meno alla circonferenza circoscritta al triangolo su cui viene richiamata.
        ///\param un punto della struct Point, oppure le sue coordinate
        ///\return true se il punto è interno, false se è esterno o di bordo
        bool CircoContainsPoint(const Point a) const;
        bool CircoContainsPoint(const double x, const double y) const;   //NOTA BENE: i vertici a,b,c vanno passati in senso antiorario!

        //funzione "triangleContainsPoint" prende in input le coordinate di un nuovo punto e stabilisce se questo sia interno o meno al triangolo su cui viene richiamata
        bool TriangleContainsPoint(const Point a) const;
        bool TriangleContainsPoint(const double x, const double y) const;
    };


    ///\brief Calcola la distanza tra due punti
    ///\param Due punti
    ///\return La distanza tra i due punti
    double Distance(const Point p1,
                    const Point p2);


    ///\brief Calcola l'angolo tra tre punti, corrispondente all'angolo al vertice nel primo dei punti passati (p1)
    ///\param Tre punti (corrispondenti ai vertici del triangolo): p1, p2, p3
    ///\return Il valore dell'angolo al vertice p1.
    double CalculateAngle(const Point p1,
                          const Point p2,
                          const Point p3);


    ///\brief Verifica se i due triangoli in input verificano l'ipotesi di Delaunay o meno
    ///\param Due triangoli t1 e t2, adiacenti
    ///\return True se l'ipotesi è rispettata, False altrimenti
    void DelaunayCondition( Triangle& t1,
                            Triangle& t2 );

    ///\brief Trova il triangolo di area massima
    ///\param points: vettore dei punti, tre di essi saranno i vertici del triangolo di area massima
    ///\param start: indice da cui iniziare la ricerca dentro points
    ///\param end: indice al quale finire la ricerca dentro points
    ///\return il triangolo di area massima
    Triangle GetMaxAreaTriangle(vector<Point>& points, int start, int end);
}

#endif // __EMPTY_H
