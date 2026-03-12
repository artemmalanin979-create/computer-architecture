#include <iomanip>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Структура для хранения состояния сумматора
struct FullAdder {
    bool a, b, cin;      // Входы
    bool sum, cout;      // Выходы
    
    void calculate() {
        sum = a ^ b ^ cin;
        cout = (a & b) | (a & cin) | (b & cin);
    }
};

class Adder4Bit {
private:
    FullAdder fa[4];     // 4 полных сумматора

    // В Linux просто выводим символы, не управляем курсором
    void drawLine(int length, char ch = '-') {
        for (int i = 0; i < length; i++) {
            cout << ch;
        }
        cout << endl;
    }

    void drawGate(const string& label) {
        cout << "[" << label << "]" << endl;
    }

    void drawConnection(int x1, int y1, int x2, int y2) {
        cout << "Connection from (" << x1 << "," << y1 << ") to (" << x2 << "," << y2 << ")" << endl;
    }

public:
    void setInputs(bool a[4], bool b[4], bool cin0) {
        fa[0].a = a[0]; fa[0].b = b[0]; fa[0].cin = cin0;
        fa[0].calculate();
        
        for (int i = 1; i < 4; i++) {
            fa[i].a = a[i];
            fa[i].b = b[i];
            fa[i].cin = fa[i-1].cout;
            fa[i].calculate();
        }
    }
    
    void draw() {
        // ASCII-таблица
        cout << "+--------------------------------------------------------------+\n";
        cout << "|          4-Х РАЗРЯДНЫЙ ПОЛНЫЙ СУММАТОР                       |\n";
        cout << "+==============================================================+\n";
        cout << "|                                                              |\n";

        // Отрисовка 4-х разрядов
        for (int i = 3; i >= 0; i--) {
            cout << "|  Разряд " << i << ":                                                   |\n";
            cout << "|    A" << i << ": " << (fa[i].a ? "1" : "0") << " --|                                                 |\n";
            cout << "|    B" << i << ": " << (fa[i].b ? "1" : "0") << " --+--[XOR]--[XOR]-- S" << i << ": " << (fa[i].sum ? "1" : "0") << (fa[i].sum ? "                           |" : "                           |") << "\n";
            string c_label = (i-1 >= 0 ? to_string(i-1) : string("in"));
            string c_val = (i == 0 ? (fa[0].cin ? "1" : "0") : (fa[i].cin ? "1" : "0"));
            cout << "|    C" << c_label << ": " << c_val << " --'    |      ^                                     |\n";
            cout << "|              [AND]--[OR]-- C" << i << ": " << (fa[i].cout ? "1" : "0") << (fa[i].cout ? "                             |" : "                             |") << "\n";
            cout << "|              [AND]--'                                        |\n";
            cout << "|                                                              |\n";
        }

        cout << "+==============================================================+\n";
        cout << "|  РЕЗУЛЬТАТ:                                                  |\n";
        cout << "|  Сумма: ";
        for (int i = 3; i >= 0; i--) cout << (fa[i].sum ? "1" : "0");
        int sum_val = getDecimalSum();
        if (sum_val < 10) {
            cout << " (десятичное: " << sum_val << ")                                |\n";
        } else {
            cout << " (десятичное: " << sum_val << ")                                |\n";
        }
        cout << "|  Перенос: " << (fa[3].cout ? "1" : "0") << "                                                  |\n";
        cout << "+--------------------------------------------------------------+\n";
    }
    
    int getDecimalSum() {
        int sum = (fa[3].cout ? 16 : 0);
        for (int i = 3; i >= 0; i--) {
            sum += (fa[i].sum ? (1 << i) : 0);
        }
        return sum;
    }
    
    void getOutputs(bool sum[4], bool& cout) {
        for (int i = 0; i < 4; i++) sum[i] = fa[i].sum;
        cout = fa[3].cout;
    }
};

int main() {
    Adder4Bit adder;
    bool a[4], b[4];
    int cin0;
    
    cout << "4-х разрядный полный сумматор\n";
    cout << "Введите число A (0-15): ";
    int numA, numB;
    cin >> numA;
    cout << "Введите число B (0-15): ";
    cin >> numB;
    cout << "Введите входной перенос (0/1): ";
    cin >> cin0;
    
    // Преобразование в двоичный вид
    for (int i = 0; i < 4; i++) {
        a[i] = (numA >> i) & 1;
        b[i] = (numB >> i) & 1;
    }
    
    adder.setInputs(a, b, cin0);
    adder.draw();
    
    cout << "\nНажмите Enter для выхода...";
    cin.ignore();
    cin.get();
    
    return 0;
}