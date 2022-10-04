#include "Node.h"

int main()
{
	auto node = Node();
	node.Print();
	node.BFS();
	std::cout<<std::endl;
	std::cout<<"Number of Nodes: "<<node.TotalNodes<<std::endl;
	std::cout<<"Number of DeadEnds: "<<node.DeadEnd<<std::endl;
	std::cout<<"Number of States: "<<node.States<<std::endl;
	std::cout<<"Number of Iterations: "<<node.TotalNodes<<std::endl;
	system("pause");
}
