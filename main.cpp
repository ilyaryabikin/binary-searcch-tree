#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

static ofstream fout("tree.gv");
static int inc = 0;

template <typename Type>
class BinarySearchTree {
private:
	Type key;
	int height;
	BinarySearchTree<Type>* left;
	BinarySearchTree<Type>* right;
public:
	BinarySearchTree(Type k): key(k), left(nullptr), right(nullptr), height(1) {}

	~BinarySearchTree() {
		delete left;
		delete right;
	}

	Type getKey() const {
		return key;
	}

	Type getLeft() const {
		return left->key;
	}

	Type getRight() const {
		return right->key;
	}

	BinarySearchTree<Type>* getRightPtr() const {
		return right;
	}

	BinarySearchTree<Type>* getLeftPtr() const {
		return left;
	}

	int subtreeHeight() {
		return this ? height : 0;
	}

	int getBalance() {
		return right->subtreeHeight() - left->subtreeHeight();
	}

	void correctHeight() {
		int heightLeft = left->subtreeHeight();
		int heightRight = right->subtreeHeight();
		height = (heightLeft > heightRight ? heightLeft : heightRight) + 1;
	}

	BinarySearchTree<Type>* rotateRight() {
		BinarySearchTree<Type>* p = left;
		left = p->right;
		p->right = this;
		correctHeight();
		p->correctHeight();
		return p;
	}

	BinarySearchTree<Type>* rotateLeft() {
		BinarySearchTree<Type>* p = right;
		right = p->left;
		p->left = this;
		correctHeight();
		p->correctHeight();
		return p;
	}

	BinarySearchTree<Type>* balance() {
		correctHeight();
		if(getBalance() == 2) {
			if(right->getBalance() < 0) {
				right = right->rotateRight();
			}
			return rotateLeft();
		}
		if(getBalance() == -2) {
			if(left->getBalance() > 0) {
				left = left->rotateLeft();
			}
			return rotateRight();
		}
		return this;
	}

	BinarySearchTree<Type>* insert(Type k) {
		if(this == nullptr) {
			return new BinarySearchTree(k);
		}
		if(k < key) {
			left = left->insert(k);
		}
		else if(k > key) {
			right = right->insert(k);
		}
		else {
			cout << "Вершина уже есть в дереве" << endl;
			return this;
		}
		return balance();
	}

	BinarySearchTree<Type>* searchNode(Type k) {
		if (this != nullptr) {
			if(key == k) {
				return this;
			}
			else if (k < key) {
				left->searchNode(k);
			}
			else if (k > key) {
				right->searchNode(k);
			}
		}
		return nullptr;
	}

	void writeInFileNodes() {
		if (this != nullptr) {
			if (left != nullptr) {
				fout << "\t\"" << key << "\" -> \"" << left->key << "\"" << ";\n";
			}
			else {
				fout << "\t\"" << "null" << inc << "\" " << "[shape=point]" << ";\n";
				fout << "\t\"" << key << "\" -> \"" << "null" << inc++ << "\"" << ";\n";
			}
			if (right != nullptr) {
				fout << "\t\"" << key << "\"  -> \"" << right->key << "\"" << ";\n";
			}
			else {
				fout << "\t\"" << "null" << inc << "\" " << "[shape=point]" << ";\n";  
				fout << "\t\"" << key << "\" -> \"" << "null" << inc++ << "\"" << ";\n";
			}
			left->writeInFileNodes();
			right->writeInFileNodes();
		}
	}

	void writeInFile() {
		fout.open("tree.gv");
		fout << "digraph BinarySearchTree {\n";
		writeInFileNodes();
		fout << "}";
		fout.close();
	}
};

template<typename Type>
void menu(BinarySearchTree<Type>*& p) {
	Type key;
	int size;
	bool choice;
	cout << "Введите количество вершин: ";
	cin >> size;
	for (int i = 0; i < size; i++) {
		cout << "Введите ключ вершины: ";
		cin >> key;
		p = p->insert(key);
		p->writeInFile();
		system("./buildScript.sh");
	}
	cout << "Осуществить поиск? (1=да, 0=нет) ";
	cin >> choice;
	if (choice) {
		Type a;
		cout << "Введите ключ вершины, которую нужно найти: ";
		cin >> a;
		BinarySearchTree<Type>* q = p->searchNode(a);
		if(q != nullptr) {
			cout << "Вершина найдена!\n" << "Ключ вершины: " << q->getKey();
			if(q->getLeftPtr() != nullptr) {
				cout << " Левый потомок: " << q->getRight();
			}
			if(q->getRightPtr() != nullptr) {
				cout << " Правый потомок: " << q->getLeft() << endl; 
			}
		}
		else {
			cout << "Вершина не найдена :(" << endl;
		}
	}
}

int main() {
	srand(time(NULL));
	int choice;
	cout << "Выберите тип данных:\n" << "1. int\n" << "2. double\n" << "3. string\n";
	cin >> choice;
	if (choice == 1) {
		BinarySearchTree<int>*p = nullptr;
		menu(p);
	}
	else if (choice == 2) {
		BinarySearchTree<double>*p = nullptr;
		menu(p);
	}
	else if (choice == 3) {
		BinarySearchTree<string>*p = nullptr;
		menu(p);
	}
	else {
		cout << "Неподдерживаемый тип данных!";
	}
	return 0;
} 