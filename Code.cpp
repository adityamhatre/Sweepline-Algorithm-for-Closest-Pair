/*
References:
	Sweepline Algorithm for Closest Pair	:	http://people.scs.carleton.ca/~michiel/lecturenotes/ALGGEOM/sweepclosestpair.pdf
	Brute Force Algorith for Closest Pair	:	http://rosettacode.org/wiki/Closest-pair_problem#C.2B.2B
	Treaps code								:	http://www.sanfoundry.com/cpp-program-implement-treap/
*/
#include "stdafx.h"
#include <algorithm>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;


int  r, l, n, numberOfLRotations, numberOfRRotations;
float h, tempH;
int maxBSTNodes = 0;

class Point {
private:int x, y;
public:Point() {};
	   Point(int x, int y) { Point::x = x;; Point::y = y; }
	   int getX() { return x; }
	   int getY() { return y; }
};
Point *a;
typedef struct ctreenode *ctree;
struct ctreenode
{
	int		priority;
	Point	key;
	ctree	left, right;
};
ctree nullnode, root;

class CTree
{
public:
	void initialize();
	void sigrotl(ctree &);
	void sigrotr(ctree &);
	void insert(ctree &, Point);
	void remove(ctree &, Point);
	ctree findInorderSuccessor(ctree);
	ctree find(int);
	CTree()
	{}
}y;

void CTree::initialize()
{
	nullnode = new ctreenode;
	nullnode->left = nullnode->right = nullnode;
	root = nullnode;
}
void CTree::sigrotl(ctree& k1)
{
	ctree k2;
	k2 = k1->right;
	k1->right = k2->left;
	k2->left = k1;
	k1 = k2;
	numberOfLRotations++;
}
void CTree::sigrotr(ctree& k1)
{
	ctree k2;
	k2 = k1->left;
	k1->left = k2->right;
	k2->right = k1;
	k1 = k2;
	numberOfRRotations++;
}
void CTree::insert(ctree& t, Point x)
{
	if (t == nullnode)
	{
		t = new ctreenode;
		t->left = t->right = nullnode;
		t->key = x;
		t->priority = rand();
	}
	else
	{
		if (t->key.getY() == x.getY())
		{
			return;
		}
		else
		{
			if (x.getY() <= t->key.getY())
			{
				insert(t->left, x);
				if (t->left->priority > t->priority)
					sigrotr(t);
			}
			else
			{
				insert(t->right, x);
				if (t->right->priority > t->priority)
					sigrotl(t);
			}
		}
	}
}

void CTree::remove(ctree& t, Point x)
{
	if (t == nullnode)
		return;
	if (x.getY() > t->key.getY())
		remove(t->right, x);
	else if (x.getY() < t->key.getY())
		remove(t->left, x);
	else
	{
		if (t->left == nullnode && t->right == nullnode)
		{
			delete t;
			t = nullnode;
		}
		else if (t->left == nullnode)
		{
			ctree tmp = t;
			t = t->right;
			delete tmp;
		}
		else if (t->right == nullnode)
		{
			ctree tmp = t;
			t = t->left;
			delete tmp;
		}
		else
		{
			if (t->left->priority < t->right->priority)
			{
				sigrotl(t);
				remove(t->left, x);
			}
			else
			{
				sigrotr(t);
				remove(t->right, x);
			}
		}
	}
}

ctree CTree::find(int y)
{
	ctree closest_node = nullnode;
	ctree temp = root;
	int min_distance = 999999;
	while (temp != nullnode)
	{
		long int distance = abs(temp->key.getY() - y);
		if (distance < min_distance) {
			min_distance = distance;
			closest_node = temp;
		}
		if (distance == 0)
			break;
		if (temp->key.getY() > y)
			temp = temp->left;
		else if (temp->key.getY() < y)
			temp = temp->right;
	}
	return closest_node;
}

ctree CTree::findInorderSuccessor(ctree temp)
{
	// if node has no successors
	if (temp == nullnode) {
		return nullnode;
	}
	// if root wasn't set with a value
	if (root->priority == 0)
		return nullnode;

	ctree current = nullnode;
	// If we have a right child get the next element via an inorder Traversal
	if (temp->right != nullnode) {
		current = temp->right;
		while (current->left != nullnode)
			current = current->left;
		return current;
	}

	ctree successor = nullnode;
	current = root;
	while (current->key.getY() != temp->key.getY()) {
		if (current->key.getY() > temp->key.getY()) {
			successor = current;
			current = current->left;
		}
		else
			current = current->right;
	}
	return successor;
}

int compareFunction(const void* a, const void* b) {
	return (((Point*)a)->getX() - ((Point*)b)->getX());
}
bool removeUnique(Point a, Point b) {
	return (a.getX() == b.getX() && a.getY() == b.getY());
}

float d(Point a, Point b) {
	return sqrt(pow(b.getX() - a.getX(), 2) + pow(b.getY() - a.getY(), 2));
}

Point p1, p2;
int currentNodes = 0;
int diffNodes = 0;
void fast_closest_pair() {
	h = d(a[0], a[1]);
	r = 2;
	Point p = a[r];
	l = 0;
	while (a[l].getX() <= (p.getX() - h)) {
		l++;
	}
	y.initialize();
	for (int i = l; i < r; i++) {
		y.insert(root, a[i]);
		currentNodes++;
	}
	maxBSTNodes = currentNodes;

	while (r < n) {
		tempH = h;
		ctree arr[6];
		arr[0] = y.find(p.getY() - h);
		for (int i = 1; i < 6; i++) {
			arr[i] = y.findInorderSuccessor(arr[i - 1]);
		}

		for (int i = 0; i < 6; i++) {
			if (arr[i] != nullnode) {
				tempH = min(tempH, d(arr[i]->key, p));
				if (tempH < h) {
					p1 = arr[i]->key;
					p2 = p;
					h = tempH;
				}
			}
		}
		y.insert(root, p);
		diffNodes++;
		if (r < n - 1) {
			p = a[r + 1];
			while (a[l].getX() <= p.getX() - h) {
				y.remove(root, a[l]);
				diffNodes--;
				l++;
			}
		}
		currentNodes = currentNodes + diffNodes;
		if (currentNodes > maxBSTNodes) {
			maxBSTNodes = currentNodes;
		}
		diffNodes = 0;
		r++;
	}
}

void bruteForce() {
	if (n < 2) {
		return;
	}
	else {
		h = d(a[0], a[1]);
		p1 = a[0];
		p2 = a[1];
		for (int i = 0; i < (n - 1); ++i) {
			for (int j = i + 1; j < n; ++j) {
				float newH = d(a[i], a[j]);
				if (newH < h) {
					h = newH;
					p1 = a[i];
					p2 = a[j];
				}
			}
		}
	}
}
int main()
{
	srand(time(NULL));
	char working_directory[MAX_PATH + 1];
	GetCurrentDirectoryA(sizeof(working_directory), working_directory); 
	string cd = working_directory;
	cd = cd + "\\file3.dat";
	ifstream input(cd);
	if (!input) {
		cout << "not found" << endl;
		system("pause");
		return 0;
	}

	string line;
	getline(input, line);
	n = stoi(line);
	a = new Point[n];
	int x, y;
	int i = 0;
	while (input >> x >> y) {
		a[i] = Point(x, y);
		i++;
	}
	/*cin >> n;
	a = new Point[n];
	for (int i = 0; i < n; i++)
	{
		int x, y;
		cin >> x;
		cin >> y;
		a[i] = Point(x, y);
	}*/
	cout << "Sorting " << n << endl;

	clock_t beforeSort = clock();
	qsort(a, n, sizeof(Point), compareFunction);
	clock_t afterSort = clock();
	cout << "qsort time " << float(afterSort - beforeSort) / float(CLOCKS_PER_SEC) << endl;


	cout << "duplicate removal" << endl;
	Point *d = unique(a, a + n, removeUnique);

	n = d - a;

	clock_t beforeSweepTime = clock();
	fast_closest_pair();
	clock_t afterSweepTime = clock();
	cout << "closest pair (" << p2.getX() << ", " << p2.getY() << ") (" << p1.getX();
	cout << ", " << p1.getY() << ") at dist**2 " << (h*h) << endl;
	cout << "sweep time " << (float)(afterSweepTime - beforeSweepTime) / CLOCKS_PER_SEC << endl;

	cout << "max BST nodes " << maxBSTNodes << endl;
	cout << "Rotations: " << numberOfLRotations << " with left " << numberOfRRotations << " with right" << endl;


	if (n > 2 && n <= 40000) {
		clock_t beforeBFTime = clock();
		bruteForce();
		clock_t afterBFTime = clock();
		cout << "closest points (" << p2.getX() << ", " << p2.getY() << ") (" << p1.getX();
		cout << ", " << p1.getY() << ") at dist**2 " << (h*h) << endl;
		cout << "brute force time " << (float)(afterBFTime - beforeBFTime) / CLOCKS_PER_SEC << endl;
	}



	system("pause");
	return 0;
}