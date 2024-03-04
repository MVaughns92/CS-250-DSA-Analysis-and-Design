//======================================================================
// Name         :   Project2.cpp
// Author       :   Molly Vaughns Feb 24, 2024
// Version      :   1.0
// Copyright    :   Copyright © 2017 SNHU COCE
//======================================================================


#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <Windows.h>
#include <vector>

#include "CSVparser.hpp"

using namespace std;

const int GLOBAL_SLEEP_TIME = 5000; // Set default time for sleep

// Create structure to hold course information

struct Course {

	// Include course information - course ID, course name, and course prereq
	string courseId;
	string courseName;
	vector<string> preList;
};

// Create Binary Search Tree class

class BinarySearchTree {

	// Create private class(es)
private:
	// Define structures to hold courses
	struct Node {
		Course course;
		Node* right;
		Node* left;

		// Default constructor, set both right and left node to null
		Node() {
			left = nullptr;
			right = nullptr;
		}

		// Initialize with a course
		Node(Course aCourse) {
			course = aCourse;
			left = nullptr;
			right = nullptr;
		}
	};

	// Set root node int
	Node* root;
	void inOrder(Node* node);
	int size = 0;

	// Create public class(es)
public:
	BinarySearchTree();
	void inOrder();
	void Insert(Course aCourse);
	Course Search(string courseId);
	int Size();
};

// Default constructor
BinarySearchTree::BinarySearchTree() {
	this->root = nullptr;
}

// Traverse the tree in order
void BinarySearchTree::inOrder() {
	inOrder(root);
}

// Create method to insert a course
void BinarySearchTree::Insert(Course aCourse) {

	Node* currentNode = root;

	// If root is NULL, add root node
	if (root == NULL) {
		root = new Node(aCourse);
	}

	// Else while current node is NOT set to null
	else {
		while (currentNode != NULL) {
			// If node is less than root, add to left 
			if (currentNode->left == nullptr) {

				// If no left node, node becomes left
				currentNode->left = new Node(aCourse);
				currentNode = NULL;
			}
			else {

				currentNode = currentNode->left;
			}
		}
		while (currentNode != NULL) {
			// If current node is greater than root, add to right
			if (currentNode->right == nullptr) {

				// If no right node, node becomes right
				currentNode->right = new Node(aCourse);
				currentNode = NULL;
			}
			else {
				currentNode = currentNode->right;
			}
		}
	}
	size++;
}


// Create method to search for a course
Course BinarySearchTree::Search(string courseId) {

	Course aCourse;

	Node* currentNode = root;

	while (currentNode != NULL) {

		if (currentNode->course.courseId == courseId) {

			return currentNode->course;
		}
		else if (courseId < currentNode->course.courseId) {

			currentNode = currentNode->right;
		}
	}

	cout << "Value not found." << endl;
	return aCourse;
}

// Create method to print sorted list
void BinarySearchTree::inOrder(Node* node) {

	if (node == NULL) {

		return;
	}
	inOrder(node->left);

	// Print node
	cout << node->course.courseId << ", " << node->course.courseName << endl;

	inOrder(node->right);
}

int BinarySearchTree::Size() {

	return size;
}

vector<string> Split(string lineFeed) {

	char delim = ',';

	lineFeed += delim; // Include delimiter at end so last word is also read
	vector<string> lineTokens;
	string temp = "";
	for (int i = 0; i < lineFeed.length(); i++) {
		if (lineFeed[i] == delim) {
			lineTokens.push_back(temp); // Store words in token vector
			temp = "";
			i++;
		}
		temp += lineFeed[i];
	}
	return lineTokens;
}

// Create method to load courses
void loadCourses(string csvPath, BinarySearchTree* courseList) {

	// Create data structure and add to colection of courses
	ifstream inFS; // insteam to read file
	string line; // line feed
	vector<string> stringTokens;

	inFS.open(csvPath); // Open read fie

	if (!inFS.is_open()) { // Error handler
		cout << "Could not open file. Please check inputs." << endl;
		return;
	}

	while (!inFS.eof()) {

		Course aCourse; // Create a structure for each line

		getline(inFS, line);
		stringTokens = Split(line); // Split line into tokens via delimiter

		if (stringTokens.size() < 2) { // If there are not 2 tokens, line is misformatted

			cout << "\nError. Skipping line." << endl;

		}
		else{
			aCourse.courseId = stringTokens.at(0);
			aCourse.courseName = stringTokens.at(1);

			for (unsigned int i = 2; i < stringTokens.size(); i++) {

				aCourse.preList.push_back(stringTokens.at(i));
			}

			// Push course to the end
			courseList->Insert(aCourse);
		}
	}

	inFS.close(); // Closes file
}

// Create method to show course and prereqs
void displayCourse(Course aCourse) {

	cout << aCourse.courseId << ", " << aCourse.courseName << endl;
	cout << "Prerequisites: ";

	if (aCourse.preList.empty()) { // If list is empty, there are no prereq

		cout << "None" << endl;
	}
	else {

		for (unsigned int i = 0; i < aCourse.preList.size(); i++) {

			cout << aCourse.preList.at(i);

			if (aCourse.preList.size() > 1 && i < aCourse.preList.size() - 1) {
				// Add comma for elements greater than 1
				cout << ", ";
			}
		}
	}

	cout << endl;
}

// Create method to force case of any string to uppercase
void convertCase(string & toConvert) {

	for (unsigned int i = 0; i < toConvert.length(); i++) {

		if (isalpha(toConvert[i])) {

			toConvert[i] = toupper(toConvert[i]);
		}
	}
}

int main(int argc, char* argv[]) {

	// Process command line arguments
	string csvPath, aCourseKey;

	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		aCourseKey = argv[2];
		break;
	default:
		csvPath = "Project2_Sample_input.csv"; // Look for csv file of course catalog
	}

	// Define table to hold all courses
	BinarySearchTree* courseList = new BinarySearchTree();

	Course course;
	bool goodInput;
	int choice = 0;

	// Create menu of choices
	while (choice != 9) {
		cout << "Menu:" << endl;
		cout << " 1. Load Data Structure" << endl;
		cout << " 2. Print Course List" << endl;
		cout << " 3. Print Course" << endl;
		cout << " 9. Exit" << endl;
		cout << "What would you like to do? ";

		aCourseKey = ""; // Clear string
		string anyKey = " "; // Clear string
		choice = 0; // Clear choice 
		
		// Allow for user input
		try {
			cin >> choice;

			if ((choice > 0 && choice < 5) || (choice == 9)) { // Limit user menu inputs to good values
				goodInput = true;
			}
			else { // Throw error for catch
				goodInput = false;
				throw 1;			
			}

			// Create switch case for all valid menu options
			switch (choice) {
			case 1:

				// Complete method call to load courses
				loadCourses(csvPath, courseList);
				cout << courseList->Size() << " courses read" << endl;

				Sleep(GLOBAL_SLEEP_TIME);

				break;

			// List courses
			case 2:
				courseList->inOrder();

				cout << "\nEnter \'y\' to continue..." << endl;

				cin >> anyKey;

				break;

			// Show specific course
			case 3:

				cout << "\nWhat course would you like to see? " << endl;
				cin >> aCourseKey; 

				convertCase(aCourseKey); // Convert case of user input

				course = courseList->Search(aCourseKey);

				if (!course.courseId.empty()) {
					displayCourse(course);
				}
				else {
					cout << "\nCourse ID " << aCourseKey << " not found." << endl;
				}

				Sleep(GLOBAL_SLEEP_TIME);

				break;

			// Exit program
			case 9:
				exit;
				break;

			default:

				throw 2;
			}
		}

		// Create catch for invalid input
		catch (int err) {

			std::cout << "\nThat is not a valid option. Please check input. " << endl;
			Sleep(GLOBAL_SLEEP_TIME);
		}

		// Clear cin operator of extra input, such as '9 9' or errors generated by badinput
		cin.clear();
		cin.ignore();

		// Clear console to redraw fresh menu
		system("cls");
	}

	// Print goodbye message after pressing '9'
	cout << "Thank you for using the course planner!" << endl;

	Sleep(GLOBAL_SLEEP_TIME);

	return 0;
}