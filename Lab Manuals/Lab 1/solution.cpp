#include <iostream>
using namespace std;

template <typename T>
class Matrix {
    T** matrix;
    int rows, columns;

public:
    Matrix(int rows = 0, int columns = 0) : rows(rows), columns(columns) {
        matrix = new T * [rows];
        for (int i = 0; i < rows; i++) {
            matrix[i] = new T[columns];
        }
    }

    Matrix(Matrix const& obj) : rows(obj.rows), columns(obj.columns) {
        matrix = new T * [rows];
        for (int i = 0; i < rows; i++) {
            matrix[i] = new T[columns];
            for (int j = 0; j < columns; j++) {
                matrix[i][j] = obj.matrix[i][j];
            }
        }
    }

    void insertElement(T const& element, int rowNo, int colNo) {
        if (rowNo < 0 || rowNo >= rows || colNo < 0 || colNo >= columns) {
            cout << "Invalid row or column mentioned\n";
        }
        else {
            matrix[rowNo][colNo] = element;
        }
    }

    Matrix<T> operator+(Matrix const& obj) {
        if (obj.rows != rows || obj.columns != columns) {
            cout << "Rows/Columns do not match\n";
            return Matrix<T>();
        }
        Matrix<T> result(rows, columns);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                result.matrix[i][j] = matrix[i][j] + obj.matrix[i][j];
            }
        }
        return result;
    }

    void print() const {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    void transpose() {
        T** transposed = new T * [columns];
        for (int i = 0; i < columns; i++) {
            transposed[i] = new T[rows];
            for (int j = 0; j < rows; j++) {
                transposed[i][j] = matrix[j][i];
            }
        }

        for (int i = 0; i < rows; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;

        int temp = rows;
        rows = columns;
        columns = temp;

        matrix = transposed;
    }

    ~Matrix() {
        for (int i = 0; i < rows; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }
};


template <typename T>
void sort(T* array, int size, int type) {
    if (type == 1) {
        // Bubble Sort - O(n^2)
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (array[j] > array[j + 1]) {
                    T temp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = temp;
                }
            }
        }
    }
    else if (type == 2) { // Insertion Sort - O(n^2)
        for (int i = 1; i < size; i++) {
            T key = array[i];
            int j = i - 1;
            while (j >= 0 && array[j] > key) {
                array[j + 1] = array[j];
                j--;
            }
            array[j + 1] = key;
        }
    }
    else if (type == 3) { // Selection Sort - O(n^2)
        for (int i = 0; i < size - 1; i++) {
            int minIndex = i;
            for (int j = i + 1; j < size; j++) {
                if (array[j] < array[minIndex]) {
                    minIndex = j;
                }
            }
            if (minIndex != i) {
                T temp = array[i];
                array[i] = array[minIndex];
                array[minIndex] = temp;
            }
        }
    }
    else
        cout << "Invalid sorting type! Use 1 for Bubble, 2 for Insertion, 3 for Selection.\n";
}

template <typename T>
void printArray(T* array, int size) {
    for (int i = 0; i < size; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}

int main() {
    Matrix<int> m1(2, 3);
    m1.insertElement(1, 0, 0);
    m1.insertElement(1, 0, 1);
    m1.insertElement(1, 0, 2);
    m1.insertElement(0, 1, 0);
    m1.insertElement(0, 1, 1);
    m1.insertElement(0, 1, 2);
    m1.transpose();

    Matrix<int> m2(2, 3);
    m2.insertElement(-1, 0, 0);
    m2.insertElement(-1, 0, 1);
    m2.insertElement(-1, 0, 2);
    m2.insertElement(10, 1, 0);
    m2.insertElement(5, 1, 1);
    m2.insertElement(1, 1, 2);
    m2.transpose();

    Matrix<int> m3(m2);
    Matrix<int> m4(m1 + m3);
    m4.transpose();
    m4.print();

    int arr1[] = { 5, 2, 9, 1, 5, 6 };
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    sort(arr1, size1, 1);
    printArray(arr1, size1);
    return 0;

    return 0;
}
