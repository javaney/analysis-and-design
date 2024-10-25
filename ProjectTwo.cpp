#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

// Define the Course structure
struct Course
{
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};

// Node structure for Binary Search Tree
struct Node
{
    Course course;
    Node *left;
    Node *right;

    Node(Course aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
};

// Binary Search Tree class
class BinarySearchTree
{
private:
    Node *root;

    void addNode(Node *node, Course course);
    void inOrder(Node *node);
    Node *searchNode(Node *node, string courseNumber);

public:
    BinarySearchTree();
    ~BinarySearchTree();
    void Insert(Course course);
    void InOrder();
    Course Search(string courseNumber);
};

// BinarySearchTree constructor
BinarySearchTree::BinarySearchTree()
{
    root = nullptr;
}

// Destructor for the tree (to free memory)
BinarySearchTree::~BinarySearchTree()
{
    // TODO: Implement memory cleanup (optional)
}

// Function to insert a course into the tree
void BinarySearchTree::Insert(Course course)
{
    if (root == nullptr)
    {
        root = new Node(course);
    }
    else
    {
        addNode(root, course);
    }
}

// Helper function to add node recursively
void BinarySearchTree::addNode(Node *node, Course course)
{
    if (course.courseNumber < node->course.courseNumber)
    {
        if (node->left == nullptr)
        {
            node->left = new Node(course);
        }
        else
        {
            addNode(node->left, course);
        }
    }
    else
    {
        if (node->right == nullptr)
        {
            node->right = new Node(course);
        }
        else
        {
            addNode(node->right, course);
        }
    }
}

// In-order traversal to print the courses in alphanumeric order
void BinarySearchTree::InOrder()
{
    inOrder(root);
}

// Helper function for in-order traversal
void BinarySearchTree::inOrder(Node *node)
{
    if (node != nullptr)
    {
        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseName;
        if (!node->course.prerequisites.empty())
        {
            cout << " Prerequisites: ";
            for (size_t i = 0; i < node->course.prerequisites.size(); ++i)
            {
                cout << node->course.prerequisites[i];
                if (i != node->course.prerequisites.size() - 1)
                {
                    cout << ", ";
                }
            }
        }
        cout << endl;
        inOrder(node->right);
    }
}

// Function to search for a course by course number
Course BinarySearchTree::Search(string courseNumber)
{
    // Convert course number to uppercase to make search case-insensitive
    transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);
    Node *result = searchNode(root, courseNumber);
    if (result != nullptr)
    {
        return result->course;
    }
    // Return an empty course if not found
    return Course();
}

// Helper function to search node recursively
Node *BinarySearchTree::searchNode(Node *node, string courseNumber)
{
    if (node == nullptr || node->course.courseNumber == courseNumber)
    {
        return node;
    }

    if (courseNumber < node->course.courseNumber)
    {
        return searchNode(node->left, courseNumber);
    }
    return searchNode(node->right, courseNumber);
}

// Function to load courses from the CSV file
void loadCourses(string fileName, BinarySearchTree *bst, bool &dataLoaded)
{
    ifstream file(fileName);
    if (!file.is_open())
    {
        cout << "Error: Unable to open file " << fileName << ". Please check the file path or file name and try again." << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string courseNumber, courseName, prerequisite;
        getline(ss, courseNumber, ',');
        getline(ss, courseName, ',');

        Course course;
        course.courseNumber = courseNumber;
        course.courseName = courseName;

        // Read prerequisites
        while (getline(ss, prerequisite, ','))
        {
            course.prerequisites.push_back(prerequisite);
        }

        bst->Insert(course);
    }
    file.close();
    dataLoaded = true;
    cout << "Courses loaded successfully!" << endl;
}

// Function to display the menu
void displayMenu()
{
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit." << endl;
}

// Function to get a valid integer input from the user
int getValidIntInput()
{
    int choice;
    while (true)
    {
        cout << "What would you like to do? ";
        if (cin >> choice)
        {
            return choice;
        }
        else
        {
            cout << "Invalid input! Please enter a valid option (1, 2, 3, or 9)." << endl;
            cin.clear();                                         // clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
        }
    }
}

// Main function
int main()
{
    BinarySearchTree *bst = new BinarySearchTree();
    string fileName;
    int choice;
    bool dataLoaded = false; // Track whether data has been loaded

    cout << "Welcome to the course planner." << endl;

    do
    {
        displayMenu();
        choice = getValidIntInput(); // Get valid menu option

        switch (choice)
        {
        case 1:
            cout << "Enter file name (e.g., courses.csv): ";
            cin >> fileName;
            loadCourses(fileName, bst, dataLoaded);
            break;
        case 2:
            if (!dataLoaded)
            {
                cout << "No data loaded yet. Please load the data first by choosing option 1." << endl;
            }
            else
            {
                cout << "Here is a sample schedule: " << endl;
                bst->InOrder();
            }
            break;
        case 3:
        {
            if (!dataLoaded)
            {
                cout << "No data loaded yet. Please load the data first by choosing option 1." << endl;
            }
            else
            {
                string courseNumber;
                cout << "What course do you want to know about? ";
                cin >> courseNumber;
                Course course = bst->Search(courseNumber);
                if (!course.courseNumber.empty())
                {
                    cout << course.courseNumber << ", " << course.courseName << endl;
                    if (!course.prerequisites.empty())
                    {
                        cout << "Prerequisites: ";
                        for (size_t i = 0; i < course.prerequisites.size(); ++i)
                        {
                            cout << course.prerequisites[i];
                            if (i != course.prerequisites.size() - 1)
                            {
                                cout << ", ";
                            }
                        }
                        cout << endl;
                    }
                    else
                    {
                        cout << "No prerequisites." << endl;
                    }
                }
                else
                {
                    cout << "Course not found. Please check the course number and try again." << endl;
                }
            }
            break;
        }
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;
        default:
            cout << choice << " is not a valid option. Please choose a valid menu option." << endl;
        }
    } while (choice != 9);

    delete bst;
    return 0;
}
