#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>

class ArrayGenerator {
private:
  std::mt19937 gen;

public:
  ArrayGenerator() : gen(std::random_device{}()) {}

  std::vector<int> generateRandomArray(int size, int minVal, int maxVal) {
    std::vector<int> arr(size);
    std::uniform_int_distribution<int> dist(minVal, maxVal);
    for (int i = 0; i < size; i++) {
      arr[i] = dist(gen);
    }
    return arr;
  }

  std::vector<int> generateReversedArray(int size) {
    std::vector<int> arr(size);
    for (int i = 0; i < size; i++) {
      arr[i] = size - i;
    }
    return arr;
  }

  std::vector<int> generateAlmostSortedArray(int size, int swapCount) {
    std::vector<int> arr(size);
    for (int i = 0; i < size; i++) {
      arr[i] = i + 1;
    }

    std::uniform_int_distribution<int> dist(0, size - 1);
    for (int i = 0; i < swapCount; i++) {
      int idx1 = dist(gen);
      int idx2 = dist(gen);
      std::swap(arr[idx1], arr[idx2]);
    }
    return arr;
  }
};

class Sorts {
public:
  static void insertionSort(std::vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
      int key = arr[i];
      int j = i - 1;
      while (j >= left && arr[j] > key) {
        arr[j + 1] = arr[j];
        j--;
      }
      arr[j + 1] = key;
    }
  }

  static void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
      L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
      R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
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

  static void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
  }

  static void hybridSort(std::vector<int>& arr, int left, int right, int threshold) {
    if (right - left + 1 <= threshold) {
      insertionSort(arr, left, right);
    } else {
      int mid = left + (right - left) / 2;
      hybridSort(arr, left, mid, threshold);
      hybridSort(arr, mid + 1, right, threshold);
      merge(arr, left, mid, right);
    }
  }
};

class SortTester {
public:
  static long long testSort(void (*sortFunc)(std::vector<int>&, int, int),
                            std::vector<int> arr) {
    auto start = std::chrono::high_resolution_clock::now();
    sortFunc(arr, 0, arr.size() - 1);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
  }

  static long long testHybridSort(void (*sortFunc)(std::vector<int>&, int, int, int),
                                  std::vector<int> arr, int threshold) {
    auto start = std::chrono::high_resolution_clock::now();
    sortFunc(arr, 0, arr.size() - 1, threshold);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
  }
};

int main() {
  ArrayGenerator generator;
  std::ofstream csvFile("sorting_results.csv");
  csvFile << "ArrayType,Size,Algorithm,TimeMicroseconds,Threshold\n";

  for (int size = 500; size <= 10000; size += 100) {
    std::vector<int> randomArr = generator.generateRandomArray(size, 0, 6000);
    std::vector<int> reversedArr = generator.generateReversedArray(size);
    std::vector<int> almostSortedArr = generator.generateAlmostSortedArray(size, 10);

    long long timeMergeRandom = SortTester::testSort(Sorts::mergeSort, randomArr);
    csvFile << "Random," << size << ",MergeSort," << timeMergeRandom << ",0\n";

    long long timeMergeReversed = SortTester::testSort(Sorts::mergeSort, reversedArr);
    csvFile << "Reversed," << size << ",MergeSort," << timeMergeReversed << ",0\n";

    long long timeMergeAlmostSorted = SortTester::testSort(Sorts::mergeSort, almostSortedArr);
    csvFile << "AlmostSorted," << size << ",MergeSort," << timeMergeAlmostSorted << ",0\n";

    std::vector<int> thresholds = {5, 10, 20, 30, 50};
    for (int threshold : thresholds) {
      long long timeHybridRandom = SortTester::testHybridSort(Sorts::hybridSort, randomArr, threshold);
      csvFile << "Random," << size << ",HybridSort," << timeHybridRandom << "," << threshold << "\n";

      long long timeHybridReversed = SortTester::testHybridSort(Sorts::hybridSort, reversedArr, threshold);
      csvFile << "Reversed," << size << ",HybridSort," << timeHybridReversed << "," << threshold << "\n";

      long long timeHybridAlmostSorted = SortTester::testHybridSort(Sorts::hybridSort, almostSortedArr, threshold);
      csvFile << "AlmostSorted," << size << ",HybridSort," << timeHybridAlmostSorted << "," << threshold << "\n";
    }
  }

  csvFile.close();
  return 0;
}