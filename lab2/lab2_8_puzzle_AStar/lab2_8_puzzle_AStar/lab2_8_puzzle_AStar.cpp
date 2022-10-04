#include "AStar.h"
#include <random>
#include <algorithm>
#include <iterator>
int main() {

	auto initial = std::array<int, 9> {3, 0, 7, 6, 1, 5, 4, 2, 8};
	std::random_device rd;
	std::mt19937 g(rd());

	
	std::shuffle(initial.begin(), initial.end(), g);

	auto initialState = AStar::state();

	for(int i=0;i<3;i++) {
		for(int j=0;j<3;j++) {
			initialState[i][j] = initial[i*3 + j];
		}
	}

	auto goalState = AStar::state{1, 2, 3, 4, 0, 5, 6, 7, 8};

	auto astar = AStar(initialState, goalState);
	if(astar.Solve()) {
		for(int i=0;i<3;i++) {
			for(int j=0;j<3;j++) {
				std::cout<<initialState[i][j]<<" ";
			}
			std::cout<<std::endl;
		}
		astar.ShowAnswer();
		system("pause");

	}
}