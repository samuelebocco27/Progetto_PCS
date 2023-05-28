#include "empty_class.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace DelaunayTriangle;

/// \brief Reads the points contained in Points.csv
/// \param filePath: path name of the input file
/// \param points: coordinates of mesh points
/// \return the result of the reading, true is success, false is error
bool ImportPoints(const string& inputFilePath,
                  vector<Point>& points);

int main()
{
    // Lettura punti
    vector<Point> points;
    string inputFilePath = "../Delaunay/Dataset/Points.csv";
    ImportPoints(inputFilePath, points);

    // Inizializzo il vettore che dovrà contenere tutti i triangoli della triangolazione
    vector<Triangle> triangulation;

    // Trovo il triangolo di area massima. Lo inserisco tra i vettori della triangolazione
    Triangle maxTriangle = GetMaxAreaTriangle(points, 0, points.size());
    triangulation.push_back(maxTriangle);

    // Costruisco la pseudo ricopertura convessa


    // Inserisco nella triangolazione i punti interni

    return 0;
}


bool ImportPoints (const string& inputFilePath,
                   vector<Point>& points)
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
