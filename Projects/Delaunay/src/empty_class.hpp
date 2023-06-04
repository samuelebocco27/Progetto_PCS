#ifndef __EMPTY_H
#define __EMPTY_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "Eigen/Eigen"


using namespace std;

// COMMENTO GUIDA: In questo file (e nel suo corrispettivo .cpp) sono state inseriti i contenuti delle struct Point e Triangle del file "versione1annalisa_class.hpp".
// Il file "versione1annalisa_class.hpp" contiene ancora tutto al suo interno.
// Le funzioni del file "versione1annalisa_class.hpp" che stanno sotto alla struct Triangle NON sono ancora state scritte da nessun'altra parte.

namespace DelaunayTriangle
{
    struct Mesh;   // serve perchè alla struct Edge e Triangle serve vedere la mesh

    struct Point
    {
        int id;
        double x, y;
        bool inserted = false;  // true se il punto è stato già utilizzato per costruire i triangoli, false altrimenti
    };

    struct Edge
    {
        // lato non orientato
        int id;
        int idOrigin, idEnd;
        vector<int> idTriangles;  // id dei triangoli che contribuisce a comporre
        // Altro metodo consigliato da Teora: memorizzare idTriangles nella struct mesh
        bool active = false;

        Edge() = default;                        // costruttore vuoto
        Edge(int a, int b);                      // input: id dei due punti estremi del segmento
        Edge(Point& a, Point& b);                // input: i due punti estremi del segmento
        Edge(Point& a, Point& b, Mesh& mesh);    // input: due punti estremi del segmento, la mesh. AD ORA, NON E' UTILIZZATO
    };


    struct Triangle
    {
        int id;

        // array di 3 puntatori a elementi della struct Point (= i 3 vertici del triangolo considerato)
        // N.B.: Sarebbe meglio convertirlo a " array<int, 3> idVertices "
        array<Point*, 3> vertices;

        // array di 3 interi (= gli id dei lati che compongono il triangolo)
        array<int, 3> idEdges;

        bool active = false;

        Triangle() = default;                                 // costruttore vuoto
        Triangle(Point& a, Point& b, Point& c);               // input: i tre punti che compongono i vertici del triangolo
        Triangle(Point& a, Point& b, Point& c, Mesh& mesh);   // input: i tre punti che compongono i vertici del triangolo e la mesh. AD ORA, NON UTILIZZATO

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
        ///\param un punto della struct Point, oppure le sue coordinate (passate in senso antiorario)
        ///\return true se il punto è interno, false se è esterno o di bordo
        bool CircoContainsPoint(const Point a) const;
        bool CircoContainsPoint(const double x, const double y) const;   //NOTA BENE: i vertici a,b,c vanno passati in senso antiorario!

        ///\brief stabilisce se il punto in input sia interno o meno al riangolo su cui viene richiamata.
        ///\param un punto della struct Point, oppure le sue coordinate (passate in senso antiorario)
        ///\return true se il punto è interno, false se è esterno o di bordo
        bool TriangleContainsPoint(const Point a) const;
        bool TriangleContainsPoint(const double x, const double y) const;
    };


    struct Mesh
    {
        vector<Point> points;
        vector<Edge> edges;
        vector<Triangle> triangles;

        Mesh() = default;  // costruttore

        /// \brief Legge i punti contenuti in Points.csv
        /// \param filePath: path del file di input
        /// \param points: coordinate della mesh
        /// \return il risultato della lettura: true se è andata a buon fine, false altrimenti
        bool ImportPoints(const string& inputFilePath);

        /// \brief Aggiunge un triangolo al vettore contenente i triangoli della triangolazione
        /// \param t: un triangolo della struct Triangle
        /// \return l'id del triangolo appena aggiunto alla triangolazione
        int AddTriangle(Triangle& t);

        /// \brief Aggiunge un lato al vettore contenente i lati della triangolazione
        /// \param e: un lato della struct Edge
        void AddEdge(Edge& e);

        ///\brief Trova il triangolo di area massima
        ///\param points: vettore dei punti, tre di essi saranno i vertici del triangolo di area massima
        ///\param start: indice da cui iniziare la ricerca dentro points
        ///\param end: indice al quale finire la ricerca dentro points
        ///\return il triangolo di area massima
        Triangle GetMaxAreaTriangle(vector<Point>& points, int start, int end);

        /// \brief Aggiunge i punti interni alla mesh
        void GenerateMesh();

        /// \brief Trova gli id dei triangoli adiacenti al triangolo di cui ho passato l'id
        /// \param source: id del triangolo di cui voglio trovare le adiacenze
        /// \return il vettore di interi contenente gli id dei triangoli adiacenti
        vector<int> GetAdjacencies(int source);

        /// \brief Controlla se la condizione di Delaunay tra due triangoli è rispettata e nel caso esegue il flip
        /// \param t1 e t2: id di due triangoli adiacenti
        /// \return true se il flip è avvenuto, false altrimenti
        bool DelaunayCondition( int t1, int t2, int& flippedT1, int& flippedT2 );
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

}

#endif // __EMPTY_H
