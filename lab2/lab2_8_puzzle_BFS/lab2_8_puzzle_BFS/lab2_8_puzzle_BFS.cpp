#include "Node.h"

int main()
{
	auto node = Node();
	node.Print();
	std::cout<<std::endl;
	if(!node.BFS()) return 0;
	std::cout<<"Number of Nodes: "<<Node::TotalNodes<<std::endl;
	std::cout<<"Number of DeadEnds: "<<node.DeadEnd<<std::endl;
	std::cout<<"Number of States: "<<node.States<<std::endl;
	std::cout<<"Number of Iterations: "<<Node::TotalNodes<<std::endl;
	system("pause");
}
