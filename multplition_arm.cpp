// multplition_arm.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace chrono;

//设置时间阈值，达到阈值不再重复计算，且要求重复次数随n变化
const double time_limit = 0.01; //（秒）

//平凡算法
void normalway(const vector<vector<long long>>& m, const vector<long long>& a, vector<long long>& r, int n) {
    for (int i = 0; i < n; i++) {
        r[i] = 0.0;

        //按列访问矩阵每一个元素
        for (int j = 0; j < n; j++) {
            r[i] += m[j][i] * a[j];
        }
    }
}

// 优化算法（Optimized）
void advanceway(const vector<vector<long long>>& m, const vector<long long>& a, vector<long long>& r, int n) {
    for (int i = 0; i < n; i++) {
        r[i] = 0.0;
    }
    //按行访问矩阵，每次计算一个部分累加和
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            r[i] += m[j][i] * a[j];
        }
    }
}

int main() {
    vector<int> sizes = { 16,32,64, 128, 256, 512, 1024, 2048, 4096, 16384 }; // 矩阵规模，注意cache大小
    ofstream outfile("multiply_results.txt");
    outfile << "Matrix Size,Trivial Time,Optimized Time,Speedup,Repeats" << endl;

    if (!outfile) {
        cerr << "Failed to open file for writing!" << endl;
        return 1;
    }

    outfile << "矩阵乘向量数据如下" << endl;

    for (int n : sizes) {
        vector<vector<long long>> m(n, vector<long long>(n, 1.0)); // 备注，测试案例全部采用long long 的1.0
        vector<long long> a(n, 1.0); // 初始化向量
        vector<long long> r(n, 0.0);

        
        int repeat = 1;
        do {
            auto start = high_resolution_clock::now();
            for (int i = 0; i < repeat; i++) normalway(m,a,r,n);
            auto end = high_resolution_clock::now();
            double cost = duration<double>(end - start).count();
            if (cost < 0.01) repeat *= 2;
            else break;
        } while (true);


        //平凡算法

        auto start_1 = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < repeat; i++) {
            normalway(m, a, r, n);
        }
        auto end_1 = std::chrono::high_resolution_clock::now();
        double time1 = duration<double>(end_1 - start_1).count()/repeat;
       

        // 优化算法
        auto start_2 = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < repeat; i++) {
            advanceway(m, a, r, n);
        }
        auto end_2 = std::chrono::high_resolution_clock::now();
        double time2=duration<double>(end_2 - start_2).count()/repeat;
  

        cout << "重复次数：" << repeat << endl;
        cout << "规模: " << n << "x" << n << endl;
        cout << "平凡算法: " << time1 << " s" << endl;
        cout << "优化算法: " << time2 << " s" << endl;
     

        outfile << n << "," << time1 << "," << time2 << "," << (time1 / time2) << "\n";
    }

    outfile.close();
    return 0;
}
