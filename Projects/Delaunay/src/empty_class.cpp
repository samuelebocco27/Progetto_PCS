#include "empty_class.hpp"

using namespace Eigen;

namespace DelaunayTriangle
{
    /// Costruttore di Edge
    ///
    Edge::Edge(int& idPoint1, int& idPoint2)
    {
        idOrigin = idPoint1;
        idEnd = idPoint2;
    }


    /// Funzioni della struct Triangle
    ///
    Triangle::Triangle(int a, int b, int c, Mesh* mesh)
    {
        idVertices[0] = a;
        idVertices[1] = b;
        idVertices[2] = c;
        OrderTrianglePoints(idVertices, *mesh);
    }


    void Triangle::OrderTrianglePoints(array<int ,3>& vertices, const Mesh& mesh)
    {
        Point a = mesh.points[vertices[0]];
        Point b = mesh.points[vertices[1]];
        Point c = mesh.points[vertices[2]];

        Vector2d vectorAB = {b.x - a.x, b.y - a.y};  // Vettore B - A
        Vector2d vectorAC = {c.x - a.x, c.y - a.y};  // Vettore C - A

        // Prodotto vettoriale: vectorAB x vectorAC = (B-A) x (C-A)
        double crossProduct = vectorAB.x() * vectorAC.y() - vectorAB.y() * vectorAC.x();

        if (crossProduct < Point::geometricTol_Squared)
        {
            // Scambio il punto B e il punto C
            int temp = vertices[1];
            vertices[1] = vertices[2];
            vertices[2] = temp;
        }
    }


    double Triangle::Area(const Mesh* mesh) const
    {
        if( idVertices[0] < 0 || idVertices[1] < 0 || idVertices[2] < 0 )
            return 0.0;

        Point a = mesh->points[idVertices[0]];
        Point b = mesh->points[idVertices[1]];
        Point c = mesh->points[idVertices[2]];
        //calcolo l'area mediante il prodotto vettoriale: area(abc) = (ab x ac)/2
        double ab_x = b.x - a.x;
        double ab_y = b.y - a.y;
        double ac_x = c.x - a.x;
        double ac_y = c.y - a.y;
        double area = std::abs(ab_x * ac_y - ab_y * ac_x)*0.5;    //(svolgo il prodotto vettoriale con il determinante)

        return area;
    }


    bool Triangle::CircoContainsPoint(const int idPoint, const Mesh& mesh) const
    {
        return CircoContainsPoint(mesh.points[idPoint].x, mesh.points[idPoint].y, mesh);
    }


    bool Triangle::CircoContainsPoint(const double x, const double y, const Mesh& mesh) const //NOTA BENE: i vertici a,b,c vanno passati in senso antiorario!
    {
        //chiamiamo a,b,c i vertici del triangolo e d il nuovo punto.
        //creo queste variabili ausiliarie dal nome "corto" per semplificare la scrittura del determinante.
        double ax = mesh.points[idVertices[0]].x; double ay = mesh.points[idVertices[0]].y;
        double bx = mesh.points[idVertices[1]].x; double by = mesh.points[idVertices[1]].y;
        double cx = mesh.points[idVertices[2]].x; double cy = mesh.points[idVertices[2]].y;
        double dx = x; double dy = y;

        double elem13 = pow((ax - dx), 2) + pow((ay -dy), 2);
        double elem23 = pow((bx - dx), 2) + pow((by -dy), 2);
        double elem33 = pow((cx - dx), 2) + pow((cy -dy), 2);

        double det = (ax - dx) * ( (by - dy) * elem33 - (cy - dy) * elem23 ) +
                     - (ay - dy) * ( (bx - dx) * elem33 - (cx - dx) * elem23 ) +
                     + elem13 * ( (bx - dx) * (cy - dy) - (by - dy) * (cx - dx) );

        if (det > Point::geometricTol)
            return true; //il punto è interno alla circonferenza
        else
            return false; //il punto è esterno o di bordo alla circonferenza
    }


    int Triangle::TriangleContainsPoint(const int idPoint, const Mesh& mesh) const
    {
        return TriangleContainsPoint(mesh.points[idPoint].x, mesh.points[idPoint].y, mesh);
    }


    int Triangle::TriangleContainsPoint(const double x, const double y, const Mesh& mesh) const
    {
        //faccio un primo check verificando se il punto d sia esterno o meno alla circonferenza circoscritta al triangolo
        if (!CircoContainsPoint(x, y, mesh))
        {
            //il punto è sicuramente esterno al triangolo
            return 0;
        }

        //chiamiamo a,b,c i vertici del triangolo e d il nuovo punto
        //creo queste variabili ausiliarie dal nome "corto" per semplificare la scrittura dei determinanti
        double ax = mesh.points[idVertices[0]].x; double ay = mesh.points[idVertices[0]].y;
        double bx = mesh.points[idVertices[1]].x; double by = mesh.points[idVertices[1]].y;
        double cx = mesh.points[idVertices[2]].x; double cy = mesh.points[idVertices[2]].y;
        double dx = x; double dy = y;

        //Questo calcolo può essere interpretato come la terza componente
        //del prodotto vettoriale tra i vettori DA e DB (con origine in D) (ovvero DA x DB)
        double detABD = (ax - dx) * (by - dy) - (bx - dx) * (ay - dy);
        //Osservo: avendo preso A,B,C in senso antiorario,
        //il segno del detABD sarà positivo se D si trova a sinistra del vettore diretto da A a B,
        //e negativo se si trova invece alla sua a destra. (interpreto destra e sinistra "volgendo gli occhi a B e dando le spalle ad A")
        double detBCD = (bx - dx) * (cy - dy) - (cx - dx) * (by - dy);
        double detCAD = (cx - dx) * (ay - dy) - (ax - dx) * (cy - dy);

        int result;
        double tol = Point::geometricTol_Squared;
        if ((detABD > tol && detBCD > tol && detCAD > tol) || (detABD < tol && detBCD < tol && detCAD < tol))
            // il punto è interno al triangolo (non di bordo)
            result = 1;
        else if ((abs(detABD) <= tol && abs(detBCD) > tol && abs(detCAD) > tol) ||
                 (abs(detABD) > tol && abs(detBCD) <= tol && abs(detCAD) > tol) ||
                 (abs(detABD) > tol && abs(detBCD) > tol && abs(detCAD) <= tol))
            // il punto è di bordo al triangolo
            result = 2;
        else
            // il punto è esterno
            result = 0;

        //NOTO: dal momento che i vertici a,b,c del triangolo sono ordinati in senso orario,
        //allora il segno dei determinanti sarà sempre positivo (qualora d fosse interno)

        return result;
    }


    /// Funzioni della struct Mesh
    ///
    bool Mesh::ImportPoints(const string& inputFilePath)
    {
        /// Apertura file
        ifstream file;
        file.open(inputFilePath);

        if (file.fail())
        {
            cerr << "Errore apertura file." << endl;
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
                if (( edges[j].idOrigin == t.idVertices[i] && edges[j].idEnd == t.idVertices[ (i+1) % 3 ] ) ||
                    ( edges[j].idOrigin == t.idVertices[ (i+1) % 3 ] && edges[j].idEnd == t.idVertices[i] ))
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
                Edge newEdge = Edge(t.idVertices[i], t.idVertices[ (i+1) % 3 ]);

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
        e.active = true;
        edges.push_back(e);
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
            return Triangle(start, start + 1, start + 2, this);
        }

        // Divido i punti in due gruppi. Trovo ricorsivamente la massima area per i punti di un set e dell'altro
        int mid = start + (end - start) / 2;
        Triangle leftMaxTriangle = GetMaxAreaTriangle(points, start, mid);
        double leftMaxArea = leftMaxTriangle.Area(this);
        Triangle rightMaxTriangle = GetMaxAreaTriangle(points, mid, end);
        double rightMaxArea = rightMaxTriangle.Area(this);

        // Trovo l'area massima per i triangoli formati da punti provenienti da entrambi i set
        double crossMax = 0.0;
        Triangle crossMaxTriangle;
        int left = mid - 1, right = mid + 1;
        while (left >= start && right < end)
        {
            crossMaxTriangle = Triangle(left, mid, right, this);
            crossMax = max(crossMax, crossMaxTriangle.Area(this));
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
                double area1 = Triangle(left - 1, left, mid, this).Area(this);
                double area2 = Triangle(mid, right, right + 1, this).Area(this);
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


    Triangle Mesh::FakeTriangleCover()
    {
        // Cerco max x, max y, min x, min y
        double xMin = points[0].x, xMax = points[0].x;
        double yMin = points[0].y, yMax = points[0].y;
        for ( Point p : points )
        {
            if( p.x < xMin )
                xMin = p.x;
            if( p.x > xMax )
                xMax = p.x;
            if( p.y < yMin )
                yMin = p.y;
            if( p.y > yMax )
                yMax = p.y;
        }

        array<double, 2> p1 = {xMin, yMin}, p2 = {xMin, 2 * yMax - yMin}, p3 = {2 * xMax - xMin, yMin};

        // Controllo se i punti del maxi-triangolo che voglio costruire coincidono con dei punti reali o se sono fittizi
        bool existsCoord1 = false;
        bool existsCoord2 = false;
        bool existsCoord3 = false;
        int id1, id2, id3;     // id dei tre vertici (fittizi o non) che concorrono a formare il triangolo che ricopre tutto il set di punti
        for ( Point p : points )
        {
            if ( p == p1 )
            {
                existsCoord1 = true;
                id1 = p.id;
            }
            if ( p == p2 )
            {
                existsCoord2 = true;
                id2 = p.id;
            }
            if ( p == p3 )
            {
                existsCoord3 = true;
                id3 = p.id;
            }
        }

        if ( !existsCoord1 )
        {
            Point coord1;
            coord1.id = points.size(); coord1.x = p1[0]; coord1.y = p1[1]; coord1.actualPoint = false;
            points.push_back( coord1 );
            id1 = coord1.id;
        }
        if ( !existsCoord2 )
        {
            Point coord2;
            coord2.id = points.size(); coord2.x = p2[0]; coord2.y = p2[1]; coord2.actualPoint = false;
            points.push_back( coord2 );
            id2 = coord2.id;
        }
        if ( !existsCoord3 )
        {
            Point coord3;
            coord3.id = points.size(); coord3.x = p3[0]; coord3.y = p3[1]; coord3.actualPoint = false;
            points.push_back( coord3 );
            id3 = coord3.id;
        }

        // Triangolo che ricopre interamente tutto il set di punti
        Triangle coverTriangle = Triangle(id1, id2, id3, this);

        return coverTriangle;
    }


    vector<int> Mesh::GetAdjacencies(const int& idSource)
    {
        vector<int> adjacentIds;
        for (unsigned int i = 0; i < 3; i++)  // itero sui lati del triangolo
        {
            int edgeId = triangles[idSource].idEdges[i];
            for (unsigned int j = 0; j < edges[edgeId].idTriangles.size(); j++)   // itero sui triangoli che possiedono quel lato
            {
                int triangleId = edges[edgeId].idTriangles[j];   // id di un triangolo costruito con il lato edges[edgeId]

                if (triangleId == idSource)
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


    bool Mesh::DelaunayCondition(const int& t1, const int& t2, int& flippedT1, int& flippedT2)
    {
        bool flipped = false;

        // gli id dei punti che compongono t1 e t2
        int a1 = triangles[t1].idVertices[0];
        int b1 = triangles[t1].idVertices[1];
        int c1 = triangles[t1].idVertices[2];
        int a2 = triangles[t2].idVertices[0];
        int b2 = triangles[t2].idVertices[1];
        int c2 = triangles[t2].idVertices[2];

        int commonEdge = -1;
        array<int, 3> edges1 = triangles[t1].idEdges;
        array<int, 3> edges2 = triangles[t2].idEdges;
        for( int e1 : edges1 )      // come " for (oggetto in lista) " di python
        {
            for( int e2 : edges2 )
            {
                if(e1 == e2)
                {
                    commonEdge = e1;
                    break;
                }
            }
            if(commonEdge != -1)
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
        if (angle1 + angle2 > M_PI + Point::geometricTol)
        {
            // creo il nuovo lato e lo aggiungo alla mesh
            Edge newEdge = Edge(notShared1, notShared2);
            AddEdge(newEdge);

            // creo i due nuovi triangoli "flippati" e li aggiungo alla mesh
            Triangle newTriangle1 = Triangle(notShared1, notShared2, sharedVertex1, this);
            Triangle newTriangle2 = Triangle(notShared1, notShared2, sharedVertex2, this);
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


    void Mesh::GenerateMesh()
    {
        for (unsigned int i = 0; i < points.size(); i++)   // itero sui punti della mesh
        {
            // alreadyEntered controlla se un punto di bordo è già stato valutato da
            bool alreadyEntered = false;
            for (unsigned int j = 0; j < triangles.size(); j++)
            {
                Triangle t = triangles[j];
                // L'iterazione va fatta solo sui triangoli attivi (se non è attivo, la seconda condizione non viene nemmeno verificata)
                int pointInTriangle = t.TriangleContainsPoint(i, *this);
                if (t.active && pointInTriangle != 0)
                {
                    vector<int> trianglesToCheckIds;
                    int a = t.idVertices[0];
                    int b = t.idVertices[1];
                    int c = t.idVertices[2];

                    // Se t è ordinato allora anche t1, t2, e t3 saranno ordinati.
                    Triangle t1 = Triangle(a, b, i, this);
                    Triangle t2 = Triangle(b, c, i, this);
                    Triangle t3 = Triangle(c, a, i, this);

                    // Ho frammentato in tre nuovi triangoli il triangolo triangles[j] --> lo disattivo
                    triangles[j].active = false;

                    // Devo verificare che la condizione di Delaunay sia rispettata per i tre nuovi triangoli
                    // Sicuramente la condizione è rispettata tra coppie di due nuovi triangoli, ma non è detto che sia rispettata con gli altri
                    // già esistenti.
                    // Controllo se ho creato un triangolo degenere, e nel caso non lo salvo.
                    // Se il triangolo è degenere, un suo lato verrà spezzato in due nuovi lati, dunque lo disattivo.
                    if (t1.Area(this) > Point::geometricTol_Squared)
                    {
                        int idT1 = AddTriangle(t1);
                        trianglesToCheckIds.push_back(idT1);
                    }
                    else
                    {
                        for (unsigned int k = 0; k < t.idEdges.size(); k++)
                        {
                            if ( ( edges[t.idEdges[k]].idOrigin == a && edges[t.idEdges[k]].idEnd == b ) ||
                                 ( edges[t.idEdges[k]].idOrigin == b && edges[t.idEdges[k]].idEnd == a ) )

                            edges[t.idEdges[k]].active = false;
                        }
                    }

                    if (t2.Area(this) > Point::geometricTol_Squared)
                    {
                        int idT2 = AddTriangle(t2);
                        trianglesToCheckIds.push_back(idT2);
                    }
                    else
                    {
                        for (unsigned int k = 0; k < t.idEdges.size(); k++)
                        {
                            if ( ( edges[t.idEdges[k]].idOrigin == c && edges[t.idEdges[k]].idEnd == b ) ||
                                 ( edges[t.idEdges[k]].idOrigin == b && edges[t.idEdges[k]].idEnd == c ) )

                            edges[t.idEdges[k]].active = false;
                        }
                    }

                    if (t3.Area(this) > Point::geometricTol_Squared)
                    {
                        int idT3 = AddTriangle(t3);
                        trianglesToCheckIds.push_back(idT3);
                    }
                    else
                    {
                        for (unsigned int k = 0; k < t.idEdges.size(); k++)
                        {
                            if ( ( edges[t.idEdges[k]].idOrigin == a && edges[t.idEdges[k]].idEnd == c ) ||
                                 ( edges[t.idEdges[k]].idOrigin == c && edges[t.idEdges[k]].idEnd == a ) )

                            edges[t.idEdges[k]].active = false;
                        }
                    }

                    while(!trianglesToCheckIds.empty())
                    {
                        int triangleToCheck = trianglesToCheckIds.back();   // ritorna l'ultimo elemento del vettore
                        trianglesToCheckIds.pop_back();    // rimuove l'ultimo elemento di trianglesToCheck e riduce la sua dimensione di 1

                        // cerco gli id dei triangoli adiacenti a quello che sto controllando. Se Dalaunay non è verificato, eseguo il
                        // flip e sistemo la nuova mesh.
                        vector<int> adjs = GetAdjacencies(triangleToCheck);
                        for (unsigned int k = 0; k < adjs.size(); k++)
                        {
                            int adj = adjs[k];
                            int flippedT1 = -1;
                            int flippedT2 = -1;
                            bool flipped = DelaunayCondition(triangleToCheck, adj, flippedT1, flippedT2);
                            if (flipped)
                            {
                                // Se entro nell'if, flippedT1 e flippedT2 non valgono -1, ma contengono gli id dei due nuovi triangoli creati da DelaunayCondition
                                trianglesToCheckIds.push_back(flippedT1);
                                trianglesToCheckIds.push_back(flippedT2);
                                break;
                            }
                        }
                    }

                    // Per i punti di bordo
                    if (!alreadyEntered && pointInTriangle == 2)
                    {
                        // Se il punto era di bordo per un triangolo, c'è la possibilità che lo sia anche per un altro (a meno che non faccia
                        // parte della pseudoricopertura convessa) --> continuo il ciclo for sui triangoli per concludere il suo inserimento
                        // il punto è condiviso da al massimo due triangoli --> entrato una volta in questo if, non ha più senso rientrarci

                        alreadyEntered = true;
                        continue;
                    }

                    break;
                }
            }
        }
    }


    void Mesh::DeactivateFakeTriangles()
    {        
        // Disattivo i lati che si collegano a vertici fittizi
        for( unsigned int i = 0; i < edges.size(); i++ )
        {
            Edge edge = edges[i];

            // I punti fittizi sono creati dopo la mesh, quindi hanno id superiore a numberActualPoints
            if( edge.active == true &&
                ( points[edge.idOrigin].actualPoint == false || points[edge.idEnd].actualPoint == false ) )
            {
                edges[i].active = false;

                // Se il lato era fittizio, allora anche i triangoli che contribuiva a formare lo sono --> disattivo tali triangoli
                for ( unsigned int idTriangle : edges[i].idTriangles )
                    triangles[idTriangle].active = false;
            }
        }
    }


    void Mesh::ExportEdges(const vector<Edge>& edges, const string& outputFileName)
    {
        ofstream file(outputFileName);  // crea il file se non esiste, lo sovrascrive altrimenti.

        if (file.fail())
        {
            cerr << "Impossibile aprire il file " << outputFileName << endl;
        }

        file << "Id xOrigin yOrigin xEnd yEnd" << endl;
        for (const Edge& edge : edges)
        {
            if ( edge.active )
                file << edge.id << " " << points[edge.idOrigin].x << " " << points[edge.idOrigin].y << " " <<  points[edge.idEnd].x <<
                        " " << points[edge.idEnd].y << endl;
        }

        file.close();
    }

    /// Funzioni esterne alle struct costruite

    double Distance(const Point& p1, const Point& p2)
    {
        double dx = p2.x - p1.x;
        double dy = p2.y - p1.y;
        return sqrt(dx * dx + dy * dy);
    }


    double CalculateAngle(const Point& p1,  const Point& p2, const Point& p3)
    {
        double d1 = Distance(p2, p3);
        double d2 = Distance(p1, p3);
        double d3 = Distance(p1, p2);
        return acos((d2 * d2 + d3 * d3 - d1 * d1) / (2 * d2 * d3));
    }


}
