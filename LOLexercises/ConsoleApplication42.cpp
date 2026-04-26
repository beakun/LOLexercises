#include <iostream>
#include <windows.h>
#include <vector>
#include <fstream>  
using namespace std;

class Exercise {
    std::string name;
    double weight;
    int sets, min_reps, max_reps;
    vector<int> rep;
public:
    Exercise() {};
    Exercise(string n, double w, int s, int minR, int maxR)
        : name(n), weight(w), sets(s), min_reps(minR), max_reps(maxR) {};
    void upWeight(double w) {
        if (weight + w >= 0) weight += w;
        else weight = 0;
    }
    void setRep() { int z; cin >> z;  rep.push_back(z); }
    void inputFromConsole() {
        setlocale(0, "RU");
        cout << "\tНазвание: "; cin >> name;
        cout << "\tДоп вес (кг): "; cin >> weight;
        cout << "\tКоличество подходов: "; cin >> sets;
        cout << "\tМинимум повторений: "; cin >> min_reps;
        cout << "\tМаксимум повторений: "; cin >> max_reps;
    }
    int getRep(int i) { return rep[i]; }
    friend ostream& operator<<(ostream& output, const Exercise a);
    friend short progress(Exercise& a);
    string getName() const { return name; }
    double getWeight() const { return weight; }
    int getSet() const { return sets; }
    int getMinReps() const { return min_reps; }
    int getMaxReps() const { return max_reps; }
    void clear() {
        rep.clear();
    }
};

void saveToFile(const vector<Exercise>& list, const string& filename) {
    ofstream out(filename);
    for (const auto& ex : list) {
        out << ex.getName() << " "
            << ex.getWeight() << " "
            << ex.getSet() << " "
            << ex.getMinReps() << " "
            << ex.getMaxReps() << "\n";
    }
}

void loadFromFile(vector<Exercise>& list, const string& filename) {
    ifstream in(filename);
    if (!in) return;

    list.clear();
    string name;
    double weight;
    int sets, minReps, maxReps;

    while (in >> name >> weight >> sets >> minReps >> maxReps) {
        list.push_back(Exercise(name, weight, sets, minReps, maxReps));
    }
}

short progress(Exercise& a) {
    short up = -1;
    for (int i = 0; i < a.sets; i++)
    {
        if (a.getRep(i) >= a.max_reps && (up == -1 || up == 1))up = 1;
        else if (up == 1)up = 0;
        if (a.getRep(i) < a.min_reps)return -2;
    }
    return (up == 1) ? up : 0;
}
ostream& operator<<(ostream& output, const Exercise a) {
    output << "Название: " << a.name << "\nВес: " << a.weight << "\nПодходы: " << a.sets << "\nДиапозон повторений: " << a.min_reps << "-" << a.max_reps << "\n";
    return output;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "ВНИМАНИЕ ВВОДИТЕ УПРАЖНЕНИЕ ОДНИМ СЛОВОМ БЕЗ ПРОБЕЛОВ(Так же нету обработок правильности значений)\n";
    cout << "Эта прога помогает(наверное в прогрессии нагрузок) если количесвто повторов \nво всех управжнениях больше или равно максимума то повышает вес на 2.5 кг а если шде-то меньше минимума то понимжаем\n это создано только для упражнений на повторения пока что.\n";
    cout << "Если хотите очистить список упражнений, введите 'clear'(Введить любое другое значение иначе): ";

    string command;
    cin >> command;

    if (command == "clear" || command == "--clear") {
        ofstream out("exercises.txt", ios::trunc);
        out.close();
        cout << "Файл exercises.txt очищен. Программа завершена.\n";
        return 0;
    }
     vector <Exercise> list;
    loadFromFile(list, "exercises.txt");

    if (list.empty()) {
        int n;
        cout << "Введите количество упражнений: "; cin >> n;
        for (int i = 0; i < n; i++) {
            cout << "Упражнение " << i + 1 << endl;
            Exercise t;
            t.inputFromConsole();
            list.push_back(t);
            saveToFile(list, "exercises.txt");
        }
    }
    

    system("cls");
    for (int i = 0; i < list.size(); i++)
    {
        cout << "Упражнение " << list[i].getName() << endl;
        for (int t = 0; t < list[i].getSet(); t++) {
            cout << "Подход " << t + 1 << " Введите количесвто повторений ";
            list[i].setRep();
        }
    }
    for (int i = 0; i < list.size(); i++)
    {
        cout << "Упражнение " << list[i].getName();
        if (progress(list[i]) == 1) {
            cout << ": Повысьте вес на 2.5 кг"; list[i].upWeight(2.5);
        }
        else if (progress(list[i]) == 0) {
            cout << ": Всё в диапозоне вес не меняем";
        }
        else { cout << ": Понижаем вес на 2.5 кг"; list[i].upWeight(-2.5); }
    }
    saveToFile(list, "exercises.txt");
    for (int i = 0; i < list.size(); i++) {
        list[i].clear();
    }
    return 0;
}
