
# ***REI*** */ Recorregut d'Estudi i Investigació*

<div style="text-align: right"><sup> Grup: [NULL_PTR*] Alex, Jordi, Marcel, Natan i Paul </sup></div>

GE*INF* | ESTADISTICA | ***Dels algorismes d'ordenacio, quin es el millor?***

En aquest treball farem un estudi estadistic dels diferents algorismes d'ordenacio, per determinar quin es el mes ***rapid***. La respostra primaria es el ***temps [ms]*** que empara cada algorisme per odenar el vecror.

> ## Index

> - [Explicació del conjunt de dades](#explicació-del-conjunt-de-dades)
> - [Variables](#variables)
>   - Algorismes
>   - Mides del vector
>   - Nivell d'ordenació
> - [Resultats](#resultats)
>   - [Seed: 21](#seed-21)
>   - [Seed: 1](#seed-1)
>   - [Seed: 2002](#seed-2002)
>   - [Favorable Case:](#favorable-case)
>   - [Unfavorable Case:](#unfavorable-case)

## ***Explicació del conjunt de dades***

> Les dades que utilizarem per fer aquest estudi estadistic seran les seguents:

- Tenim un Vector [ *int* ]:

  - Pot tenir diverses ***mides***.
  
  - Difera el nivell d'***ordenació*** dels seus elements.
  
- S'apliquen diversos ***algorismes*** d'ordenacio.

>Donades les variables anteriors obtindrem diferents mesures de ***temps***.

## ***Explicació del Codi*** 

> Una petita explicació de les parts del codi

A la capçalera del programa tenim diverses constants que podem modificar.
Les mes destacables son:

 - **N_MIDES:** Nombre de mides de les que volem agafar mostres
 - **MIDES:** Llista de les mides
 - **N_SEEDS:** Nombre de llavors diferents que volem fer servir per al testeig
 - **SEEDS:** Llista de les llavors

Seguidament tenim definits els 4 algoritmes de ordenació:

 - **BubbleSort** ``bubbleSort()``
 - **SelectionSort** ``selectionSort()`` 
 - **QuickSort** ``quickSort(), partition(), qSort()`` 
 - **MergeSort** ``MergeSort(), merge(), mSort()``

``qSort()`` i ``mSort()`` són dos mètodes adicionals que son necessaris per a una tècnica utilitzada posteriorment

A continuació tenim una línia important en el codi.

```cpp
void (*callbacks[N_ALGORITMES])(int arr[],int size) = {bubbleSort,selectionSort,qSort,mSort};
```

aquesta linia defineix la taula **callbacks** com a vector de punters a funcions, cosa que permet guardar crides a funcions en un vector, les qual les podem cridar seqüencialment amb un bucle:

```cpp
for(int i = 0; i < N_ALGORITMES; i++){
  callbacks[i](arr, size);
}
```

on array és una taula de ints qualsevol i size és la mida de la taula.
D'aquesta manera s'executa les 4 funcions per la mateixa taula.

Una de les condicions per a poder crear aquesta array de punters, és que les funcions contingudes tinguin els mateixos paràmetres (els paràmetres amb valors predeterminats poden ser ignorats).

----------------------------------------------------------------------
Si seguim llegint el codi, podem veure una altra part important del codi, les funcions que creen els vectors desordenats. En aquest cas en tenim de 3 tipus:

- **Aleatòria:** Els vectors estan creats de manera aleatoria ``createArrayRand()``
- **Favorable:** Els vectors estan creats gairebé ordenats ``createArrayGood(), intercambiar_random()``
- **Desfavorable:** Els vectors estan creats en order invers ``createArrayBad()``

Tots els nombres aleatoris són generats per la funció:

```csharp
int aleatori(int valor) {
  // Pre: --; Post: retorna un valor aleatori entre 0 i valor-1
  LLAVOR = LLAVOR * 1103515245 + 12345;
  return ((LLAVOR / 32) % 32768) % valor;
}
```
**LLAVOR** és una variable global. Aquesta funció ha sigut proporcionada en un exercici en MTP1 del 1r curs de GEINF. És una funció que genera valors pseudo-aleatoris a partir d'una llavor. Aquesta funció ha sigut necessària en el moment de cridar el programa des d'R, ja que per raons desconegudes, la funció ``srand()`` que permet assignar una llavor a la funció generadora de valors aleatoris ``rand()`` d'*stdlib* no funcionava correctament.

> Per a facilitar la comprovació que els algoritmes s'han executat satisfactoriament, fou necessari implementar la funció ``testSorted()``, que donada una array verifica si està ordenada. 

----
Finalment, les 3 funcions més importants del programa:

 - **test_algorithm(...)**
 - **createTables()**
- **createRTables(...)**

#### ***test_algorithm(...)***

```csharp
bool test_algorithm(void (*func[])(int arr[], int size), 
                    int *(*createFunc)(int size), 
                    int createFunction, 
                    int **durations, 
                    int i, 
                    int j, 
                    int s = 0) {
```

Aquesta funció té com a objectiu executar un algoritme. Els paràmtres més importants són

 - ``void (*func[])(int arr[], int size)`` Representa un vector de punters a funcions, en el nostre cas serà *callbacks*.
 - ``int *(*createFunc)(int size)`` Representa un punter a la funció que crea el vector per ordenar, com per exemple *``createArrayRand()``*.
 - ``int **durations`` Representa el vector on guardem els temps d'execució d'aquest algoritme.

 > Altres paràmetres: createFunction, i, j, s són paràmetres utilitzats per a la sortida en consola, no s'utilitzen en la crida a R. 

 test_algorithm agafa les funcions *func[]* i les executa sobre vectors creats amb la funció *createFunction*. escollim de *func[]* amb el paràmetre *j*, i escollim la mida d'entre la constant global *MIDES[]* amb el paràmetre *i* i la passem com a paràmetre a la funció de creació.
 Important mencionar que s'utilitza la llibreria ``<chrono>`` per a mesurar el temps. El temps es mesura just en cridar la funció amb ``callbacks[j](arr, size)`` de la següent manera:

 ```cpp
  auto start = high_resolution_clock::now();
  callbacks[j](arr, size);
  auto stop = high_resolution_clock::now();
 ```

al final, s'opera amb les variables *start* i *stop*, i el resultat s'emmagatzema en ``durations[j][i]``. Finalment la funció esborra el vector creat.

> És important esborrar de memòria el vector, ja que s'estan emmagatzemant molts elements. En versions prèvies d'aquest codi (versions estàtiques) que pretendia crear tots els vectors i després utilitzar-los, el programa llançava una excepció sobre la falta de memòria.

Aquest mètode retorna un booleà que diu si el vector s'ha ordenat.

####  ***createRTables(...)***

Aquest mètode s'ocupa de crear el DataFrame per a l'R. Utilitzant la llibreria ``<Rcpp.h>`` incorporada de manera automàtica en crear un script de c++ en RStudio, es pot aconseguir utilitzar diferents variables i mètodes propis de l'R. Les capacitats més importants són el fet de declarar i utilitzar mètodes dels objectes **DataFrame, CharacterVector, IntegerVector, NumericVector** que representen una taula, un vector de string, un vector de enters i un vector de reals respectivament. D'aquesta manera en cridar el un dels mètodes en R, es pot retornar dades compatibles amb R. La capçalera del mètode és aquesta:

```csharp
DataFrame createRTables(int ***durations)
```

durations és un vector en 3 dimensions que guarda els temps d'execució. Tot i així, és més fàcil entendre com una estructura de dades que rep com a paràmetres enters que representen la **llavor**, l'**algoritme** i la **mida respectivament** 

> durations[2][3][2] faria referència al temps d'execució de la llavor en posició 2, per tant la 3a (perquè els vectors comencen del 0), l´algoritme en posició 3 i la mida en posició 2. En el nostre cas, aquest temps pertany a la llavor *2002*, l'algoritme *MergeSort* i la mida *10.000*.

El mètode és molt senzill. Simplement recorre aquesta taula *duracions* i es guarda en vectors importats de ``<Rcpp.h>`` a fi d'executar un ``DataFrame::create()`` amb aquesta forma:

```cpp
DataFrame::create( Named("Temps (milisegons)") = durades,
                   Named("Seed (0 favorable, -1 desfavorable)") = seeds,
                   Named("Algoritme") = algoritmes,
                   Named("Mida") = mides);
```


#### ***DataFrame createTables()***

Finalment, aquesta és la funció que s'executa a l'script d'R. Per a poder aconseguir això, cal afegir un comentari: ``// [[Rcpp::export]]``. Aquest tros de "codi" comunica al script d'R que la funció declarada a continuació s'ha de poder cridar al script d'R, i exporta la funció.

Aquesta funció no té cap mena de complicació, simplement s'utilitzen diversos bucles per a seleccionar de les constants de la capçalera la *llavor*, l'*algoritme* i la *mida* que es comunica a la funció ``test_algorithm()``.
Al final, aquesta invoca la funció ``createRTables(durationsAll)`` on *durationsAll* és el vector tridimensional on hem guardat tots els temps d'execució mesurats i retorna el **DataFrame** resultant.

> És important destacar que en entrar al primer bucle (el que selecciona la llavor) la primera instrucció és ``LLAVOR = SEEDS[s];``. Aquesta actualitza la variable global LLAVOR que s'utilitza per generar els nombres aleatoris en el mètode ``aleatori()``

## Variables

>Les variables que utilizarem per cada mesura de temps seran les seguents :

|*Algorismes*|*Mides del vector*|*Nivell d’ordenació*|
| -------- | ------- | ------- |
|BubbleSort|500|3 llavors aleatories|
|SelectionSort|5.000| ( 21, 1, 2002)
|QuickSort|10.000|Cas Favorable (Vector semiordenat)|
|MergeSort|50.000|Cas Desfavorable (Ordenat inversament)|
| |100.000| |

## Resultats

>Els resultats obtinguts per cada situacio son els seguents :

### Seed: 21

|Algorithm: BubbleSort||Algorithm: SelectionSort||Algorithm: QuickSort||Algorithm: MergeSort||
| -------- | ------- |-------- | ------- | -------- | ------- | -------- | ------- |
|Size|Execution Time (ms)|Size|Execution Time (ms) |Size|Execution Time (ms) |Size|Execution Time (ms) |
| 500  | 0.973| 500 | 0.728| 500 |0.099| 500 | 0.105|
| 5000 | 32.383| 5000 | 23.104| 5000 | 0.463| 5000 | 0.481|
| 10000 | 123.369| 10000 | 92.376| 10000 | 1.015| 10000 | 1.018|
| 50000 | 5030.716| 50000 | 2290.249| 50000 | 6.165| 50000 | 5.820|
| 100000 |  21624.236| 100000 | 9227.423| 100000 | 13.386| 100000 | 12.250|

### Seed: 1

|Algorithm: BubbleSort||Algorithm: SelectionSort||Algorithm: QuickSort||Algorithm: MergeSort||
| -------- | ------- |-------- | ------- | -------- | ------- | -------- | ------- |
|Size|Execution Time (ms)|Size|Execution Time (ms) |Size|Execution Time (ms) |Size|Execution Time (ms) |
| 500 | 0.354|500 | 0.269| 500 |0.044| 500 |0.040|
| 5000 | 30.397|5000 | 23.524| 5000 | 0.460| 5000 | 0.452|
| 10000 | 127.693|10000 | 91.978| 10000 | 1.070| 10000 | 1.049|
| 50000 | 5254.236|50000 | 2313.786| 50000 | 6.947| 50000 | 5.789|
| 100000 |  22403.393|100000 | 9409.887| 100000 | 13.577| 100000 |12.599|

### Seed: 2002

|Algorithm: BubbleSort||Algorithm: SelectionSort||Algorithm: QuickSort||Algorithm: MergeSort||
| -------- | ------- |-------- | ------- | -------- | ------- | -------- | ------- |
|Size|Execution Time (ms)|Size|Execution Time (ms) |Size|Execution Time (ms) |Size|Execution Time (ms) |
| 500 | 0.348| 500 | 0.268| 500 |0.036| 500 |0.041|
| 5000 | 30.511| 5000 | 24.097| 5000 | 0.511| 5000 | 0.480|
| 10000 | 129.677| 10000 | 97.088| 10000 | 1.102| 10000 | 1.085|
| 50000 | 5485.316| 50000 | 2352.977| 50000 | 6.448| 50000 | 6.033|
| 100000 |  22775.931| 100000 | 9509.118| 100000 | 13.827| 100000 | 12.390|

### Favorable Case:

|Algorithm: BubbleSort||Algorithm: SelectionSort||Algorithm: QuickSort||Algorithm: MergeSort||
| -------- | ------- |-------- | ------- | -------- | ------- | -------- | ------- |
|Size|Execution Time (ms)|Size|Execution Time (ms) |Size|Execution Time (ms) |Size|Execution Time (ms) |
| 500 | 0.236| 500 | 0.262| 500 |0.097| 500 |0.027|
| 5000 | 21.623| 5000 | 23.753| 5000 | 9.170| 5000 | 0.293|
| 10000 | 85.734| 10000 | 95.211| 10000 | 33.903| 10000 | 0.641|
| 50000 | 2166.049| 50000 | 2338.841| 50000 | 814.487| 50000 | 3.519|
| 100000 | 8717.117| 100000 | 9510.341| 100000 | 4250.628| 100000 | 8.406|

### Unfavorable Case:

|Algorithm: BubbleSort||Algorithm: SelectionSort||Algorithm: QuickSort||Algorithm: MergeSort||
| -------- | ------- |-------- | ------- | -------- | ------- | -------- | ------- |
|Size|Execution Time (ms)|Size|Execution Time (ms) |Size|Execution Time (ms) |Size|Execution Time (ms) |
| 500 | 0.435| 500 | 0.280| 500 | 0.572| 500 |0.025|
| 5000 | 42.398| 5000 | 24.719| 5000 | 52.369| 5000 | 0.304|
| 10000 | 160.382| 10000 | 91.025| 10000 | 194.086| 10000 | 0.598|
| 50000 | 3762.648| 50000 | 2126.765| 50000 | 4782.342| 50000 | 3.476|
| 100000 |  14991.956| 100000 | 8492.889| 100000 |  19363.119| 100000 | 7.272|


