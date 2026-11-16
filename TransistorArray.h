#ifndef TRANSISTOR_ARRAY_H
#define TRANSISTOR_ARRAY_H

#include <cstddef>
#include <string>

// Один транзистор (варіант 18): тип, коефіцієнт підсилення, максимальний струм
struct Transistor {
    std::string type;
    double gain;
    double maxCurrent;
};

// Динамічний масив транзисторів з ручним керуванням пам'яттю ("правило п'яти")
class TransistorArray {
private:
    Transistor* data;      // вказівник на масив у динамічній пам'яті
    std::size_t size;      // кількість заповнених елементів
    std::size_t capacity;  // скільки елементів виділено
    int id;                // номер об'єкта, щоб у консолі було видно, хто є хто

    static int nextId;

public:
    explicit TransistorArray(std::size_t capacity);              // конструктор з параметром
    ~TransistorArray();                                          // деструктор
    TransistorArray(const TransistorArray& other);               // конструктор копіювання
    TransistorArray& operator=(const TransistorArray& other);    // присвоювання копіюванням
    TransistorArray(TransistorArray&& other) noexcept;           // конструктор переміщення
    TransistorArray& operator=(TransistorArray&& other) noexcept;// присвоювання переміщенням

    bool add(const std::string& type, double gain, double maxCurrent);
    void print(const std::string& title) const;

    std::size_t getSize() const;
    std::size_t getCapacity() const;
    int getId() const;
    double totalCurrent() const;
    const Transistor* maxGain() const;
};

#endif
