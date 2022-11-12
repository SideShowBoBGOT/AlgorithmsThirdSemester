#ifndef LAB5_TTESTER_H
#define LAB5_TTESTER_H
#include <iostream>
#include <cassert>
#include <future>
#include <thread>
#include <mutex>
#include <filesystem>
#include "include/pbPlots.hpp"
#include "include/supportLib.hpp"
#include "TGeneticAlgorithm.h"

class TTester {
    public:
    TTester()=default;
    TTester(const TTester&)=default;
    TTester(TTester&&)=default;
    virtual ~TTester()=default;

	protected:
    static constexpr unsigned s_uInputSize = 100;
	static constexpr unsigned s_uMaxWeight = 500;
	static constexpr unsigned s_uLPrice= 2;
	static constexpr unsigned s_uMPrice= 30;
	static constexpr unsigned s_uLWeight = 1;
	static constexpr unsigned s_uMWeight= 20;
	static constexpr unsigned s_uInitialPopulation = 10;
	static constexpr unsigned s_uStartTime = 100;
	static constexpr unsigned s_uEndTime = 10000;
	static constexpr unsigned s_uAddTime = 100;

	protected:
	using Chromosome = typename TGeneticAlgorithm<s_uInputSize>::Chromosome;
	using Input = typename TGeneticAlgorithm<s_uInputSize>::Input;
	using Crossovers = typename TGeneticAlgorithm<s_uInputSize>::Crossovers;
    using Mutators = typename TGeneticAlgorithm<s_uInputSize>::Mutators;
    using Improvers = typename TGeneticAlgorithm<s_uInputSize>::Improvers;
	using AlgData = std::pair<std::vector<double>, std::vector<double>>;
	using Results = std::vector<AlgData>;
	using Tasks = std::vector<std::future<AlgData>>;

	protected:
	Input m_vInput;
	Crossovers m_vCrossovers;
	Mutators m_vMutators;
    Improvers m_vImprovers;

	protected:

	#define VAR_TO_STR(x) \
		#x
	#define ADD_FUNC_TO_VECTOR(func, vec, str) \
		vec.push_back(func);                \
		AddFuncToStr(str, #func)

	static void AddAttributeToStr(std::string& str, std::string&& attrStr, unsigned attr) {
		str += attrStr + ": " + std::to_string(attr) + "\n";
	}
	static void AddFuncToStr(std::string& str, std::string&& funcStr) {
		str += funcStr + ": " + "\n";
	}

	public:
	void Test() {
	    auto str = std::string();
		AddAttributeToStr(str, VAR_TO_STR(s_uInputSize), s_uInputSize);
		AddAttributeToStr(str, VAR_TO_STR(s_uMaxWeight), s_uMaxWeight);
		AddAttributeToStr(str, VAR_TO_STR(s_uInitialPopulation), s_uInitialPopulation);

		ADD_FUNC_TO_VECTOR(PointCrossover<2>, m_vCrossovers, str);
		ADD_FUNC_TO_VECTOR(PointCrossover<1>, m_vCrossovers, str);
		ADD_FUNC_TO_VECTOR(SequenceCrossover, m_vCrossovers, str);


//		ADD_FUNC_TO_VECTOR(ChangeSignMutator, m_vMutators, str);
//		ADD_FUNC_TO_VECTOR(SwapMutator, m_vMutators, str);
//
//		ADD_FUNC_TO_VECTOR(BestWeightPriceImprover, m_vImprovers, str);

		GenerateInput();

		auto results = Results();
		auto tasks = Tasks();
		tasks.push_back(std::async(std::launch::async, DoTest, 100, 3600, 100, m_vCrossovers, m_vMutators, m_vImprovers, m_vInput));
		tasks.push_back(std::async(std::launch::async, DoTest, 3600, 5000, 100, m_vCrossovers, m_vMutators, m_vImprovers, m_vInput));
		tasks.push_back(std::async(std::launch::async, DoTest, 5000, 6100, 100, m_vCrossovers, m_vMutators, m_vImprovers, m_vInput));
		tasks.push_back(std::async(std::launch::async, DoTest, 6100, 7100, 100, m_vCrossovers, m_vMutators, m_vImprovers, m_vInput));
		tasks.push_back(std::async(std::launch::async, DoTest, 7100, 7700, 100, m_vCrossovers, m_vMutators, m_vImprovers, m_vInput));
		tasks.push_back(std::async(std::launch::async, DoTest, 7700, 8300, 100, m_vCrossovers, m_vMutators, m_vImprovers, m_vInput));
		tasks.push_back(std::async(std::launch::async, DoTest, 8300, 8600, 100, m_vCrossovers, m_vMutators, m_vImprovers, m_vInput));
		tasks.push_back(std::async(std::launch::async, DoTest, 8600, 8950, 100, m_vCrossovers, m_vMutators, m_vImprovers, m_vInput));
		tasks.push_back(std::async(std::launch::async, DoTest, 8950, 9200, 100, m_vCrossovers, m_vMutators, m_vImprovers, m_vInput));
		tasks.push_back(std::async(std::launch::async, DoTest, 9200, 9500, 100, m_vCrossovers, m_vMutators, m_vImprovers, m_vInput));
		tasks.push_back(std::async(std::launch::async, DoTest, 9500, 9800, 100, m_vCrossovers, m_vMutators, m_vImprovers, m_vInput));
		tasks.push_back(std::async(std::launch::async, DoTest, 9800, 10000, 100, m_vCrossovers, m_vMutators, m_vImprovers, m_vInput));

		auto e = std::vector<wchar_t>();
		for(auto& t : tasks) {
			results.push_back(t.get());
		}

		auto x = std::vector<double>();
		auto y = std::vector<double>();
		for(const auto& res : results) {
			x.insert(x.end(), res.first.begin(), res.first.end());
			y.insert(y.end(), res.second.begin(), res.second.end());
		}

		RGBABitmapImageReference* imageRef = CreateRGBABitmapImageReference();
		StringReference* errorMessage = CreateStringReference(&e);
		DrawScatterPlot(imageRef, 640, 480, &x, &y, errorMessage);

		auto pngData = ConvertToPNG(imageRef->image);
		WriteToFile(pngData, "plot.png");
		DeleteImage(imageRef->image);
    }

	protected:
	void DistributeTasksTimeEvenly(Tasks& tasks) {

	}

	protected:
	static AlgData DoTest(double start, double end, double add,
    	const Crossovers& crossovers, const Mutators& mutators, const Improvers& improvers, const Input& input) {

		auto x = std::vector<double>();
		auto y = std::vector<double>();
		for(auto i=start;i<end;i+=add) {
			x.push_back(i);
			auto alg = TGeneticAlgorithm<s_uInputSize>(s_uInitialPopulation, i, s_uMaxWeight, input,
			crossovers, mutators, improvers);
			alg.Solve();
			auto price = alg.CalculatePrice(alg.m_vBestChromosome);
			auto weight = alg.CalculateWeight(alg.m_vBestChromosome);
			std::cout<<"Milliseconds: "<<i<<"\t"<<"Price: "<<price<<"\t"<<"Weight: "<<weight<<"\n";
			y.push_back(price);
		}
		return std::make_pair(x, y);

	}

	protected:
	void GenerateInput() {
		for(unsigned i=0;i<s_uInputSize;++i) {
			auto weight = s_uLWeight + rand() % (s_uMWeight - s_uLWeight - 1);
			auto price = s_uLPrice + rand() % (s_uMPrice - s_uLPrice - 1);
			m_vInput[i] = std::make_pair(price, weight);
		}
	}

    template<unsigned pointNumber>
    static Chromosome PointCrossover(TGeneticAlgorithm<s_uInputSize>* alg, const Chromosome& one, const Chromosome& two) {
		assert(pointNumber>0);

    	auto chromo = Chromosome();
    	auto number = (s_uInputSize<pointNumber)?s_uInputSize:pointNumber;
    	auto partition = s_uInputSize / (number + 1);
    	auto i = 1;
    	auto index = 0;
    	for(;i<=number;++i) {
    		for(;index<i*partition;++index) {
    			chromo[index] = (i%2)?one[index]:two[index];
    		}
    	}
    	for(;index<s_uInputSize;++index) {
			chromo[index] = (i%2)?one[index]:two[index];
    	}
    	return chromo;
    }

    static Chromosome SequenceCrossover(TGeneticAlgorithm<s_uInputSize>* alg, const Chromosome& one, const Chromosome& two) {
    	auto chromo = Chromosome();
    	for(unsigned i=0;i<s_uInputSize;++i) {
			chromo[i] = (i%2)?one[i]:two[i];
		}
		return chromo;
	}

	static void SwapMutator(TGeneticAlgorithm<s_uInputSize>* alg, Chromosome& chromo) {
		auto indexOne = rand() % s_uInputSize;
		auto indexTwo = rand() % s_uInputSize;
		std::swap(chromo[indexOne], chromo[indexTwo]);
	}

	static void ChangeSignMutator(TGeneticAlgorithm<s_uInputSize>* alg, Chromosome& chromo) {
		auto index = rand() % s_uInputSize;
		chromo[index] = !chromo[index];
	}

	static void BestWeightPriceImprover(TGeneticAlgorithm<s_uInputSize>* alg, Chromosome& chromo) {
		unsigned index = 0;
		for(unsigned i=0;i<s_uInputSize;++i) {
			if(chromo[i] and alg->m_vInput[i].first<alg->m_vInput[index].first) {
				index = i;
			}
		}
		auto weight = alg->CalculateWeight(chromo);
		weight -= alg->m_vInput[index].first;
		auto isImproved = false;
		for(unsigned i=0;i<s_uInputSize and !isImproved;++i) {
			if(!chromo[i]) {
				auto posWeight = weight + alg->m_vInput[i].second;
				if(posWeight<=alg->m_uMaxWeight) {
					chromo[index] = false;
					chromo[i] = true;
					isImproved = true;
				}
			}
		}
	}
};
#endif //LAB5_TTESTER_H
