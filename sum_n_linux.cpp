#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace chrono;

// 平凡算法
long long normalway(const vector<long long>& a) {
    long long sum = 0;
    for (int i = 0; i < a.size(); i++) {
        sum += a[i];
    }
    return sum;
}

// 多链路累加
//传入数据和链数，链数可改变，暂定（2，4，8，16,32），根据具体的cpu核心修改
long long parallel_advance(const vector<long long>& a, int lanes) {
    vector<long long> sums(lanes, 0);
    int n = a.size();

    //把所有的和累加到最前面的lane个，注意防止溢出，如不足lane
    for (int i = 0; i < n; i += lanes) {
        
        for (int j = 0; j < lanes && (i + j) < n; j++) { 
            sums[j] += a[i + j];
        }
        
    }

    long long total_sum = 0;
    for (int j = 0; j < lanes; j++) {
        total_sum += sums[j];
    }
    return total_sum;
}

// 递归累加
//输入数据，部分和个数，暂定（（2, 4, 8, 16））,注意防止溢出！不足step
long long recursive_advance(vector<long long>& a, int step) {
    int m = a.size();
    while (m > 1) {
        int new_m = (m + step - 1) / step;
        for (int i = 0; i < new_m; i++) {
            long long temp_sum = 0;
            for (int j = 0; j < step && (i * step + j) < m; j++){
                temp_sum += a[i * step + j];
            }
            //把部分和累加到最前面，逐渐缩短为step/1
            a[i] = temp_sum;
        }
        m = new_m;
    }
    return a[0];
}

// 测试函数
void benchmark(int n, ofstream& outfile) {
    vector<long long> data(n, 1.0);//测试案例，全部初始化为1.0

    //当单词运行时间较短时，多次实验求平均值
    int repeat = 1;
    do {
        auto start = high_resolution_clock::now();
        for (int i = 0; i < repeat; i++) normalway(data);
        auto end = high_resolution_clock::now();
        double cost = duration<double>(end - start).count();
        if (cost < 0.01) repeat *= 2;
        else break;
    } while (true);

    // 逐个累加
    auto start = high_resolution_clock::now();
    for (int i = 0; i < repeat; i++) {
        normalway(data);
    }
    auto end = high_resolution_clock::now();

    //得到单词运行时间
    double time1 = duration<double>(end - start).count() / repeat;
    cout << "重复次数" << repeat << endl;
    cout << "数据规模n: " << n << " 平凡算法耗时: " << time1 << " s" << endl;

    // 多链路累加优化算法（2, 4, 8, 16,32）
    vector<long long> times_parallel;
    for (int lanes = 2; lanes <= 32; lanes *= 2) {
        start = high_resolution_clock::now();
        for (int i = 0; i < repeat; i++) {
            parallel_advance(data, lanes);
        }
        end = high_resolution_clock::now();
        double time2 = duration<double>(end - start).count() / repeat;
        times_parallel.push_back(time2);
        cout << "多链路累加(" << lanes << " 链数) time: " << time2 << " s" << endl;
    }

   
    // 结果存入文件
    outfile << "n: " << n << " Serial Time: " << time1 << " ";
    for (size_t i = 0; i < times_parallel.size(); i++) {
        outfile << "Parallel(" << (i + 1) * 2 << " lanes) Time: " << times_parallel[i] << " ";
    }
    for (size_t i = 0; i < times_recursive.size(); i++) {
        outfile << "Recursive(" << (1 << (i + 1)) << " elements) Time: " << times_recursive[i] << " ";
    }
    outfile << endl;
}

int main() {
    ofstream outfile("sum_results.txt");

    //暂定数据大小，根据缓存变化
    vector<int> sizes = { 32,64,128,256,512,1024, 4096, 16384, 65536, 262144, 1048576 };
    for (int n : sizes) {
        benchmark(n, outfile);
    }

    outfile.close();
    cout << "finish" << endl;
    return 0;
}
