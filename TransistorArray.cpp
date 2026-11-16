#include "TransistorArray.h"
#include <iomanip>
#include <iostream>

using namespace std;

int TransistorArray::nextId = 1;

// ---------- Конструктор з параметром: виділяє пам'ять ----------
TransistorArray::TransistorArray(size_t cap)
    : data(nullptr), size(0), capacity(cap), id(nextId++) {
    if (capacity > 0) data = new Transistor[capacity];
    cout << "  [#" << id << "] parameterized constructor called"
         << " (new Transistor[" << capacity << "])\n";
}

// ---------- Деструктор: звільняє пам'ять ----------
TransistorArray::~TransistorArray() {
    cout << "  [#" << id << "] destructor called";
    if (data) cout << " (delete[] " << size << " ел.)";
    else      cout << " (пам'ять порожня, нічого звільняти)";
    cout << "\n";
    delete[] data;
}

// ---------- Конструктор копіювання: глибока копія ----------
TransistorArray::TransistorArray(const TransistorArray& other)
    : data(nullptr), size(other.size), capacity(other.capacity), id(nextId++) {
    if (capacity > 0) {
        data = new Transistor[capacity];           // своя, окрема пам'ять
        for (size_t i = 0; i < size; ++i) data[i] = other.data[i];
    }
    cout << "  [#" << id << "] copy constructor called (копія з #" << other.id << ")\n";
}

// ---------- Присвоювання копіюванням ----------
TransistorArray& TransistorArray::operator=(const TransistorArray& other) {
    cout << "  [#" << id << "] copy assignment operator called (з #" << other.id << ")\n";
    if (this == &other) {
        cout << "       самоприсвоювання, нічого не робимо\n";
        return *this;
    }
    // Спочатку виділяємо нову пам'ять, потім звільняємо стару:
    // якщо new кине виняток, об'єкт залишиться в коректному стані
    Transistor* newData = nullptr;
    if (other.capacity > 0) {
        newData = new Transistor[other.capacity];
        for (size_t i = 0; i < other.size; ++i) newData[i] = other.data[i];
    }
    delete[] data;
    data = newData;
    size = other.size;
    capacity = other.capacity;
    return *this;
}

// ---------- Конструктор переміщення: забирає пам'ять ----------
TransistorArray::TransistorArray(TransistorArray&& other) noexcept
    : data(other.data), size(other.size), capacity(other.capacity), id(nextId++) {
    other.data = nullptr;   // джерело більше не володіє пам'яттю
    other.size = 0;
    other.capacity = 0;
    cout << "  [#" << id << "] move constructor called (пам'ять забрано з #" << other.id << ")\n";
}

// ---------- Присвоювання переміщенням ----------
TransistorArray& TransistorArray::operator=(TransistorArray&& other) noexcept {
    cout << "  [#" << id << "] move assignment operator called (з #" << other.id << ")\n";
    if (this == &other) return *this;
    delete[] data;          // звільняємо власну стару пам'ять
    data = other.data;      // забираємо чужу
    size = other.size;
    capacity = other.capacity;
    other.data = nullptr;
    other.size = 0;
    other.capacity = 0;
    return *this;
}

// ---------- Робота з даними ----------
bool TransistorArray::add(const string& type, double gain, double maxCurrent) {
    if (type.empty()) {
        cout << "  ! Помилка: тип не може бути порожнім.\n";
        return false;
    }
    if (gain <= 0 || gain > 10000) {
        cout << "  ! Помилка: коефіцієнт підсилення " << gain << " некоректний (0 < h <= 10000).\n";
        return false;
    }
    if (maxCurrent <= 0 || maxCurrent > 100) {
        cout << "  ! Помилка: струм " << maxCurrent << " А некоректний (0 < I <= 100).\n";
        return false;
    }
    if (size >= capacity) {
        cout << "  ! Помилка: масив #" << id << " заповнений (" << capacity << " ел.).\n";
        return false;
    }
    data[size++] = Transistor{type, gain, maxCurrent};
    return true;
}

void TransistorArray::print(const string& title) const {
    cout << "  " << title << " [#" << id << ", " << size << "/" << capacity << " ел.]";
    if (size == 0) {
        cout << " — порожній\n";
        return;
    }
    cout << "\n";
    for (size_t i = 0; i < size; ++i)
        cout << "     " << i + 1 << ". " << left << setw(10) << data[i].type << right
             << " h = " << setw(7) << fixed << setprecision(1) << data[i].gain
             << "  Imax = " << setw(6) << setprecision(2) << data[i].maxCurrent << " А\n";
}

size_t TransistorArray::getSize() const { return size; }
size_t TransistorArray::getCapacity() const { return capacity; }
int TransistorArray::getId() const { return id; }

double TransistorArray::totalCurrent() const {
    double sum = 0;
    for (size_t i = 0; i < size; ++i) sum += data[i].maxCurrent;
    return sum;
}

const Transistor* TransistorArray::maxGain() const {
    if (size == 0) return nullptr;
    const Transistor* best = &data[0];
    for (size_t i = 1; i < size; ++i)
        if (data[i].gain > best->gain) best = &data[i];
    return best;
}
