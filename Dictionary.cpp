/***
* Brian Tran
* btran54
* 2024 Winter CSE101 PA7
* Dictionary.cpp
* Dictionary CPP file containing the operation of functions.
***/

#include<iostream>
#include<string>
#include<stdexcept>
#include <stack>

#include"Dictionary.h"

Dictionary::Node::Node(keyType k, valType v){
	key = k;
	val = v;
	parent = nullptr;
	left = nullptr;
	right = nullptr;
}

// Helper Functions (Optional) ---------------------------------------------

void Dictionary::inOrderString(std::string& s, Node* R) const {
	if (R != nil) {
		inOrderString(s, R->left);
		//s += R->key + " : " + std::to_string(R->val) + "\n";
		s.append(R->key + " : " + std::to_string(R->val) + "\n");
		inOrderString(s, R->right);
	}
}

void Dictionary::preOrderString(std::string& s, Node* R) const {
	if (R != nil) {
		//s += R->key + "\n";
		s.append(R->key + "\n");
		preOrderString(s, R->left);
		preOrderString(s, R->right);
	}
}

void Dictionary::preOrderCopy(Node* R, Node* N) {
	if (R != N) {
		this->setValue(R->key, R->val);
		preOrderCopy(R->left, N);
		preOrderCopy(R->right, N);
	}
}

void Dictionary::postOrderDelete(Node* R) {
	if (R != nil) {
		postOrderDelete(R->left);
		postOrderDelete(R->right);
		remove(R->key);
	}
}

Dictionary::Node* Dictionary::search(Node* R, keyType k) const {
	if (R == nil || k == R->key) {
		return R;
	}

	else if (k < R->key) {
		return search(R->left, k);
	}

	else {
		return search(R->right, k);
	}
}

Dictionary::Node* Dictionary::findMin(Node* R) {
	if (R == nil) {
		return nil;
	}

	else if (R != nil) {

		while(R->left != nil) {
			R = R->left;
		}
	}

	return R;
}

Dictionary::Node* Dictionary::findMax(Node* R){
	if (R == nil) {
		return nil;
	}

	else if (R != nil) {

		while(R->right != nil) {
			R = R->right;
		}
	}

	return R;
}

Dictionary::Node* Dictionary::findNext(Node* N){
	Node* node = N->parent;

	if (N == nil) {
		return nil;
	}

	else if (N->right != nil) {
		return findMin(N->right);
	}
	
	while (node != nil && N == node->right) {
		N = node;
		node = node->parent;
	}

	return node;
}

Dictionary::Node* Dictionary::findPrev(Node* N) {
	Node* node = N->parent;

	if (N->left != nil) {
		return findMax(N->left);
	}
	
	while (node != nil && N == node->left) {
		N = node;
		node = node->parent;
	}
	
	return node;
}

// transplant()
void Dictionary::transplant(Node* R, Node* N) {
	if (R->parent == nil) {
		root = N;
	}

	else if (R == R->parent->left) {
		R->parent->left = N;
	}

	else {
		R->parent->right = N;
	}

	if (N != nil) {
		N->parent = R->parent;
	}
}

// Class Constructors & Destructors ----------------------------------------

Dictionary::Dictionary() {
	nil = new Node("nil", -1);

	nil->left = nil;
	nil->right = nil;
	nil->parent = nil;
	
	root = nil;
	root->left = nil;
	root->right = nil;
	root->parent = nil;

	current = nil;
	num_pairs = 0;
}

Dictionary::Dictionary(const Dictionary& D) {
	nil = new Node("\000", -1);

	nil->left = nil;
	nil->right = nil;
	nil->parent = nil;
	
	root = nil;
	root->left = nil;
	root->right = nil;
	root->parent = nil;

	current = nil;
	num_pairs = 0;
	
	preOrderCopy(D.root, D.nil);
}

Dictionary::~Dictionary() {
	clear();
	delete(nil);
	
}

// Access functions --------------------------------------------------------

int Dictionary::size() const {
	if (num_pairs < 0) {
		throw std::logic_error("Dictionary::size(): Invalid size.\n");
	}

	else {
		return num_pairs;
	}
}

bool Dictionary::contains(keyType k) const {
	Node *rootSearch = search(root, k);

	if(rootSearch->key == k) {
		return true;
	}

	else {
		return false;
	}
}

valType& Dictionary::getValue(keyType k) const {
	if ((this->contains(k)) == false) {
		throw std::logic_error("Dictionary::getValue(): key " + k + " DNE.\n");
	}
	
	else {
		Node* rootSearch = search(root, k);

		return rootSearch->val;
	}
}
	
bool Dictionary::hasCurrent() const {
	if (current != nil) {
		return true;
	}

	else {
		return false;
	}
}
	
keyType Dictionary::currentKey() const {
	if (hasCurrent() == true) {
		return current->key;
	}

	else {
		throw std::logic_error("Dictionary::currentKey(): Current is undefined.\n");
	}
}
	
valType& Dictionary::currentVal() const {
	if (hasCurrent() == true) {
		return current->val;
	}

	else {
		throw std::logic_error("Dictionary::currentVal(): Current is undefined.\n");
	}		
}

// Manipulation procedures -------------------------------------------------

void Dictionary::clear() {
	postOrderDelete(root);
	root = nil;
	num_pairs = 0;
	current = nil;
}
	
void Dictionary::setValue(keyType k, valType v) {
    Node* parentNode = nil;
    Node* successor = root;

    while (successor != nil) {
        parentNode = successor;

        if (k == parentNode->key) {
            parentNode->val = v;
            return;

        } 

		else if (k < parentNode->key) {
            successor = parentNode->left;
        } 
		
		else {
            successor = parentNode->right;
        }
    }

    Node* newNode = new Node(k, v);

    newNode->parent = parentNode;
    newNode->left = nil;
    newNode->right = nil;
    num_pairs++;

    if (parentNode == nil) {
        root = newNode;
    } 

	else if (k < parentNode->key) {
        parentNode->left = newNode;
    } 
	
	else {
        parentNode->right = newNode;
    }
}
   
void Dictionary::remove(keyType k) {
    if (!contains(k)) {
        throw std::logic_error("Dictionary::remove(): key " + k + " does not exist.\n");
    }

    Node* rootNode = search(root, k);

    if (rootNode->left == nil || rootNode->right == nil) {
        Node* child = (rootNode->right == nil) ? rootNode->left : rootNode->right;

        if (current == rootNode) {
            current = nil;
        }

        transplant(rootNode, child);

    } 

	else {
        Node* successor = findMin(rootNode->right);

        if (successor->parent != rootNode) {
            transplant(successor, successor->right);
            successor->right = rootNode->right;
            successor->right->parent = successor;
        }

        transplant(rootNode, successor);
        successor->left = rootNode->left;
        successor->left->parent = successor;
    }

    num_pairs--;
}
   
void Dictionary::begin(){
	if (num_pairs > 0) {
		Node* rootNode = findMin(root);
		current = rootNode;
	}

	else {
		current = nil;
	}
}

void Dictionary::end(){
	if (num_pairs > 0) {
		Node* rootNode = findMax(root);
		current = rootNode;
	}

	else {
		current = nil;
	}
}

void Dictionary::next(){
	if (hasCurrent() == false) {
		throw std::logic_error("Dictionary::next(): Current undefined.\n");
	}

	if (hasCurrent() == true) {
		Node* rootNode = findMax(root);

		if (current != rootNode) {
			current = findNext(current);
		}

		else if (current == rootNode) {
			current = nil;
		}
	}
}

void Dictionary::prev() {
	if (hasCurrent() == false) {
		throw std::logic_error("Dictionary::prev(): Current undefined.\n");
	}

	else if (hasCurrent() == true) {
		Node* rootNode = findMin(root);

		if (current != rootNode) {
			current = findPrev(current);
		}

		else if (current == rootNode) {
			current = nil;
		}
	}
}

// Other Functions ---------------------------------------------------------

std::string Dictionary::to_string() const {
	std::string s = "";
	s.reserve(20);
	inOrderString(s, root);
	
	return s;
}

std::string Dictionary::pre_string() const {
	std::string s = "";
	s.reserve(10);
	preOrderString(s, root);

	return s;
}

bool Dictionary::equals(const Dictionary& D) const {
    if (num_pairs != D.num_pairs) {
        return false;
    }

    std::stack<Node*> stackA, stackB;
    Node* currA = root, *currB = D.root;

    while (currA != nil || currB != nil || !stackA.empty() || !stackB.empty()) {

        while (currA != nil && currB != nil) {
            stackA.push(currA);
            stackB.push(currB);
            currA = currA->left;
            currB = currB->left;
        }

        if (currA != currB) {
            return false;
        }

        currA = stackA.top();
        stackA.pop();
        currB = stackB.top();
        stackB.pop();

        if (currA->key != currB->key || currA->val != currB->val) {
            return false;
        }

        currA = currA->right;
        currB = currB->right;
    }

    return true;
}

// Overloaded Operators ----------------------------------------------------

std::ostream& operator<<( std::ostream& stream, Dictionary& D ) {
	return stream << D.to_string();
}

bool operator==( const Dictionary& A, const Dictionary& B ) {
	if (A.num_pairs == 0 && B.num_pairs == 0) {
		return true;
	}

	return A.equals(B);
}

Dictionary& Dictionary::operator=( const Dictionary& D ) {
	if (this != &D) {
		Dictionary temp = D;

		std::swap(nil, temp.nil);
		std::swap(root, temp.root);
		std::swap(num_pairs, temp.num_pairs);
		std::swap(current, temp.current);
	}

	return *this;
}