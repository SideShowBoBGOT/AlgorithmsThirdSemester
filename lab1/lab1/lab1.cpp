#include "BalancedMultiwayMergeSort.h"
#include "ModifiedSort.h"
#include "Timer.h"
#include <iostream>
#include <fstream>
#include <random>
#include <thread>
#include <future>

int main() {
    // тут сам код
    
    Timer timer;
    timer.start();
    
    auto bsort = ModifiedSort("monster.txt", 20000, 14, true);
    timer.stop();
    std::cout << "Seconds: " << timer.elapsedSeconds() << std::endl;
    std::cout << "Milliseconds: " << timer.elapsedMilliseconds() << std::endl;


    
    
   /* auto file = std::fstream("lab.txt", std::ios::out|std::ios::app);
    std::default_random_engine generator;
    generator.seed(std::time(0));
    
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    while(true) {
        file<<distribution(generator)<<"\n";
    }*/
}
