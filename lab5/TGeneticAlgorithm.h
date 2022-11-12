//@formatter:off
#ifndef LAB5_TGENETICALGORITHM_H
#define LAB5_TGENETICALGORITHM_H
#include <cstdlib>
#include <random>
#include <vector>
#include <functional>
#include <algorithm>
#include <memory>
#include "TTimer.h"

class TTester;

template<unsigned inputSize>
class TGeneticAlgorithm {
    friend class TTester;
    public:
    using Input = std::array<std::pair<unsigned, unsigned>, inputSize>;
    using Chromosome = std::array<bool, inputSize>;
    using Generation = std::vector<Chromosome>;
    using Crossovers = std::vector<std::function<Chromosome(TGeneticAlgorithm<inputSize>*, const Chromosome&, const Chromosome&)>>;
    using Mutators = std::vector<std::function<void(TGeneticAlgorithm<inputSize>*, Chromosome&)>>;
    using Improvers = std::vector<std::function<void(TGeneticAlgorithm<inputSize>*, Chromosome&)>>;

    public:
    TGeneticAlgorithm()=default;
    TGeneticAlgorithm(unsigned initialPopulationsNumber, unsigned milliseconds, unsigned maxWeight, const Input& input,
    const Crossovers& crossovers, const Mutators& mutators, const Improvers& improvers) {
        m_uInitialPopulationNumber = initialPopulationsNumber;
        m_uMilliseconds = milliseconds;
        m_uMaxWeight = maxWeight;
        m_vInput = input;
        m_vCrossovers = crossovers;
        m_vMutators = mutators;
        m_vImprovers = improvers;
    }

    public:
    void Solve() {
        StartInitialGeneration();
        TTimer timer;
        timer.start();
        SortGeneration();
        m_vBestChromosome = m_vGeneration.back();
        while(timer.elapsedMilliseconds()<m_uMilliseconds) {
            // pick parent
            auto size = m_vGeneration.size();
            auto& parentOne = m_vGeneration[size - 1];
            auto& parentTwo = m_vGeneration[size - 2];
            // crossover
            auto chrome = Crossover(parentOne, parentTwo);
            if(!isValid(chrome)) continue;
            // mutate
            auto chromeCopy = chrome;
            Mutate(chrome);
            if(!isValid(chrome)) chrome = chromeCopy;
            // improve
            chromeCopy = chrome;
            Improve(chrome);
            if(!isValid(chrome)) chrome = chromeCopy;
            if(CalculatePrice(chrome)>CalculatePrice(m_vBestChromosome)) {
                m_vBestChromosome = chrome;
            }
            m_vGeneration.push_back(m_vBestChromosome);
            SortGeneration();
            m_vGeneration.erase(m_vGeneration.begin());
        }
    }

    protected:
    bool isValid(const Chromosome& chrome) {
        return CalculateWeight(chrome)<=m_uMaxWeight;
    }

    Chromosome Crossover(const Chromosome& one, const Chromosome& two) {
        // randomly choose crossover
        auto index = rand() % m_vCrossovers.size();
        return m_vCrossovers[index](this, one, two);
    }

    void Mutate(Chromosome& chrome) {
    	if(m_vMutators.empty()) return;
        auto index = rand() % m_vMutators.size();
        m_vMutators[index](this, chrome);
    }

    void Improve(Chromosome& chrome) {
		if(m_vImprovers.empty()) return;
        auto index = rand() % m_vImprovers.size();
        m_vImprovers[index](this, chrome);
    }

    void StartInitialGeneration() {
        m_vGeneration.reserve(m_uInitialPopulationNumber);
        for(unsigned i=0;i<m_uInitialPopulationNumber;++i) {
           m_vGeneration.push_back(FindFirstPossibleSolution());
        }
    }

    void SortGeneration() {
        std::sort(m_vGeneration.begin(), m_vGeneration.end(),
            [this](auto& chromeOne, auto& chromeTwo) {
                return CalculatePrice(chromeOne)<CalculatePrice(chromeTwo);
            }
        );
    }

    unsigned CalculatePrice(const Chromosome& chrome) {
        auto price = 0;
        for(unsigned i=0;i<inputSize;++i) {
            if(chrome[i]) {
                price += m_vInput[i].first;
            }
        }
        return price;
    }

    unsigned CalculateWeight(const Chromosome& chrome) {
        auto weight = 0;
        for(unsigned i=0;i<inputSize;++i) {
            if(chrome[i]) {
                weight += m_vInput[i].second;
            }
        }
        return weight;
    }

    Chromosome FindFirstPossibleSolution() {
        srand(time(NULL));
        auto chromo = Chromosome();
        auto curWeight = 0;
        while(true) {
            auto index = rand() % inputSize;
            curWeight += m_vInput[index].second;
            if(curWeight>m_uMaxWeight) break;
            chromo[index] = true;
        }
        return chromo;
    }


    protected:
    unsigned m_uInitialPopulationNumber = 0;
    unsigned m_uMilliseconds = 0;
    unsigned m_uMaxWeight = 0;

    protected:
    Input m_vInput;
    Generation m_vGeneration;
    Crossovers m_vCrossovers;
    Mutators m_vMutators;
    Improvers m_vImprovers;
    Chromosome m_vBestChromosome;
};

#endif //LAB5_TGENETICALGORITHM_H
//@formatter:on
