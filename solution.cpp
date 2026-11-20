#include <iostream>
#include <limits>
#include <string>

enum Color { RED, BLACK };

struct Node {
    int key;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    Node(int k, Color c, Node* nil)
        : key(k), color(c), left(nil), right(nil), parent(nil) {}
};

class RedBlackTree {
private:
    Node* root;
    Node* nil;

    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != nil) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nil) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != nil) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nil) {
            root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    void insertFixup(Node* z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* uncle = z->parent->parent->right;
                if (uncle->color == RED) {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                Node* uncle = z->parent->parent->left;
                if (uncle->color == RED) {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void transplant(Node* u, Node* v) {
        if (u->parent == nil) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    Node* minimum(Node* node) const {
        while (node->left != nil) {
            node = node->left;
        }
        return node;
    }

    void deleteFixup(Node* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    Node* findNode(int key) const {
        Node* current = root;
        while (current != nil && key != current->key) {
            current = (key < current->key) ? current->left : current->right;
        }
        return current;
    }

    void printTree(Node* node, int depth) const {
        if (node == nil) {
            return;
        }
        printTree(node->right, depth + 1);
        std::cout << std::string(depth * 6, ' ') << node->key
                  << (node->color == RED ? "(R)" : "(B)") << "\n";
        printTree(node->left, depth + 1);
    }

    void printInorder(Node* node) const {
        if (node == nil) {
            return;
        }
        printInorder(node->left);
        std::cout << node->key << (node->color == RED ? "(R) " : "(B) ");
        printInorder(node->right);
    }

    void destroy(Node* node) {
        if (node == nil) {
            return;
        }
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    RedBlackTree() {
        nil = new Node(0, BLACK, nullptr);
        nil->left = nil;
        nil->right = nil;
        nil->parent = nil;
        root = nil;
    }

    RedBlackTree(const RedBlackTree&) = delete;
    RedBlackTree& operator=(const RedBlackTree&) = delete;

    ~RedBlackTree() {
        destroy(root);
        delete nil;
    }

    bool insert(int key) {
        Node* parent = nil;
        Node* current = root;
        while (current != nil) {
            parent = current;
            if (key == current->key) {
                return false;
            }
            current = (key < current->key) ? current->left : current->right;
        }
        Node* z = new Node(key, RED, nil);
        z->parent = parent;
        if (parent == nil) {
            root = z;
        } else if (key < parent->key) {
            parent->left = z;
        } else {
            parent->right = z;
        }
        insertFixup(z);
        return true;
    }

    bool search(int key) const {
        return findNode(key) != nil;
    }

    bool remove(int key) {
        Node* z = findNode(key);
        if (z == nil) {
            return false;
        }
        Node* y = z;
        Node* x;
        Color yOriginalColor = y->color;
        if (z->left == nil) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == nil) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (yOriginalColor == BLACK) {
            deleteFixup(x);
        }
        nil->parent = nil;
        return true;
    }

    void print() const {
        if (root == nil) {
            std::cout << "Дерево пусто\n";
            return;
        }
        std::cout << "Структура дерева (повернуто на 90 градусов, корень слева):\n";
        printTree(root, 0);
        std::cout << "Обход по возрастанию: ";
        printInorder(root);
        std::cout << "\n";
    }
};

int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        }
        if (std::cin.eof()) {
            return 0;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Введите целое число\n";
    }
}

int main() {
    RedBlackTree tree;
    while (true) {
        std::cout << "\n1 - Вставить\n2 - Удалить\n3 - Найти\n4 - Вывести\n0 - Выход\n";
        if (std::cin.eof()) {
            break;
        }
        int choice = readInt("Выбор: ");
        if (choice == 0) {
            break;
        }
        if (choice == 1) {
            int key = readInt("Ключ: ");
            std::cout << (tree.insert(key) ? "Вставлено\n" : "Такой ключ уже есть\n");
        } else if (choice == 2) {
            int key = readInt("Ключ: ");
            std::cout << (tree.remove(key) ? "Удалено\n" : "Ключ не найден\n");
        } else if (choice == 3) {
            int key = readInt("Ключ: ");
            std::cout << (tree.search(key) ? "Найден\n" : "Не найден\n");
        } else if (choice == 4) {
            tree.print();
        } else {
            std::cout << "Неверный пункт меню\n";
        }
    }
    return 0;
}
