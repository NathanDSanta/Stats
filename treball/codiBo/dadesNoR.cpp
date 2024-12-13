#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

unsigned LLAVOR = 2002;

const double PERCENT_ITERACIONS_COEFICIENT = 0.5;

const int N_VECTORS = 40;
const int N_MIDES = 1;
const int N_ALGORITMES = 2;
const int N_EXPERIMENTS = 200;
const int ITERACIONS_BASE = 100; 

const int MIDES[N_MIDES] = {1000};

const string ALGORITMES[N_ALGORITMES] = {"QuickSort", "MergeSort"};

int aleatori(int valor) {
  LLAVOR = LLAVOR * 1103515245 + 12345;
  return ((LLAVOR / 32) % 32768) % valor;
}

int partition(int arr[], int low, int high) {
  int pivot = arr[high];
  int i = low - 1;

  for (int j = low; j < high; j++) {
    if (arr[j] <= pivot) {
      i++;
      swap(arr[i], arr[j]);
    }
  }

  swap(arr[i + 1], arr[high]);
  return i + 1;
}

void quickSort(int arr[], int low, int high) {
  if (low < high) {
    int pi = partition(arr, low, high);
    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
  }
}

void merge(int arr[], int left, int mid, int right) {
  int i, j, k;
  int n1 = mid - left + 1;
  int n2 = right - mid;
  int L[n1], R[n2];

  for (i = 0; i < n1; i++) L[i] = arr[left + i];

  for (j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

  i = 0;
  j = 0;
  k = left;

  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }

    k++;
  }

  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
}

void mergeSort(int arr[], int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
  }
}

void qSort(int arr[], int size) {
  quickSort(arr, 0, size - 1);
}

void mSort(int arr[], int size) {
  mergeSort(arr, 0, size - 1);
}

void (*callbacks[N_ALGORITMES])(int arr[], int size) = {qSort, mSort};

void intercambiar_random(int arr[], int size) {
  int a = aleatori(size + 1);
  int b = aleatori(size + 1);
  int temp = arr[a];
  arr[a] = arr[b];
  arr[b] = temp;
}

int *crearVector(int mida,int grauDeDesordre, bool invers = false){
  int *vector = new int[mida];

  for (int i = 0; i < mida; i++){
    if (invers) vector[i] = mida - i;
    else vector[i] = i;
  }

  for (int i = 0; i < grauDeDesordre * (2 *mida / N_VECTORS) ; i++) intercambiar_random(vector, mida);

  return vector;
}

double calcularCoeficientOrdenacio(const int arr[], int mida) {
  int n_ordenats = 0;
  for (int i = 0; i < mida * PERCENT_ITERACIONS_COEFICIENT; i++) {
    int a = aleatori(mida);
    int b = aleatori(mida);
    while (b == a) {
      b = aleatori(mida);
    }
    if (a > b && arr[a] > arr[b]) n_ordenats++;
    else if(a < b && arr[a] < arr[b]) n_ordenats++;
  }
  return (n_ordenats/(mida * PERCENT_ITERACIONS_COEFICIENT));
}

void Log(int mida, int grauDesordre, int algoritme, int experiment, int durada, double coeficient){
  cout << "Log: ";
  cout << ALGORITMES[algoritme] 
    << " Mida: " << MIDES[mida]
    << " Iteracions: " << grauDesordre * ( 2*MIDES[mida] / N_VECTORS ) 
    << " Experiment nr: " << experiment
    << " Temps: " << durada
    << " Coeficient: " << coeficient;
  cout << endl;
}

int test_algorithm(int arr[], int mida, void (*func)(int arr[], int mida)) {
  auto start = high_resolution_clock::now();
  func(arr, mida);
  auto stop = high_resolution_clock::now();
  auto microsegons = duration_cast<microseconds>(stop - start);
  int time = microsegons.count();
  return time;
}

int main (int argc, char *argv[]) { 

  // Inicialitzem vectors on guardem totes les dades
  int ****durationsAll = new int ***[N_MIDES];
  double ****coeficientAleatoritat = new double ***[N_MIDES];

  for (int i = 0; i < N_MIDES; i++) {
    durationsAll[i] = new int **[N_VECTORS];
    coeficientAleatoritat[i] = new double **[N_VECTORS];

    for (int j = 0; j < N_VECTORS; j++) {
      durationsAll[i][j] = new int *[N_ALGORITMES];
      coeficientAleatoritat[i][j] = new double *[N_ALGORITMES];

      for (int k = 0; k < N_ALGORITMES; k++) {
        durationsAll[i][j][k] = new int[N_EXPERIMENTS];
        coeficientAleatoritat[i][j][k] = new double [N_EXPERIMENTS];
      }
    }
  }

  // Executem els algoritmes per tots els casos
  for (int i = 0; i < N_MIDES; i++) {
    for (int j = 0; j < N_VECTORS; j++) {
      for (int k = 0; k < N_ALGORITMES; k++) {
        for (int l = 0; l < N_EXPERIMENTS; l++) {
          int grauDesordre = j >= N_VECTORS/2 ? j%(N_VECTORS/2) : j;
          bool invertit = j >= N_VECTORS/2;
          int *arr = crearVector(MIDES[i], grauDesordre, invertit); 
          coeficientAleatoritat[i][j][k][l] = calcularCoeficientOrdenacio(arr, MIDES[i]);
          durationsAll[i][j][k][l] = test_algorithm(arr, MIDES[i], callbacks[k]);
          Log(i,grauDesordre,k,l,durationsAll[i][j][k][l],coeficientAleatoritat[i][j][k][l]);
          delete [] arr;
        }
      }
    }
  }  
  return 0;
}
