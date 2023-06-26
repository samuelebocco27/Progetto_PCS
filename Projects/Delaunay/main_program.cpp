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

    // Creo un triangolo fittizio (i suoi vertici non sono necessariamente punti reali) che contenga tutti i punti della mesh.
    // Aggiungo il triangolo alla triangolazione.
    Triangle maxTriangle = mesh.FakeTriangleCover();
    mesh.AddTriangle(maxTriangle);

    // Inserisco nella triangolazione tutti i punti, che ora saranno tutti interni
    mesh.GenerateMesh();

    // Disattivo lati e triangoli costruiti con i vertici fittizi
    mesh.DeactivateFakeTriangles();

    // Esporto in un file csv gli edge attivi della mesh, in modo da poterli visualizzare
    string outputFileName = "../Delaunay/Dataset/Edges_export.csv";
    mesh.ExportEdges(mesh.edges, outputFileName);

    return 0;
}



