
#include <iostream>
using namespace std;

// Struct for car details
struct CarDetails {
    int floorNumber;
    int position;
};

// Class to manage dynamic car parking records
class ParkingDetails {
    CarDetails* details;
    int capacity;
    int size;

    void resize() {
        capacity *= 2;
        CarDetails* newDetails = new CarDetails[capacity];
        for (int i = 0; i < size; i++) {
            newDetails[i] = details[i];
        }
        delete[] details;
        details = newDetails;
    }

public:
    ParkingDetails(int initialCapacity = 10) {
        capacity = initialCapacity;
        size = 0;
        details = new CarDetails[capacity];
    }

    void addCarDetails(int floorNumber, int position) {
        if (size == capacity) {
            resize();
        }
        details[size++] = { floorNumber, position };
    }

    void removeCarDetails() {
        if (size > 0) {
            size--;
        }
    }

    void display() {
        if (size == 0) {
            cout << "No cars in parking details.\n";
        } else {
            cout << "Parking Details:\n";
            for (int i = 0; i < size; i++) {
                cout << "Floor " << details[i].floorNumber << ", Position " << details[i].position << "\n";
            }
        }
    }
};

// Node structure for overflow waitlist
struct Node {
    int carNumber;
    Node* next;
};

// Linked list-based waitlist
class OverflowWaitlist {
    Node* head, * tail;

public:
    OverflowWaitlist() {
        head = tail = nullptr;
    }

    void addToWaitlist(int carNumber) {
        Node* newNode = new Node{ carNumber, nullptr };
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        cout << "Car " << carNumber << " added to the waitlist.\n";
    }

    int removeFromWaitlist() {
        if (!head) return -1;
        int carNumber = head->carNumber;
        Node* temp = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete temp;
        return carNumber;
    }

    bool isEmpty() {
        return head == nullptr;
    }

    void display() {
        if (!head) {
            cout << "Overflow waitlist is empty.\n";
        } else {
            cout << "Overflow Waitlist: ";
            Node* current = head;
            while (current) {
                cout << current->carNumber << " ";
                current = current->next;
            }
            cout << "\n";
        }
    }
};

// Circular queue for car entry
class EntryQueue {
    int front, rear, size, capacity;
    int* queue;

public:
    EntryQueue(int cap) {
        capacity = cap;
        size = 0;
        front = 0;
        rear = -1;
        queue = new int[capacity];
    }

    void enqueue(int carNumber, OverflowWaitlist& waitlist) {
        if (isFull()) {
            waitlist.addToWaitlist(carNumber);
        } else {
            rear = (rear + 1) % capacity;
            queue[rear] = carNumber;
            size++;
            cout << "Car " << carNumber << " added to the entry queue.\n";
        }
    }

    int dequeue() {
        if (isEmpty()) {
            cout << "Entry queue is empty.\n";
            return -1;
        }
        int car = queue[front];
        front = (front + 1) % capacity;
        size--;
        return car;
    }

    bool isEmpty() { return size == 0; }

    bool isFull() { return size == capacity; }

    void display() {
        if (isEmpty()) {
            cout << "Entry queue is empty.\n";
        } else {
            cout << "Entry Queue: ";
            for (int i = 0; i < size; i++) {
                cout << queue[(front + i) % capacity] << " ";
            }
            cout << "\n";
        }
    }
};

// Stack for parking floor
class Floor {
    int top, size;
    int* stack;

public:
    Floor(int s) {
        size = s;
        top = -1;
        stack = new int[size];
    }

    bool isEmpty() { return top == -1; }

    bool isFull() { return top == size - 1; }

    void push(int carNumber, int floorNumber, ParkingDetails& details, OverflowWaitlist& waitlist) {
        if (isFull()) {
            waitlist.addToWaitlist(carNumber);
        } else {
            stack[++top] = carNumber;
            details.addCarDetails(floorNumber, top + 1);
            cout << "Car " << carNumber << " parked on Floor " << floorNumber << ", Position " << top + 1 << ".\n";
        }
    }

    void pop(ParkingDetails& details) {
        if (isEmpty()) {
            cout << "Floor is empty.\n";
        } else {
            details.removeCarDetails();
            top--;
        }
    }

    void display(const string& floorName) {
        if (isEmpty()) {
            cout << floorName << " is empty.\n";
        } else {
            cout << floorName << ": ";
            for (int i = top; i >= 0; i--) {
                cout << stack[i] << " ";
            }
            cout << "\n";
        }
    }
};

// Main program
int main() {
    int floor1Size = 1, floor2Size = 1, queueSize = 2;
    Floor floor1(floor1Size);
    Floor floor2(floor2Size);
    EntryQueue entryQueue(queueSize);
    OverflowWaitlist overflowWaitlist;
    ParkingDetails parkingDetails;

    int choice, carNumber;

    while (true) {
        cout << "\nParking Lot Menu:\n";
        cout << "1. Enter car (Add to entry queue or overflow waitlist)\n";
        cout << "2. Park car from queue\n";
        cout << "3. Remove car from floor\n";
        cout << "4. Display parking lot, queues, and waitlist\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter car number to add: ";
                cin >> carNumber;
                entryQueue.enqueue(carNumber, overflowWaitlist);
                break;

            case 2:
                if (!entryQueue.isEmpty()) {
                    carNumber = entryQueue.dequeue();
                    if (!floor1.isFull()) {
                        floor1.push(carNumber, 1, parkingDetails, overflowWaitlist);
                    } else if (!floor2.isFull()) {
                        floor2.push(carNumber, 2, parkingDetails, overflowWaitlist);
                    } else {
                        overflowWaitlist.addToWaitlist(carNumber);
                    }
                }
                break;

            case 3:
                floor1.pop(parkingDetails);
                floor2.pop(parkingDetails);
                break;

            case 4:
                floor1.display("Floor 1");
                floor2.display("Floor 2");
                entryQueue.display();
                overflowWaitlist.display();
                parkingDetails.display();
                break;

            case 5:
                return 0;

            default:
                cout << "Invalid choice.\n";
        }
    }
}
