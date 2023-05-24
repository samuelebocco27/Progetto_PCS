#ifndef __EMPTY_H
#define __EMPTY_H

#include <iostream>
#include "Eigen/Eigen"


// COMMENTO GUIDA: In questo file (e nel suo corrispettivo .cpp) sono state inseriti i contenuti delle struct Point e Triangle del file "versione1annalisa_class.hpp".
// Il file "versione1annalisa_class.hpp" contiene ancora tutto al suo interno.
// Le funzioni del file "versione1annalisa_class.hpp" che stanno sotto alla struct Triangle NON sono ancora state scritte da nessun'altra parte.

namespace DelaunayTriangle
{
    struct Point
    {
        double x, y;
    };


    struct Triangle
    {
        //un array di 3 elementi della struct Point (= i 3 vertici del triangolo considerato)
        Point vertices[3];

        //un array di 3 elementi: si tratta di 3 puntatori a oggetti della struct Triangle (= i 3 triangoli adiacenti al triangolo considerato)
        Triangle* adjacent_triangles[3];

        //il costruttore
        Triangle(double x1 = 0.0, double y1 = 0.0, double x2 = 0.0, double y2 = 0.0, double x3 = 0.0, double y3 = 0.0);

        ///\brief ordina i vertici del triangolo in senso antiorario. Il controllo avviene con il prodotto vettoriale
        ///\param vertices: array di tre elementi di tipo Point, che sono i vertici del triangolo
        void OrderTrianglePoints( Point (&vertices)[3] );

        /* CONSIDERAZIONI SU const posto dopo un metodo:
        La parola "const" scritta alla fine della dichiarazione del metodo "area()" e "circoContainsPoint()" e "triangleContainsPoint()"
        indica che questo metodo non modifica lo stato interno dell'oggetto Triangle su cui viene chiamato.
        In altre parole, il metodo non modifica i valori dei vertici del triangolo o qualsiasi altra informazione all'interno dell'oggetto Triangle.
        L'utilizzo della parola "const" consente anche di chiamare il metodo su oggetti "const Triangle", che sono oggetti che non possono essere modificati.*/

        //funzione "area" calcola l'area del triangolo
        double area() const;

        //funzione "circoContainsPoint" prende in input le coordinate di un nuovo punto e stabilisce se questo sia interno o meno alla circonferenza circoscritta al triangolo su cui viene richiamata.
        bool circoContainsPoint(const double x, const double y) const;   //NOTA BENE: i vertici a,b,c vanno passati in senso antiorario!

        //funzione "triangleContainsPoint" prende in input le coordinate di un nuovo punto e stabilisce se questo sia interno o meno al triangolo su cui viene richiamata
        bool triangleContainsPoint(const double x, const double y) const;
    };


    ///\brief Calcola la distanza tra due punti
        ///\param Due punti
        ///\return La distanza tra i due punti
        double Distance(const Point& p1,
                        const Point& p2);


        ///\brief Calcola l'angolo tra tre punti, corrispondente all'angolo al vertice nel primo dei punti passati (p1)
        ///\param Tre punti (corrispondenti ai vertici del triangolo): p1, p2, p3
        ///\return Il valore dell'angolo al vertice p1.
        double calculateAngle(const Point& p1,
                              const Point& p2,
                              const Point& p3);


        ///\brief Verifica se i due triangoli in input verificano l'ipotesi di Delaunay o meno
        ///\param Due triangoli t1 e t2, adiacenti
        ///\return True se l'ipotesi è rispettata, False altrimenti
        void delaunayCondition( Triangle& t1,
                                Triangle& t2 );


}

#endif // __EMPTY_H
