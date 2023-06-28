#ifndef __EMPTY_H
#define __EMPTY_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;


namespace DelaunayTriangle
{
    constexpr double max_tolerance(const double& x, const double& y)
    {
        return x > y ? x : y;
    }

    struct Mesh;   // serve perchè alla struct Edge e Triangle serve vedere la mesh

    struct Point
    {
        int id;
        double x, y;
        bool actualPoint = true;  // false se il punto è fittizio, true altrimenti

        static constexpr double geometricTol = 1.0e-12;
        static constexpr double geometricTol_Squared = max_tolerance(Point::geometricTol * Point::geometricTol, numeric_limits<double>::epsilon());
    };

    struct Edge
    {
        // lato non orientato
        int id;
        int idOrigin, idEnd;
        vector<int> idTriangles;  // id dei triangoli che contribuisce a comporre
        // Altro metodo consigliato da Teora: memorizzare idTriangles nella struct mesh
        bool active = false;

        Edge() = default;         // costruttore vuoto

        ///\param idPoint1: un estremo del segmento
        ///\param idPoint2: un estremo del segmento
        Edge(int& idPoint1, int& idPoint2);
    };


    struct Triangle
    {
        int id;
        array<int, 3> idVertices;     // array di 3 interi (= gli id dei punti che sono i vertici del triangolo)
        array<int, 3> idEdges;        // array di 3 interi (= gli id dei lati che compongono il triangolo)
        bool active = false;

        Triangle() = default;       // costruttore vuoto

        ///\param a: id di un vertice
        ///\param b: id di un vertice
        ///\param c: id di un vertice
        ///\param mesh: la mesh che stiamo riempiendo
        Triangle(int a, int b, int c, Mesh* mesh);   // input: i tre id dei punti che compongono i vertici del triangolo e la mesh.

        ///\brief ordina i vertici del triangolo in senso antiorario. Il controllo avviene con il prodotto vettoriale
        ///\param vertices: array di tre elementi di tipo int, che sono gli id dei vertici del triangolo
        ///\param mesh: la mesh che stiamo costruendo
        void OrderTrianglePoints(array<int, 3>& vertices, const Mesh& mesh);

        /* CONSIDERAZIONI SU const posto dopo un metodo:
        La parola "const" scritta alla fine della dichiarazione del metodo "area()" e "circoContainsPoint()" e "triangleContainsPoint()"
        indica che questo metodo non modifica lo stato interno dell'oggetto Triangle su cui viene chiamato.
        In altre parole, il metodo non modifica i valori dei vertici del triangolo o qualsiasi altra informazione all'interno dell'oggetto Triangle.
        L'utilizzo della parola "const" consente anche di chiamare il metodo su oggetti "const Triangle", che sono oggetti che non possono essere modificati.*/

        ///\brief calcola l'area del triangolo
        ///\param mesh: la mesh che stiamo costruendo
        ///\return area del triangolo
        double Area(const Mesh* mesh) const;

        ///\brief stabilisce se il punto in input sia interno o meno alla circonferenza circoscritta al triangolo su cui viene richiamata.
        ///\param l'id di un punto della struct Point, oppure le sue coordinate (passate in senso antiorario)
        ///\param mesh: la mesh che stiamo costruendo
        ///\return true se il punto è interno, false se è esterno o di bordo
        bool CircoContainsPoint(const int idPoint, const Mesh& mesh) const;
        bool CircoContainsPoint(const double x, const double y, const Mesh& mesh) const;   //NOTA BENE: i vertici a,b,c vanno passati in senso antiorario!

        ///\brief stabilisce se il punto in input sia interno, do bordo o esterno al triangolo su cui viene richiamata.
        ///\param l'id di un punto della struct Point, oppure le sue coordinate (passate in senso antiorario)
        ///\param mesh: la mesh che stiamo costruendo
        ///\return 0 se il punto è esterno, 1 se è interno, 2 se è di bordo
        int TriangleContainsPoint(const int idPoint, const Mesh& mesh) const;
        int TriangleContainsPoint(const double x, const double y, const Mesh& mesh) const;
    };


    struct Mesh
    {
        vector<Point> points;
        vector<Edge> edges;
        vector<Triangle> triangles;

        Mesh() = default;   // costruttore

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

        /// \brief Trova un triangolo con vertici fittizi che ricopre tutti i punti della mesh
        /// \return Un triangolo che contiene tutti i punti della mesh
        Triangle FakeTriangleCover();

        /// \brief Trova gli id dei triangoli adiacenti al triangolo di cui ho passato l'id
        /// \param idTriangle: id del triangolo di cui voglio trovare le adiacenze
        /// \return il vettore di interi contenente gli id dei triangoli adiacenti
        vector<int> GetAdjacencies(const int& idTriangle);

        /// \brief Controlla se la condizione di Delaunay tra due triangoli è rispettata e nel caso esegue il flip
        /// \param t1 e t2: id di due triangoli adiacenti
        /// \return true se il flip è avvenuto, false altrimenti
        bool DelaunayCondition(const int& t1, const int& t2, int& flippedT1, int& flippedT2);

        /// \brief Aggiunge i punti interni alla mesh
        void GenerateMesh();

        /// \brief Disattiva lati e triangoli costruiti con i vertici fittizi
        void DeactivateFakeTriangles();

        /// \brief Esporta gli edges (attivi) in un file csv. Ogni riga è del tipo "idEdge idOrigin idEnd". La prima riga è l'intestazione.
        /// \param edges: il vettore contenente gli edges del programma
        /// \param outputFileName: il file che viene prodotto
        void ExportEdges(const vector<Edge>& edges, const string& outputFileName);

    };


    ///\brief Calcola la distanza tra due punti
    ///\param Due punti
    ///\return La distanza tra i due punti
    double Distance(const Point& p1, const Point& p2);


    ///\brief Calcola l'angolo tra tre punti, corrispondente all'angolo al vertice nel primo dei punti passati (p1)
    ///\param Tre punti (corrispondenti ai vertici del triangolo): p1, p2, p3
    ///\return Il valore dell'angolo al vertice p1.
    double CalculateAngle(const Point& p1,  const Point& p2, const Point& p3);


    inline double normSquared(const double& x, const double& y)
    {
        return x * x + y * y;
    }


    inline bool operator == (Point& p, array<double, 2>& coord)
    {
        return ( normSquared(p.x - coord[0], p.y - coord[1]) <= Point::geometricTol * Point::geometricTol *
                                                                          max( normSquared(p.x, p.y), normSquared(coord[0], coord[1]) ) );
    }


    inline bool operator != (Point& p, array<double, 2>& coord)
    {
        return !( p == coord );
    }
}

#endif // __EMPTY_H
