#include <iostream>
#include "TTester.h"
int main()
{
		srand(time(NULL));

	TTester test;
	test.Test();
//	auto tasks = std::vector<std::future<void>>();
//	tasks.push_back(std::async(std::launch::async, [](){while(true) std::cout<<""<<"\n";}));
//	tasks.push_back(std::async(std::launch::async, [](){while(true) std::cout<<"2"<<"\n";}));
//	for(auto& t : tasks) {
//		t.get();
//	}

//	auto vec = std::vector<int>();
//	auto vec1 = std::vector<int>();
//	for(int i=0;i<10;++i) {
//		vec.push_back(i);
//		vec1.push_back(i);
//	}
//	vec.insert(vec.end(), vec1.begin(), vec1.end());
//	for(const auto& el : vec) {
//		std::cout<<el<<"\n";
//	}
}