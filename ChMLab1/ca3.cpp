#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::vector;
using std::cin;
using std::string;
using std::ifstream;
using std::cerr;
using std::stringstream;

//Функция для построения многочлена Лагранжа
double langrange(const vector<double>& xvector, const vector<double>& yvector, int n, double x_value) {
	// xvector - вектор табличных значений х заданной функции
	// yvector - вектор табличных значений у заданной функции
	// n - кол-во пар ху в таблице заданной функции
	// x_value - точка в которой будет считаться интерполяция

	double res = 0.0; // для ханения сумм yi *Li(x)

	for (int i = 0; i < n; ++i) {
		double multip = yvector[i]; // значение функции y которое умножается на базисный полином лагранжа
		
		for (int k = 0; k < n; ++k) {
			if (i != k)
				multip = multip * ((x_value - xvector[k]) / (xvector[i] - xvector[k])); // y * базисный полином Лагранжа
		}

		res = res + multip; // складываем суммы yi *Li(x)
	}

	cout << res << endl;
	return res;
}

// Функция для вычисления разделенных разностей для многочлена Ньютона
vector<vector<double>> dividedDifferences(const vector<double>& xvector, const vector<double>& yvector, int n) {
	// таблица размером n х n, где n — количество точек.
	vector<vector<double>> diff(n, vector<double>(n, 0.0));

	// Начальная инициализация
	// i - строка
	// j - столбец
	// цикл заполняет нулевой столбец таблицы значениями y в узловых точках
	for (int i = 0; i < n; ++i) {
		diff[i][0] = yvector[i];
	}

	// Вычисление разделенных разностей для каждого столбца j
	// каждую разность можно выразить через разности предыдущих столбцов.
	// перебираем столбцы таблицы diff, начиная с первого столбца j=1
	// также j - порядок разности, которую мы рассчитываем:
	for (int j = 1; j < n; ++j) {
		// этот цикл перебирает строки в таблице diff для текущего столбца j
		// в каждом следующем столбце строк будет меньше: n - j
		for (int i = 0; i < (n - j); ++i) {
			diff[i][j] = (diff[i + 1][j - 1] - diff[i][j - 1]) / (xvector[i + j] - xvector[i]);
			// diff[i + 1][j - 1] - разность для предыдущего столбца и предыдущего порядка
			// diff[i][j - 1] разность для предыдущего порядка, но для текущей строки.
			// (xvector[i + j] - xvector[i]) - это разница между узловыми точками xi и xi+j, которые соответствуют текущему порядку разности.
		}
	}
	return diff;
}

// Функция для вычисления многочлена Ньютона
double newton(const vector<double>& xvector, const vector<double>& yvector, int n, double x_value) {
    vector<vector<double>> diff = dividedDifferences(xvector, yvector, n);
	
	// result - начальная величина для интерполяционного многочлена Ньютона.
    double res = diff[0][0]; // f(x0)
    double multip = 1.0; // переменная, для хранения промежуточных произведений
    
    for (int i = 1; i < n; i++) {
		// cout << "i: " << i << ", x_value: " << x_value << ", xvector[i-1]: " << xvector[i - 1] << endl; // Отладочное сообщение
		multip *= (x_value - xvector[i - 1]); // (x-x0)(x-x1)(x-x2)(x-x3)...
		res += diff[0][i] * multip;
    }

	cout << res << endl;
    return res;
}

// Считываение значений х их файла в вектора
// первое — значение x, второе — соответствующее значение y.
vector<double> read_file_x(const string& filename) {
	vector<double> vectorx;
	ifstream file(filename);

	if (!file) {
		cerr << "Ошибка при открытии файла." << endl;
		return vectorx;
	}

	double x, y;
	string line;

	while (getline(file, line)) {
		stringstream ss(line);
		ss >> x >> y;  // Считываем x и y из строки
		vectorx.push_back(x);  // Добавляем x в вектор
	}

	file.close();
	return vectorx;
}
// Функция для считывания значений y из файла
vector<double> read_file_y(const string& filename) {
	vector<double> yvector;
	ifstream file(filename);

	if (!file) {
		cerr << "Ошибка при открытии файла." << endl;
		return yvector;
	}

	double x, y;
	string line;

	while (getline(file, line)) {
		stringstream ss(line);
		ss >> x >> y;  // Считываем x и y из строки
		yvector.push_back(y);  // Добавляем y в вектор
	}

	file.close();
	return yvector;
}

void update_vectors(vector<double>& xvector, vector<double>& yvector, int& n) {
	string filename = "TextFile1.txt";  // Имя файла с данными
	xvector = read_file_x(filename);
	yvector = read_file_y(filename);
	n = yvector.size(); // кол-во элементов 

	// Отладочные сообщения
	std::cout << "Кол-во элементов: " << n << std::endl;
	std::cout << "Элементы вектора х: ";
	for (const int& element : xvector) {
		std::cout << element << " "; // Выводим каждый элемент
	}
	std::cout << std::endl; // Переход на новую строку
	std::cout << "Элементы вектора y: ";
	for (const int& element : yvector) {
		std::cout << element << " "; // Выводим каждый элемент
	}
	std::cout << std::endl; // Переход на новую строку
}


//Взаимодействие с пользователем в консоли
void concoleinterface(vector<double>& xvector, vector<double>& yvector, int n) {
	cout << "\nРассчитать значение функци с помощью интерполяционных многочленов в точке(точках) х:" << endl;
	cout << "1. Ввести х" << endl;
	cout << "2. Ввести несколько х" << endl;
	cout << "3. Обновить таблично заданную функцию" << endl;
	int choice1 = 1;
	int choice2 = 1;
	int x_count = 1;
	double x = 1;

	cin >> choice1;
	if (choice1 == 1) {
		cout << "Введите х: ";
		cin >> x;
		cout << "\nС помощью: " << endl;
		cout << "1. Многочлена Лагранжа" << endl;
		cout << "2. Многочлена Ньютона" << endl;
		cin >> choice2;
		if (choice2 == 1)
			langrange(xvector, yvector, n, x);
		else
			newton(xvector, yvector, n, x);
	}
	else if (choice1 == 2) {
		cout << "С помощью: " << endl;
		cout << "1. Многочлена Лагранжа" << endl;
		cout << "2. Многочлена Ньютона" << endl;
		cin >> choice2;
		cout << "Сколько иксов вы введете? ";
		cin >> x_count;
		cout << "\nВводите по очереди " << x_count << " иксов:" << endl;
		if (choice2 == 1)
			for (int i = 0; i < x_count; ++i) {
				cin >> x;
				langrange(xvector, yvector, n, x);
			}
		else {
			for (int i = 0; i < x_count; ++i) {
				cin >> x;
				newton(xvector, yvector, n, x);
			}
		}
	}
	else {
		update_vectors(xvector, yvector, n);
	}
}


int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	// При запуске программы считываем файлы
	vector<double> xvector;
	vector<double> yvector;
	int n; // кол-во элементов 
	update_vectors(xvector, yvector, n);

	concoleinterface(xvector, yvector, n);
	concoleinterface(xvector, yvector, n);
	concoleinterface(xvector, yvector, n);
	concoleinterface(xvector, yvector, n);
	concoleinterface(xvector, yvector, n);
	concoleinterface(xvector, yvector, n);
	concoleinterface(xvector, yvector, n);
	concoleinterface(xvector, yvector, n);
	concoleinterface(xvector, yvector, n);
	concoleinterface(xvector, yvector, n);
}