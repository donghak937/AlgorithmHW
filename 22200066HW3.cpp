#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

int recursive(int k, int n) {
    if (k == 1) return n; //구슬 한개면 다 던져야함
    if (n == 1) return 1; //층 한개면 1
    if (n == 0) return 0; // 0 이면 0  

    int min_drops = 1e9; 

    
    for (int x = 1; x <= n; x++) {
        int case1 = recursive(k - 1, x - 1);
        int case2 = recursive(k, n - x);
        
        int worst_case = 1 + max(case1, case2);
        min_drops = min(min_drops, worst_case);
    }
    return min_drops;
}

int DP(int k, int n) {

    // DP 테이블
    vector<vector<int>> dp(k + 1, vector<int>(n + 1, 0));

    for (int i = 1; i <= k; i++) {
        dp[i][1] = 1; // 층이 1개일 때
        dp[i][0] = 0; // 층이 0개일 때
    }
    for (int j = 1; j <= n; j++) {
        dp[1][j] = j; // 구슬이 1개일 때
    }

    // DP 테이블 채우기
    for (int i = 2; i <= k; i++) { // 구슬 수
        for (int j = 2; j <= n; j++) { // 층 수
            dp[i][j] = 1e9; //짱 큰수

            for (int x = 1; x <= j; x++) {
                int b = dp[i - 1][x - 1]; // 구슬이 깨질 때
                int s = dp[i][j - x];     // 구슬이 살아남을 때
                
                int worst = 1 + max(b, s);
                dp[i][j] = min(dp[i][j], worst); // 모든 시도 중 최솟값
            }
        }
    }

    return dp[k][n];
}

int main() {
    int n, k;

    cout << "Enter number of floors (N) and glass balls (K): ";
    cin >> n >> k;

    // --- 재귀 함수 실행 및 시간 측정 --- 
    // 인터넷 및 AI 참고
    // reference site : https://giveme-happyending.tistory.com/221
    
    auto start_rec = high_resolution_clock::now();
    int res_rec = recursive(k, n);

    auto stop_rec = high_resolution_clock::now();
    auto duration_rec = duration_cast<microseconds>(stop_rec - start_rec);

    cout << "\nRecursive" << endl;
    cout << "Min : " << res_rec << endl;
    cout << "Time: " << duration_rec.count() << " ms" << endl;

    // --- DP 함수 실행 및 시간 측정 ---
    auto start_dp = high_resolution_clock::now();
    int res_dp = DP(k, n);
    auto stop_dp = high_resolution_clock::now();
    auto duration_dp = duration_cast<microseconds>(stop_dp - start_dp);

    cout << "\nDynamic Programming" << endl;
    cout << "Min : " << res_dp << endl;
    cout << "Time: " << duration_dp.count() << " ms" << endl; 
    return 0;
}