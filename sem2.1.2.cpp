#include <iostream>
#include <fstream>

using namespace std;

// здесь и далее, а - символ множества А (как в условии), q - состояние автомата, act - одна из трёх Л,П,Н, как следующее действие

const int Q[4]{ 0,1,2,3 }; // множество состояний, нам по факту же не принципиально, как они выглядят, поэтому пусть будет ничего, просто идентификатор

const int A[4]{ 0,1,2,3 }; // то же самое для множества значений ячейки

char acts[3]{ 'Л', 'П', 'Н' }; // массив с возможными действиями над ячейкой

struct Cell { // так как лента бесконечна в обе стороны, реализуем двусвязный список, т.е. у каждого элемента есть ссылка на предыдущий и на следующий
	char a;
	Cell* next;
	Cell* prev;
};

typedef Cell* TuringStrip; // просто для удобства

struct ProgramCell { // ячейка таблицы устройства
	int a;
	int q;
	int act;
};

ProgramCell program[4][3]{ //придётся вручную записывать список действий для машины
	{{A[0], Q[1], acts[1]}, {A[0], Q[1], acts[1]}, {A[2], Q[2], acts[2]}},
	{{A[1], Q[1], acts[1]}, {A[3], Q[3], acts[0]}, {A[0], Q[2], acts[2]}},
	{{A[0], Q[3], acts[1]}, {A[2], Q[2], acts[0]}, {A[2], Q[3], acts[1]}},
	{{A[0], Q[0], acts[2]}, {A[3], Q[2], acts[0]}, {A[3], Q[3], acts[1]}}
};

void createCell(TuringStrip current, bool w) { // создадим новую клетку, если у нас ещё она никогда не использовалась, причём если w true то сзади, иначе спереди
	if (w) {
		current->prev = new Cell{ 0, current, nullptr };
	}
	else current->next = new Cell{ 0, nullptr, current };
}

typedef int processor; // просто для удобства

bool process(processor& q, TuringStrip& st) { // обрабатываем клетку st с состоянием q, причём если состояние 0, выходим из программы и возвращаем является ли это выходом из программы вообще
	if (q == 0) return true;

	else {
		ProgramCell cell = program[st->a][q]; // находим клетку, соответствующую нашему состоянию в таблице программы и выбираем действие, которое надо совершить с ячейкой
		st->a = cell.a;
		q = cell.q;
		switch (cell.act) {
		case 'Л':
			if (st->prev == nullptr) createCell(st, true);
			st = st->prev;
			break;
		case 'Н':
			break;
		case 'П':
			if (st->next == nullptr) createCell(st, false);
			st = st->next;
			break;
		}
	}
}

void getStrip(ifstream& in, TuringStrip pr, int k) { // короче функция чтобы считать из файла начальное состояние ленты, по циферкам - номер состояния, если 0(что неправильно по условию) - выход из функции
	int b;
	for (int i = 0; i <= k; i++) {
		pr->next = new Cell{ 0,nullptr, pr }; // k раз считывем, создаём каждый раз в качестве следующей новую клетку и связываем её с предыдущей
		in >> b;
		switch (b) {
		case 0:
			return;
		case 1:
			pr->a = A[1];
			break;
		case 2:
			pr->a = A[2];
			break;
		case 3:
			pr->a = A[3];
		}
		pr = pr->next; // переходим к следующей клетке
	}
}

int main() {
	setlocale(LC_ALL, "rus");
	TuringStrip machine = new Cell; // лента, вообще говоря текующий элемент
	ifstream in("strip.txt");
	getStrip(in, machine, 10);
	in.close();
	int T;
	processor q = Q[1];

	cin >> T;
	for (int i = 1; i < T; i++) {
		bool b = process(q, machine); // обрабатываем каждую клетку и записываем, пришло ли нам в конечном счёте стоп значение
		if (b) {
			cout << "программа завершилась в связи с нулевым состоянием"; // если да, то здесь выходим из программы и уточняем условие выхода
			return 0;
		}
		
	}
	cout << "программа выполнила все T тактов"; // теперь надо доделать функцию, которая задана в задании, но это позже
	return 0;
}