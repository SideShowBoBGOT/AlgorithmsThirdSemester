#include <algorithm>
#include <random>
#include <chrono>
#include "TAntAlgorithm.h"
TAntAlgorithm::TAntAlgorithm(const std::vector<std::vector<unsigned>>& inputMat, unsigned totalAnts, unsigned eliteAnts,
	unsigned alpha, unsigned beta, double density): m_uTotalAnts{totalAnts},
	m_uEliteAnts{eliteAnts}, m_uAlpha{alpha}, m_uBeta{beta}, m_dR{density} {
	CreateNodes(inputMat);
	CreateEdges(inputMat);
	CreateAnts();
}

void TAntAlgorithm::RenewPheromones() {
	for(auto& node : m_vNodes) {
		for(auto& edge : node->Edges) {
			edge->Pheromone = NNHelpFunctions::GenerateBetweenZereOne();
		}
	}
}

void TAntAlgorithm::CreateNodes(const std::vector<std::vector<unsigned>>& inputMat) {
	auto inputSize = static_cast<unsigned>(inputMat.size());
	m_vNodes.reserve(inputSize);
	for(unsigned i=0;i<inputSize;++i) {
		auto node = std::make_shared<SNode>();	
		node->Index = i;
		m_vNodes.push_back(std::move(node));
	}
}

void TAntAlgorithm::CreateEdges(const std::vector<std::vector<unsigned>>& inputMat) {
	auto inputSize = static_cast<unsigned>(inputMat.size());
	for(unsigned i=0;i<inputSize;++i) {
		auto& node = m_vNodes[i];
		node->Edges.reserve(inputSize);
		for(unsigned j=0;j<inputSize;++j) {
			auto edge = std::make_shared<SEdge>();
			edge->StartNode = node;
			edge->EndNode = m_vNodes[j];
			edge->Distance = inputMat[i][j];
			edge->Sight = (inputMat[i][j])?1.0/inputMat[i][j]:0;
			edge->Pheromone = NNHelpFunctions::GenerateBetweenZereOne();
			node->Edges.push_back(std::move(edge));
		}
	}
}

void TAntAlgorithm::CreateAnts() {
	m_vAnts.reserve(m_uTotalAnts);
	auto commonAntsNumber = m_uTotalAnts - m_uEliteAnts;
	for(unsigned i=0;i<commonAntsNumber;++i) {
		m_vAnts.push_back(std::make_shared<SAnt>());
	}
	for(unsigned i=0;i<m_uEliteAnts;++i) {
		m_vAnts.push_back(std::make_shared<SEliteAnt>());
	}
	for(auto& ant : m_vAnts) {
		ant->ToVisit.reserve(m_vNodes.size());
		ant->path.reserve(m_vNodes.size());
	}
}

void TAntAlgorithm::DistributeAntsRandomly() {
	auto indexes = std::vector<unsigned>();
	indexes.reserve(m_uTotalAnts);
	for(auto& ant : m_vAnts) {
		auto index = rand() % m_uTotalAnts;
		auto it = std::find(indexes.begin(), indexes.end(), index);
		while(it!=indexes.end()) {
			index = rand() % m_uTotalAnts;
			it = std::find(indexes.begin(), indexes.end(), index);
		}
		indexes.push_back(index);
		ant->NodeHere = m_vNodes[index];
		ant->ToVisit = m_vNodes;
		ant->ToVisit.erase(std::remove(ant->ToVisit.begin(), ant->ToVisit.end(), ant->NodeHere), ant->ToVisit.end());
	}
}

void TAntAlgorithm::PrepareForNewIteration() {
	PrepareEdgesForNewIteration();
	PrepareAntsForNewIteration();
}

void TAntAlgorithm::PrepareEdgesForNewIteration() {
	for(auto& node : m_vNodes) {
		for(auto& edge : node->Edges) {
			edge->AntVisitor.clear();
		}
	}
}

void TAntAlgorithm::PrepareAntsForNewIteration() {
	for(auto& ant : m_vAnts) {
		ant->path.clear();
	}
	DistributeAntsRandomly();
}

double TAntAlgorithm::GetEdgeCoefficient(PEdge& edge) const {
	auto alphaPheromone = std::pow(edge->Pheromone, m_uAlpha);
	auto betaSight = std::pow(edge->Sight, m_uBeta);
	return alphaPheromone * betaSight;
}

void TAntAlgorithm::CalculateEdgeProbabilitiesForAnt(PAnt& ant) {
	auto coefSum = 0.0;
	for(auto& edge : ant->NodeHere->Edges) {
		edge->Coefficient = !IsAntVisitedNode(ant, edge->EndNode)*GetEdgeCoefficient(edge);
		coefSum += edge->Coefficient;
	}
	for(auto& edge : ant->NodeHere->Edges) {
		edge->Probability = edge->Coefficient / coefSum;
	}
}

bool TAntAlgorithm::IsAntVisitedNode(const PAnt& ant, const PNode& node) const {
	auto it = std::find(ant->ToVisit.begin(), ant->ToVisit.end(), node);
	return it==ant->ToVisit.end();
}

TAntAlgorithm::PEdge TAntAlgorithm::ChooseRandomEdge(PAnt& ant) {
	CalculateEdgeProbabilitiesForAnt(ant);
	auto prevProbability = 0.0;
	for(auto& edge : ant->NodeHere->Edges) {
		edge->Probability += prevProbability;
		prevProbability = edge->Probability;
	}
	auto randomValue = NNHelpFunctions::GenerateBetweenZereOne();
	for(auto& edge : ant->NodeHere->Edges) {
		if(randomValue < edge->Probability) {
			return edge;
		}
	}
	return nullptr;
}

unsigned TAntAlgorithm::GetPathDistance(const Path& path) const {
	unsigned totalDistance = 0;
	for(const auto& edge : path) {
		totalDistance += edge->Distance;
	}
	return totalDistance;
}

TAntAlgorithm::Path TAntAlgorithm::GetShortestPath(const std::vector<Path>& paths) const {
	return (*(std::min_element(paths.begin(), paths.end(), [this](const Path& one, const Path& two) {
		auto firstDist = GetPathDistance(one);
		auto secondDist = GetPathDistance(two);
		return firstDist < secondDist;
	 })));
}

void TAntAlgorithm::DoCrawlForAntPerIteration(PAnt& ant) {
	while(!ant->ToVisit.empty()) {
		auto choosenEdge = ChooseRandomEdge(ant);
		ant->path.push_back(choosenEdge);
		choosenEdge->AntVisitor.push_back(std::make_pair(ant, GetPathDistance(ant->path)));
		ant->NodeHere = choosenEdge->EndNode;
		ant->ToVisit.erase(std::remove(ant->ToVisit.begin(), ant->ToVisit.end(), choosenEdge->EndNode), ant->ToVisit.end());
	}
}

double TAntAlgorithm::GetIncomePheromone(const PEdge& edge) {
	auto income = 0.0;
	for(const auto& antVis : edge->AntVisitor) {
		income += 1.0 / antVis.first->PheromoneMult()*antVis.second;
	}
	income *= m_uLMin;
	return income;
}

void TAntAlgorithm::UpdatePheromones() {
	for(auto& node : m_vNodes) {
		for(auto& edge : node->Edges) {
			auto income = GetIncomePheromone(edge);
			edge->Pheromone = (1 - m_dR)*edge->Pheromone + income;
		}
	}
}

void TAntAlgorithm::PrepareLMin(unsigned iterations) {
	auto saveBest = BestPath;
	BestPath = Path();
	auto saveAlpha = m_uAlpha;
	m_uAlpha = 0;
	m_uLMin = Solve(iterations);
	RenewPheromones();
	m_uAlpha = saveAlpha;
	BestPath = saveBest;
}


unsigned TAntAlgorithm::Solve(unsigned iterations) {
	m_uIterations = iterations;
	auto paths = std::vector<Path>();
	paths.reserve(m_vAnts.size());
	for(unsigned i=0;i<m_uIterations;++i) {
		PrepareForNewIteration();
		for(auto& ant : m_vAnts) {
			DoCrawlForAntPerIteration(ant);
			paths.push_back(std::move(ant->path));
		}
		auto shortestPath = GetShortestPath(std::move(paths));
		if(BestPath.empty()) {
			BestPath = std::move(shortestPath);
		} else if(GetPathDistance(shortestPath)<GetPathDistance(BestPath)) {
			BestPath = std::move(shortestPath);
		}
		paths.clear();
		UpdatePheromones();
	}
	return GetPathDistance(BestPath);
}
