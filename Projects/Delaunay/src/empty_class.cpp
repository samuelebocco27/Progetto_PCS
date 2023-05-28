#include "empty_class.hpp"

using namespace Eigen;

namespace DelaunayTriangle
{
    /// Funzioni della struct Triangle

    Triangle::Triangle( Point& a, Point& b, Point& c )
    {
        vertices[0] = &a;
        vertices[1] = &b;
        vertices[2] = &c;
        //OrderTrianglePoints(vertices);  // Ordino i vertici del triangolo in senso antiorario
        //adjacent_triangles[0] = nullptr;
        //adjacent_triangles[1] = nullptr;
        //adjacent_triangles[2] = nullptr;
        //Inizializziamo i puntatori a 'nullptr', non avendo ancora definito gli oggetti Triangle adiacenti.
        active = true;
    }


    void Triangle::OrderTrianglePoints( array<Point*, 3>& vertices )
    {
        Point a = *vertices[0];
        Point b = *vertices[1];
        Point c = *vertices[2];

        Vector2d vectorAB = {b.x - a.x, b.y - a.y};  // "Vettore" B - A
        Vector2d vectorAC = {c.x - a.x, c.y - a.y};  // "Vettore" C - A

        // Prodotto vettoriale: vectorAB x vectorAC = (B-A) x (C-A)
        double crossProduct = vectorAB.x() * vectorAC.y() - vectorAB.y() * vectorAC.x();

        if (crossProduct < 0)
        {
            // Scambio il punto B e il punto C
            vertices[1] = vertices[2];
            vertices[2] = &b;
        }
    }


    double Triangle::Area() const
    {
        if( vertices[0] == nullptr || vertices[1] == nullptr || vertices[2] == nullptr )
            return 0.0;

        Point a = *vertices[0];
        Point b = *vertices[1];
        Point c = *vertices[2];
        //calcolo l'area mediante il prodotto vettoriale: area(abc) = (ab x ac)/2
        double ab_x = b.x - a.x;
        double ab_y = b.y - a.y;
        double ac_x = c.x - a.x;
        double ac_y = c.y - a.y;
        double area = std::abs(ab_x * ac_y - ab_y * ac_x)*0.5; //(svolgo il prodotto vettoriale con il determinante)

        return area;
    }


    bool Triangle::CircoContainsPoint(const Point a) const
    {
        return CircoContainsPoint( a.x, a.y );
    }


    bool Triangle::CircoContainsPoint(const double x, const double y) const //NOTA BENE: i vertici a,b,c vanno passati in senso antiorario!
    {
        //chiamiamo a,b,c i vertici del triangolo e d il nuovo punto.
        //creo queste variabili ausiliarie dal nome "corto" per semplificare la scrittura del determinante.
        double ax = (*vertices[0]).x; double ay = (*vertices[0]).y;
        double bx = (*vertices[1]).x; double by = (*vertices[1]).y;
        double cx = (*vertices[2]).x; double cy = (*vertices[2]).y;
        double dx = x; double dy = y;

        double elem13 = pow((ax - dx), 2) + pow((ay -dy), 2);
        double elem23 = pow((bx - dx), 2) + pow((by -dy), 2);
        double elem33 = pow((cx - dx), 2) + pow((cy -dy), 2);

        double det = (ax - dx) * ( (by - dy) * elem33 - (cy - dy) * elem23 ) +
                     - (ay - dy) * ( (bx - dx) * elem33 - (cx - dx) * elem23 ) +
                     + elem13 * ( (bx - dx) * (cy - dy) - (by - dy) * (cx - dx) );

        if (det > 0)
            return true; //il punto è interno alla circonferenza
        else
            return false; //il punto è esterno o di bordo alla circonferenza
    }


    bool Triangle::TriangleContainsPoint(const Point a) const
    {
        return TriangleContainsPoint(a.x, a.y);
    }


    bool Triangle::TriangleContainsPoint(const double x, const double y) const
    {
        //faccio un primo check verificando se il punto d sia esterno o meno alla circonferenza circoscritta al triangolo
        if (!CircoContainsPoint(x, y))
        {
            //il punto è sicuramente esterno al triangolo
            return false;
        }

        //chiamiamo a,b,c i vertici del triangolo e d il nuovo punto
        //creo queste variabili ausiliarie dal nome "corto" per semplificare la scrittura dei determinanti
        double ax = (*vertices[0]).x; double ay = (*vertices[0]).y;
        double bx = (*vertices[1]).x; double by = (*vertices[1]).y;
        double cx = (*vertices[2]).x; double cy = (*vertices[2]).y;
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
        bool result = (detABD > 0 && detBCD > 0 && detCAD >= 0) || (detABD < 0 && detBCD < 0 && detCAD <= 0);
        //NOTO: dal momento che i vertici a,b,c del triangolo sono ordinati in senso orario,
        //allora il segno dei determinanti sarà sempre positivo (qualora d fosse interno)

        return result;
    }


    /// Funzioni esterne alla struct Triangle

    double Distance(const Point p1,
                    const Point p2)
    {
        double dx = p2.x - p1.x;
        double dy = p2.y - p1.y;
        return sqrt(dx * dx + dy * dy);
    }


    double CalculateAngle(const Point p1,
                          const Point p2,
                          const Point p3)
    {
        double d1 = Distance(p2, p3);
        double d2 = Distance(p1, p3);
        double d3 = Distance(p1, p2);
        return acos((d2 * d2 + d3 * d3 - d1 * d1) / (2 * d2 * d3));
    }


    void DelaunayCondition( Triangle& t1,
                            Triangle& t2 )
    {
        Point& a1 = *t1.vertices[0];
        Point& b1 = *t1.vertices[1];
        Point& c1 = *t1.vertices[2];
        Point& a2 = *t2.vertices[0];
        Point& b2 = *t2.vertices[1];
        Point& c2 = *t2.vertices[2];

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
        double angle1 = CalculateAngle(*notShared1, *sharedVertex1, *sharedVertex2);  // gli input sono di tipo Point
        double angle2 = CalculateAngle(*notShared2, *sharedVertex1, *sharedVertex2);  // gli input sono di tipo Point

        // Verifico se l'ipotesi di Delaunay è soddisfatta.
        // Caso in cui l'ipotesi non è soddisfatta --> Eseguire l'operazione di flip.
        if (angle1 + angle2 > M_PI)
        {
            // Elimino il lato (sharedVertex1, sharedVertex2) e creo il lato (notShared1, notShared2)
            t1 = Triangle(*notShared1, *notShared2, *sharedVertex1);
            t2 = Triangle(*notShared1, *notShared2, *sharedVertex2);
        }
    }


    Triangle GetMaxAreaTriangle(vector<Point>& points, int start, int end)
    {
        // Se ci sono meno di due punti, l'area non può essere calcolata e quindi il processo ritorna il triangolo di area nulla, che
        // sicuramente non è quello di area massima
        if( end - start < 2 )
        {
            return Triangle();
        }

        // Caso base: se ci sono solo tre punti, il triangolo di area massima è l'unico triangolo possibile
        if (end - start == 2) {
            return Triangle(points[start], points[start + 1], points[start + 2]);
        }

        // Divido i punti in due gruppi. Trovo ricorsivamente la massima area per i punti di un set e dell'altro
        int mid = start + (end - start) / 2;
        Triangle leftMaxTriangle = GetMaxAreaTriangle(points, start, mid);
        double leftMaxArea = leftMaxTriangle.Area();
        Triangle rightMaxTriangle = GetMaxAreaTriangle(points, mid, end);
        double rightMaxArea = rightMaxTriangle.Area();

        // Trovo l'area massima per i triangoli formati da punti provenienti da entrambi i set
        double crossMax = 0.0;
        Triangle crossMaxTriangle;
        int left = mid - 1, right = mid + 1;
        while (left >= start && right < end)
        {
            crossMaxTriangle = Triangle(points[left], points[mid], points[right]);
            crossMax = max(crossMax, crossMaxTriangle.Area());
            if (left == start)
            {
                right++;
            }
            else if (right == end - 1)
            {
                left--;
            }
            else
            {
                double area1 = Triangle(points[left - 1], points[left], points[mid]).Area();
                double area2 = Triangle(points[mid], points[right], points[right + 1]).Area();
                if (area1 > area2)
                {
                    left--;
                }
                else
                {
                    right++;
                }
            }
        }

        // Ritorno l'area massima tra i triangoli con area massima dei due set e di quello che li mischia entrambi.
        double max_area = max(crossMax, max(leftMaxArea, rightMaxArea));
        if( max_area == leftMaxArea )
            return leftMaxTriangle;
        else if( max_area == rightMaxArea )
            return rightMaxTriangle;
        else
            return crossMaxTriangle;
    }



}
