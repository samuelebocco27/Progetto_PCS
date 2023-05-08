# Progetto Delaunay

Dato un set di punti $P \in \mathbb{R^2}$ trovare la triangolazione che rispetti l'ipotesi di Delaunay, ovvero tale che nessun punto appartenente a P sia interno al circumcerchio di un triangolo della triangolazione.

<p style="text-align:center;"><img  src="./Images/delaunay.jpg"  width="60%" height="30%"></p>

## Algoritmo

Dato il set di punti P:

* estrarre tre punti non collineari e creare il primo triangolo;
* per ogni nuovo punto Q:
  * se Q è interno alla triangolazione:
    * localizzare il triangolo T contenente Q;
    * unire il punto Q con i vertici del triangolo T per creare una sottotriangolazione;
  * se Q è esterno alla triangolazione:
    * unire Q con i due vertici più vicini della triangolazione;
  * verificare l'ipotesi di Delaunay sulla nuova triangolazione creata. Tale ipotesi risulta verificata se, dati due triangoli adiacenti ABC e BDC, gli angoli opposti al lato di adiacenza BC sommano meno di $180$°. Se questo non accade, eliminando il lato BC e aggiungendo il nuovo lato AD, si producono due nuovi triangoli ADC e ABD che soddisfano l'ipotesi di Delaunay (operazione di "flip");
  
<p style="text-align:center;"><img src="./Images/flip.png"  width="30%" height="30%"></p>

  
## Suggerimenti

* Per ogni triangolo creato è opportuno ordinare i suoi vertici in senso antiorario;
* Durante la creazione della mesh triangolare servirà tenere traccia della adiacenza dei triangoli per potere verificare l'ipotesi di Delaunay;
* Per ricercare i due punti più vicini, nel caso di punto esterno alla triangolazione, occorre utilizzare una struttura dati scelta tra quelle viste durante l'anno, che permetta la ricerca degli elementi più vicini in maniera rapida;
* Per escludere in modo rapido se un punto è interno ad un triangolo si può prima verificare se tale punto risulta essere esterno al quadrato o al cerchio che circoscrive quel triangolo.
* Un metodo per identificare se un punto D cade all'interno della circonferenza circoscritta ad un triangolo $T_{ABC}$ è quello di valutare se: 
<p style="text-align:center;"><img src="./Images/determinant.png"  width="80%" height="30%"></p>
Tale metodo richiede che i vertici siano ordinati in senso antiorario.

Per altri suggerimenti è possibile consultare la pagina: 
https://en.wikipedia.org/wiki/Delaunay_triangulation
