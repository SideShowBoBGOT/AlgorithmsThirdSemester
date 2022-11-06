#pragma once
#include "TAntAlgorithm.h"
class TAntAlgorithmTester {
	friend class TAntAlgorithm;
	public:
	TAntAlgorithmTester()=default;
	virtual ~TAntAlgorithmTester()=default;
	void Test();

	protected:
	using InputMat = std::vector<std::vector<unsigned>>;

	protected:
	void GenerateInput();
	void DoTest();
	void PrintInput();

	protected:
	unsigned m_uRangeStart = 0;
	unsigned m_uRange = 0;
	unsigned m_uInputSize = 0;
	InputMat m_mInputMat;
	unsigned m_uTotalAnts = 0;
	unsigned m_uEliteAnts = 0;
	unsigned m_uAlpha = 0;
	unsigned m_uBeta = 0;
	unsigned m_uIterationsStart = 0;
	unsigned m_uIterationsEnd = 0;
	double m_dR = 0.0;
};

