#include "empty_class.hpp"

namespace ProjectLibrary
{
    Triangle::Triangle(double x1, double y1, double x2, double y2, double x3, double y3)
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


    void Triangle::OrderTrianglePoints( Point (&vertices)[3] )
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


    double Triangle::area() const
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


    bool Triangle::circoContainsPoint(const double x, const double y) const //NOTA BENE: i vertici a,b,c vanno passati in senso antiorario!
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


    bool Triangle::triangleContainsPoint(const double x, const double y) const
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
}
