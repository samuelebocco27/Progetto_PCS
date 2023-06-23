# Namespace DelaunayTriangle

Il namespace `DelaunayTriangle` contiene le seguenti classi:

## Struct `Point`

```cpp
struct Point
{
    int id;
    double x, y;
    bool actualPoint = true;  // false se il punto è fittizio, true altrimenti

    static constexpr double geometricTol = 1.0e-12;
    static constexpr double geometricTol_Squared = max_tolerance(Point::geometricTol * Point::geometricTol,
                                                                 numeric_limits<double>::epsilon());
};
```

La struttura `Point` rappresenta un punto nel piano cartesiano. Ha i seguenti membri:

- `id`: l'identificatore univoco del punto.
- `x`: la coordinata x del punto.
- `y`: la coordinata y del punto.
- `actualPoint`: un flag che indica se il punto è un punto reale (true) o fittizio (false).
- `geometricTol`: una costante che rappresenta la tolleranza geometrica utilizzata per confrontare valori double.
- `geometricTol_Squared`: il quadrato della tolleranza geometrica, calcolato come il massimo tra `geometricTol * geometricTol` e `numeric_limits<double>::epsilon()`.

## Struct `Edge`

```cpp
struct Edge
{
    int id;
    int idOrigin, idEnd;
    vector<int> idTriangles;
    bool active = false;

    Edge() = default;

    Edge(int& idPoint1, int& idPoint2);
};
```

La struttura `Edge` rappresenta un lato non orientato. Ha i seguenti membri:

- `id`: l'identificatore univoco del lato.
- `idOrigin`: l'identificatore del punto di origine del lato.
- `idEnd`: l'identificatore del punto di arrivo del lato.
- `idTriangles`: un vettore contenente gli identificatori dei triangoli che il lato contribuisce a comporre.
- `active`: un flag che indica se il lato è attivo (true) o inattivo (false).

### Costruttore `Edge`

```cpp
Edge::Edge(int& idPoint1, int& idPoint2)
```
**Input:**

-  `int& idPoint1`, `int& idPoint2`: gli ID dei punti che costituiscono l'edge.

**Descrizione:**
Il costruttore `Edge` crea un oggetto `Edge` con i punti di origine e arrivo specificati dagli identificatori `idPoint1` e `idPoint2`.


## Struct Triangle

La classe `Triangle` rappresenta un triangolo nella struttura dati `Mesh`.

### Costruttore `Triangle(int a, int b, int c, Mesh* mesh)`

```cpp
Triangle(int a, int b, int c, Mesh* mesh)
```
**Input:**
-  `int a`, `int b`, `int c`: gli ID dei punti che costituiscono il triangolo.
- `Mesh* mesh`: un puntatore all'oggetto `Mesh` che contiene i punti.

**Descrizione:**
Costruisce un oggetto `Triangle` con i vertici specificati dagli ID `a`, `b` e `c`. Il parametro `mesh` rappresenta la mesh a cui il triangolo appartiene.

#### Metodo `OrderTrianglePoints(array<int, 3>& vertices, const Mesh& mesh)`

```cpp
void OrderTrianglePoints(array<int, 3>& vertices, const Mesh& mesh)
```
**Input:**
- Input: `array<int ,3>& vertices` : array di ID dei vertici del triangolo
- `const Mesh& mesh`: un riferimento costante all'oggetto `Mesh` che contiene i punti.

**Descrizione:**
La funzione `OrderTrianglePoints` ordina i vertici del triangolo in senso antiorario. Questo metodo utilizza il *prodotto vettoriale* per determinare l'orientamento dei vertici. I parametri `vertices` rappresentano gli ID dei vertici del triangolo e `mesh` rappresenta la mesh a cui il triangolo appartiene.

### Metodo `Area(const Mesh* mesh) const`

```cpp
double Area(const Mesh* mesh) const
```
**Input:**
- `const Mesh* mesh`: un puntatore costante all'oggetto `Mesh` che contiene i punti.
**Output:**
- `double`: l'area del triangolo.

**Descrizione:**
Calcola l'area del triangolo utilizzando il prodotto vettoriale.

### Metodo `CircoContainsPoint(const int idPoint, const Mesh& mesh) const`

```cpp
bool CircoContainsPoint(const int idPoint, const Mesh& mesh) const
```
**Input:**
- Input: `const int idPoint` : ID del punto da verificare
-  `const Mesh& mesh` : un riferimento costante all'oggetto `Mesh` che contiene i punti.

**Output:**
-  `bool`: `true` se il punto è interno alla circonferenza, `false` altrimenti.

**Descrizione:**
Determina se il punto specificato dall'ID `idPoint` è interno o esterno alla circonferenza circoscritta al triangolo. 

### Metodo `CircoContainsPoint(const double x, const double y, const Mesh& mesh) const`

```cpp
bool CircoContainsPoint(const double x, const double y, const Mesh& mesh) const
```
**Input:**
- `const double x`, `const double y`: coordinate del punto da verificare.
-  `const Mesh& mesh`: un riferimento costante all'oggetto `Mesh` che contiene i punti.

**Output:**
- `bool`: `true` se il punto è interno alla circonferenza, `false` altrimenti.

**Descrizione:**
Determina se il punto specificato dalle coordinate `(x, y)` è interno o esterno alla circonferenza circoscritta al triangolo.

### Metodo `TriangleContainsPoint(const int idPoint, const Mesh& mesh) const`

```cpp
int TriangleContainsPoint(const int idPoint, const Mesh& mesh) const
```
**Input:**
-  `const int idPoint` : ID del punto da verificare
-  `const Mesh& mesh` : un riferimento costante all'oggetto `Mesh` che contiene i punti.

**Output:**
- `int`: `0` se il punto è esterno, `1` se è interno, `2` se è di bordo.

**Descrizione:**
Determina se il punto specificato dall'ID `idPoint` è interno, di bordo o esterno al triangolo.

### Metodo `TriangleContainsPoint(const double x, const double y, const Mesh& mesh) const`

```cpp
int TriangleContainsPoint(const double x, const double y, const Mesh& mesh) const
```
**Input:**
- `const double x`, `const double y`: coordinate del punto da verificare.
-  `const Mesh& mesh`: un riferimento costante all'oggetto `Mesh` che contiene i punti.

**Output:**
- `int`: `0` se il punto è esterno, `1` se è interno, `2` se è di bordo.

**Descrizione:**
Determina se il punto specificato dalle coordinate `(x, y)` è interno, di bordo o esterno al triangolo. Restituisce `0` se il punto è esterno, `1` se è interno o `2` se è di bordo. Il parametro `mesh` rappresenta la mesh a cui il triangolo appartiene.

## Struct `Mesh`

```cpp
struct Mesh
{
        vector<Point> points;
        vector<Edge> edges;
        vector<Triangle> triangles;
        
	    bool ImportPoints(const string& inputFilePath);
	    int AddTriangle(Triangle& t);
	    void AddEdge(Edge& e);
};
```

La classe `Mesh` rappresenta una mesh geometrica composta da punti, triangoli e lati. Ha i seguenti membri:

- `points`: un vettore contenente i punti della mesh.
- `triangles`: un vettore contenente i triangoli della mesh.
- `edges`: un vettore contenente i lati della mesh.

### Metodo `Mesh::ImportPoints`

```cpp
bool Mesh::ImportPoints(const string& inputFilePath)
```

**Input:**
- `inputFilePath`: una stringa rappresentante il percorso del file contenente i punti della mesh.

**Output:**
- `bool`: valore booleano che indica se l'importazione ha avuto successo o meno.

**Descrizione:**
La funzione  `ImportPoints` apre il file specificato e legge i punti in esso contenuti. Ogni riga del file rappresenta un punto e contiene l'identificatore del punto, la coordinata x e la coordinata y. I punti vengono aggiunti al vettore `points` della mesh. Se l'apertura del file fallisce, viene restituito `false`. Altrimenti, viene restituito `true`, dopo aver chiuso il file.

### Metodo `Mesh::AddTriangle`

```cpp
int Mesh::AddTriangle(Triangle& t)
```
**Input:**
- `t`: un riferimento a un oggetto `Triangle` che rappresenta il triangolo da aggiungere alla mesh.

**Output:**
- `int`: l'identificatore assegnato al triangolo aggiunto.

**Descrizione:**
La funzione assegna un identificatore univoco al triangolo specificato e lo aggiunge al vettore `triangles` della mesh. Inoltre, per ciascuno dei tre lati del triangolo, verifica se il lato esiste già nella mesh o se deve essere creato. Se il lato esiste, aggiunge l'identificatore del triangolo corrente al vettore `idTriangles` del lato e assegna l'identificatore del lato al vettore `idEdges` del triangolo aggiunto. Se il lato non esiste, crea un nuovo oggetto `Edge`, aggiunge l'identificatore del triangolo corrente al vettore `idTriangles` del nuovo lato, lo aggiunge al vettore `edges` della mesh, e assegna l'identificatore del nuovo lato al vettore `idEdges` del triangolo aggiunto. Infine, imposta il flag `active` del triangolo a `true` per indicare che è attivo. La funzione restituisce l'identificatore assegnato al triangolo.

### Metodo `Mesh::AddEdge`

```cpp
void Mesh::AddEdge(Edge& e)
```
**Input:**
- `e`: un riferimento a un oggetto `Edge` che rappresenta il lato da aggiungere alla mesh.

**Descrizione:**
La funzione `AddEdge` assegna un identificatore univoco al lato specificato e lo aggiunge al vettore `edges` della mesh. Il flag `active` del lato viene impostato a `true` per indicare che è attivo.


### Metodo `FakeTriangleCover()`

```cpp
Triangle TriangleMesh::FakeTriangleCover()
```

**Descrizione:**
Questo metodo restituisce un triangolo che "copre" l'intero set di punti. Il triangolo coprente è ottenuto attraverso un procedimento che coinvolge il calcolo delle coordinate massime e minime dei punti del dataset.


**Output:**
- `Triangle`: il triangolo coprente.

**Descrizione:**
Una volta individuati i valori massimi e minimi delle coordinate x e y dei punti del dataset, vengono calcolati i vertici del triangolo fittizio nel seguente modo:
   - Vertice 1: Coordinata minima di x e coordinata minima di y.
   - Vertice 2: Coordinata minima di x e il doppio della coordinata massima di y meno la coordinata minima di y.
   - Vertice 3: Il doppio della coordinata massima di x meno la coordinata minima di x e coordinata minima di y.
Viene quindi verificato se i punti del triangolo coprente corrispondono o meno a punti reali del dataset. Se uno o più vertici del triangolo coprente sono fittizi, vengono creati nuovi punti corrispondenti ai vertici mancanti e vengono aggiunti al vector `points` della `mesh`.
Infine si crea il triangolo fittizio, utilizzando gli id dei vertici (fittizi o meno) che formano tale triangolo.

### Metodo `GetAdjacencies(const int& idSource)`

```cpp
vector<int> TriangleMesh::GetAdjacencies(const int& idSource)
```

**Descrizione:**
Questo metodo restituisce gli id dei triangoli adiacenti a un triangolo specificato dalla sua id nella mesh.

**Input:**
- `const int& idSource`: l'id del triangolo di cui si vogliono ottenere gli adiacenti.

**Output:**
- `vector<int>`: un vettore contenente gli id dei triangoli adiacenti al triangolo di origine.

**Descrizione dettagliata:**
1. Viene inizializzato un vettore vuoto `adjacentIds` che conterrà gli id dei triangoli adiacenti.
2. Per ogni lato del triangolo di origine (sono presenti 3 lati), viene eseguito il seguente ciclo:
   - Viene ottenuto l'id dell'edge corrispondente al lato.
   - Per ogni triangolo che condivide l'edge, viene eseguito il seguente ciclo:
     - Viene ottenuto l'id del triangolo.
     - Se l'id del triangolo è uguale all'id del triangolo di origine, viene saltato al prossimo ciclo.
     - Se il triangolo non è attivo, viene saltato al prossimo ciclo.
     - Se sono stati superati i controlli precedenti, l'id del triangolo viene aggiunto a `adjacentIds`.
3. Alla fine del ciclo, il vettore `

adjacentIds` contenente gli id dei triangoli adiacenti viene restituito come output.

