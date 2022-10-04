#include "State.h"
State::State() noexcept {
	srand(time(0));
	std::random_shuffle(Board.begin(), Board.end());
}

void State::Print() const noexcept {
	for(int i=0; i<BoardSize; i++) {
		if(i>0 && i%3==0) std::cout << std::endl;
		std::cout << Board[i] << "\t";
	}
}

int State::FindZero() const noexcept {
	for(int i=0;i<BoardSize;i++) {
		if(Board[i]==0) return i;
	}
}

State State::Exchange(int i, int j) {
	auto b = *this;
	std::swap(b.Board[i], b.Board[j]);
	return b;
}

bool State::IsEqual(const State& s) const noexcept {
	return Board==s.Board;
}

bool State::IsGoal() const noexcept {
	return Board==Goal;
}