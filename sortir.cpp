#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <stack>
#include <chrono>

int size_of_array;
int heap_size;
std::vector <double> array;

bool CheckSorted() {
    for (int i = 1; i < size_of_array; ++i) {
        std::cout << array[i] << ' ';
        if (array[i] < array[i - 1])
            return false;
    }
    return true;   
}

void SiftDown(int i) {
    while (2 * i + 1 < heap_size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int j = left;
        if (right < heap_size && array[right] > array[left])
            j = right;
        if (array[i] >= array[j])
            break;
        std::swap(array[i], array[j]);
        i = j;
    }
}


void HeapSort() {
    heap_size = size_of_array;
    for (int i = size_of_array / 2; i >= 0; --i) {
        SiftDown(i);
    }

    for (int i = 0; i < size_of_array; ++i) {
        std::swap(array[0], array[size_of_array - i - 1]);
        heap_size--;
        SiftDown(0);
    } for (int i = size_of_array / 2; i >= 0; --i) {
        SiftDown(i);
    }
}
void QuickSort(int left, int right) {
    int i = left;
    int j = right + i - 1;
    double pivot = (array[left] + array[j]) / 2;
    while (i <= j) {
        while (array[i] < pivot) ++i;
        while (array[j] > pivot) --j;
        if (i <= j) {
            std::swap(array[i], array[j]);
            i++;
            j--;
        }
    }
    if (i < right + left) {
        QuickSort(i, right + left - i);
    }
    if (j > left) {
        QuickSort(left, j - left + 1);
    }
}

std::vector <double> Radix(std::vector <double> &the_array_we_sort) {
    int n = the_array_we_sort.size();
    long long digit = 1ll << 16ll;
    for (int shift = 0; shift < 4; ++shift) {
        int count[65536] = { 0 };
        for (int i = 0; i < n; ++i) {
            long long bits = *((long long*)(&the_array_we_sort[i]));
            int ind = ((bits & ((1ll << 16ll) - 1) << (16ll * shift)) >> (16ll * shift));
            count[ind]++;
        }
        for (int i = 1; i <= 65535; ++i) {
            count[i] = count[i - 1] + count[i];
        }
        std::vector <double> tmp(n);
        for (int i = n-1; i >= 0; --i) {
            long long bits = *((long long*)(&the_array_we_sort[i]));
            int ind = ((bits & (digit - 1) << (16ll * shift)) >> (16ll * shift));
            tmp[count[ind] - 1] = the_array_we_sort[i];
            count[ind]--;
        }
        the_array_we_sort = tmp;
    }
    return the_array_we_sort;
}

void RadixSort() {
    int count = 0;
    for (int i = 0; i < size_of_array; ++i) {
        if (array[i] < 0) count++;
        array[i];
    }
    int tmp_count = count;
    std::vector <double> positive_numbers(size_of_array - count);
    std::vector <double> negative_numbers(count);
    int tmp_p = 0, tmp_o = 0;
    for (int i = 0; i < size_of_array; ++i) {
        if (array[i] >= 0) {
            positive_numbers[tmp_p] = array[i];
            tmp_p++;
        } else {
            negative_numbers[tmp_o] = -array[i];
            tmp_o++;
        }
    }
    negative_numbers = Radix(negative_numbers);
    positive_numbers = Radix(positive_numbers);
    int i = 0;
    for (int j = negative_numbers.size() - 1; j >= 0; --j, ++i) {
        array[i] = -negative_numbers[j];
    }
    for (int j = 0; j < positive_numbers.size(); ++j, ++i) {
        array[i] = positive_numbers[j];
    }

}


int MinRun() {
    int n = size_of_array;
    int flag = 0;
    while (n >= 64) {
        flag = flag | (n & 1);
        n >>= 1;
    }
    return n + flag;
}


void Insertion(int beg, int length, int ind) {
    int end = std::min(beg + length + 1, size_of_array);
    for (int i = ind + 1; i < end; ++i) {
        int j = i - 1;
        int key = array[i];
        while (j >= beg && array[j] > key) {
            array[j + 1] = array[j];
            j -= 1;
        }
        array[j + 1] = key;
    }
}

void StepTwo(int beg, int minrun) {
    int ptr = beg; 
    int end = std::min(beg + minrun, size_of_array);
    while (ptr + 1 < size_of_array && array[ptr] <= array[ptr + 1]) { 
        ptr++;
    }
    if (ptr == beg) {
        while (ptr + 1 < size_of_array && array[ptr] > array[ptr + 1]) {
            ptr++;
        }
        int ptr_left = beg;
        int ptr_right = ptr;
        while (ptr_right > ptr_left) {
            std::swap(array[ptr_left], array[ptr_right]);
            ptr_left++, ptr_right--;
        }
    }
    if (ptr + 1 < beg + minrun) { 
        Insertion(beg, (beg + minrun) - ptr, ptr); 
    }
    if (ptr + 1 < size_of_array) { 
        StepTwo(std::max(ptr + 1, end), minrun); 
    }

}


void Merge(std:: pair <int, int> lhs, std:: pair <int, int> rhs) {
    std::vector <int> tmp(lhs.second);
    for (int j = 0, i = lhs.first; i < lhs.first + lhs.second; ++i, ++j) {
        tmp[j] = array[i];
    }
    int ptr_left = 0;
    int ptr_right = rhs.first;
    int ptr_push = lhs.first;
    for (; ptr_left < lhs.second &&
        ptr_right < rhs.first + rhs.second; ptr_push++) {
        if (tmp[ptr_left] < array[ptr_right]) {
            array[ptr_push] = tmp[ptr_left];
            ptr_left++;
        } else {
            array[ptr_push] = array[ptr_right];
            ptr_right++;
        }
    }
    int beg = 0, end = 0;
    if (ptr_right != rhs.first + rhs.second) {
        for (; ptr_right < rhs.first + rhs.second; ++ptr_right, ++ptr_push) {
            array[ptr_push] = array[ptr_right];
        }
    } else if (ptr_left != lhs.second) {
        for (; ptr_left < lhs.second; ++ptr_left, ++ptr_push) {
            array[ptr_push] = tmp[ptr_left];
        }
    }

}
void TimSort() {

    std::stack <std::pair <int, int> > s;

    int minrun = MinRun();
    StepTwo(0, minrun);

    int ind = 1;
    int size = 1;
    int beg = 0;
    while (ind < size_of_array) {
        if (array[ind] < array[ind - 1] || ind == size_of_array - 1) {
            std::pair <int, int> p = std::make_pair(beg, size);
            beg += size;
            size = 1;
            s.push(p);
            if (ind == size_of_array - 1) {
                std::pair <int, int> p = std::make_pair(beg, size);
                s.push(p);
            }
            while (s.size() >= 3) {
                std::pair <int, int> X, Y, Z;
                X = s.top();
                s.pop();
                Y = s.top();
                s.pop();
                Z = s.top();
                s.pop();
                if (!(Z.second > X.second + Y.second && Y.second > X.second)) {
                    if (X.second < Y.second) {
                        Merge(Y, X);
                        s.push(Z);
                        s.push(std::make_pair(Y.first, Y.second + X.second));
                    } else {
                        Merge(Z, Y);
                        s.push(std::make_pair(Z.first, Y.second + Z.second));
                        s.push(X);

                    }
                } else {
                    s.push(Z);
                    s.push(Y);
                    s.push(X);
                    break;
                }

            }
            while (s.size() >= 2) {
                std::pair <int, int> X, Y;
                X = s.top();
                s.pop();
                Y = s.top();
                s.pop();
                if (X.second >= Y.second) {
                    Merge(Y, X);
                    s.push(std::make_pair(Y.first, Y.second + X.second));
                } else {
                    s.push(Y);
                    s.push(X);
                    break;
                }
            }
        } else {
            size++;
        }
        ind++;
    }

    while (s.size() > 1) {
        std::pair <int, int> X, Y;
        X = s.top();
        s.pop();
        Y = s.top();
        s.pop();
        Merge(Y, X);
        s.push(std::make_pair(Y.first, Y.second + X.second));
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    srand((unsigned)time(NULL));
    int sort_number;

    std::cout << "Choose your fighter. \n 1 - Quick Sort      2 - Heap Sort\n";
    std::cout << " 3 - Radix Sort       4 - Timsort\n";
    std::cin >> sort_number;
    std::cout << "Size of array: ";
    std::cin >> size_of_array;
    std::cout << '\n';
    array.resize(size_of_array);

    for (int i = 0; i < size_of_array; i++) {
        int64_t A = rand();
        array[i] = (-3e10 + A * 1745397) * 0.0001;
        std::cout << array[i] << ' ';
    }

    std::cout << std::endl;

    clock_t start = clock();
    if (sort_number == 1) {
        std::cout << "\nQuick sort.\nSize: " << size_of_array << "." << std::endl;
        QuickSort(0, size_of_array);
    } else if (sort_number == 2) {
        std::cout << "\nHeap sort.\nSize: " << size_of_array << "." << std::endl;
        HeapSort();
    } else if (sort_number == 3) {
        std::cout << "\Radix sort.\nSize: " << size_of_array << "." << std::endl;
        RadixSort();
    } else if (sort_number == 4) {
        std::cout << "\Tim sort.\nSize: " << size_of_array << "." << std::endl;
        TimSort();
    }

    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    std::cout << "The time: " << seconds;

   
    if (CheckSorted()) {
        std::cout << "\nRESULT: ";
        std::cout << "SORTED";
    } else {
        std::cout << "\nRESULT: ";
        std::cout << "NOT SORTED";
    }

}
