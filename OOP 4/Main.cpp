#include <iostream>
#include <vector>
#include <chrono> // библиотека для работы со временем
// Создание шаблонного связного списка.

template<typename Type>

class LinkedList {
public:
	// Констрктор по умолчанию
	LinkedList() :start_(nullptr), end_(nullptr), size_(0) { } // не стоит использовать макрос NULL (могут быть конфликты типов)

	// Лист с уже созданными n элементами
	LinkedList(int n) :LinkedList() {
		if (n < 0) throw - 1;
		if (n == 0) return;
		Node* tmp{};

		// Вариант 1:
		/*size_ = n;
		do {
			tmp = new Node;
			if (start_ == nullptr) {
				start_ = tmp;
			}
			n - +1;
		} while (n > 0);*/

		// Вариант 2:
		int addedNodes{ 0 };
		tmp = new Node{};
		start_ = tmp;
		end_ = tmp;
		addedNodes += 1;
		while (addedNodes < n) {
			tmp = new Node{}; // создание новой точки 
			end_->Next(tmp); // старому концу списка говорим, что новый конечный элемент существует 
			end_ = tmp; // запоминаем в списке, какой элемент сейчас последний
			addedNodes += 1;
		}
		size_ = n;

	}
	// Конструктор копирования
	LinkedList(const LinkedList& other) :LinkedList() {
		if (other.size() == 0) return;
		start_ = new Node(other.start_->Data()); // перенесли информацию стартовой точки из переданного в стартовую точку нового
		end_ = start_;
		//for (int i = 1; i < other.size(); i++) 
		for (Node* current = other.start_->Next(); current != nullptr; current = current->Next()) {
			//Node* tmp = new Node(other[i]); // медленная компиляция 
			Node* tmp = new Node(current->Data()); // либо (*current).Data()
			end_->Next(tmp);
			end_ = tmp;
		}
		size_ = other.size_;
	}


	// Конструктор переноса
	LinkedList(LinkedList&& other) :LinkedList() {
		std::swap(start_, other.start_);
		std::swap(end_, other.end_);
		std::swap(size_, other.size_);
	}
	// можно не делать, но для оптимизации скорости работы программы полезно 
	// тут не будет реализации конструктора от списка инициализации, внесём позже 


	// Деструктор
	~LinkedList() {
		Node* current = star_;
		while (current != nullptr) {
			Node* tmp = current->Next();
			delete current;
			current = tmp;
		}
	}

	void clear() {
		Node* current = star_;
		while (current != nullptr) {
			Node* tmp = current->Next();
			delete current;
			current = tmp;
		}
		start_ = nullptr;
		end_ = nullptr;
		size_ = 0;
	}

	// Присваивание копированием 
	LinkedList& operator = (const LinkedList& other) {
		clear();
		if (other.size() == 0) return *this;
		start_ = new Node(other.start_->Data()); // перенесли информацию стартовой точки из переданного в стартовую точку нового
		end_ = start_;
		//for (int i = 1; i < other.size(); i++) 
		for (Node* current = other.start_->Next(); current != nullptr; current = current->Next()) {
			//Node* tmp = new Node(other[i]); // медленная компиляция 
			Node* tmp = new Node(current->Data()); // либо (*current).Data()
			end_->Next(tmp);
			end_ = tmp;
		}
		return *this;
	}

	// Присваивание переносом
	LinkedList& operator = (LinkedList&& other) {
		clear();
		std::swap(start_, other.start_);
		std::swap(end_, other.end_);
		std::swap(size_, other.size_);
		return *this;
	}

	// Оператор доступа к хранящимся в списке элементам
	Type& operator[](int i) {
		int count{};
		Node* tmp = start_;
		while (count < i) {
			tmp = tmp->Next();
			count += 1;
		}
		return tmp->Data();
	}


	// Константный оператор доступа к хранящимся в списке элементам
	const Type& operator[](int i) const {
		int count{};
		Node* tmp = start_;
		while (count < i) {
			tmp = tmp->Next();
			count += 1;
		}
		return tmp->Data();
	}

	Type& at(int i) {
		if (i < 0 || i >= size_) throw - 1;
		int count{};
		Node* tmp = start_;
		while (count < i) {
			tmp = tmp->Next();
			count += 1;
		}
		return tmp->Data();
	}



	const Type& at(int i) const {
		if (i < 0 || i >= size_) throw - 1;
		int count{};
		Node* tmp = start_;
		while (count < i) {
			tmp = tmp->Next();
			count += 1;
		}
		return tmp->Data();
	}

	// добавить новый элемент в конец списка
	void push_back(const Type& obj) {
		Node* tmp = new Node{ obj };
		if (size_ == 0) {
			start_ = tmp;
			end_ = tmp;
		}
		else {
			end_->Next(tmp);
			end_ = tmp;
		}
		size_ += 1;
	}


	// добавить новый элемент в начало списка
	void push_front(const Type& obj) {
		Node* tmp = new Node{ obj };
		if (size_ == 0) { // или if (!size)
			start_ = tmp;
			end_ = tmp;
		}
		else {
			tmp->Next(start_);
			start_ = tmp;
		}
		size_ += 1;
	}

	// уничтожить данные в конце списка
	void pop_back() {
		if (end_) {
			auto tmp = start_;
			// цикл поиска предпоследнего элемента
			while (tmp->Next() != end_) {
				tmp = tmp->Next();
			}
			// освобождение и замена последнего элемента
			delete end_;
			end_ = tmp;
			size_--;
		}
		if (!size_) {
			end_ = start_;
		}
	}
	
	
	// уничтожить данные в начале списка
	void pop_front() {
		if (start_) {
			auto tmp = start_;
			start_ = start_->Next();
			delete tmp;
		size_--;
		}
		if (!size_) {
			end_ = start_;
		}
	}

	/*
	 emplace (новое)/insert (копия) - это методы новых данных в произвольное место коллекции
	 erise - это метод для уничтожения данных в произвольной точке коллекции
	*/

	//int size()const  // можно удалить


	int size()const; {
		return size_;
	}


	/*
	 методы для прохода по коллекции умными указателями мы пока сознательно опустим
	*/

private:
	class Node {
	public:
		Node() :Node(Type()) {} // при делигации мы вызываем конструктор по умолчанию хранимого типа данных.
		// Данный синтаксис корректно отрабатывает даже если прямой вызов такого метода 
		// (например конструктора встроенных типов) недоступен.
		Node(const Type& data, Node* place = nullptr) :data_(data), next_(place) {}

		Type& Data() {
			return data_;
		}
		const Type& Data() const {
			return data_;
		}

		Node* Next() const { // предоставление информации для информации о местоположении следующей точки
			return next_;
		}

		void Next(Node* next) { // устанока нового значения для следующей точки
			next_ = next;
		}

	private:
		Type data_;
		Node* next_;


		Node* start_;
		Node* end_;
		int size_;
	};

};

int main() {
	setlocale(LC_ALL, "rus");
	int arr[10];
	std::vector<int>vec;
	LinkedList<int> list;
	arr;
	for (int i = 0; i < 1000; i++) {
		vec.push_back(i + 1);
	}
	arr;
	for (int i = 0; i < 1000; i++) {
		list.push_back(i + 1);
	}
	arr;
	for (int i = 0; i < vec.size(); i++) {
		vec[1];
	}
	arr;
	for (int i = 0; i < list.size(); i++) {
		list[i];
	}
	arr;



	arr[5] = 34;
	vec[5] = 23;
	list[5] = 12;
	std::size(vec);
	std::size(list);

	/*
	for (переменная для хранения элементов : коллекция элементы которой нужно перебрать) { }
	*/


	// for для перебора коллекций. 
	/*for (int& e : arr) {
		std::cout << e << ' ';
	}

	for (auto& e : vec) {
		std::cout << e << ' ';
	}*/

	// std::chrono::duration<float> time_length;






















	return 0;
}