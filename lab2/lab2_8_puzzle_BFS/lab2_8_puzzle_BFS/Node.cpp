#include "Node.h"
#include <list>

unsigned long long Node::TotalNodes=0;
unsigned long long Node::Iterations=0;

Node::Node(const State& state, Node* father, const int action, const int depth) noexcept {
	m_xState = state;
	m_pFather = father;
	m_iAction = action;
	m_iDepth = depth;
}

void Node::Print() const noexcept {
	m_xState.Print();
}


void  Node::ShowPath() const noexcept {
	auto actions = std::list<std::string>();
	auto node = this;
	while(node->m_pFather!=nullptr) {
		switch(node->m_iAction) {
			case 0: actions.push_back("UP"); break;
			case 1: actions.push_back("DOWN"); break;
			case 2: actions.push_back("RIGHT"); break;
			case 3: actions.push_back("LEFT"); break;
		}
		node = node->m_pFather;
	}
	actions.reverse();
	for(const auto& action:actions) {
		std::cout<<action<<std::endl;
	}
}


void Node::Expand(std::deque<Node*>& deque) noexcept {
	auto p = m_xState.FindZero();
	//UP
	if((p!=0 and p!=1 and p!=2) and m_iAction!=1) {
		TotalNodes++;
		deque.push_back(new Node(m_xState.Exchange(p, p-3), this, 0, m_iDepth+1));
	}
	//DOWN
	if((p!=6 and p!=7 and p!=8) and m_iAction!=0) {
		TotalNodes++;

		deque.push_back(new Node(m_xState.Exchange(p, p+3), this, 1, m_iDepth+1));
	}
	//RIGHT
	if((p!=2 and p!=5 and p!=8) and m_iAction!=3) {
		TotalNodes++;

		deque.push_back(new Node(m_xState.Exchange(p, p+1), this, 2, m_iDepth+1));
	}
	//LEFT
	if((p!=0 and p!=3 and p!=6) and m_iAction!=2) {
		TotalNodes++;

		deque.push_back(new Node(m_xState.Exchange(p, p-1), this, 3, m_iDepth+1));
	}
}

bool Node::IsExpanded(const std::deque<State>& deq) const noexcept {
	auto max = deq.size()>m_iDepth?m_iDepth:deq.size();
	for(int i=0;i<max;i++) {
		if(m_xState.IsEqual(deq[i])) {
			return true;
		}
	}
	return false;
}


int Node::BFS() noexcept {
	auto ToExpand = std::deque<Node*>();
	auto expanded = std::deque<State>();

	ToExpand.push_back(this);
	while(!ToExpand.empty()) {
		
		if(ToExpand.front()->m_xState.IsGoal()) {
			ToExpand.front()->ShowPath();
			Iterations++;
			States = static_cast<int>(expanded.size());
			m_iCost = ToExpand.front()->m_iCost;
			ToExpand.clear();
			return m_iCost;
		} else {
			Iterations++;
			DeadEnd++;
			if(!(ToExpand.front()->IsExpanded(expanded))) {
				ToExpand.front()->Expand(ToExpand);
				expanded.push_front(ToExpand.front()->m_xState);
				ToExpand[1]->m_iCost = ToExpand[0]->m_iCost+1;
			}
			ToExpand.pop_front();
		}
	}
	if(ToExpand.empty()) std::cout << std::endl << "Solution NOT found!" << std::endl;
	return 0;
}