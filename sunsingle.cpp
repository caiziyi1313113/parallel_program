#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace chrono;


// 递归累加
//输入数据，部分和个数，暂定（（2, 4, 8, 16））,注意防止溢出！不足step
long long recursive_advance(vector<long long>& a, int step) {
    int m = a.size();
    for (int new_m = m; new_m > 1; new_m /= 2) {
        for (int i = 0; i < new_m/2; i++) {
            a[i] = a[i * 2] + a[i * 2 + 1];
        }
    }
    return a[0];
}

// 测试函数
void benchmark(int n) {
    vector<long long> data(n, 1.0);//测试案例，全部初始化为1.0
    int s = 2;
    //当单词运行时间较短时，多次实验求平均值
    int repeat = 1;
    do {
        auto start = high_resolution_clock::now();
        for (int i = 0; i < repeat; i++) recursive_advance(data,2);
        auto end = high_resolution_clock::now();
        double cost = duration<double>(end - start).count();
        if (cost < 0.01) repeat *= 2;
        else break;
    } while (true);


    // 递归累加（2)
  
  
    vector<long long> temp_data = data;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < repeat; i++) {
        recursive_advance(temp_data, 2);
    }
    auto end = high_resolution_clock::now();
    double time3 = duration<double>(end - start).count() / repeat;
    cout << "递归累加(" << 2 << " 步长) Time: " << time3 << " s" << endl;
       
    

}
int main() {

    //暂定数据大小，根据缓存变化
    vector<int> sizes = { 64,512, 4096, 16384, 65536, 262144, 1048576 };
    for (int n : sizes) {
        benchmark(n);
    }
    return 0;
}