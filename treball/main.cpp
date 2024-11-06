#include <chrono>
#include <iostream>
#include <string>
using namespace std;
using namespace std::chrono;

const int N_ALGORITMES = 4;
const int N_MIDES = 5;
const int N_SEEDS = 3;
const int N_ITER_GOOD = 10;
const int MIDES[N_MIDES] = {500, 5000, 10000, 50000, 100000};
const int SEEDS[N_SEEDS] = {21, 1, 2002};
const string ALGORITHMS[N_ALGORITMES] = {"BubbleSort", "SelectionSort",
                                         "QuickSort", "MergeSort"
                                        };
const string CREATE_FUNTIONS[3] = {"Random => Seed: ", "Good", "Bad"};

// Algoritmes

void bubbleSort(int arr[], int size) {
	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				// Intercambiar los elementos
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
		}
		else {
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

// Crides a funcions amb nomes arr[] i size
void qSort(int arr[], int size) {
	quickSort(arr, 0, size - 1);
}

void mSort(int arr[], int size) {
	mergeSort(arr, 0, size - 1);
}

// Array de les funcions
void (*callbacks[N_ALGORITMES])(int arr[], int size) = {
	bubbleSort, selectionSort, qSort, mSort
};

// Metodes per l'array
void printArray(int arr[], int size) {
	cerr << "Mida: " << size << "\nArray:";

	for (int i = 0; i < size; i++)
		cerr << arr[i] << " ";

	cerr << endl;
}

int *createArrayRand(int size) {
	int *array = new int[size];

	for (int i = 0; i < size; i++)
		array[i] = rand() % size;

	return array;
}

void intercambiar_random(int arr[], int size) {
	int a = rand() % size;
	int b = rand() % size;
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

// Tests

bool testSorted(int arr[], int size) {
	for (int i = 1; i < size; i++)
		if (arr[i - 1] > arr[i])
			return false;

	return true;
}

void test_algorithm(void (*func[])(int arr[], int size),
                    int *(*createFunc)(int size), int createFunction,
                    int **durations, int i, int j, int s) {
	int size = MIDES[i];
	int *arr = createFunc(size);
	// cout << arr[1] << endl;
	// printArray(arr, size);
	cout << "Before: " << (testSorted(arr, size) ? "passed" : "failed") << endl;
	auto start = high_resolution_clock::now();
	callbacks[j](arr, size);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	auto milisegons = duration_cast<milliseconds>(stop - start);
	auto segons = duration_cast<seconds>(stop - start);
	cout << "After: " << (testSorted(arr, size) ? "passed" : "failed")
	     << "\nAlgorithm: " << ALGORITHMS[j] << "\nSize: " << size
	     << "\nArray Creation Method: " << CREATE_FUNTIONS[createFunction]
	     << ((createFunction == 0) ? to_string(SEEDS[s]) : "") << endl;
	double time = duration.count();
	cout << "Execution Time:  " << endl;
	cout << "\tIn Seconds: " << segons.count() << endl
	     << "\tIn Milliseconds: " << milisegons.count() << endl
	     << "\tIn Microseconds: " << duration.count() << endl
	     << endl;
	durations[j][i] = time;
	delete[] arr;
}

int main() {
	for (int s = 0; s < N_SEEDS; s++) {
		srand(SEEDS[s]);
		int **durations = new int *[N_ALGORITMES];

		for (int i = 0; i < N_ALGORITMES; i++)
			durations[i] = new int[N_MIDES];

		for (int i = 0; i < N_MIDES; i++) {
			for (int j = 0; j < N_ALGORITMES; j++)
				test_algorithm(callbacks, createArrayRand, 0, durations, i, j, s);
		}
	}

	// // Creacio de les matriu que conte totes les arrays
	// int **baseMatrix = new int *[N_ALGORITMES];
	//
	// for (int  i = 0; i < N_MIDES; i++)
	// baseMatrix[i] = createArray(MIDES[i]);
	//
	// cerr << "Base Arrays: ";
	// printArrays(baseMatrix);
	// // Agrupant les matrius en una sola
	// int ***a llMatrixes = new int **[N_ALGORITMES];
	//
	// for (int i = 0; i < N_ALGORITMES; i++)
	// allMatrixes[i] = copyArrays(baseMatrix);
	//
	// // Matriu per als temps d'execucio
	// double durations[N_ALGORITMES][N_MIDES];
	//
	// // Executant els algoritmes
	// for (int i = 0; i < N_ALGORITMES; i++) {
	// for (int j = 0; j < N_MIDES; j++) {
	// auto start = high_resolution_clock::now();
	// callbacks[i](allMatrixes[i][j], MIDES[j]);
	// auto stop = high_resolution_clock::now();
	// auto duration = duration_cast<microseconds>(stop - start);
	// durations[i][j] = duration.count();
	// }
	// }
	//
	// for (int i = 0; i < N_ALGORITMES; i++) {
	// for (int j = 0; j < N_MIDES; j++)
	// delete[] allMatrixes[i][j];
	//
	// delete[] allMatrixes[i];
	// }
	//
	// delete[] allMatrixes;
	return 0;
}
