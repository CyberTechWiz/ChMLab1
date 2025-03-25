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

//������� ��� ���������� ���������� ��������
double langrange(const vector<double>& xvector, const vector<double>& yvector, int n, double x_value) {
	// xvector - ������ ��������� �������� � �������� �������
	// yvector - ������ ��������� �������� � �������� �������
	// n - ���-�� ��� �� � ������� �������� �������
	// x_value - ����� � ������� ����� ��������� ������������

	double res = 0.0; // ��� ������� ���� yi *Li(x)

	for (int i = 0; i < n; ++i) {
		double multip = yvector[i]; // �������� ������� y ������� ���������� �� �������� ������� ��������
		
		for (int k = 0; k < n; ++k) {
			if (i != k)
				multip = multip * ((x_value - xvector[k]) / (xvector[i] - xvector[k])); // y * �������� ������� ��������
		}

		res = res + multip; // ���������� ����� yi *Li(x)
	}

	cout << res << endl;
	return res;
}

// ������� ��� ���������� ����������� ��������� ��� ���������� �������
vector<vector<double>> dividedDifferences(const vector<double>& xvector, const vector<double>& yvector, int n) {
	// ������� �������� n � n, ��� n � ���������� �����.
	vector<vector<double>> diff(n, vector<double>(n, 0.0));

	// ��������� �������������
	// i - ������
	// j - �������
	// ���� ��������� ������� ������� ������� ���������� y � ������� ������
	for (int i = 0; i < n; ++i) {
		diff[i][0] = yvector[i];
	}

	// ���������� ����������� ��������� ��� ������� ������� j
	// ������ �������� ����� �������� ����� �������� ���������� ��������.
	// ���������� ������� ������� diff, ������� � ������� ������� j=1
	// ����� j - ������� ��������, ������� �� ������������:
	for (int j = 1; j < n; ++j) {
		// ���� ���� ���������� ������ � ������� diff ��� �������� ������� j
		// � ������ ��������� ������� ����� ����� ������: n - j
		for (int i = 0; i < (n - j); ++i) {
			diff[i][j] = (diff[i + 1][j - 1] - diff[i][j - 1]) / (xvector[i + j] - xvector[i]);
			// diff[i + 1][j - 1] - �������� ��� ����������� ������� � ����������� �������
			// diff[i][j - 1] �������� ��� ����������� �������, �� ��� ������� ������.
			// (xvector[i + j] - xvector[i]) - ��� ������� ����� �������� ������� xi � xi+j, ������� ������������� �������� ������� ��������.
		}
	}
	return diff;
}

// ������� ��� ���������� ���������� �������
double newton(const vector<double>& xvector, const vector<double>& yvector, int n, double x_value) {
    vector<vector<double>> diff = dividedDifferences(xvector, yvector, n);
	
	// result - ��������� �������� ��� ����������������� ���������� �������.
    double res = diff[0][0]; // f(x0)
    double multip = 1.0; // ����������, ��� �������� ������������� ������������
    
    for (int i = 1; i < n; i++) {
		// cout << "i: " << i << ", x_value: " << x_value << ", xvector[i-1]: " << xvector[i - 1] << endl; // ���������� ���������
		multip *= (x_value - xvector[i - 1]); // (x-x0)(x-x1)(x-x2)(x-x3)...
		res += diff[0][i] * multip;
    }

	cout << res << endl;
    return res;
}

// ����������� �������� � �� ����� � �������
// ������ � �������� x, ������ � ��������������� �������� y.
vector<double> read_file_x(const string& filename) {
	vector<double> vectorx;
	ifstream file(filename);

	if (!file) {
		cerr << "������ ��� �������� �����." << endl;
		return vectorx;
	}

	double x, y;
	string line;

	while (getline(file, line)) {
		stringstream ss(line);
		ss >> x >> y;  // ��������� x � y �� ������
		vectorx.push_back(x);  // ��������� x � ������
	}

	file.close();
	return vectorx;
}
// ������� ��� ���������� �������� y �� �����
vector<double> read_file_y(const string& filename) {
	vector<double> yvector;
	ifstream file(filename);

	if (!file) {
		cerr << "������ ��� �������� �����." << endl;
		return yvector;
	}

	double x, y;
	string line;

	while (getline(file, line)) {
		stringstream ss(line);
		ss >> x >> y;  // ��������� x � y �� ������
		yvector.push_back(y);  // ��������� y � ������
	}

	file.close();
	return yvector;
}

void update_vectors(vector<double>& xvector, vector<double>& yvector, int& n) {
	string filename = "TextFile1.txt";  // ��� ����� � �������
	xvector = read_file_x(filename);
	yvector = read_file_y(filename);
	n = yvector.size(); // ���-�� ��������� 

	// ���������� ���������
	std::cout << "���-�� ���������: " << n << std::endl;
	std::cout << "�������� ������� �: ";
	for (const int& element : xvector) {
		std::cout << element << " "; // ������� ������ �������
	}
	std::cout << std::endl; // ������� �� ����� ������
	std::cout << "�������� ������� y: ";
	for (const int& element : yvector) {
		std::cout << element << " "; // ������� ������ �������
	}
	std::cout << std::endl; // ������� �� ����� ������
}


//�������������� � ������������� � �������
void concoleinterface(vector<double>& xvector, vector<double>& yvector, int n) {
	cout << "\n���������� �������� ������ � ������� ���������������� ����������� � �����(������) �:" << endl;
	cout << "1. ������ �" << endl;
	cout << "2. ������ ��������� �" << endl;
	cout << "3. �������� �������� �������� �������" << endl;
	int choice1 = 1;
	int choice2 = 1;
	int x_count = 1;
	double x = 1;

	cin >> choice1;
	if (choice1 == 1) {
		cout << "������� �: ";
		cin >> x;
		cout << "\n� �������: " << endl;
		cout << "1. ���������� ��������" << endl;
		cout << "2. ���������� �������" << endl;
		cin >> choice2;
		if (choice2 == 1)
			langrange(xvector, yvector, n, x);
		else
			newton(xvector, yvector, n, x);
	}
	else if (choice1 == 2) {
		cout << "� �������: " << endl;
		cout << "1. ���������� ��������" << endl;
		cout << "2. ���������� �������" << endl;
		cin >> choice2;
		cout << "������� ����� �� �������? ";
		cin >> x_count;
		cout << "\n������� �� ������� " << x_count << " �����:" << endl;
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

	// ��� ������� ��������� ��������� �����
	vector<double> xvector;
	vector<double> yvector;
	int n; // ���-�� ��������� 
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