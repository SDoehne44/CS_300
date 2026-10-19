/* CS 300 Project Two
** Samantha Doehne
** 10/18/2026
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

using namespace std;


// A structure to hold Course objects.
struct Course {
	string courseNumber;
	string name;
	vector<string> prereqList;
	Course();
	Course(string courseNumber);
};

// A default Course constructor with no parameters.
Course::Course() {
	courseNumber = "";
	name = "none";
	prereqList = {};
}

// Course constructor with courseNumber parameter.
Course::Course(string courseNumber) {
	this->courseNumber = courseNumber;
	name = "none";
	prereqList = {};
}

// A structure to create a node that holds a course
// and the pointers to the left and right child.
struct Node {
	Course course;
	Node* left;
	Node* right;
	Node();
	Node(Course course);
};

// Default Node construcor
Node::Node() {
	left = nullptr;
	right = nullptr;
}

// Node constructor that uses a Course parameter
// to initialize the node's course.
Node::Node(Course course) {
	this->course = course;
	left = nullptr;
	right = nullptr;
}

// Binary Search Tree (BST) class
class BinarySearchTree {
private:
	Node* root;
	void addNode(Node* node, Course course);
	void printAllHelper(Node* node);

public:
	BinarySearchTree();
	Node* getRoot() const;
	void insertCourse(Course course);
	Course searchCourse(string courseNumber);
	void printAll();
	void printSingleCourse(Course course);
};

/* Recursive method to add a node with a course into the Binary Search Tree.
*  Called by insertCourse wrapper to correctly place nodes alphanumerically
*  by courseNumber.
*/
void BinarySearchTree::addNode(Node* node, Course course) {
	if (node->course.courseNumber > course.courseNumber) {
		if (node->left == nullptr) {
			node->left = new Node(course);
		}
		else {
			this->addNode(node->left, course);
		}
	}
	else {
		if (node->right == nullptr) {
			node->right = new Node(course);
		}
		else {
			this->addNode(node->right, course);
		}
	}
}

// A recursive method to print the BST in order using the printAll wrapper function.
// Prints all courses with an in-order traversal sorted by courseNumber.
void BinarySearchTree::printAllHelper(Node* node) {
	if (node != nullptr) {
		printAllHelper(node->left);
		cout << node->course.courseNumber << ", " << node->course.name << endl;
		printAllHelper(node->right);
	}
}

// Binary Search Tree default constructor
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}

/* A getter method to return the root pointer. 
*  Used to check if the binary search tree is empty.
*/
Node* BinarySearchTree::getRoot() const {
	return root;
}

/* Used by the loadCourse method to insert courses into BST.
*  Calls the recursive addNode method to traverse the tree and insert new node
*  in alphanumeric order by courseNumber.
*/
void BinarySearchTree::insertCourse(Course course) {
	if (root == nullptr) {
		root = new Node(course);
	}
	else {
		addNode(root, course);
	}
}

/* Searches the BST for a course using the courseNumber.
*  Is used by the single course print function to find and return the
*  course if there is a course with a matching courseNumber.
*  Returns empty course if matching courseNumber is not found. 
*/
Course BinarySearchTree::searchCourse(string courseNumber) {
    // set current node equal to root
    Node* current = root;

    while (current != nullptr) {
        // if match found, return current course
        if (current->course.courseNumber == courseNumber) {
            return current->course;
        }
        // if courseNumber is smaller than current node then traverse left
        else if (current->course.courseNumber > courseNumber) {
            current = current->left;
        }
        // else courseNumber is larger so traverse right
        else {
            current = current->right;
        }
    }
    Course course;
    return course; 
}

/* A wrapper method that calls the recursive printAllHelper
*  to traverse the binary search tree instance in order to print in
*  alphanumeric order.
*/
void BinarySearchTree::printAll() {
	cout << "Here is a sample schedule:" << endl;
	cout << endl;
	this->printAllHelper(root);
}

/* Takes in a course object (in this case, from the searchCourse
*  method in menu option 3) and prints the course information if
*  the course is found.
*/
void BinarySearchTree::printSingleCourse(Course course) {
	if (course.courseNumber.empty()) {
		cout << "Course not found." << endl;
		cout << endl;
	}
	else {
		cout << course.courseNumber << ", " << course.name << endl;
		cout << "Prerequisites: ";
		if (course.prereqList.size() > 0) {
			for (unsigned int i = 0; i < course.prereqList.size(); ++i) {
				if (i == course.prereqList.size() - 1) {
					cout << course.prereqList.at(i) << endl;
					cout << endl;
				}
				else {
					cout << course.prereqList.at(i) << ", ";
				}
			}
		}
		else {
			cout << "None." << endl;
			cout << endl;
		}

	}
}

/* Loads courses from a csv file input by the user. 
*  Opens the file and checks to make sure it is open.
*  Parses the file to enter each line into a new course
*  object and then insert that object into the tree.
*  Returns the binary search tree.
*/
BinarySearchTree loadCourses() {
	string filename;
	ifstream file; 
	vector<string> lineValues;
	BinarySearchTree bst;

	cout << "Please enter file name that holds the course information." << endl;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	getline(cin, filename);
	cout << endl;
	
	file.open(filename);

	// If file does not open, user gets one more attempt and then routed back to menu.
	if (!file.is_open()) { 
		cout << "File failed to open. Please try again." << endl;
		cout << "Please enter file name that holds the course information." << endl;
		getline(cin,filename);
		cout << endl;

		file.clear();
		file.open(filename);

		if (!file.is_open()) {
			cout << "File failed to open. Returning to main menu." << endl;
			cout << endl;
			return bst;
		}
	}

	string line;
	while (getline(file, line)) {
		lineValues.clear();
		stringstream ss(line);
		string value;

		while (getline(ss, value, ',')) {
			lineValues.push_back(value);
		}
		if (lineValues.size() < 2) {
			cout << "Error: Line lacks sufficient information.";
			continue;
		}

		Course course;
		course.courseNumber = lineValues.at(0);
		course.name = lineValues.at(1);
		if (lineValues.size() > 2) {
			for (unsigned int j = 2; j < lineValues.size(); ++j) {
				if (!lineValues.at(j).empty()) {
					course.prereqList.push_back(lineValues.at(j));
				}
			}
		}
		bst.insertCourse(course);
	}
	file.close();
	return bst;
}

// Displays the menu options for the user.
void displayMenu() {

	cout << "  1. Load Data Structure." << endl;
	cout << "  2. Print Course List." << endl;
	cout << "  3. Print Course." << endl;
	cout << "  9. Exit" << endl;
	cout << endl;
	cout << "What would you like to do?" << endl;
}

/* A method to convert all of the alphabetical characters in a string
* to uppercase in order to match the format for the courseNumbers.
*/
string toUpper(const string& userString) {
	string formattedString = userString;
	for (char& character : formattedString) {
		character = toupper(static_cast<unsigned char>(character));
	}
	return formattedString;
}

/* Main method contains the user interface and switch statement that 
*  takes in the user's choice from the menu, validates it, and handles
*  the request.
*/
int main() {
	int userInput = 0; // Initialize the user's input to zero to start the while loop
	string courseChoice;
	BinarySearchTree bst;
	Course foundCourse;
	cout << "Welcome to the course planner." << endl;

	while (userInput != 9) {

		displayMenu(); // Display the menu

		cin >> userInput; // Take in the user's choice based on the menu
		cout << endl;

		if (!cin) { // Check to make sure cin is clear and the userInput an integer.
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "That is not a valid option. Please enter 1, 2, 3, or 9." << endl;
			cout << endl;
			continue;
		} 

		switch (userInput) {

			case 1: {
				bst = loadCourses(); // load courses into the bst instance.
				break;
			}

			case 2: { 
				// Check to see if the courses have been loaded
				if (bst.getRoot() == nullptr) {
					cout << "Please load courses with option 1." << endl;
					cout << endl;
					break;
				}
				// In-order traversal of the bst that prints all courses 
				bst.printAll();
				cout << endl;
				break;
			}

			case 3: {
				// Check to see if the courses have been loaded
				if (bst.getRoot() == nullptr) {
					cout << "Please load courses with option 1." << endl;
					cout << endl;
					break;
				}
				/* Prompt user for the course they want to know about.
				* Take in courseChoice and change the alphabetical characters to
				* upper case to match the csv format within the searchCourse call.
				*/
				cout << "What course do you want to know about?" << endl;
				cout << "Please enter the course number. Example: ABCD100" << endl;
				cin >> courseChoice;
				cout << endl;
				foundCourse = bst.searchCourse(toUpper(courseChoice));
				/* Use the printSingleCourse method to use the course returned by 
				the searchCourse method to print the course information.
				*/
				bst.printSingleCourse(foundCourse); 
				break;
			}

			case 9: { // Exit the loop
				break;
			}

			default: { // Handles integers that are not options on the menu.
				cout << userInput << " is not a valid option." << endl;
				cout << endl;
				break;
			}
		}
	}

	cout << "Thank you for using the course planner!" << endl;

	return 0;
}