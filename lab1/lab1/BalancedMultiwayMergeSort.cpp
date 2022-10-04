#include "BalancedMultiwayMergeSort.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <string.h>
#include <cmath>
//#define DEBUG
std::string BalancedMultiwayMergeSort::s_sScratchDirOne = "One";
std::string BalancedMultiwayMergeSort::s_sScratchDirTwo = "Two";
std::string BalancedMultiwayMergeSort::s_sType = ".txt";
std::string BalancedMultiwayMergeSort::s_sResultBinary = "result_binary";
std::string BalancedMultiwayMergeSort::s_sResultText = "result_text";
double BalancedMultiwayMergeSort::s_dSentinel = -1.0;

BalancedMultiwayMergeSort::BalancedMultiwayMergeSort(const std::string& fileName, int runSize, int numWays, bool isText) {
    m_sFileName = fileName;
    m_iRunSize = runSize;
    m_iNumWays = numWays;
    m_bIsText = isText;
    Sort();
}

//------------------------- MAIN FUNCTIONS -------------------------//
void BalancedMultiwayMergeSort::Sort() {
    ClearScratchFiles();
    SplitIntoFiles();
    InitFileArrays();
    SortMerge();
    if(m_bIsText) FromBinaryToText();
    //ClearScratchFiles();
}

void BalancedMultiwayMergeSort::SplitIntoFiles() {
    auto file = std::ifstream(m_sFileName);
    auto line = std::string();
    auto arr = std::vector<double>();

    if(file.is_open()) for(auto i = 0;!file.eof();i++) {
        arr.reserve(m_iRunSize);
        auto fileNameOne = s_sScratchDirOne+"/"+std::to_string(i)+s_sType;
        auto FileOne = std::ofstream(fileNameOne, std::ios::binary|std::ios::app);

        auto j = 0;
        for(;j<m_iRunSize && !file.eof();j++) {
            getline(file, line);
            arr.push_back(atof(line.c_str()));
        }
        std::sort(arr.begin(), arr.begin()+j);
        for(int k = 0;k<j;k++) {
            FileOne.write((char*)&arr, sizeof(arr));
        }
        if(i==0) m_iNumRunsPerOutputFile++;
        if(i==m_iNumWays-1) i = -1;
        FileOne.write((char*)&s_dSentinel, sizeof(double));
        arr.clear();
    }
    m_iNumWays = GetDirectotyNumberOfFiles(s_sScratchDirOne);
    for(int i = 0;i<m_iNumWays;i++) {
        auto fileNameTwo = s_sScratchDirTwo+"/"+std::to_string(i)+s_sType;
        auto FileTwo = std::ofstream(fileNameTwo, std::ios::binary|std::ios::app);
        FileTwo.write((char*)&s_dSentinel, sizeof(double));
    }
}

void BalancedMultiwayMergeSort::SortMerge() {
    bool isOne = true;
    m_vMergeArray = std::vector<double>(m_iNumWays);
    for(int i = m_iNumRunsPerOutputFile;i>1;i = ceil(i/m_iNumWays)) {
        OpenFiles(isOne);
        PrepareRun(i);
        CloseFiles();
        isOne = !isOne;
    }
    OpenFiles(isOne);
    GoToNextChunk();
    LastRun();
    CloseFiles();
}

//------------------------- CONVERT BINARY TO TEXT -----------------//
void BalancedMultiwayMergeSort::FromBinaryToText() {
    auto file = std::fstream(s_sResultBinary + s_sType, std::ios::in|std::ios::binary);
    auto fileOut = std::fstream(s_sResultText + s_sType, std::ios::out);
    auto num = 0.0;
    while(!file.eof()) {
        file.read((char*)&num, sizeof(double));
        fileOut<<num<<"\n";
    }
}

//------------------------- CLEAR GARBAGE --------------------------//
void BalancedMultiwayMergeSort::ClearScratchFiles() {
    std::filesystem::remove_all("One");
    std::filesystem::remove_all("Two");
    std::filesystem::create_directory("One");
    std::filesystem::create_directory("Two");
}

//------------------------- HELP FUNCTIONS -------------------------//
int BalancedMultiwayMergeSort::GetDirectotyNumberOfFiles(const std::string& dirName) {
    auto dirOne = std::filesystem::path("One");
    auto dirIter = std::filesystem::directory_iterator(dirOne);

    auto fileCount = std::count_if(
        begin(dirIter),
        end(dirIter),
        [](auto& entry) { return entry.is_regular_file(); }
    );

    return fileCount;
}

void BalancedMultiwayMergeSort::Sentinel(std::fstream& file) {
    file.write((char*)&s_dSentinel, sizeof(double));
}

//------------------------- RUNS -----------------------------------//
void BalancedMultiwayMergeSort::PrepareRun(int i) {
    auto j = 0;
    while(!IsEndOfFiles()) {
        GoToNextChunk();
        Run(m_vOutput[j]);
        Sentinel(m_vOutput[j]);
        j++;
        if(j==m_iNumWays) j = 0;
    }
    if((i%m_iNumWays)!=0) for(;j<m_iNumWays&&j!=0;j++) {
        Sentinel(m_vOutput[j]);
    }
}

void BalancedMultiwayMergeSort::Run(std::fstream& outFile) {
    InitMergeArray();
    while(!IsEndOfChunks()) {
        auto i = FindMinElement();
        if(i==s_dSentinel) break;
        outFile.write((char*)&m_vMergeArray[i], sizeof(double));
        UpdateMergeArray(i);
    }
}

void BalancedMultiwayMergeSort::LastRun() {
    auto fileOut = std::fstream(s_sResultBinary + s_sType, std::ios::out|std::ios::binary);
    Run(fileOut);
}

//------------------------- MERGE ARRAY ----------------------------//
void BalancedMultiwayMergeSort::InitMergeArray() {
    auto line = std::string();

    for(int i = 0;i<m_iNumWays;i++) {
        if(!m_vInput[i].eof()) {
            auto cursor = m_vInput[i].tellg();
            auto num = s_dSentinel;
            m_vInput[i].read((char*)&num, sizeof(double));

            if(num!=s_dSentinel) {
                m_vMergeArray[i] = num;
            } else if(num==s_dSentinel && !m_vInput[i].eof()) {
                m_vMergeArray[i] = s_dSentinel;
                m_vInput[i].seekg(cursor);
            }
        }
    }
}

void BalancedMultiwayMergeSort::UpdateMergeArray(int index) {
    if(!m_vInput[index].eof()) {
        auto cursor = m_vInput[index].tellg();
        auto num = s_dSentinel;
        m_vInput[index].read((char*)&num, sizeof(double));
        if(num!=s_dSentinel) {
            m_vMergeArray[index] = num;
        } else if(num==s_dSentinel) {
            m_vMergeArray[index] = s_dSentinel;
            m_vInput[index].seekg(cursor);
        }
    }
}

int BalancedMultiwayMergeSort::FindMinElement() {
    auto minIndex = s_dSentinel;
    for(int i = 0;i<m_iNumWays;i++) {
        if(m_vMergeArray[i]!=s_dSentinel) {
            minIndex = i;
        }
    }
    for(int i = 0;i<m_iNumWays;i++) {
        if(m_vMergeArray[i]!=s_dSentinel) {
            if(m_vMergeArray[minIndex]>=m_vMergeArray[i]) {
                minIndex = i;
            }
        }
    }
    return minIndex;
}

//------------------------- END OF CHUNKS FILES --------------------//
bool BalancedMultiwayMergeSort::IsEndOfChunks() {
    for(int i = 0;i<m_iNumWays;i++) {
        if(m_vMergeArray[i]!=s_dSentinel) return false;
    }

    return true;
}

bool BalancedMultiwayMergeSort::IsEndOfFiles() {
    for(int i = 0;i<m_iNumWays;i++) {
        if(!m_vInput[i].eof()) return false;
    }
    return true;
}

//------------------------- NEXT CHUNKS ----------------------------//
void BalancedMultiwayMergeSort::GoToNextChunk() {
    auto num = s_dSentinel;
    for(int i = 0;i<m_iNumWays;i++) {
        if(!m_vInput[i].eof()) {
            auto cursor = m_vInput[i].tellg();
            m_vInput[i].read((char*)&num, sizeof(double));
            if(num!=s_dSentinel) m_vInput[i].seekg(cursor);
        }
    }
}

//------------------------- OPEN INIT CLOSE FILES ------------------//
void BalancedMultiwayMergeSort::InitFileArrays() {
    m_vInput = std::vector<std::fstream>(m_iNumWays);
    m_vOutput = std::vector<std::fstream>(m_iNumWays);
}

void BalancedMultiwayMergeSort::OpenFiles(bool isOne) {
    if(isOne) {
        for(int i = 0;i<m_iNumWays;i++) {
            auto fileNameOne = s_sScratchDirOne+"/"+std::to_string(i)+s_sType;
            auto fileNameTwo = s_sScratchDirTwo+"/"+std::to_string(i)+s_sType;

            m_vInput[i].open(fileNameOne, std::ios::in|std::ios::out|std::ios::binary);
            m_vOutput[i].open(fileNameTwo, std::ios::in|std::ios::out|std::ios::binary);
        }
    } else {
        for(int i = 0;i<m_iNumWays;i++) {
            auto fileNameOne = s_sScratchDirOne+"/"+std::to_string(i)+s_sType;
            auto fileNameTwo = s_sScratchDirTwo+"/"+std::to_string(i)+s_sType;

            m_vOutput[i].open(fileNameOne, std::ios::in|std::ios::out|std::ios::binary);
            m_vInput[i].open(fileNameTwo, std::ios::in|std::ios::out|std::ios::binary);
        }
    }

}

void BalancedMultiwayMergeSort::CloseFiles() {
    for(int i = 0;i<m_iNumWays;i++) {
        m_vInput[i].close();
        m_vOutput[i].close();
    }
}