#pragma once
#include <vector>
#include <memory>
#include "NNHelpFunctions.h"

class TAntAlgorithmTester;
class TAntAlgorithm {
	friend class TAntAlgorithmTester;

	TAntAlgorithm()=delete;
	virtual ~TAntAlgorithm()=default;
	TAntAlgorithm(const std::vector<std::vector<unsigned>>& inputMat, unsigned totalAnts, unsigned eliteAnts,
		unsigned alpha, unsigned beta, double density);
	unsigned Solve(unsigned iterations);
	void PrepareLMin(unsigned iterations);

	protected:
	struct SAnt;
	struct SNode;
	struct SEdge;
	using PAnt = std::shared_ptr<SAnt>;
	using PNode = std::shared_ptr<SNode>;
	using PEdge = std::shared_ptr<SEdge>;
	using AntVec = std::vector<PAnt>;
	using NodeVec = std::vector<PNode>;
	using EdgeVec = std::vector<PEdge>;
	using Path = std::vector<PEdge>;
	using AntPathVec = std::vector<std::pair<PAnt, unsigned>>;

	protected:
	// CREATION OF ELEMENTS
	void CreateNodes(const std::vector<std::vector<unsigned>>& inputMat);
	void CreateEdges(const std::vector<std::vector<unsigned>>& inputMat);
	void CreateAnts();

	// PREPARE FOR ITERATION
	void DistributeAntsRandomly();
	void PrepareForNewIteration();
	void PrepareAntsForNewIteration();
	void PrepareEdgesForNewIteration();

	// HELP FUNCTIONS

	// ALGORITHM ITSELF
	double GetEdgeCoefficient(PEdge& edge) const;
	unsigned GetPathDistance(const Path& path) const;
	Path GetShortestPath(const std::vector<Path>& paths) const;
	PEdge ChooseRandomEdge(PAnt& ant);
	void CalculateEdgeProbabilitiesForAnt(PAnt& ant);
	void DoCrawlForAntPerIteration(PAnt& ant);
	double GetIncomePheromone(const PEdge& edge);
	bool IsAntVisitedNode(const PAnt& ant, const PNode& node) const;
	void UpdatePheromones();
	void RenewPheromones();

	protected:
	struct SAnt {
		public:
		PNode NodeHere = nullptr;
		NodeVec ToVisit;
		NodeVec Visited;
		Path path;
		virtual double PheromoneMult() { return 1.0; };
	};
	struct SEliteAnt : public SAnt {
		public:
		virtual double PheromoneMult() override { return 2.0; };
	};
	struct SEdge {
		PNode StartNode = nullptr;
		PNode EndNode = nullptr;
		double Sight = 0.0;
		double Pheromone = 0.0;
		unsigned Distance = 0;
		double Probability = 0.0;
		double Coefficient = 0.0;
		AntPathVec AntVisitor;
	};
	struct SNode {
		unsigned Index = 0;
		EdgeVec Edges;
	};

	protected:
	unsigned m_uTotalAnts = 0;
	unsigned m_uEliteAnts = 0;
	unsigned m_uAlpha = 0;
	unsigned m_uBeta = 0;
	unsigned m_uLMin = 0;
	unsigned m_uL = 0;
	unsigned m_uIterations = 0;
	double m_dR = 0.0;
	Path BestPath;
	NodeVec m_vNodes;
	AntVec m_vAnts;
};

