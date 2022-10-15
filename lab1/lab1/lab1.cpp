#include "BalancedMultiwayMergeSort.h"

#include "Timer.h"
#include <iostream>
#include <fstream>
#include <random>
#include <thread>
#include <future>

int main() {    
    srand(time(NULL));
    auto file = std::fstream("tiny.txt", std::ios::out|std::ios::binary);

    for(int i=0;i<1000;i++) {
		auto val = double(rand()%60000);
        file.write(reinterpret_cast<char*>(&val), sizeof(double));
		
	}
    file.close();
    auto bsort = BalancedMultiwayMergeSort<double>("tiny.txt", 10, 14);
    bsort.SentinelValue = -1.0;
    bsort.Sort();
    file.open("result_binary.txt", std::ios::in|std::ios::binary);
    auto val = 0.0;
    for(;!file.eof();) {
        file.read(reinterpret_cast<char*>(&val), sizeof(val));
        std::cout<<val<<std::endl;
    }
}
