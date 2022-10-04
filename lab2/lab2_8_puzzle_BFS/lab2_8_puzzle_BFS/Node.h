#pragma once
#include "State.h"
#include <deque>
#include <stack>

class Node {
	public:
	Node() = default;
	Node(const Node& other) = default;
	Node(const State& state, Node* father, const int action, const int depth) noexcept;
	virtual ~Node() = default;

	public:
	virtual void Print() const noexcept;
	virtual void Expand(std::deque<Node>& deque) noexcept;
	virtual bool IsExpanded(const std::deque<State>& deque) const noexcept;

	public:
	virtual int BFS() noexcept;
	
	protected:
	State m_xState;
	Node* m_pFather = nullptr;
	int m_iAction = -1;
	int m_iCost = 1;
	int m_iDepth = 0;

	public:
	unsigned long long TotalNodes = 0;
	unsigned long long DeadEnd = 0;
	unsigned long long States = 0;
	unsigned long long Iterations = 0;
};

