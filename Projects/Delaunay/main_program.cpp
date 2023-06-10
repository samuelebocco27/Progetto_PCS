#include "empty_class.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace DelaunayTriangle;



int main()
{
    Mesh mesh;  // inizializzazione mesh

    // Lettura punti
    string inputFilePath = "../Delaunay/Dataset/Points.csv";
    if(!mesh.ImportPoints(inputFilePath))
    {
        cerr << "Import Points failed!";
        return -1;
    }

    // Trovo il triangolo di area massima. Lo inserisco tra i triangoli della triangolazione
    Triangle maxTriangle = mesh.GetMaxAreaTriangle(mesh.points, 0, mesh.points.size());
    mesh.AddTriangle(maxTriangle);

    // Contrassegno i punti del triangolo di area massima come inseriti nella mesh
    for (unsigned int i = 0; i < 3; i++)
    {
        mesh.points[ mesh.triangles[0].idVertices[i] ].inserted = true;
    }

    // Costruisco la pseudo ricopertura convessa ( TO DO fare una ricopertura vera invece di questa farsa. Così abbiamo solo aggiunto il triangolo
    // con vertici (0,0), (0,1), (1,1) )
    Triangle t = Triangle(0, 2, 3, &mesh);
    mesh.AddTriangle(t);
    for (unsigned int i = 0; i < 3; i++)
    {
        mesh.points[ mesh.triangles[1].idVertices[i] ].inserted = true;
    }

    // Inserisco nella triangolazione i punti interni
    mesh.GenerateMesh();

    // Stampo per verifiche
    cout << "Numero di punti: " << mesh.points.size() << endl;
    cout << "Numero di edge: " << mesh.edges.size() << endl;
    cout << "Numero di triangoli: " << mesh.triangles.size() << endl;

    cout << "Costruzione della mesh terminata." << endl;
    /*for( size_t i = 0; i < mesh.edges.size(); i++ )
    {
        //if ( mesh.edges[i].active == true )
        cout << "Edge " << i << ": (" << mesh.points[mesh.edges[i].idOrigin].x << ", " << mesh.points[mesh.edges[i].idOrigin].y
                 << ") -> (" << mesh.points[mesh.edges[i].idEnd].x << ", " << mesh.points[mesh.edges[i].idEnd].y << ")" << "    Stato: "<<
                 mesh.edges[i].active << endl;
    }*/

    return 0;
}



