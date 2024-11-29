 
#include <chrono>
#include <iostream>
#include <string>
using namespace std;
using namespace std::chrono;

const int N_EXPERIMENTS = 5;
const int N_ALGORITMES = 4;
const int N_MIDES = 16;
const int N_SEEDS = 3;
const int N_ITER_GOOD = 10;
const int MIDES[N_MIDES] = {50,    100,   200,   300,  400,  500,
                            1000,  2000,  3000,  4000, 5000, 10000,
                            20000, 30000, 40000, 50000};
const int SEEDS[N_SEEDS] = {21, 1, 2002};
const string ALGORITHMS[N_ALGORITMES] = {"BubbleSort", "SelectionSort",
                                         "QuickSort", "MergeSort"};
const string CREATE_FUNTIONS[3] = {"Random => Seed: ", "Good", "Bad"};

// Semilla 0 = cas favorable, Semilla -1 = cas desfavorable
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

int test_algorithm(void (*func)(int arr[], int size), int size,
                   int *(*createFunc)(int size)) {
  int *arr = createFunc(size);
  auto start = high_resolution_clock::now();
  func(arr, size);
  auto stop = high_resolution_clock::now();
  auto microsegons = duration_cast<microseconds>(stop - start);
  int time = microsegons.count();
  delete[] arr;
  return time;
}

// DataFrame createRTables(int ****durations) {
//   CharacterVector seeds;
//   CharacterVector algoritmes;
//   IntegerVector mides;
//   IntegerVector experiments;
//   IntegerVector durades;
//
//   for (int s = 0; s < 5; s++) {
//     for (int i = 0; i < N_ALGORITMES; i++) {
//       for (int j = 0; j < N_MIDES; j++) {
//         for (int k = 0; k < N_EXPERIMENTS; k++) {
//           seeds.push_back(STRING_SEEDS[s]);
//           algoritmes.push_back(ALGORITHMS[i]);
//           mides.push_back(MIDES[j]);
//           experiments.push_back(k);
//           durades.push_back(durations[s][i][j][k]);
//         }
//       }
//     }
//   }
//
//   return DataFrame::create(Named("Temps (microsegons)") = durades,
//                            Named("Seed (0 favorable, -1 desfavorable)") = seeds,
//                            Named("Algoritme") = algoritmes,
//                            Named("Mida") = mides,
//                            Named("Experiment nr. ") = experiments);
// }

// [[Rcpp::export]]
int main() {
  int ****durationsAll = new int ***[5];

  for (int s = 0; s < 5; s++) {
    durationsAll[s] = new int **[N_ALGORITMES];

    for (int i = 0; i < N_ALGORITMES; i++) {
      durationsAll[s][i] = new int *[N_MIDES];

      for (int j = 0; j < N_MIDES; j++) {
        durationsAll[s][i][j] = new int[N_EXPERIMENTS];
      }
    }
  }

  for (int s = 0; s < N_SEEDS; s++) {
    LLAVOR = SEEDS[s];
    for (int i = 0; i < N_ALGORITMES; i++) {
      for (int j = 0; j < N_MIDES; j++) {
        for (int k = 0; k < N_EXPERIMENTS; k++){
          durationsAll[s][i][j][k] =
              test_algorithm(callbacks[i], MIDES[j], createArrayRand);
        cout << "Log: " << SEEDS[s]
             << " " << ALGORITHMS[i]
             << " " << MIDES[j]
             << " " << k
             << " Temps: " << durationsAll[s][i][j][k] << endl;;
        }
      }
    }
  }

  for (int i = 0; i < N_ALGORITMES; i++) {
    for (int j = 0; j < N_MIDES; j++) {
      for (int k = 0; k < N_EXPERIMENTS; k++) {
        durationsAll[3][i][j][k] =
            test_algorithm(callbacks[i], MIDES[j], createArrayGood);
        cout << "Log: Good "
             << " " << ALGORITHMS[i]
             << " " << MIDES[j]
             << " " << k
             << " Temps: " << durationsAll[3][i][j][k] << endl;
      }
    }
  }

  for (int i = 0; i < N_ALGORITMES; i++) {
    for (int j = 0; j < N_MIDES; j++) {
      for (int k = 0; k < N_EXPERIMENTS; k++) {
        durationsAll[4][i][j][k] =
            test_algorithm(callbacks[i], MIDES[j], createArrayBad);
        cout << "Log: Bad "
             << " " << ALGORITHMS[i]
             << " " << MIDES[j]
             << " " << k
             << " Temps: " << durationsAll[4][i][j][k] << endl;
      }
    }
  }
  return 0;
}
