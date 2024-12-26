

#include <iostream>


void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        int len = (int) std::to_string(arr[i]).length();
        for (int j = 0; j < 4 - len; j++)
            std::cout << " ";
        if (i != size - 1)
            std::cout << arr[i];
        else
            std::cout << arr[i] << std::endl;
    }
}

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        printArray(arr, n);

        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    printArray(arr, n);
}

int main() {
    std::cout << "BubbleSort:" << std::endl;

    int arr[] = {5, 3, 889, 276, 9, 1, 4, 6, 75, 0};
    int n = sizeof(arr) / sizeof(arr[0]);

    bubbleSort(arr, n);

    std::cout << std::endl << "Result:" << std::endl;
    printArray(arr, n);

    return 0;
}
