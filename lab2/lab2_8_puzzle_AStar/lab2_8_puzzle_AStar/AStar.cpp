#include "AStar.h"
#include <algorithm>
AStar::Node::Node(state& st, int depth, double fscore, AStar::ActionType action, Node* parent) noexcept {
	State = st;
	Depth = depth;
	fScore = fscore;
	Action = action;
	Parent = parent;
}

AStar::Node::Node(state&& st, int depth, double fscore, AStar::ActionType action, Node* parent) noexcept {
	State = st;
	Depth = depth;
	fScore = fscore;
	Action = action;
	Parent = parent;
}

bool AStar::Node::operator<(const Node& other) const noexcept {
	return fScore < other.fScore;
}

bool AStar::Node::operator>(const Node& other) const noexcept {
	return fScore > other.fScore;
}

bool AStar::Node::operator==(const Node& other) const noexcept {
	return fScore == other.fScore;
}


std::ostream& AStar::Node::operator<<(std::ostream& out) const noexcept {
	for(int i=0;i<State.size();i++) {
		for(int j=0;j<State[i].size();j++) {
			out<<State[i][j]<<" ";
		}
		out<<"\n";
	}
	out<<"Depth: "<<Depth<<"\n";
	out<<"Action: "<<magic_enum::enum_name(Action)<<"\n";

	return out;
}

AStar::AStar(state initialState, state goalState) noexcept {
	m_xInitialState = initialState;
	m_xGoalState = goalState;
}

bool AStar::Solve() noexcept {
	if(!IsSolvable()) {
		return false;
	}
	
	auto initialNode = new Node(m_xInitialState, 0, 0, AStar::ActionType::None, nullptr);
	initialNode->fScore = CalculateFScore(initialNode);
	
	m_vFrontier.push(initialNode);

	m_iTotalNodesGenerated++;

	m_xEndNode = ExploreFrontier();
	
	return true;
}

void AStar::ShowAnswer() const noexcept {
	PrintStatistic();
	for(const auto& action:m_vActions) {
		std::cout<<magic_enum::enum_name(action)<<std::endl;
	}
}


AStar::Node* AStar::ExploreFrontier() noexcept {

	while(!m_vFrontier.empty()) {
		m_iIterations++;
		auto currentNode = m_vFrontier.top();
		m_vFrontier.pop();

		m_iNumNodesExplored++;

		m_vExploredStates.insert(&currentNode->State);

		if(currentNode->State==m_xGoalState) {
			TracePath(currentNode);
			return currentNode;
			break;
		}


		auto children = GenerateChildren(currentNode);


		for(const auto& child:children) {
			m_vFrontier.push(child);
		}
			
		m_iTotalNodesGenerated += static_cast<int>(children.size());

		m_iMaxFrontierSize = std::max<int>(m_iMaxFrontierSize, static_cast<int>(m_vFrontier.size()));
	}
	return nullptr;
}

void AStar::TracePath(AStar::Node* node) noexcept {
	m_iNumMoves = node->Depth;
	while(node->Parent!=nullptr) {
		m_vActions.push_back(ReverseAction(node->Action));
		node = node->Parent;
	}
	m_vActions.reverse();
}

void AStar::PrintStatistic() const noexcept {
	std::cout<<"Total nodes generated: " << m_iTotalNodesGenerated<<"\n";
	std::cout<<"Dead Ends: " << m_iNumNodesExplored<<"\n";
	std::cout<<"Max nodes in memory: "<< m_iMaxFrontierSize << "\n";
	std::cout<<"Iterations: "<< m_iIterations << "\n";
	std::cout<<"Number of moves: "<< m_iNumMoves << "\n";
}

bool AStar::IsSolvable() const noexcept {
	auto inversion = 0;
	for(int i=0;i<s_iBoardRows*s_iBoardCols;i++) {
		auto y1 = i / s_iBoardRows;
		auto x1 = i % s_iBoardCols;
		for(int j=i+1;j<s_iBoardRows*s_iBoardCols;j++) {
			auto y2 = j / s_iBoardRows;
			auto x2 = j % s_iBoardCols;

			if(m_xInitialState[y1][x1]>0) {
				if(m_xInitialState[y2][x2]>0) {
					if(m_xInitialState[y2][x2] < m_xInitialState[y1][x1]) {
						inversion++;
					}
				}
			}
		}
	}

	return (inversion % 2)==0;
}

double AStar::CalculateFScore(Node* currentNode) const noexcept {
	auto score = double(CalculateHamming(currentNode->State) + CalculateManhattan(currentNode->State)) / 2.0;
	return currentNode->Depth + score;
}

double AStar::CalculateHamming(const AStar::state& currentState) const noexcept {
	auto score = 0;
	for(int i=0;i<s_iBoardRows;i++) {
		for(int j=0;j<s_iBoardCols;j++) {
			if(currentState[i][j] != m_xGoalState[i][j] and currentState[i][j]!=0)
				score++;
		}
	}
	return score;
}

double AStar::CalculateManhattan(const state& currentState) const noexcept {
	auto score = 0;
	for(int i=0;i<s_iBoardRows;i++) {
		for(int j=0;j<s_iBoardCols;j++) {
			if(currentState[i][j]!=0) {
				auto x = (currentState[i][j] - 1) / s_iBoardCols;
				auto y = (currentState[i][j] - 1) % s_iBoardCols;
				score += static_cast<int>(std::abs(x - i) + abs(y - j));
			}
		}
	}
	return score;
}


std::vector<AStar::Node*> AStar::GenerateChildren(Node* parent) const noexcept {
	auto children = std::vector<Node*>();
	auto actions = {ActionType::Up, ActionType::Down, ActionType::Left, ActionType::Right};
	
	for(const auto& action:actions) {
		if(action==ReverseAction(parent->Action)) continue;
		auto newState = GetNewState(parent->State, action);
		if(newState==parent->State) continue;
		auto& ex = m_vExploredStates;
		if(std::find_if(ex.begin(), ex.end(), [&newState](state* st) {return *st==newState;})!=ex.end()) continue;
		auto child = new Node(std::move(newState), parent->Depth + 1, 0, action, parent);
		child->fScore = CalculateFScore(child);
		children.push_back(child);
	}

	return children;
}

AStar::ActionType AStar::ReverseAction(const AStar::ActionType& action) const noexcept {
	auto newAction = ActionType::None;

	switch(action) {
		case ActionType::Up: newAction = ActionType::Down; break;
		case ActionType::Down: newAction = ActionType::Up; break;
		case ActionType::Left: newAction = ActionType::Right; break;
		case ActionType::Right: newAction = ActionType::Left; break;
	}

	return newAction;
}

AStar::state AStar::GetNewState(const AStar::state& st, const AStar::ActionType& action) const noexcept {
	
	auto newState = st;

	auto zeroPos = FindBlankPosition(st);

	auto y1 = zeroPos.first;
	auto x1 = zeroPos.second;

	auto y2 = 0;
	auto x2 = 0;

	switch(action) {
		case ActionType::Up: y2 = y1 - 1; x2 = x1; break;
		case ActionType::Down: y2 = y1 + 1; x2 = x1; break;
		case ActionType::Left: y2 = y1; x2 = x1 - 1; break;
		case ActionType::Right: y2 = y1; x2 = x1 + 1; break;
	}

	if(0<=y2 and y2<s_iBoardRows and 0<=x2 and x2<s_iBoardCols) {
		std::swap(newState[y2][x2], newState[y1][x1]);
	}

	return newState;
}

std::pair<int, int> AStar::FindBlankPosition(const AStar::state& st) const noexcept {
	for(int i=0;i<st.size();i++) {
		for(int j=0;j<st[i].size();j++) {
			if(st[i][j]==0) 
				return std::make_pair(i, j);
		}
	}
}






