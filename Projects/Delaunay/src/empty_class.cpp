#include "empty_class.hpp"

using namespace Eigen;

namespace DelaunayTriangle
{
    /// Costruttori di Edge

    Edge::Edge(Point& a, Point& b, Mesh& mesh)
    {
        idOrigin = a.id;
        idEnd = b.id;
        id = mesh.edges.size();
    }


    Edge::Edge(Point &a, Point &b)
    {
        idOrigin = a.id;
        idEnd = b.id;
    }


    Edge::Edge(int a, int b)
    {
        idOrigin = a;
        idEnd = b;
    }


    /// Funzioni della struct Triangle

    // costruttore scritto da Sara e Annalisa che mischia il contenuto di Triangle(Point& a, Point& b, Point& c) e di AddEdge()
    Triangle::Triangle(Point& a, Point& b, Point& c, Mesh& mesh)
    {
        id = mesh.triangles.size();

        vertices[0] = &a;
        vertices[1] = &b;
        vertices[2] = &c;
        OrderTrianglePoints(vertices);

        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < mesh.edges.size(); j++)
            {
                // Controllo se la coppia di punti è già inserita negli estremi di un edge
                if (( mesh.edges[j].idOrigin == (*vertices[i]).id && mesh.edges[j].idEnd == (*vertices[(i+1)%3]).id ) ||
                    ( mesh.edges[j].idOrigin == (*vertices[(i+1)%3]).id && mesh.edges[j].idEnd == (*vertices[i]).id ))
                {
                    // Entro nell'if se il lato esiste già

                    // aggiungo l'id del triangolo costruito a idTriangles del lato in questione
                    mesh.edges[j].idTriangles.push_back(id);

                    // aggiungo l'id del lato a idEdges del triangolo che sto costruendo
                    idEdges[i] = mesh.edges[j].id;
                    break;
                }
                else
                {
                    // Il lato non c'era ancora --> lo creo
                    Edge newEdge = Edge(*vertices[i], *vertices[(i+1) % 3], mesh);

                    // Aggiungo l'id del triangolo costruito a idTriangles del lato in questione, che è newEdge
                    newEdge.idTriangles.push_back(id);

                    // aggiungo l'id del lato a idEdges del triangolo che sto costruendo
                    idEdges[i] = newEdge.id;

                    // Aggiungo il lato alla mesh
                    mesh.edges.push_back(newEdge);
                }
            }
        }

        active = true;
    }


    Triangle::Triangle(Point& a, Point& b, Point& c)
    {
        vertices[0] = &a;
        vertices[1] = &b;
        vertices[2] = &c;
        OrderTrianglePoints(vertices);
    }


    void Triangle::OrderTrianglePoints(array<Point*, 3>& vertices)
    {
        Point a = *vertices[0];
        Point b = *vertices[1];
        Point c = *vertices[2];

        Vector2d vectorAB = {b.x - a.x, b.y - a.y};  // Vettore B - A
        Vector2d vectorAC = {c.x - a.x, c.y - a.y};  // Vettore C - A

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


    /// Funzioni della struct Mesh

    bool Mesh::ImportPoints(const string& inputFilePath)
    {
        /// Apertura file
        ifstream file;
        file.open(inputFilePath);

        if (file.fail())
        {
            cerr<< "Errore apertura file"<< endl;
            return false;
        }

        ///  Lettura File
        string line;

        // ignoro la prima riga (è l'intestazione della tabella)
        getline(file, line);

        while (!file.eof())   // .eof() --> end of file
        {
            getline(file, line);
            istringstream iss(line);
            Point p;
            iss >> p.id;
            iss >> p.x;
            iss >> p.y;
            points.push_back(p);
        }

        /// Chiusura File
        file.close();

        return true;
    }


    int Mesh::AddTriangle(Triangle& t)
    {
        // Assegno qui l'id e non nel costruttore, in modo che io possa creare triangoli "fittizi" che non fanno parte della mesh, senza perdere
        // la connessione tra id del triangolo e la sua posizione dentro mesh.triangles.
        // Infatti, la funzione GetMaxAreaTriangle deve costruire tanti triangoli da confrontare tra di loro per trovare quello di area max,
        // che è l'unico che va inserito nella mesh.
        t.id = triangles.size();

        for (unsigned int i = 0; i < 3; i++)
        {
            bool found = false;
            for (unsigned int j = 0; j < edges.size(); j++)
            {
                // Controllo se la coppia di punti è già inserita negli estremi di un edge
                if (( edges[j].idOrigin == (*t.vertices[i]).id && edges[j].idEnd == (*t.vertices[ (i+1) % 3 ]).id ) ||
                    ( edges[j].idOrigin == (*t.vertices[ (i+1) % 3 ]).id && edges[j].idEnd == (*t.vertices[i]).id ))
                {
                    // Entro nell'if se il lato esiste già

                    // aggiungo l'id del triangolo costruito a idTriangles del lato in questione
                    edges[j].idTriangles.push_back(t.id);

                    // aggiungo l'id del lato a idEdges del triangolo che sto costruendo
                    t.idEdges[i] = edges[j].id;

                    found = true;
                    break;
                }
            }

            if(!found)
            {
                // Il lato non c'era ancora --> lo creo
                Edge newEdge = Edge(*t.vertices[i], *t.vertices[ (i+1) % 3 ]);

                // Aggiungo l'id del triangolo costruito a idTriangles del lato in questione, che è newEdge
                newEdge.idTriangles.push_back(t.id);

                // Aggiungo il lato alla mesh
                AddEdge(newEdge);

                // aggiungo l'id del lato a idEdges del triangolo che sto costruendo
                t.idEdges[i] = newEdge.id;
            }
        }

        // se sto aggiungendo il triangolo alla mesh, vuol dire che non era fittizio --> il triangolo è attivo
        t.active = true;
        triangles.push_back(t);
        return t.id;
    }


    void Mesh::AddEdge(Edge& e)
    {
        // Analogamente ad AddTriangle, non assegno l'id del lato nel costruttore Edge() in modo da poter costruire lati fittizi.
        // Stessa cosa per il flag active.
        e.id = edges.size();
        edges.push_back(e);
        e.active = true;
    }


    void Mesh::GenerateMesh()
    {
        for (size_t i = 0; i < points.size(); i++)   // itero sui punti della mesh
        {
            cout << "Working on point " << i << ": (" << points[i].x << ", " << points[i].y << ")" << endl;

            Point p = points[i];
            if (!p.inserted)
            {
                for (size_t j = 0; j < triangles.size(); j++)
                {
                    Triangle t = triangles[j];
                    // L'iterazione va fatta solo sui triangoli attivi (se non è attivo, la seconda condizione non viene nemmeno verificata)
                    if (t.active && t.TriangleContainsPoint(p))
                    {
                        vector<int> trianglesToCheckIds;
                        Point* a = t.vertices[0];
                        Point* b = t.vertices[1];
                        Point* c = t.vertices[2];

                        // Se t è ordinato allora anche t1, t2, e t3 saranno ordinati.
                        Triangle t1 = Triangle(*a, *b, p);
                        Triangle t2 = Triangle(*b, *c, p);
                        Triangle t3 = Triangle(*c, *a, p);

                        // Devo verificare che la condizione di Delaunay sia rispettata per i tre nuovi triangoli
                        // Sicuramente la condizione è rispettata tra coppie di due nuovi triangoli, ma non è detto che sia rispettata con gli altri
                        // già esistenti.
                        // Controllo se ho creato un triangolo degenere, e nel caso non lo salvo.
                        if (t1.Area() != 0)
                        {
                            int idT1 = AddTriangle(t1);
                            trianglesToCheckIds.push_back(idT1);
                        }

                        if (t2.Area() != 0)
                        {
                            int idT2 = AddTriangle(t2);
                            trianglesToCheckIds.push_back(idT2);
                        }

                        if (t3.Area() != 0)
                        {
                            int idT3 = AddTriangle(t3);
                            trianglesToCheckIds.push_back(idT3);
                        }

                        while(!trianglesToCheckIds.empty())
                        {
                            int triangleToCheck = trianglesToCheckIds.back();   // ritorna l'ultimo elemento del vettore
                            trianglesToCheckIds.pop_back();    // rimuove l'ultimo elemento di trianglesToCheck e riduce la sua dimensione di 1

                            // cerco gli id dei triangoli adiacenti a quello che sto controllando. Se Dalaunay non è verificato, eseguo il
                            // flip e sistemo la nuova mesh.
                            vector<int> adjs = GetAdjacencies(triangleToCheck);
                            for (size_t k = 0; k < adjs.size(); k++)
                            {
                                int adj = adjs[k];
                                int flippedT1 = -1;
                                int flippedT2 = -1;
                                bool flipped = DelaunayCondition(triangleToCheck, adj, flippedT1, flippedT2);
                                if (flipped)
                                {
                                    // Se entro nell'if, fllippedT1 e flippedT2 non valgono -1, ma contengono gli id dei due nuovi triangoli creati da DelaunayCondition
                                    trianglesToCheckIds.push_back(flippedT1);
                                    trianglesToCheckIds.push_back(flippedT2);
                                    break;
                                }
                            }
                        }

                        points[i].inserted = true;
                        // Ho frammentato in tre nuovi triangoli il triangolo triangles[j] --> lo disattivo
                        triangles[j].active = false;
                        break;
                    }
                }
            }
        }
    }


    // Il debugger si schianta in questa funzione
    vector<int> Mesh::GetAdjacencies(int sourceId)
    {
        vector<int> adjacentIds;
        for (size_t i = 0; i < 3; i++)  // itero sui lati del triangolo
        {
            int edgeId = triangles[sourceId].idEdges[i];
            for (size_t j = 0; j < edges[edgeId].idTriangles.size(); j++)   // itero sui triangoli che possiedono quel lato
            {
                int triangleId = edges[edgeId].idTriangles[j];   // id di un triangolo costruito con il lato edges[edgeId]

                if (triangleId == sourceId)
                {
                    // se sto confrontando con me stesso
                    // salto direttamente al ciclo successivo (continue fa proprio questo)
                    continue;
                }

                if (!triangles[triangleId].active)
                {
                    continue;
                }

                // Se sono arrivato qui, non sto prendendo il triangolo triangles[sourceId] come adiacente a sé stesso e sto valutando solo
                // triangoli attivi.
                adjacentIds.push_back(triangleId);
            }
        }
        return adjacentIds;
    }


    bool Mesh::DelaunayCondition(int t1, int t2, int& flippedT1, int& flippedT2)
    {
        bool flipped = false;
        // flippedT1 = -1;
        // flippedT2 = -1;

        // gli id dei punti che compongono t1 e t2
        int a1 = triangles[t1].vertices[0]->id;
        int b1 = triangles[t1].vertices[1]->id;
        int c1 = triangles[t1].vertices[2]->id;
        int a2 = triangles[t2].vertices[0]->id;
        int b2 = triangles[t2].vertices[1]->id;
        int c2 = triangles[t2].vertices[2]->id;

        int commonEdge = -1;
        array<int, 3> edges1 = triangles[t1].idEdges;
        array<int, 3> edges2 = triangles[t2].idEdges;
        for( int e1 : edges1 )      // come for (oggetto in lista) di python
        {
            for( int e2 : edges2 )
            {
                if( e1 == e2 )
                {
                    commonEdge = e1;
                    break;
                }
            }
            if( commonEdge != -1 )
                break;
        }

        int sharedVertex1 = edges[commonEdge].idOrigin;
        int sharedVertex2 = edges[commonEdge].idEnd;

        int notShared1 = -1;
        if (a1 != sharedVertex1 && a1 != sharedVertex2)
            notShared1 = a1;
        else if (b1 != sharedVertex1 && b1 != sharedVertex2)
            notShared1 = b1;
        else if (c1 != sharedVertex1 && c1 != sharedVertex2)
            notShared1 = c1;

        int notShared2 = -1;
        if (a2 != sharedVertex1 && a2 != sharedVertex2)
            notShared2 = a2;
        else if (b2 != sharedVertex1 && b2 != sharedVertex2)
            notShared2 = b2;
        else if (c2 != sharedVertex1 && c2 != sharedVertex2)
            notShared2 = c2;

        // Calcolo gli angoli al vertice per notShared1 e notShared2.
        double angle1 = CalculateAngle(points[notShared1], points[sharedVertex1], points[sharedVertex2]);
        double angle2 = CalculateAngle(points[notShared2], points[sharedVertex1], points[sharedVertex2]);

        // Verifico se l'ipotesi di Delaunay è soddisfatta.
        // Caso in cui l'ipotesi non è soddisfatta --> Eseguire l'operazione di flip.
        if (angle1 + angle2 > M_PI)
        {
            // creo il nuovo lato e lo aggiungo alla mesh
            Edge newEdge = Edge(notShared1, notShared2);
            AddEdge(newEdge);

            // creo i due nuovi triangoli "flippati" e li aggiungo alla mesh
            Triangle newTriangle1 = Triangle(points[notShared1], points[notShared2], points[sharedVertex1]);
            Triangle newTriangle2 = Triangle(points[notShared1], points[notShared2], points[sharedVertex2]);
            flippedT1 = AddTriangle(newTriangle1);
            flippedT2 = AddTriangle(newTriangle2);

            // Disabilito i due triangoli di origine
            triangles[t1].active = false;
            triangles[t2].active = false;

            // Disabilito il vecchio edge
            edges[commonEdge].active = false;

            flipped = true;
        }

        return flipped;
    }


    Triangle Mesh::GetMaxAreaTriangle(vector<Point>& points, int start, int end)
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


    /// Funzioni esterne alle struct costruite

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


}
