# Steps to Execute the Code
2. To make the executable file for gatorLibrary.cpp just run the command: make
3. To run the code for any specific input file run the command: ./ gatorLibrary <filename.txt>
4. Open the corresponding output file to check the output.

# Overview

GatorLibrary is a fictional library that needs a software system to efficiently manage its books, patrons, and
borrowing operations. The system should utilize a Red-Black tree data structure to ensure efficient management of the books. Implement a priority-queue mechanism using Binary Min-heaps as a data structure for managing book reservations in case a book is not currently available to be borrowed. Each book will have its own min-heap to keep track of book reservations made by the patrons.
Each node in the Red-Black tree will represent a book and will have the following structure: ● BookId // Integer ID
● BookName //Name of the book
● AuthorName //Name of the Author
● AvailabilityStatus //To indicate whether it is currently borrowed
● BorrowedBy //ID of the Patron who borrowed the book
● ReservationHeap: Implement Binary Min-heap for managing book reservations and waitlists for the book ordered by the patron’s priority which is an integer. (Priority 1 has precedence over Priority 2 and so on). Ties need be broken by considering the timestamp at which the reservation was made (first come first serve basis). Every node of the Min-heap should contain (patronID, priorityNumber, timeOfReservation) Note*:
- Assume that each waitlist is limited to 20.
- While taking timestamps, ensure the precision is high enough.
The system should support the following operations:

1. PrintBook(bookID): Print information about a specific book identified by its unique bookID (e.g., title,
author, availability status).
Note*: If not found, Print “Book <BookID> not found in the Library.”

2. PrintBooks(bookID1, bookID2): Print information about all books with bookIDs in the range [bookID1, bookID2].
   
3. InsertBook(bookID, bookName, authorName, availabilityStatus, borrowedBy, reservationHeap):
Add a new book to the library. BookID should be unique, and availability indicates whether the book is available for borrowing.
Note*: There is only one copy of a book i.e. all books are unique.

4. BorrowBook(patronID, bookID, patronPriority): Allow a patron to borrow a book that is available and update the status of the book. If a book is currently unavailable, create a reservation node in the heap as per the patron’s priority (patronPriority).
*Note: It is assumed that a book will not be borrowed by same patron twice.

5. ReturnBook(patronID, bookID): Allow a patron to return a borrowed book. Update the book's status and assign the book to the patron with highest priority in the Reservation Heap. (if there’s a reservation).
   
6. DeleteBook(bookID): Delete the book from the library and notify the patrons in the reservation list that the book is no longer available to borrow.
   
7. FindClosestBook(targetID) : Find the book with an ID closest to the given ID (checking on both sides of the ID). Print all the details about the book. In case of ties, print both the books ordered by bookIDs.
   
8. ColorFlipCount(): GatorLibrary's Red-Black tree structure requires an analytics tool to monitor and analyze the frequency of color flips in the Red-Black tree. Track the occurrence of color changes in the Red-Black tree nodes during tree operations, such as insertion, deletion, and rotations.
Note*: Only color flips should be counted i.e. when black changes to red and vice versa. (Only count the color flip if the color of the node has changed from previous state (before the operation) to current state)

# Introduction

Red-Black trees and Binary Min-heaps stand as fundamental data structures celebrated for their efficiency and distinctive characteristics, finding practical use in diverse applications, including library management systems such as the one employed by GatorLibrary. Red-Black trees, functioning as self-balancing binary search trees, guarantee logarithmic time complexity for crucial operations like insertion, deletion, and search. Their equilibrium is maintained through color-coding and rotation mechanisms, streamlining book management by enabling swift retrieval and modification of book information while preserving an optimal tree structure. Conversely, Binary Min-heaps are complete binary trees where each node's value is less than its children's values, ensuring that the smallest element is readily accessible at the root. This inherent property makes Min-heaps well-suited for constructing priority queues, a vital component in effectively managing book reservations. Through the strategic implementation of these structures, GatorLibrary can adeptly organize its book collection with Red-Black trees, ensuring balance, and seamlessly handle book reservations with Binary Min-heaps. This approach guarantees patrons efficient and rapid access to library resources while optimizing the allocation and management of resources within the system.
Programming Environment
C++ 14, Sublime Text ( Code Editor )

