#ifndef LAB5_TTESTER_H
#define LAB5_TTESTER_H
#include <iostream>
#include <cassert>
#include <future>
#include <thread>
#include <mutex>
#include <fstream>
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
	static constexpr char s_sBasePath[] = "/home/choleraplague/university/Algorithms_KPI_2_Year/lab5/results/";
    static constexpr unsigned s_uInputSize = 100;
	static constexpr unsigned s_uMaxWeight = 500;
	static constexpr unsigned s_uLPrice= 2;
	static constexpr unsigned s_uMPrice= 30;
	static constexpr unsigned s_uLWeight = 1;
	static constexpr unsigned s_uMWeight= 20;
	static constexpr unsigned s_uInitialPopulation = 10;
	static constexpr unsigned s_uThreads = 12;
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
		""#x
	#define ADD_FUNC_TO_VECTOR(func, vec, str) \
		vec.push_back(func);                \
		AddFuncToStr(str, #func)

	static void AddAttributeToStr(std::string& str, std::string&& attrStr, unsigned attr) {
		str += attrStr + "_" + std::to_string(attr) + " ";
	}
	static void AddFuncToStr(std::string& str, std::string&& funcStr) {
		str += funcStr + " ";
	}

	public:
	void Test() {
	    auto legend = std::string();
		AddAttributeToStr(legend, VAR_TO_STR(s_uInputSize), s_uInputSize);
		AddAttributeToStr(legend, VAR_TO_STR(s_uMaxWeight), s_uMaxWeight);
		AddAttributeToStr(legend, VAR_TO_STR(s_uInitialPopulation), s_uInitialPopulation);
		AddAttributeToStr(legend, VAR_TO_STR(s_uStartTime), s_uStartTime);
		AddAttributeToStr(legend, VAR_TO_STR(s_uEndTime), s_uEndTime);
		AddAttributeToStr(legend, VAR_TO_STR(s_uAddTime), s_uAddTime);

	    auto directory = std::string();
		ADD_FUNC_TO_VECTOR(PointCrossover<2>, m_vCrossovers, directory);
		ADD_FUNC_TO_VECTOR(PointCrossover<4>, m_vCrossovers, directory);
		ADD_FUNC_TO_VECTOR(PointCrossover<1>, m_vCrossovers, directory);
		ADD_FUNC_TO_VECTOR(SequenceCrossover, m_vCrossovers, directory);


		ADD_FUNC_TO_VECTOR(ChangeSignMutator, m_vMutators, directory);
		ADD_FUNC_TO_VECTOR(SwapMutator, m_vMutators, directory);
////
		ADD_FUNC_TO_VECTOR(WorstPriceImprover, m_vImprovers, directory);
		ADD_FUNC_TO_VECTOR(BestPriceImprover, m_vImprovers, directory);

		auto funcs = s_sBasePath + directory;
		auto params = funcs + "/" + legend;
		std::filesystem::create_directory(funcs);
		std::filesystem::create_directory(params);
		std::filesystem::current_path(params);

		GenerateInput();

		auto results = Results();
		auto tasks = Tasks();
		DistributeTasks(tasks);
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

		auto series = GetDefaultScatterPlotSeriesSettings();
		series->xs = &x;
		series->ys = &y;
		auto vs = std::vector<ScatterPlotSeries*>();
		vs.push_back(series);

		auto titleVec = std::vector<wchar_t>(legend.begin(), legend.end());
		auto xlabelVec = std::vector<wchar_t>(directory.begin(), directory.end());

		auto settings = GetDefaultScatterPlotSettings();
		settings->width = 1280;
		settings->height = 720;
		settings->autoBoundaries = true;
		settings->autoPadding = true;
		settings->title = &titleVec;
		settings->xLabel = &xlabelVec;
		settings->scatterPlotSeries = &vs;

		RGBABitmapImageReference* imageRef = CreateRGBABitmapImageReference();
		StringReference* errorMessage = CreateStringReference(&e);
		DrawScatterPlotFromSettings(imageRef, settings, errorMessage);

		auto pngData = ConvertToPNG(imageRef->image);

		auto name = GenerateFileName();
		WriteToFile(pngData, name+".png");
		auto file = std::fstream(name+".csv", std::ios::out);
		for(auto i=0;i<x.size();++i) {
			file<<std::to_string(x[i])+"\t"+std::to_string(y[i])+"\n";
		}

		DeleteImage(imageRef->image);
    }

	protected:
	static std::basic_string<char> GenerateFileName() {
		auto path = std::filesystem::current_path();
		auto p = std::filesystem::current_path().string();
		auto it = std::filesystem::directory_iterator(std::filesystem::current_path());
		auto paths = std::vector<std::string>();
		while(it!=std::filesystem::end(it)) {
			paths.push_back(it->path().string());
			++it;
		}
		auto index = 0;
		while(std::find(paths.begin(), paths.end(), p + "/" + std::to_string(index) + ".png")!=paths.end()) {
			++index;
		}
		return std::to_string(index) ;
	}

	void DistributeTasks(Tasks& tasks) {
		auto timePoints = std::array<std::pair<unsigned, unsigned>, s_uThreads>();
		DistributeTimeEvenly(timePoints);
		for(const auto& t : timePoints) {
			tasks.push_back(std::async(std::launch::async, DoTest, t.first, t.second, s_uAddTime, m_vCrossovers, m_vMutators, m_vImprovers, m_vInput));
		}
	}

	static void DistributeTimeEvenly(std::array<std::pair<unsigned, unsigned>, s_uThreads>& timePoints) {
		auto initial = std::make_pair(s_uStartTime, s_uEndTime);
		auto splitOne = SplitArea(initial, 2);

		auto splitTwo = SplitArea(splitOne[0], 3);
		auto sixthOne = splitTwo[0];
		auto [sixthTwo, sixthThree] = SplitArea(splitTwo[1], 2);

		auto splitThree = SplitArea(splitOne[1], 3);
		auto sixthFour = splitThree[0];
		auto [sixthFive, sixthSix] = SplitArea(splitThree[1], 2);

		auto [one, two] = SplitArea(sixthOne, 2);
		timePoints[0] = one;
		timePoints[1] = two;
		auto [three, four] = SplitArea(sixthTwo, 2);
		timePoints[2] = three;
		timePoints[3] = four;
		auto [five, six] = SplitArea(sixthThree, 2);
		timePoints[4] = five;
		timePoints[5] = six;
		auto [seven, eight] = SplitArea(sixthFour, 2);
		timePoints[6] = seven;
		timePoints[7] = eight;
		auto [nine, ten] = SplitArea(sixthFive, 2);
		timePoints[8] = nine;
		timePoints[9] = ten;
		auto [eleven, twelve] = SplitArea(sixthSix, 2);
		timePoints[10] = eleven;
		timePoints[11] = twelve;
	}

	static std::array<std::pair<unsigned, unsigned>, 2> SplitArea(const std::pair<unsigned, unsigned>& p, unsigned ratio) {
		auto pairs = std::array<std::pair<unsigned , unsigned>, 2>();
		auto len = unsigned(double(p.second - p.first) / sqrt(ratio)) + p.first;
		pairs[0] = std::make_pair(p.first, len);
		pairs[1] = std::make_pair(len, p.second);
		return pairs;
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

	static void WorstPriceImprover(TGeneticAlgorithm<s_uInputSize>* alg, Chromosome& chromo) {
		unsigned index = 0;
		for(unsigned i=0;i<s_uInputSize;++i) {
			if(chromo[i] and alg->m_vInput[i].first<alg->m_vInput[index].first) {
				index = i;
			}
		}
		auto weight = alg->CalculateWeight(chromo);
		weight -= alg->m_vInput[index].second;
		auto isImproved = false;
		for(unsigned i=0;i<s_uInputSize and !isImproved;++i) {
			if(!chromo[i] and alg->m_vInput[index].first <= alg->m_vInput[i].first) {
				auto posWeight = weight + alg->m_vInput[i].second;
				if(posWeight<=alg->m_uMaxWeight) {
					chromo[index] = false;
					chromo[i] = true;
					isImproved = true;
				}
			}
		}
	}

	static void BestPriceImprover(TGeneticAlgorithm<s_uInputSize>* alg, Chromosome& chromo) {
		unsigned index = 0;
		for(unsigned i=0;i<s_uInputSize;++i) {
			if(chromo[i] and alg->m_vInput[i].first>alg->m_vInput[index].first) {
				index = i;
			}
		}
		auto weight = alg->CalculateWeight(chromo);
		weight -= alg->m_vInput[index].second;
		auto isImproved = false;
		for(unsigned i=0;i<s_uInputSize and !isImproved;++i) {
			if(!chromo[i] and alg->m_vInput[index].first <= alg->m_vInput[i].first) {
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
