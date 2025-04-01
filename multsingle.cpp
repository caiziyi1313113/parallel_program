// multsingle.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace chrono;

// 多链路累加
//传入数据和链数，链数可改变，暂定（2，4，8，16,32），根据具体的cpu核心修改
long long parallel_advance(const vector<long long>& a) {
    //vector<long long> sums(2, 0.0);
    int n = a.size();
    long long sum1 = 0, sum2 = 0;
        //把所有的和累加到最前面的lane个，注意防止溢出，如不足lane
    for (int i = 0; i + 1 < n; i += 2) {
        sum1 += a[i];
        sum2 += a[i + 1];

    }

    long long sum = sum1 + sum2;
    return sum;
}

void benchmark(int n) {

    vector<long long> data(n, 1.0);//测试案例，全部初始化为1.0
    //当单词运行时间较短时，多次实验求平均值
    int repeat = 1;
    do {
        auto start = high_resolution_clock::now();
        for (int i = 0; i < repeat; i++) parallel_advance(data);
        auto end = high_resolution_clock::now();
        double cost = duration<double>(end - start).count();
        if (cost < 0.01) repeat *= 2;
        else break;
    } while (true);

    // 多链路累加优化算法（2, 4, 8, 16,32）

    auto start = high_resolution_clock::now();
    for (int i = 0; i < repeat; i++) {
        parallel_advance(data);
    }
    auto end = high_resolution_clock::now();
    double time2 = duration<double>(end - start).count() / repeat;
    std::cout << "多链路累加(" << 2 << " 链数) time: " << time2 << " s" << endl;

}

int main() {

    //暂定数据大小，根据缓存变化
    vector<int> sizes = { 64,512, 4096, 16384, 65536, 262144, 1048576 };
    for (int n : sizes) {
        benchmark(n);
    }
    std::cout << "finish" << endl;
    return 0;
}