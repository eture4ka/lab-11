#include <iostream>
#include <utility>
#include "TransistorArray.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

void header(const string& text) {
    cout << "\n===== " << text << " =====\n";
}

// Передача за значенням: параметр створюється конструктором копіювання
void showByValue(TransistorArray arr) {
    cout << "  Всередині showByValue():\n";
    arr.print("копія-параметр");
    cout << "  Вихід з showByValue() -> копія знищується:\n";
}

// Повернення з функції локального об'єкта.
// Компілятор зазвичай застосовує RVO і створює об'єкт одразу на місці,
// тому move constructor тут може не викликатися взагалі.
TransistorArray createPowerSet() {
    TransistorArray local(2);
    local.add("2N3055", 20, 15);
    local.add("IRF540N", 1500, 33);
    return local;
}

// Повернення параметра: RVO неможливе, тому гарантовано викликається move constructor
TransistorArray addAndReturn(TransistorArray arr) {
    arr.add("BC547", 110, 0.1);
    return arr;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    cout << "Лабораторна робота. Варіант 18: Транзистор (тип, підсилення, макс. струм)\n";

    header("1. Конструктор з параметром");
    TransistorArray a(4);
    a.add("KT315B", 200, 0.1);
    a.add("KT815", 40, 1.5);
    cout << "  Перевірка валідності:\n";
    a.add("", 100, 1);
    a.add("BAD", -5, 1);
    a.add("BAD", 100, 500);
    a.print("a");

    header("2. Передача об'єкта у функцію за значенням");
    showByValue(a);
    a.print("a після виклику (не змінився)");

    header("3. Повернення об'єкта з функції");
    TransistorArray b = createPowerSet();
    b.print("b");
    cout << "  Повернення параметра (тут RVO неможливе):\n";
    TransistorArray c = addAndReturn(a);
    c.print("c");

    header("4. Конструктор копіювання (глибока копія)");
    TransistorArray d(a);
    d.add("BD139", 100, 1.5);
    a.print("a (оригінал)");
    d.print("d (копія, змінена)");
    cout << "  Оригінал не змінився -> пам'ять у них різна.\n";

    header("5. Присвоювання копіюванням");
    TransistorArray e(1);
    e = b;
    e.print("e");
    cout << "  Самоприсвоювання:\n";
    TransistorArray& eRef = e;
    e = eRef;

    header("6. std::move -> конструктор переміщення");
    TransistorArray f(std::move(d));
    f.print("f (отримав дані)");
    d.print("d (після переміщення)");

    header("7. std::move -> присвоювання переміщенням");
    TransistorArray g(2);
    g.add("TEMP", 10, 1);
    g = std::move(c);
    g.print("g (отримав дані)");
    c.print("c (після переміщення)");

    header("8. Результат обчислень для f");
    cout << "  Сумарний максимальний струм: " << f.totalCurrent() << " А\n";
    if (const Transistor* t = f.maxGain())
        cout << "  Найбільше підсилення: " << t->type << " (h = " << t->gain << ")\n";

    header("9. Кінець main -> деструктори (у зворотному порядку створення)");
    return 0;
}
