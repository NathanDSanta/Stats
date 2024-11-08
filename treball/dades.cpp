#include <Rcpp.h>
#include <chrono>
#include <iostream>
#include <string>
using namespace std;
using namespace std::chrono;
using namespace Rcpp;

const int N_ALGORITMES = 4;
const int N_MIDES = 5;
const int N_SEEDS = 3;
const int N_ITER_GOOD = 10;
const int MIDES[N_MIDES] = {500, 5000, 10000, 50000, 100000};
const int SEEDS[N_SEEDS] = {21, 1, 2002};
const string ALGORITHMS[N_ALGORITMES] = {"BubbleSort", "SelectionSort",
                                         "QuickSort", "MergeSort"};
const string CREATE_FUNTIONS[3] = {"Random => Seed: ", "Good", "Bad"};

//Semilla 0 = cas favorable, Semilla -1 = cas desfavorable
const string STRING_SEEDS[5] = {"21", "1", "2002", "0", "-1"}; 

unsigned LLAVOR;

int aleatori(int valor) {
  // Pre: --; Post: retorna un valor aleatori entre 0 i valor-1
  LLAVOR = LLAVOR * 1103515245 + 12345;
  return ((LLAVOR / 32) % 32768) % valor;
}

void bubbleSort(int arr[], int size) {
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        int temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}

void selectionSort(int arr[], int size) {
  for (int i = 0; i < size - 1; i++) {
    int minIndex = i;
    
    for (int j = i + 1; j < size; j++) {
      if (arr[j] < arr[minIndex])
        minIndex = j;
    }
    
    swap(arr[i], arr[minIndex]);
  }
}

void merge(int arr[], int left, int mid, int right) {
  int i, j, k;
  int n1 = mid - left + 1;
  int n2 = right - mid;
  int L[n1], R[n2];
  
  for (i = 0; i < n1; i++)
    L[i] = arr[left + i];
  
  for (j = 0; j < n2; j++)
    R[j] = arr[mid + 1 + j];
  
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

int partition(int arr[], int low, int high) {
  int pivot = arr[high];
  int i = low - 1;
  
  for (int j = low; j < high; j++) {
    if (arr[j] < pivot) {
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

void qSort(int arr[], int size) { quickSort(arr, 0, size - 1); }

void mSort(int arr[], int size) { mergeSort(arr, 0, size - 1); }

void (*callbacks[N_ALGORITMES])(int arr[], int size) = {
  bubbleSort, selectionSort, qSort, mSort};

int *createArrayRand(int size) {
  int *array = new int[size];
  
  for (int i = 0; i < size; i++)
    array[i] = aleatori(size + 1);
  
  return array;
}

void intercambiar_random(int arr[], int size) {
  int a = aleatori(size + 1);
  int b = aleatori(size + 1);
  int temp = arr[a];
  arr[a] = arr[b];
  arr[b] = temp;
}

int *createArrayGood(int size) {
  int *array = new int[size];
  
  for (int i = 0; i < size; i++)
    array[i] = i;
  
  for (int i = 0; i < N_ITER_GOOD; i++)
    intercambiar_random(array, size);
  
  return array;
}

int *createArrayBad(int size) {
  int *array = new int[size];
  
  for (int i = 0; i < size; i++)
    array[i] = size - i;
  
  return array;
}
bool testSorted(int arr[], int size) {
  for (int i = 1; i < size; i++)
    if (arr[i - 1] > arr[i])
      return false;
    
    return true;
}

bool test_algorithm(void (*func[])(int arr[], int size),
                    int *(*createFunc)(int size), int createFunction,
                    int **durations, int i, int j, int s = 0) {
  int size = MIDES[i];
  int *arr = createFunc(size);
  auto start = high_resolution_clock::now();
  callbacks[j](arr, size);
  auto stop = high_resolution_clock::now();
  auto microsegons = duration_cast<microseconds>(stop - start);
  bool aftertest = testSorted(arr, size);
  double time = microsegons.count();
  durations[j][i] = time;
  delete[] arr;
  return aftertest;
}

void resumen(int **duration) {
  for (int i = 0; i < N_ALGORITMES; i++) {
    for (int j = 0; j < N_MIDES; j++) {
    }
  }
}

void resumenall(int ***durationsseed, int **durationsgood, int **durationsbad) {
  for (int i = 0; i < N_SEEDS; i++)
    resumen(durationsseed[i]);
  
  resumen(durationsgood);
  resumen(durationsbad);
}

DataFrame createRTables(int ***durations) {
  CharacterVector algoritmes;
  CharacterVector seeds;
  IntegerVector mides;
  NumericVector durades;
  for (int i = 0; i < 5; i++){
    for(int j = 0; j < N_ALGORITMES; j++){
      for(int k = 0; k < N_MIDES; k++){
        seeds.push_back(STRING_SEEDS[i]);
        algoritmes.push_back(ALGORITHMS[j]);
        mides.push_back(MIDES[k]);
        durades.push_back(durations[i][j][k]/1000.0);
      }
    }
  }
  return DataFrame::create(
    Named("Temps (milisegons)") = durades,
    Named("Seed (0 favorable, -1 desfavorable)") = seeds,
    Named("Algoritme") = algoritmes,
    Named("Mida") = mides
  );
}

// [[Rcpp::export]]
DataFrame createTables() {
  bool allTestsPassed = true;
  int ***durationsAll = new int **[5];
  
  for (int s = 0; s < N_SEEDS; s++) {
    LLAVOR = SEEDS[s];
    durationsAll[s] = new int *[N_ALGORITMES];
    
    for (int i = 0; i < N_ALGORITMES; i++)
      durationsAll[s][i] = new int[N_MIDES];
    
    for (int i = 0; i < N_MIDES; i++) {
      for (int j = 0; j < N_ALGORITMES; j++) {
        bool passed = test_algorithm(callbacks, createArrayRand, 0,
                                     durationsAll[s], i, j, s);
        
        if (!passed)
          allTestsPassed = false;
      }
    }
  }
  
  for(int i = 3; i < 5; i++){
    durationsAll[i] = new int *[N_ALGORITMES];
    
    for (int j = 0; j < N_ALGORITMES; j++)
      durationsAll[i][j] = new int[N_MIDES];
    
  }
  
  for (int i = 0; i < N_MIDES; i++) {
    for (int j = 0; j < N_ALGORITMES; j++) {
      bool passed =
        test_algorithm(callbacks, createArrayGood, 0, durationsAll[3], i, j);
      
      if (!passed)
        allTestsPassed = false;
    }
  }
  
  for (int i = 0; i < N_MIDES; i++) {
    for (int j = 0; j < N_ALGORITMES; j++) {
      bool passed =
        test_algorithm(callbacks, createArrayBad, 0, durationsAll[4], i, j);
      
      if (!passed)
        allTestsPassed = false;
    }
  }
    
  return createRTables(durationsAll);
  
    return 0;
}
