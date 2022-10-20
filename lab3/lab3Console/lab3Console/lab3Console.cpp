#include <iostream>
#include <vector>
#include <cassert>

bool SharrahSearch(int id, std::vector<int>& block) {
    auto flag = true;
    auto num = int(block.size());
    auto k = int(std::log2(num));
    auto i = int(std::pow(2, k));
    auto b = 0;
    if(id<block[i]) {
        b = int(std::pow(2, k - 1));
    } else {
        auto l = int(std::log2(num - int(std::pow(2, k)) + 1));
        i = num + 1 - int(std::pow(2, l));
        b = int(std::pow(2, l - 1));
    }  
    while(flag) {
        if(b>=0) {
            if(block[i]==id) {
                return true;
            } else if(block[i]<id) {
                i += b/2 + 1;
                b = b/2;
            } else if(block[i]>id){
                i -= b/2 + 1;
                b = b/2;
            }
        } else {
            flag = false;
        }
    }
    return false;
}
void main() {
	srand(time(NULL));
    auto vec = std::vector<int>(1000);
    for(int i = 0; i < 1000; i++) {
        vec[i] = i;
    }

    for(int i = 0; i < 1000; i++) {
        std::cout<<"Search: "<<i<<std::endl;
        const auto res = SharrahSearch(i, vec);
        assert(res);
    }
}