#include <iostream>
#include <vector>
#include <set>
#include <memory>
#include <array>
struct STown;
struct SRoad;

class TGraph {
	public:
	TGraph()=default;
	virtual ~TGraph()=default;
	void GenerateTowns() noexcept;
	void Search() noexcept;
	void PrintResult() noexcept;

	public:
	int alpha = 3;
	int beta = 2;
	double density = 0.6;
	int totalAnts = 45;
	int eliteAnts = 15;
	int commonAnts = 30;

	std::vector<std::shared_ptr<STown>> allTowns;

	struct SRoad {
		std::shared_ptr<STown> NeighbourTown = nullptr;
		int Cost = 0;
		double Pheromone = 0.0;
	};
	struct STown {
		std::vector<SRoad> Distances;
	};
	class TAnt {
		std::shared_ptr<STown> CurrentTown = nullptr;
	};
};



int main() {

}