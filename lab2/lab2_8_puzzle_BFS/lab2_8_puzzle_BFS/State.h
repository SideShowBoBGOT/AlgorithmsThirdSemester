#pragma once
#include <iostream>
#include <array>
#include <random>
class State {

	public: 
	State() noexcept;
	State(const State& other) = default;
	virtual ~State() = default;
	virtual int FindZero() const noexcept;
	virtual State Exchange(int i, int j);
	virtual bool IsEqual(const State& s) const noexcept;
	virtual bool IsGoal() const noexcept;
	virtual void Print() const noexcept;

	public:
	static constexpr int BoardSize = 9;
	static constexpr std::array<int, BoardSize> Goal = {1, 2, 3, 8, 0, 4, 7, 6, 5};
	std::array<int, BoardSize> Board = {1, 2, 3, 4, 5, 6, 7, 8, 0};

	
};
