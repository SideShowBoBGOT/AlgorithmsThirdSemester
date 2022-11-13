#include <iostream>
#include "pbPlots.hpp"
#include "supportLib.hpp"
#include "TAntAlgorithmTester.h"


void TAntAlgorithmTester::Test() {
	m_uInputSize = 60;
	m_uRangeStart = 1;
	m_uRange = 40;
	m_uTotalAnts = 10;
	m_uEliteAnts = 3;
	m_uAlpha = 3;
	m_uBeta = 2;
	m_uIterationsStart = 20;
	m_uIterationsEnd = 100;
	m_dR = 0.6;
	GenerateInput();
	DoTest();
}

void TAntAlgorithmTester::GenerateInput() {
	srand(time(NULL));
	m_mInputMat.reserve(m_uInputSize);
	for(unsigned i=0;i<m_uInputSize;++i) {
		auto row = std::vector<unsigned>();
		row.reserve(m_uInputSize);
		for(unsigned j=0;j<m_uInputSize;++j) {
			auto number = m_uRangeStart + rand() % (m_uRangeStart + m_uRange);
			row.push_back(static_cast<unsigned>(number));
		}
		m_mInputMat.push_back(std::move(row));
	}
}

void TAntAlgorithmTester::DoTest() {
	//PrintInput();
	auto x = std::vector<double>();
	auto y = std::vector<double>();
	auto e = std::vector<wchar_t>();
	auto alg = TAntAlgorithm(m_mInputMat, m_uTotalAnts, m_uEliteAnts, m_uAlpha, m_uBeta, m_dR);
	for(unsigned i=m_uIterationsStart;i<m_uIterationsEnd;++i) {
		x.push_back(i);
		alg.PrepareLMin(i);
		auto l = alg.Solve(i);
		std::cout<<"Iterations "<<i<<"\t"<<"Length: "<<l<<"\t"<<"LMin: "<<alg.m_uLMin<<"\n";
		y.push_back(l);
	}
	RGBABitmapImageReference* imageRef = CreateRGBABitmapImageReference();
	StringReference* errorMessage = CreateStringReference(&e);
	DrawScatterPlot(imageRef, 640, 480, &x, &y, errorMessage);

	auto pngData = ConvertToPNG(imageRef->image);
	WriteToFile(pngData, "plot.png");
	DeleteImage(imageRef->image);
}

void TAntAlgorithmTester::PrintInput() {
	std::cout<<"Input:\n";
	for(unsigned i=0;i<m_uInputSize;++i) {
		for(unsigned j=0;j<m_uInputSize;++j) {
			std::cout<<m_mInputMat[i][j]<<" ";
		}
		std::cout<<"\n";
	}
}