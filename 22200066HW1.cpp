#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct StudentRecord {
    string studentName;
    int score;
    string className;
};

class priQueue{
    private:
    vector<StudentRecord> heap = vector<StudentRecord>(31);
    int heapSize = 0;
    const int MAX_SIZE = 30;

    public:
    

    StudentRecord maximum(){
        //isEmpty검사는 main에서 한다.
        return heap[1];
    }
    StudentRecord extract_max(){
        //isEmpty검사는 main에서 한다.
        StudentRecord max = heap[1];
        heap[1] = heap[heapSize--];
        heapify(1);
        return max;
    }
    
    bool insert(const StudentRecord& x){
        if (isFull()) return false;
        heapSize++;
        heap[heapSize] = x;
        int i = heapSize;

        while (i > 1 && heap[i / 2].score < heap[i].score){
            StudentRecord temp = heap[i / 2];
            heap[i / 2] = heap[i];
            heap[i] = temp;
            i = i / 2;
        }
        
        return true;
        
    }
    
    bool isFull(){
        return heapSize == MAX_SIZE;
    }
    bool isEmpty(){
        return heapSize == 0;
    }
    
    void heapify(int root) {
        int left = root * 2;
        int right = root * 2 + 1;
        int largest = root;

        if (left <= heapSize && heap[left].score > heap[largest].score) {
            largest = left;
        }

        if (right <= heapSize && heap[right].score > heap[largest].score) {
            largest = right;
        }

        if (largest != root) {
            StudentRecord temp = heap[root];
            heap[root] = heap[largest];
            heap[largest] = temp;

            heapify(largest);
        }
    }

    void increase(int i, int k) {
        heap[i].score = k;

        while (i > 1 && heap[i / 2].score < heap[i].score) {
            StudentRecord temp = heap[i / 2];
            heap[i / 2] = heap[i];
            heap[i] = temp;
            i = i / 2;
        }
    }
    int getSize(){
        return heapSize;
    }
    void printQueue(){
        for (int i = 1; i <= heapSize; i++) {
            cout << i << ". " << "[" << heap[i].studentName << ", " << heap[i].score << ", " << heap[i].className << "]" << '\n';
        }
    }
    StudentRecord getRecord(int index){
        return heap[index];
    }
    
};

void printMenu() {
    cout << "*************** MENU ****************" << '\n';
    cout << "I : Insert a new element into the queue." << '\n';
    cout << "D : Delete the element with the largest key from the queue." << '\n';
    cout << "R : Retrieve the element with the largest key." << '\n';
    cout << "N : Increase the key of an element in the queue." << '\n';
    cout << "P : Print all elements in the queue." << '\n';
    cout << "Q : Quit." << '\n';
}


int getValidScore() {
    string input;

    while (true) {
        cout << "Enter the new score: ";
        getline(cin, input);
        bool valid = true;

        if ((int)input.length() == 0) {
            cout << "Invalid score. Please enter an integer from 0 to 100." << '\n';
            continue;
        }
        int score = 0;

        for (int i = 0; i < (int)input.length(); i++) {
            if (input[i] < '0' || input[i] > '9') {
                valid = false;
                break;
            }
            score = score * 10 + (input[i] - '0');
        }

        if (valid && score >= 0 && score <= 100) {
            return score;
        }

        cout << "Invalid score. Please enter a valid integer between 0 and 100. " << '\n';
    }
}

int getValidMenuIndex(){
    string input;

    while (true) {
        
        getline(cin, input);
        bool valid = true;
        int index = 0;

        if ((int)input.length() == 0) {
            cout << "Invalid index. Please enter an integer from 1 to 30: ";
            continue;
        }

        for (int i = 0; i < (int)input.length(); i++) {
            if (input[i] < '0' || input[i] > '9') {
                valid = false;
                break;
            }
            index = index * 10 + (input[i] - '0');
        }

        if (valid && index >= 1 && index <= 30) {
            return index;
        }

        cout << "Invalid index. Please enter an integer from 1 to 30: ";
    }
}

int main(){
    priQueue q;
    string choice;

    while (true) {
        printMenu();
        cout << "Choose menu: ";
        getline(cin, choice);

        while (!choice.empty() && choice.front() == ' ') {
            choice.erase(choice.begin());
        }
        while (!choice.empty() && choice.back() == ' ') {
            choice.pop_back();
        }

        if (choice.empty()) {
            cout << '\n';
            continue;
        }

        char command = choice[0];

        if (command == 'Q' || command == 'q') {
            cout << "Program terminated." << '\n';
            break;
        }

        else if (command == 'I' || command == 'i') {
            if (q.isFull()) {
                cout << "Error: Heap is full. Cannot insert more elements." << '\n';
                cout << '\n';
                continue;
            }

            StudentRecord newRecord;

            cout << "Enter the name of the student: ";
            getline(cin, newRecord.studentName);

            
            newRecord.score = getValidScore();

            cout << "Enter the class name: ";
            getline(cin, newRecord.className);

            if (q.insert(newRecord)) {
                cout << "New element ["<< newRecord.studentName << ", "<< newRecord.score << ", "<< newRecord.className << "] has been inserted." << '\n';
            } 
            else cout << "Error: Heap is full. Cannot insert more elements." << '\n';
        }

        else if (command == 'D' || command == 'd') {
            if (q.isEmpty()) cout << "Cannot delete from an empty queue." << '\n';
            else{ 
                StudentRecord removed = q.extract_max();
                cout << "Deleted element: [" << removed.studentName << ", " << removed.score << ", " << removed.className << "]" << '\n';
            }
        }

        else if (command == 'R' || command == 'r') {
            if (q.isEmpty()) cout << "Queue is empty." << '\n';
            else {
                StudentRecord top = q.maximum();
                cout << "Element with the largest key: [" << top.studentName << ", " << top.score << ", " << top.className << "]" << '\n';
            }
        }

        else if (command == 'N' || command == 'n') {
            if (q.isEmpty()) {
                cout << "Queue is empty." << '\n';
                cout << '\n';
                continue;
            }

            cout << "Enter the index of the element: ";
            int index = getValidMenuIndex();

            if (index > q.getSize()) {
                cout << "Invalid index. No element exists at that position." << '\n';
                cout << '\n';
                continue;
            }

            StudentRecord currentScore = q.getRecord(index);

            while (true) {
                int newScore = getValidScore();

                if (newScore <= currentScore.score) {
                    cout << "New score should be larger than current score. Please enter again." << '\n';
                } 
                else {
                    q.increase(index, newScore);
                    cout << "Key updated. ";
                    cout << "[" << currentScore.studentName << ", " << newScore << ", " << currentScore.className << "] has been repositioned in the queue." << '\n';
                    break;
                }
            }
        }

        else if (command == 'P' || command == 'p') {
            if (q.isEmpty()) cout << "Queue is empty." << '\n';
            else{
                cout << "Current queue elements: " << '\n';
                q.printQueue();
            } 
        }

        else {
            cout << "Invalid menu choice. Please try again." << '\n';
        }

        cout << '\n';
    }
};
