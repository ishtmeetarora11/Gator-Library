#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <regex>
#include <climits>
#include <fstream>
#include <sstream>

using namespace std;

// Stores the patron ID, priority number and time of Reservation
class ReservationNode {
public:
    int patronID;
    int priorityNumber;
    time_t timeOfReservation;

    ReservationNode(int id, int priority, time_t timeOfReservation = time(0)) : patronID(id), priorityNumber(priority) {}
};

// This is the min heap that stores the reservationNode
class ReservationHeap {
private:
    vector<ReservationNode> heap;

    // Helper function to maintain heap property
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index].priorityNumber < heap[parent].priorityNumber ||
                (heap[index].priorityNumber == heap[parent].priorityNumber &&
                 heap[index].timeOfReservation < heap[parent].timeOfReservation)) {
                swap(heap[index], heap[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    // Helper function to maintain heap property
    void heapifyDown(int index) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;
        int n=heap.size();

        if (left < n &&
            (heap[left].priorityNumber < heap[smallest].priorityNumber ||
             (heap[left].priorityNumber == heap[smallest].priorityNumber &&
              heap[left].timeOfReservation < heap[smallest].timeOfReservation))) {
            smallest = left;
        }

        if (right < n &&
            (heap[right].priorityNumber < heap[smallest].priorityNumber ||
             (heap[right].priorityNumber == heap[smallest].priorityNumber &&
              heap[right].timeOfReservation < heap[smallest].timeOfReservation))) {
            smallest = right;
        }

        if (smallest != index) {
            std::swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:

    // Function to insert a new reservation node into the heap
    void insertReservation(int patronID, int priorityNumber) {
        ReservationNode newNode(patronID, priorityNumber);
        heap.push_back(newNode);
        heapifyUp(heap.size() - 1);
    }

    // Function to get the top (min) element from the heap
    ReservationNode extractMin() {
        if (heap.empty()) {
            throw std::out_of_range("Heap is empty");
        }
        ReservationNode m=heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
        return m;
    }

    // Function to check if the heap is empty
    bool isEmpty() {
        return heap.empty();
    }

    // Function to get a string of all the patrons that have reserved the book.
    string getHeapString() const {
        std::stringstream ss;
        ss<<"[";
        int count1=0;
        int n=heap.size();
        for (ReservationNode ele : heap) {
            count1++;
            ss << ele.patronID;
            if(count1<n){
               ss<< ", ";
            }
        }
        ss<<"]";
        return ss.str();
    }
};

// Book Node structure
struct BookNode {
    int BookId;
    string BookName;
    string AuthorName;
    bool AvailabilityStatus;
    int BorrowedBy;
    ReservationHeap reservationHeap;

    BookNode(int id, string name, string author, bool available, int borrowedBy)
        : BookId(id), BookName(name), AuthorName(author), AvailabilityStatus(available), BorrowedBy(borrowedBy) {}
};

// Color Enum which is used in the red black tree.
enum Color
{
    RED,
    BLACK
};

// Node of the red black tree that contains the book details, parent node, children and color of the node.
class RBNode {
public:
  BookNode* book;
  RBNode* parent;
  RBNode* left;
  RBNode* right; 
  Color color;
 
  RBNode(BookNode* book) : book(book) {
    parent = left = right = NULL;
    color = RED;
  }
 
  // returns pointer to uncle
  RBNode *uncle() {
    // If no parent or grandparent, then no uncle
    if (parent == NULL or parent->parent == NULL)
      return NULL;
 
    if (parent->isOnLeft())
      // uncle on right
      return parent->parent->right;
    else
      // uncle on left
      return parent->parent->left;
  }
 
  // check if node is left child of parent
  bool isOnLeft() { return this == parent->left; }
 
  // returns pointer to sibling
  RBNode *sibling() {
    // sibling null if no parent
    if (parent == NULL)
      return NULL;
 
    if (isOnLeft())
      return parent->right;
 
    return parent->left;
  }
 
  // moves node down and moves given node in its place
  void moveDown(RBNode *nParent) {
    if (parent != NULL) {
      if (isOnLeft()) {
        parent->left = nParent;
      } else {
        parent->right = nParent;
      }
    }
    nParent->parent = parent;
    parent = nParent;
  }
 
  bool hasRedChild() {
    return (left != NULL and left->color == RED) or
           (right != NULL and right->color == RED);
  }
};

//The red black tree containing the the node and the counter of the color flip count.
class RedBlackTree {

private:

    RBNode* root;
    int colorFlipCount=0;  // Counter for color flips
    
    //Coloring the specified node with the specific color.
    void colorNode(RBNode *node, Color color){
        if (node->color!=color){
            colorFlipCount++;
        }
        node->color=color;
    }

    // Helper functions for Red-Black Tree

    //Print the books between the 1 book IDs provided.
    void print(int bookID1, int bookID2, RBNode* node, ofstream &outputFile);

    //Print the book with the book ID provided.
    void printInfo(int bookID, RBNode* node, ofstream &outputFile);

    //Find the books closest to the book ID provided.
    void find(int targetID, RBNode* node, ofstream &outputFile, int minans);

    //Left rotation of the red black tree.
    void rotateLeft(RBNode *x){
        RBNode *y = x->right;
        x->right = y->left;

        if (y->left)
        {
            y->left->parent = x;
        }

        y->parent = x->parent;

        if (!x->parent)
        {
            root = y;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;
    }

    //Right rotation of the red black tree.
    void rotateRight(RBNode *y){
        RBNode *x = y->left;
        y->left = x->right;

        if (x->right)
        {
            x->right->parent = y;
        }

        x->parent = y->parent;

        if (!y->parent)
        {
            root = x;
        }
        else if (y == y->parent->right)
        {
            y->parent->right = x;
        }
        else
        {
            y->parent->left = x;
        }

        x->right = y;
        y->parent = x;
    }

    //Fixing the red black tree after insertion.
    void fixInsert(RBNode *node){
        while (node->parent && node->parent->color == RED)
        {
            // If parent of node is left child of grandparent of node
            if (node->parent == node->parent->parent->left)
            {
                RBNode *uncle = node->parent->parent->right;

                // If uncle of node is red, recolor parent, uncle and grandparent of node
                if (uncle && uncle->color == RED)
                {
                    colorNode(node->parent, BLACK);
                    colorNode(uncle, BLACK);
                    colorNode(node->parent->parent, RED);
                    node = node->parent->parent;
                } // If uncle of node is black, rotate the tree
                else
                {
                    // If node is right child of parent, rotate left at parent
                    if (node == node->parent->right)
                    {
                        node = node->parent;
                        rotateLeft(node);
                    }
                    colorNode(node->parent, BLACK);
                    colorNode(node->parent->parent, RED);
                    rotateRight(node->parent->parent);
                }
            } // If parent of node is right child of grandparent of node
            else
            {
                RBNode *uncle = node->parent->parent->left;

                // If uncle of node is red, recolor parent, uncle and grandparent of node
                if (uncle && uncle->color == RED)
                {
                    colorNode(node->parent, BLACK);
                    colorNode(uncle, BLACK);
                    colorNode(node->parent->parent, RED);
                    node = node->parent->parent;
                }
                else // If uncle of node is black, rotate the tree
                {
                    // If node is left child of parent, rotate right at parent
                    if (node == node->parent->left)
                    {
                        node = node->parent;
                        rotateRight(node);
                    }
                    colorNode(node->parent, BLACK);
                    colorNode(node->parent->parent, RED);
                    rotateLeft(node->parent->parent);
                }
            }
        }
        colorNode(root, BLACK);

    }

    //Finding the successor of the node given.
    RBNode *successor(RBNode *x) {
    RBNode *temp = x;
 
    while (temp->left != NULL)
      temp = temp->left;
 
    return temp;
  }

    RBNode *BSTreplace(RBNode *x) {
    // when node have 2 children
    if (x->left != NULL and x->right != NULL)
      return successor(x->right);
 
    // when leaf
    if (x->left == NULL and x->right == NULL)
      return NULL;
 
    // when single child
    if (x->left != NULL)
      return x->left;
    else
      return x->right;
  }

  // Swapping books of 2 nodes.
  void swapValues(RBNode *u, RBNode *v) {
    BookNode *temp;
    temp = u->book;
    u->book = v->book;
    v->book = temp;
  }

  // Deleting a node.
  void deleteNode(RBNode *v) {
    RBNode *u = BSTreplace(v);
 
    // True when u and v are both black
    bool uvBlack = ((u == NULL or u->color == BLACK) and (v->color == BLACK));
    RBNode *parent = v->parent;
 
    if (u == NULL) {
      // u is NULL therefore v is leaf
      if (v == root) {
        // v is root, making root null
        root = NULL;
      } else {
        if (uvBlack) {
          // u and v both black
          // v is leaf, fix double black at v
          fixDoubleBlack(v);
        } else {
          // u or v is red
          if (v->sibling() != NULL)
            // sibling is not null, make it red"
            v->sibling()->color = RED;
        }
 
        // delete v from the tree
        if (v->isOnLeft()) {
          parent->left = NULL;
        } else {
          parent->right = NULL;
        }
      }
      delete v;
      return;
    }
 
    if (v->left == NULL or v->right == NULL) {
      // v has 1 child
      if (v == root) {
        // v is root, assign the value of u to v, and delete u
        v->book = u->book;
        v->left = v->right = NULL;
        delete u;
      } else {
        // Detach v from tree and move u up
        if (v->isOnLeft()) {
          parent->left = u;
        } else {
          parent->right = u;
        }
        delete v;
        u->parent = parent;
        if (uvBlack) {
          // u and v both black, fix double black at u
          fixDoubleBlack(u);
        } else {
          // u or v red, color u black
          u->color = BLACK;
        }
      }
      return;
    }
 
    // v has 2 children, swap values with successor and recurse
    swapValues(u, v);
    deleteNode(u);
  }

  //Fixing the double black property of the red black tree after deletion.
  void fixDoubleBlack(RBNode *x) {
    if (x == root)
      // Reached root
      return;
 
    RBNode *sibling = x->sibling(), *parent = x->parent;
    if (sibling == NULL) {
      // No sibling, double black pushed up
      fixDoubleBlack(parent);
    } else {
      if (sibling->color == RED) {
        // Sibling red
        parent->color = RED;
        sibling->color = BLACK;
        if (sibling->isOnLeft()) {
          // left case
          rotateRight(parent);
        } else {
          // right case
          rotateLeft(parent);
        }
        fixDoubleBlack(x);
      } else {
        // Sibling black
        if (sibling->hasRedChild()) {
          // at least 1 red children
          if (sibling->left != NULL and sibling->left->color == RED) {
            if (sibling->isOnLeft()) {
              // left left
              sibling->left->color = sibling->color;
              sibling->color = parent->color;
              rotateRight(parent);
            } else {
              // right left
              sibling->left->color = parent->color;
              rotateRight(sibling);
              rotateLeft(parent);
            }
          } else {
            if (sibling->isOnLeft()) {
              // left right
              sibling->right->color = parent->color;
              rotateLeft(sibling);
              rotateRight(parent);
            } else {
              // right right
              sibling->right->color = sibling->color;
              sibling->color = parent->color;
              rotateLeft(parent);
            }
          }
          parent->color = BLACK;
        } else {
          // 2 black children
          sibling->color = RED;
          if (parent->color == BLACK)
            fixDoubleBlack(parent);
          else
            parent->color = BLACK;
        }
      }
    }
  }

    void transplant(RBNode *oldNode, RBNode *newNode){
        if (!oldNode->parent)
        {
            root = newNode;
        }
        else if (oldNode == oldNode->parent->left)
        {
            oldNode->parent->left = newNode;
        }
        else
        {
            oldNode->parent->right = newNode;
        }

        if (newNode)
        {
            newNode->parent = oldNode->parent;
        }
    }

// Compare the nodes to sort.
bool static compare(RBNode *node1, RBNode *node2)
{
    return (node1->book->BookId <= node2->book->BookId);
}

public:
    RedBlackTree() : root(nullptr), colorFlipCount(0) {}

    // Public functions

    //Searching for a book in the tree.
    RBNode *search(int bookId)
    {
        RBNode *current = root;
        while (current && current->book->BookId != bookId)
        {
            if (bookId < current->book->BookId)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }
        return current;
    }

    //Deleting a book in the tree.
    void deleteBook(int bookID, ofstream &outputFile) {
    if (root == NULL)
         return;
    RBNode *node = search(bookID);
    if(node->book->reservationHeap.isEmpty()){
                outputFile << "Book " << bookID << " is no longer available." <<endl;
            }
            else if(!node->book->reservationHeap.isEmpty()){
                string s= node->book->reservationHeap.getHeapString();
                outputFile << "Book " << bookID << " is no longer available. Reservations made by patrons " 
                << s.substr(1,s.length()-2) << " have been cancelled!" << endl;
            }
    deleteNode(node);
}
    // Printing books within the 2 given bookID's.
    void printBooks(int bookID1, int bookID2, ofstream &outputFile);

    // Printing book with the given bookID.
    void printBook(int bookID, ofstream &outputFile);

    // Inserting book into the tree.
    void insertBook(int id, string name, string author, bool available, int borrowedBy);

    // Allowing the patron to borrow the book or adding the patron to reservation heap.
    void borrowBook(int patronID, int bookID, int patronPriority, ofstream &outputFile);

    // Updating the book status by making the patron return the book so that it is available.
    void returnBook(int patronID, int bookID, ofstream &outputFile);

    // Finding the closest books to the target ID in the tree.
    void findClosestBook(int targetID, ofstream &outputFile);

    // This function will print the color flip count.
    void countColorFlip(ofstream &outputFile);  
};

// Function to print information about a specific book
void printBookInfo(BookNode* book, ofstream &outputFile) {
    outputFile << "BookID = " << book->BookId << "\n"
         << "Title = " << book->BookName << "\n"
         << "Author = " << book->AuthorName << "\n"
         << "Availability = " << (book->AvailabilityStatus ? "\"Yes\"" : "\"No\"") << "\n"
         << "BorrowedBy = " << (book->BorrowedBy==-1 ?  "None" : to_string(book->BorrowedBy)) << "\n"
         << "Reservations = " << book->reservationHeap.getHeapString() << "\n";
         outputFile << "\n";
}

// Function to print information about all books with bookIDs in the range [bookID1, bookID2].
void RedBlackTree::print(int bookID1, int bookID2, RBNode* node, ofstream &outputFile) {
    if (node == nullptr) return;

    if (node->book->BookId > bookID1) {
        print(bookID1, bookID2, node->left, outputFile);
    }

    if (node->book->BookId >= bookID1 && node->book->BookId <= bookID2) {
        printBookInfo(node->book, outputFile);
    }

    if (node->book->BookId < bookID2) {
        print(bookID1, bookID2, node->right, outputFile);
    }
}

// Public function to print information about all books with bookIDs in the range [bookID1, bookID2].
void RedBlackTree::printBooks(int bookID1, int bookID2, ofstream &outputFile) {
    print(bookID1, bookID2, root, outputFile);
}

// Function to print information about a specific book identified by its unique bookID
void RedBlackTree::printInfo(int bookID, RBNode* node, ofstream &outputFile) {
    if (node == nullptr) {
        outputFile << "Book " << bookID << " not found in the Library" << endl;
        return;
    }

    if (node->book->BookId == bookID) {
        printBookInfo(node->book, outputFile);
    } else if (node->book->BookId > bookID) {
        printInfo(bookID, node->left, outputFile);
    } else {
        printInfo(bookID, node->right, outputFile);
    }
}

// Public function to print information about a specific book identified by its unique bookID
void RedBlackTree::printBook(int bookID, ofstream &outputFile) {
    printInfo(bookID, root, outputFile);
}

// Function to add a new book to the library
void RedBlackTree::insertBook(int id, string name, string author, bool available, int borrowedBy) {
    BookNode* newBook = new BookNode(id, name, author, available, borrowedBy);
    RBNode* newNode = new RBNode(newBook);
    RBNode* parent = nullptr;
    RBNode* temp = root;
    while (temp != nullptr) {
        parent = temp;
        if (id < temp->book->BookId)
            temp = temp->left;
        else
            temp = temp->right;
    }

    newNode->parent = parent;
    if (parent == nullptr)
        root = newNode;
    else if (id < parent->book->BookId)
        parent->left = newNode;
    else
        parent->right = newNode;
    fixInsert(newNode);
}

// Function to allow a patron to borrow a book
void RedBlackTree::borrowBook(int patronID, int bookID, int patronPriority, ofstream &outputFile) {
    // Find the book
    RBNode* node = root;
    while (node != nullptr) {
        if (bookID == node->book->BookId) {
            if (node->book->AvailabilityStatus) {
                // Book is available, update book status
                node->book->AvailabilityStatus = false;
                node->book->BorrowedBy = patronID;
                outputFile << "Book "<<bookID<<" Borrowed by Patron "<<patronID << "\n";
            } else {
                // Book is not available, add reservation to the heap
                node->book->reservationHeap.insertReservation(patronID, patronPriority);
                outputFile << "Book "<<bookID<<" Reserved by Patron "<<patronID << "\n";
            }
            return;
        } else if (bookID < node->book->BookId)
            node = node->left;
        else
            node = node->right;
    }

}

// Function to allow a patron to return a book
void RedBlackTree::returnBook(int patronID, int bookID, ofstream &outputFile) {
    
    RBNode* node = root;
    while (node != nullptr) {
        if (bookID == node->book->BookId) {
            // Update book status
            node->book->AvailabilityStatus = true;
            node->book->BorrowedBy = -1;
            outputFile << "Book "<<bookID<<" Returned by Patron "<<patronID<< "\n";
            outputFile <<"\n";

            // If there are reservations, assign the book to the patron with highest priority
            if (!node->book->reservationHeap.isEmpty()) {
                ReservationNode reservation = node->book->reservationHeap.extractMin();
                node->book->AvailabilityStatus = false;
                node->book->BorrowedBy = reservation.patronID;
                outputFile << "Book "<<bookID<<" Allotted to Patron "<< reservation.patronID<< "\n";
                outputFile <<"\n";
            }
            return;
        } else if (bookID < node->book->BookId)
            node = node->left;
        else
            node = node->right;
    }
}

// Function to allow a patron to find the closest books to the targetID.
void RedBlackTree::find(int targetID, RBNode* node, ofstream &outputFile, int minans) {
   RBNode* temp=node;
   vector<RBNode*>ans;
    while(temp){
        if (abs(temp->book->BookId - targetID) < minans) {
            minans = abs(temp->book->BookId - targetID);
        } 
        if (temp->book->BookId < targetID) {
            temp = temp->right;
        } else {
            temp = temp->left;
        }
    }
    while(node){
         if (abs(node->book->BookId - targetID) == minans) {
            ans.push_back(node);
        }
        if (node->book->BookId > targetID) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    sort(ans.begin(), ans.end(),compare);
    int n=ans.size();
    for(int i=0;i<n;i++){
        printBookInfo(ans[i]->book, outputFile);
    }
}

// Public function to find the book with an ID closest to the given ID
void RedBlackTree::findClosestBook(int targetID, ofstream &outputFile) {
    int minans=INT_MAX;
    find(targetID, root, outputFile, minans);
}

// Public function to print the color flip count
void RedBlackTree::countColorFlip(ofstream &outputFile) {
    outputFile << "Color Flip Count: " << colorFlipCount << endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " file_name" << std::endl;
        return 1;
    }

    string fileName = argv[1];
    int fileLength=fileName.length();
    ifstream inputFile(fileName);
    ofstream outputFile(fileName.substr(0,fileLength-4)+"_output_file.txt");

    if (!inputFile.is_open())
    {
        std::cout << "Error opening file: " << fileName << std::endl;
        return 1;
    }
    RedBlackTree library;
    string line; 
    while (getline(inputFile, line))
    {
        istringstream iss(line);
        string operation;
        getline(iss, operation, '(');

        if (operation == "InsertBook")
        {   int bookId;
            string title, author, availability;
            iss >> ws >> bookId;  // Read the integer
            iss.ignore();  // Ignore the comma
            getline(iss >> ws, title, ',');  // Read the title
            getline(iss >> ws, author, ',');  // Read the author
            getline(iss >> ws, availability, ')');
            if (library.search(bookId))
            {
                outputFile << "Book already exists";
            }

            library.insertBook(bookId, title, author, true, -1);
        }
        else if (operation == "PrintBook")
        {
            int bookId;
            iss >> bookId;
            if (!library.search(bookId))
            {
                outputFile << "Book "<<bookId<<" not found in the Library"<<endl<<endl;
            }
            else{
            library.printBook(bookId, outputFile);
        }
        }
        else if (operation == "PrintBooks")
        {
            int bookID1, bookID2;
            iss >> ws >> bookID1;
            iss.ignore();
            iss >> ws>> bookID2;
            library.printBooks(bookID1,bookID2, outputFile);

        }
        else if (operation == "BorrowBook")
        {
            int patronID, bookID, patronPriority;
            iss >> ws >> patronID;
            iss.ignore();
            iss >> ws>> bookID;
            iss.ignore();
            iss >> ws >> patronPriority;
            library.borrowBook(patronID, bookID, patronPriority, outputFile);
            outputFile <<"\n";
        }
        else if (operation == "ReturnBook")
        {
            int patronID, bookID;
            iss >> ws >> patronID;
            iss.ignore();
            iss >> ws>> bookID;
            library.returnBook(patronID, bookID, outputFile);
        }
        else if (operation == "ColorFlipCount")
        {
            library.countColorFlip(outputFile);
            outputFile<<"\n";
        }
         else if (operation == "DeleteBook")
        {
            int bookID;
            iss >> ws >> bookID;
            library.deleteBook(bookID, outputFile);
            outputFile <<"\n";
        }
         else if (operation == "FindClosestBook")
        {
            int targetID;
            iss >> ws >> targetID;
            library.findClosestBook(targetID, outputFile);
            outputFile << "\n";
        }
        else if (operation == "Quit")
        {
            outputFile << "Program Terminated!!";
            break;
        }
    }
    inputFile.close();
    outputFile.close();
    return 0;
}
