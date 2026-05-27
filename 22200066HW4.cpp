/*
Algorithm Analysis Homework 4 - 0/1 Knapsack Comparison
References:
(1) Lecture slide Knapsack Problem(ch.16) page 12 - 19. 
(2) ChatGPT 5.5 Thinking, used for code structure and explanation support
(3) General algorithm knowledge: brute force enumeration, fractional knapsack greedy, DP, branch-and-bound knapsack
(4) Branch and Bound Links
    https://velog.io/@mazorika/%EC%95%8C%EA%B3%A0%EB%A6%AC%EC%A6%98Branch-and-Bound 
    https://sepang2.tistory.com/40
    https://grace7040.tistory.com/77
    https://velog.io/@jiye0710/%EC%95%8C%EA%B3%A0%EB%A6%AC%EC%A6%98-6%EC%9E%A5.-Branch-and-Bound-%EB%B6%84%EA%B8%B0-%ED%95%9C%EC%A0%95%EB%B2%95  
    
(5) fractional knapsack greedy Links
    https://zoomkoding.github.io/algorithm/2019/04/11/knapsack-1.html
    https://jelong.tistory.com/entry/%EC%95%8C%EA%B3%A0%EB%A6%AC%EC%A6%98-%EB%B6%84%EC%84%9D-Greedy-%EC%95%8C%EA%B3%A0%EB%A6%AC%EC%A6%98-%EC%89%BD%EA%B2%8C-%EC%9D%B4%ED%95%B4%ED%95%98%EA%B8%B0-Fractional-Knapsack-Problem%EB%B6%84%EC%88%98-%EB%B0%B0%EB%82%AD-%EB%AC%B8%EC%A0%9C
    https://luz0911.tistory.com/111
*/

#include <bits/stdc++.h>
using namespace std;


//하나의 물건을 저장한다. 
struct Item {
    int benefit;
    int weight;
    double ratio;
};

//알고리즘 실행 결과
struct Result {
    double ms;
    double value;
};

// 아이템 랜덤 생성 기능은 Chat GPT를 사용했다. 
vector<Item> generateItems(int n) {
    mt19937 rng(100); // 시드 값은 100으로 고정한다.
    // Benefit, weight는 각각 500과 100 사이로 한다.
    uniform_int_distribution<int> benefitDist(1, 500); 
    uniform_int_distribution<int> weightDist(1, 100);


    //새로운 아이템 벡터를 만든 다음에 값을 전부 넣고 반환한다.
    vector<Item> items(n);
    for (int i = 0; i < n; i++) {
        items[i].benefit = benefitDist(rng);
        items[i].weight = weightDist(rng);
        items[i].ratio = (double)items[i].benefit / items[i].weight;
    }
    return items; //반환
}


//시간을 재는 함수의 경우 AI를 사용했다.
template <typename Func>
Result measure(Func f) {
    auto start = chrono::high_resolution_clock::now();
    double value = f();
    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();
    return {ms, value};
}

// Brute force 알고리즘도 AI를 사용했다.
// Brute force with meet-in-the-middle subset enumeration.
// It still enumerates subsets, but avoids direct 2^31 looping.
long long bruteForceMITM(const vector<Item>& items, int W) {
    int n = (int)items.size();
    int n1 = n / 2;
    int n2 = n - n1;

    vector<pair<int, long long>> left;
    vector<pair<int, long long>> right;

    int totalLeft = 1 << n1;
    int totalRight = 1 << n2;

    left.reserve(totalLeft);
    right.reserve(totalRight);

    for (int mask = 0; mask < totalLeft; mask++) {
        int w = 0;
        long long b = 0;
        for (int i = 0; i < n1; i++) {
            if (mask & (1 << i)) {
                w += items[i].weight;
                b += items[i].benefit;
            }
        }
        if (w <= W) left.push_back({w, b});
    }

    for (int mask = 0; mask < totalRight; mask++) {
        int w = 0;
        long long b = 0;
        for (int i = 0; i < n2; i++) {
            if (mask & (1 << i)) {
                w += items[n1 + i].weight;
                b += items[n1 + i].benefit;
            }
        }
        if (w <= W) right.push_back({w, b});
    }

    sort(right.begin(), right.end());

    // Remove dominated pairs from right.
    // After this, benefit is strictly increasing as weight increases.
    vector<pair<int, long long>> filtered;
    long long best = -1;
    for (auto &p : right) {
        if (p.second > best) {
            filtered.push_back(p);
            best = p.second;
        }
    }

    long long ans = 0;
    for (auto &p : left) {
        int remain = W - p.first;
        auto it = upper_bound(filtered.begin(), filtered.end(), make_pair(remain, LLONG_MAX));
        if (it != filtered.begin()) {
            --it;
            ans = max(ans, p.second + it->second);
        } else {
            ans = max(ans, p.second);
        }
    }
    return ans;
}

// Greedy는 0 1 이 아니라 Fractional이다.
double greedyFractional(vector<Item> items, int W) {
    //일단 아이템들을 ratio순으로 정렬한다. 
    sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return a.ratio > b.ratio;
    });

    double totalBenefit = 0.0;
    int remain = W;


    //ratio가 큰 순서대로 전부 채워 넣는다.
    for (const auto& item : items) {
        if (remain <= 0) break; //종료


        // 가방안에 통째로 전부 들어가는 경우 그냥 넣기 
        if (item.weight <= remain) {
            remain -= item.weight;
            totalBenefit += item.benefit;
        } 
        //아니면 잘라서 남은 만큼 넣기
        else {
            totalBenefit += item.ratio * remain;
            remain = 0;
        }
    }
    return totalBenefit; //반환
}

long long dynamicProgramming(const vector<Item>& items, int W) {
    vector<long long> dp(W + 1, 0); //Table 만들기 초기값은 전부 0이다.


    for (const auto& item : items) {
        for (int cap = W; cap >= item.weight; cap--) {
            dp[cap] = max(dp[cap], dp[cap - item.weight] + item.benefit);
            //아이템을 안 넣는 경우, dp[cap]. 
            //아이템을 넣는 경우 dp[cap - item.weight] + item.benefit
            //둘중에 더 큰 값을 선택해서 dp에 넣는다. 
        }
    }
    return dp[W];
}

// 너무 느려서 AI에게 어떠한 방법을 사용해야 속도를 올릴 수 있는지 질문한 결과
// 시작 지점에 좋은 최적해를 누해서 그 밑으로는 전부 자른다는 아이디어.
long long zeroOneGreedyLowerBound(const vector<Item>& sortedItems, int W) {
    long long profit = 0;
    int weight = 0;

    //가능한 수준에서 최적해를 구한다.
    for (const auto& item : sortedItems) {
        if (weight + item.weight <= W) {
            weight += item.weight;
            profit += item.benefit;
        }
    }
    return profit;
}


// 트리에서 상태를 저장할 공간
struct Node {
    int level;
    int weight;
    long long profit;
    double bound;
};


//Bound가 더 큰 노드를 먼저 꺼내기 위한 람다함수 구조체
//우선순위를 높여준다.
struct CompareNode {
    bool operator()(const Node& a, const Node& b) const {
        return a.bound < b.bound; // max heap by upper bound
    }
};


double fractionalBound(const vector<Item>& items, int W, int level, int weight, long long profit) {
    if (weight > W) return 0.0; //종료조건

    double result = profit; 
    int totalWeight = weight;
    int n = (int)items.size();


    //위랑 논리는 유사하다.
    for (int i = level; i < n; i++) {

        //넣을 수 있다면 전부 넣기
        if (totalWeight + items[i].weight <= W) {
            totalWeight += items[i].weight;
            result += items[i].benefit;
        } 

        //넣을 수 없다면 잘라서 넣는다.
        else {
            int remain = W - totalWeight;
            result += items[i].ratio * remain;
            break;
        }
    }
    return result;
}



long long branchAndBound(vector<Item> items, int W) {

    //비율이 더 큰 순서로 Sort한다 (람다 사용)
    sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return a.ratio > b.ratio;
    });

    //여기서 위에 만들었던 최적화 구하는 함수를 사용한다.
    long long maxProfit = zeroOneGreedyLowerBound(items, W); 

    // PQ를 사용해서 가장 bound가 높은 노드부터 꺼낸다.
    priority_queue<Node, vector<Node>, CompareNode> pq;
    //초기 노드는 당연히 아무것도 없는 상태
    Node root{0, 0, 0, fractionalBound(items, W, 0, 0, 0)};
    pq.push(root);

    int n = (int)items.size();

    //가장 큰 노느 u 꺼내면서 시작
    while (!pq.empty()) {
        Node u = pq.top();
        pq.pop();

        if (u.bound <= maxProfit) continue; //만약에 최적해 보다 좋아질 수 없다? 버림
        if (u.level >= n) continue; // 이미 전부 선택한 노드? 버림

        const Item& current = items[u.level]; //현재 확인해볼 노드를 저장한다. 

        // 지금 아이템을 넣는 경우를 비교한다. 
        Node include;
        include.level = u.level + 1;
        include.weight = u.weight + current.weight;
        include.profit = u.profit + current.benefit;

        // 만약 넣었는데 용량도 안넘고 최적해보다 좋다면, 최적해 갱신
        if (include.weight <= W && include.profit > maxProfit) {
            maxProfit = include.profit;
        }

        include.bound = fractionalBound(items, W, include.level, include.weight, include.profit);
        // 만약 이 바운드가 지금의 최고 정답보다 크면 더 좋아질 여지가 있기에 큐에 넣는다.
        if (include.bound > maxProfit) {
            pq.push(include);
        }

        // 안 넣는 경우

        //전부 그대로 내려간다.
        Node exclude;
        exclude.level = u.level + 1;
        exclude.weight = u.weight;
        exclude.profit = u.profit;
        exclude.bound = fractionalBound(items, W, exclude.level, exclude.weight, exclude.profit);

        // 만약 이 바운드가 지금의 최고 정답보다 크면 더 좋아질 여지가 있기에 큐에 넣는다.
        if (exclude.bound > maxProfit) {
            pq.push(exclude);
        }
    }

    return maxProfit; // 전부 했다면 현재 maxProfit을 이길 수 있는 후보가 없다는 뜻. 리턴한다.
}


//단순 결과 출력
void printResult(const Result& r) {
    cout << fixed << setprecision(3) << r.ms << " / " << fixed << setprecision(2) << r.value;
}


// 출력문은 AI의 도움을 받았다.
// 출력문 양식 지키기 + 시간 출력 + bruteForce 출력
int main() {
    cout << "[Brute Force]\n";
    cout << left << setw(18) << "Number of Items" << "Processing time in milliseconds / Maximum benefit value\n";

    vector<int> bruteSizes = {11, 21, 31}; //사이즈를 벡터에 넣고 반복문을 돌린다. 
    for (int n : bruteSizes) {
        int W = n * 25;
        vector<Item> items = generateItems(n); //랜덤 아이템을 만들고
        Result r = measure([&]() { return (double)bruteForceMITM(items, W); }); 
        cout << left << setw(18) << n; // 출력한다. 
        printResult(r);
        cout << "\n";
    }

    cout << "\n[Greedy / D.P. / B. & B.]\n"; //나머지 그리디, Dp, branch bound 출력
    cout << left << setw(12) << "Items"
         << setw(28) << "Greedy"
         << setw(28) << "D.P."
         << setw(28) << "B. & B." << "\n";

    vector<int> sizes = {10, 100, 1000, 10000}; //사이즈는 주어진 표를 사용한다. 
    for (int n : sizes) {
        int W = n * 25;
        vector<Item> items = generateItems(n);

        Result greedy = measure([&]() { return greedyFractional(items, W); });
        Result dp = measure([&]() { return (double)dynamicProgramming(items, W); });
        Result bb = measure([&]() { return (double)branchAndBound(items, W); });
        // 결과를 받아서 넣고 

        cout << left << setw(12) << n;
        stringstream ss1, ss2, ss3;
        ss1 << fixed << setprecision(3) << greedy.ms << " / " << fixed << setprecision(2) << greedy.value;
        ss2 << fixed << setprecision(3) << dp.ms << " / " << fixed << setprecision(2) << dp.value;
        ss3 << fixed << setprecision(3) << bb.ms << " / " << fixed << setprecision(2) << bb.value;
        cout << setw(28) << ss1.str()
             << setw(28) << ss2.str()
             << setw(28) << ss3.str() << "\n";
    }
    //출력

    return 0;
}
    