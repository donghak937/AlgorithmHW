#include <iostream>
#include <string>
#include <vector>

using namespace std;

/*
References:
- 알고리즘 분석 heap part pseudocode
- 우선 순위 큐 구현 (https://yoongrammer.tistory.com/81)
*/

// 학생 한 명의 정보를 저장하는 구조체
// - studentName : 학생 이름
// - score       : 학생 점수
// - className   : 수업 이름
struct StudentRecord {
    string studentName;
    int score;
    string className;
};

/*
최대 힙(max-heap)을 이용한 priority queue 클래스

private 멤버
 - heap      : 1번 인덱스부터 사용하는 힙 배열
 - heapSize  : 현재 저장된 원소의 개수
 - MAX_SIZE  : 힙의 최대 크기

public 멤버 함수
 - maximum(StudentRecord& result)      : 가장 큰 key를 가진 원소를 result에 저장하고 성공 여부를 반환한다.
 - extract_max(StudentRecord& result)  : 가장 큰 key를 가진 원소를 삭제하여 result에 저장하고 성공 여부를 반환한다.
 - insert(const StudentRecord& x)      : 새로운 원소를 삽입한다.
 - isFull()                            : 큐가 가득 찼는지 확인한다.
 - isEmpty()                           : 큐가 비어 있는지 확인한다.
 - heapify(int root)                   : root를 기준으로 max-heap property를 복구한다.
 - increase(int i, int k)              : i번째 원소의 key를 증가시키고 위로 올린다.
 - getSize()                           : 현재 큐 크기를 반환한다.
 - printQueue()                        : 큐에 있는 모든 원소를 출력한다.
 - getRecord(int index)                : 특정 위치의 원소를 반환한다.
*/
class priQueue {
private:
    vector<StudentRecord> heap = vector<StudentRecord>(31);
    int heapSize = 0;
    const int MAX_SIZE = 30;

public:
    bool maximum(StudentRecord& result) {
        if (isEmpty()) return false;   // 빈 큐면 실패
        result = heap[1];
        return true;
    }

    bool extract_max(StudentRecord& result) {
        if (isEmpty()) return false;   // 빈 큐면 실패

        result = heap[1];              // 루트에 있는 최대 원소를 저장
        heap[1] = heap[heapSize--];    // 마지막 원소를 루트로 옮기고 크기 감소

        if (!isEmpty()) {              // 삭제 후에도 원소가 남아 있을 때만 heapify
            heapify(1);
        }

        return true;
    }

    bool insert(const StudentRecord& x) {
        if (isFull()) return false;    // 큐가 가득 찬 경우 삽입 실패

        heapSize++;                    // 힙 크기 증가
        heap[heapSize] = x;            // 마지막 위치에 새 원소 삽입
        int i = heapSize;              // 삽입된 위치부터 위로 검사

        // 부모보다 점수가 크면 swap하며 위로 올린다.
        while (i > 1 && heap[i / 2].score < heap[i].score) {
            StudentRecord temp = heap[i / 2];
            heap[i / 2] = heap[i];
            heap[i] = temp;
            i = i / 2;
        }

        return true;
    }

    bool isFull() {
        return heapSize == MAX_SIZE;   // 최대 크기와 같으면 true
    }

    bool isEmpty() {
        return heapSize == 0;          // 원소가 없으면 true
    }

    void heapify(int root) {
        int left = root * 2;           // 왼쪽 자식
        int right = root * 2 + 1;      // 오른쪽 자식
        int largest = root;            // 가장 큰 값을 가진 위치를 저장

        if (left <= heapSize && heap[left].score > heap[largest].score) {
            largest = left;
        }

        if (right <= heapSize && heap[right].score > heap[largest].score) {
            largest = right;
        }

        // 자식 중 더 큰 값이 있으면 교환 후 재귀적으로 복구
        if (largest != root) {
            StudentRecord temp = heap[root];
            heap[root] = heap[largest];
            heap[largest] = temp;

            heapify(largest);
        }
    }

    void increase(int i, int k) {
        heap[i].score = k; // i번째 원소의 점수를 새로운 값으로 갱신

        // 부모보다 크면 위로 올리며 max-heap property 유지
        while (i > 1 && heap[i / 2].score < heap[i].score) {
            StudentRecord temp = heap[i / 2];
            heap[i / 2] = heap[i];
            heap[i] = temp;
            i = i / 2;
        }
    }

    int getSize() {
        return heapSize; // 현재 큐의 크기 반환
    }

    void printQueue() {
        // 지정된 출력 형식에 맞게 전체 큐를 출력
        for (int i = 1; i <= heapSize; i++) {
            cout << i << ". "
                 << "[" << heap[i].studentName << ", "
                 << heap[i].score << ", "
                 << heap[i].className << "]" << '\n';
        }
    }

    StudentRecord getRecord(int index) {
        return heap[index]; // 주어진 인덱스의 원소 반환
    }
};

// ------------------------------------------------------------
// 클래스 외부 함수
// - printMenu()         : 메뉴를 출력한다.
// - getValidScore()     : 점수가 정수이며 0~100 범위인지 검사한다.
// - getValidMenuIndex() : 인덱스가 정수이며 1~30 범위인지 검사한다.

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

        if ((int)input.length() == 0) { // 아무 입력도 없는 경우
            cout << "Invalid score. Please enter an integer from 0 to 100." << '\n';
            continue;
        }

        int score = 0;

        // 입력이 숫자로만 이루어져 있는지 검사
        for (int i = 0; i < (int)input.length(); i++) {
            if (input[i] < '0' || input[i] > '9') {
                valid = false;
                break;
            }
            score = score * 10 + (input[i] - '0');
        }

        // 올바른 정수이고 범위가 0~100이면 반환
        if (valid && score >= 0 && score <= 100) {
            return score;
        }

        // 조건을 만족하지 않으면 다시 입력받음
        cout << "Invalid score. Please enter a valid integer between 0 and 100. " << '\n';
    }
}

int getValidMenuIndex() {
    string input;

    while (true) {
        getline(cin, input);
        bool valid = true;
        int index = 0;

        // 빈 입력은 허용하지 않음
        if ((int)input.length() == 0) {
            cout << "Invalid index. Please enter an integer from 1 to 30: ";
            continue;
        }

        // 입력이 숫자인지 확인하고 정수로 변환
        for (int i = 0; i < (int)input.length(); i++) {
            if (input[i] < '0' || input[i] > '9') {
                valid = false;
                break;
            }
            index = index * 10 + (input[i] - '0');
        }

        // 인덱스가 유효한 범위이면 반환
        if (valid && index >= 1 && index <= 30) {
            return index;
        }

        cout << "Invalid index. Please enter an integer from 1 to 30: ";
    }
}

int main() {
    priQueue q;
    string choice;

    while (true) {
        printMenu();
        cout << "Choose menu: ";
        getline(cin, choice);

        // 앞쪽 공백 제거
        while (!choice.empty() && choice.front() == ' ') {
            choice.erase(choice.begin());
        }

        // 뒤쪽 공백 제거
        while (!choice.empty() && choice.back() == ' ') {
            choice.pop_back();
        }

        // 공백만 입력된 경우 다시 메뉴 출력
        if (choice.empty()) {
            cout << '\n';
            continue;
        }

        if (choice.length() != 1) {
            cout << "Invalid menu choice. Please try again." << '\n';
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

            while (true) {
                cout << "Enter the name of the student: ";
                getline(cin, newRecord.studentName);

                //이름 길이 제한 

                if (newRecord.studentName.length() > 20) {
                    cout << "Name must be 20 characters or fewer. Please enter again." << '\n';                }
                else {
                    break;
                }
            }

            newRecord.score = getValidScore();

            cout << "Enter the class name: ";
            getline(cin, newRecord.className);

            if (q.insert(newRecord)) {
                cout << "New element ["
                     << newRecord.studentName << ", "
                     << newRecord.score << ", "
                     << newRecord.className << "] has been inserted." << '\n';
            }
            else {
                cout << "Error: Heap is full. Cannot insert more elements." << '\n';
            }
        }

        else if (command == 'D' || command == 'd') {
            StudentRecord removed;
            if (q.extract_max(removed)) {
                cout << "Deleted element: ["
                     << removed.studentName << ", "
                     << removed.score << ", "
                     << removed.className << "]" << '\n';
            }
            else {
                cout << "Cannot delete from an empty queue." << '\n';
            }
        }

        else if (command == 'R' || command == 'r') {
            StudentRecord top;
            if (q.maximum(top)) {
                cout << "Element with the largest key: ["
                     << top.studentName << ", "
                     << top.score << ", "
                     << top.className << "]" << '\n';
            }
            else {
                cout << "Queue is empty." << '\n';
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

            // 현재 큐 크기를 넘는 인덱스는 사용할 수 없음
            if (index > q.getSize()) {
                cout << "Invalid index. No element exists at that position." << '\n';
                cout << '\n';
                continue;
            }

            StudentRecord currentScore = q.getRecord(index);

            while (true) {
                int newScore = getValidScore();

                // 기존 점수보다 더 큰 값만 허용
                if (newScore <= currentScore.score) {
                    cout << "New score should be larger than current score. Please enter again." << '\n';
                }
                else {
                    q.increase(index, newScore);
                    cout << "Key updated. ";
                    cout << "[" << currentScore.studentName << ", "
                         << newScore << ", "
                         << currentScore.className
                         << "] has been repositioned in the queue." << '\n';
                    break;
                }
            }
        }

        else if (command == 'P' || command == 'p') {
            if (q.isEmpty()) {
                cout << "Queue is empty." << '\n';
            }
            else {
                cout << "Current queue elements: " << '\n';
                q.printQueue();
            }
        }

        else {
            cout << "Invalid menu choice. Please try again." << '\n';
        }

        cout << '\n';
    }
}