#pragma once
#include <magic_enum.hpp>
#include <iostream>
#include <vector>
#include <set>
#include <array>
#include <list>
#include <queue>

class AStar {
	public:
	static constexpr int s_iBoardCols = 3;
	static constexpr int s_iBoardRows = 3;
	
	public:
	using row = std::array<int, s_iBoardCols>;
	using state = std::array<row, s_iBoardRows>;

	protected:
	enum class ActionType {
		Up,
		Down,
		Left,
		Right,
		None
	};

	protected:
	class Node {
		public:
		Node() = default;
		Node(state& st, int depth, double fScore, ActionType action, Node* parent) noexcept;
		Node(state&& st, int depth, double fScore, ActionType action, Node* parent) noexcept;
		virtual ~Node() = default;

		virtual bool operator<(const Node& other) const noexcept;
		virtual bool operator==(const Node& other) const noexcept;
		virtual bool operator>(const Node& other) const noexcept;
		virtual std::ostream& operator<<(std::ostream& out) const noexcept;

		public:
		state State;
		int Depth = 0;
		double fScore = 0;
		ActionType Action = ActionType::None;
		Node* Parent = nullptr;
	};

	struct compare {
		bool operator()(Node* n1, Node* n2) {
			return n1->operator>(*n2);
		}
	};

	protected:
	state m_xInitialState;
	state m_xGoalState;
	std::list<ActionType> m_vActions;
	std::set<state*> m_vExploredStates;
	std::priority_queue<Node*, std::vector<Node*>, compare> m_vFrontier;

	protected:
	int m_iNumMoves = 0;
	int m_iNumNodesExplored = 0;
	int m_iTotalNodesGenerated = 0;
	int m_iIterations = 0;
	int m_iMaxFrontierSize = 0;
	Node* m_xEndNode = nullptr;

	public:
	AStar(state initialState, state goalState) noexcept;
	virtual bool Solve() noexcept;
	virtual void ShowAnswer() const noexcept;
	virtual void TracePath() noexcept;

	protected:
	virtual Node* ExploreFrontier() noexcept;
	virtual void PrintStatistic() const noexcept;
	virtual bool IsSolvable() const noexcept;
	virtual double CalculateFScore(Node* currentNode) const noexcept;
	virtual double CalculateHamming(const state& currentState) const noexcept;
	virtual double CalculateManhattan(const state& currentState) const noexcept;
	virtual std::vector<Node*> GenerateChildren(Node* parent) const noexcept;
	virtual ActionType ReverseAction(const ActionType& action) const noexcept;
	virtual state GetNewState(const state& st, const ActionType& action) const noexcept;
	virtual std::pair<int, int> FindBlankPosition(const state& st) const noexcept;
};