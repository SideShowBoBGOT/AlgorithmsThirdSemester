#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <string.h>
#include <cmath>
template<typename T>
class BalancedMultiwayMergeSort {
    public:
    BalancedMultiwayMergeSort();
    BalancedMultiwayMergeSort(const std::string& fileName, int runSize, int numWays, bool isText=false) {
        Init(fileName, runSize, numWays, isText);
    }
    void Init(const std::string& fileName, int runSize, int numWays, bool isText=false) {
        m_sFileName = fileName;
        m_iRunSize = runSize;
        m_iNumWays = numWays;
        m_bIsText = isText;
    }
    ~BalancedMultiwayMergeSort() = default;

    public: // MAIN FUNCTIONS
    void Sort() { // Main procedure 
        ClearScratchFiles();
        std::filesystem::create_directory("One");
        std::filesystem::create_directory("Two");
        SplitIntoFiles();
        InitFileArrays();
        SortMerge();
        //if(m_bIsText) FromBinaryToText();
        ClearScratchFiles();
        std::filesystem::remove(m_sFileName);
        std::filesystem::rename(s_sResultBinary+s_sType, m_sFileName);
    }

    protected:
    void SplitIntoFiles() { // Splits big file into smaller ones
        auto file = std::ifstream(m_sFileName, std::ios::in|std::ios::binary);
        auto line = std::string();
        auto arr = std::vector<T>();
        auto obj = T();
        if(file.is_open()) for(auto i = 0;!file.eof();i++) {
            arr.reserve(m_iRunSize);
            auto fileNameOne = s_sScratchDirOne+"/"+std::to_string(i)+s_sType;
            auto FileOne = std::ofstream(fileNameOne, std::ios::binary|std::ios::app);

            auto j = 0;
            while(file.read(reinterpret_cast<char*>(&obj), sizeof(obj))) {
                arr.push_back(obj);
                j++;
            }
            std::sort(arr.begin(), arr.begin()+j);
            for(int k = 0;k<j;k++) {
                FileOne.write((char*)&arr[k], sizeof(arr[k]));
            }
            if(i==0) m_iNumRunsPerOutputFile++;
            if(i==m_iNumWays-1) i = -1;
            FileOne.write((char*)&SentinelValue, sizeof(SentinelValue));
            arr.clear();
            if(file.eof()) break;
        }
        m_iNumWays = GetDirectotyNumberOfFiles(s_sScratchDirOne);
        for(int i = 0;i<m_iNumWays;i++) {
            auto fileNameTwo = s_sScratchDirTwo+"/"+std::to_string(i)+s_sType;
            auto FileTwo = std::ofstream(fileNameTwo, std::ios::binary|std::ios::app);
            FileTwo.write((char*)&SentinelValue, sizeof(SentinelValue));
        }
    } 
    void SortMerge() { // Merges files into bigger ones Procedure
        bool isOne = true;
        m_vMergeArray = std::vector<T>(m_iNumWays);
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

    protected: // CONVERT BINARY TO TEXT
    void FromBinaryToText() { // Converts binary result into the text one if caller wants
        auto file = std::fstream(s_sResultBinary + s_sType, std::ios::in|std::ios::binary);
        auto fileOut = std::fstream(s_sResultText + s_sType, std::ios::out);
        auto val = T();
        while(!file.eof()) {
            file.read((char*)&val, sizeof(val));
            fileOut<<val<<"\n";
        }
    }

    protected: //CLEAR GARBAGE
    void ClearScratchFiles() { // Clears directories of scratch files
        std::filesystem::remove_all("One");
        std::filesystem::remove_all("Two");
    } 

    protected: // HELP FUNCTIONS
    int GetDirectotyNumberOfFiles(const std::string& dirName) { // Returns number of files in specified directory
        auto dirOne = std::filesystem::path("One");
        auto dirIter = std::filesystem::directory_iterator(dirOne);

        auto fileCount = std::count_if(
            begin(dirIter),
            end(dirIter),
            [](auto& entry) { return entry.is_regular_file(); }
        );

        return fileCount;
    } 

    void Sentinel(std::fstream& outFile) { // Adds sentinel value to the end of chunk of the output file
        outFile.write((char*)&SentinelValue, sizeof(SentinelValue));
    }    

    protected: // RUNS
    void PrepareRun(int i) { // Prepares runs controlling what file is the output one to be merged into
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
    void Run(std::fstream& outFile) { // Creates a run to merge one chunk from each of the input files into the single chunk of the output file
        InitMergeArray();
        while(!IsEndOfChunks()) {
            auto i = FindMinElement();
            if(m_vMergeArray[i]==SentinelValue) break;
            outFile.write((char*)&m_vMergeArray[i], sizeof(m_vMergeArray[i]));
            UpdateMergeArray(i);
        }
    } 
    void LastRun() { // Creates last run where all the files are merger into the resulting one
        auto fileOut = std::fstream(s_sResultBinary + s_sType, std::ios::out|std::ios::binary);
        Run(fileOut);
    } 

    protected: // MERGE ARRAY
    void InitMergeArray() {  // Initializes merge array of size of the number of ways
        for(int i = 0;i<m_iNumWays;i++) {
            if(!m_vInput[i].eof()) {
                auto cursor = m_vInput[i].tellg();
                auto val = SentinelValue;
                m_vInput[i].read((char*)&val, sizeof(val));
                if(val!=SentinelValue) {
                    m_vMergeArray[i] = val;
                } else if(val==SentinelValue && !m_vInput[i].eof()) {
                    m_vMergeArray[i] = SentinelValue;
                    m_vInput[i].seekg(cursor);
                }
            }
        }
    }
    void UpdateMergeArray(int index) { // Updates merge array with the index of the smallest element
        if(!m_vInput[index].eof()) {
            auto cursor = m_vInput[index].tellg();
            auto val = SentinelValue;
            m_vInput[index].read((char*)&val, sizeof(val));
            if(val!=SentinelValue) {
                m_vMergeArray[index] = val;
            } else if(val==SentinelValue) {
                m_vMergeArray[index] = SentinelValue;
                m_vInput[index].seekg(cursor);
            }
        }
    }
 
    
    int FindMinElement() { // Returns the index of the input file where the smallest element is
        auto minIndex = 0;
        for(int i = 0;i<m_iNumWays;i++) {
            if(m_vMergeArray[i]!=SentinelValue) {
                minIndex = i;
            }
        }
        for(int i = 0;i<m_iNumWays;i++) {
            if(m_vMergeArray[i]!=SentinelValue) {
                if(m_vMergeArray[minIndex]>=m_vMergeArray[i]) {
                    minIndex = i;
                }
            }
        }
        return minIndex;
    }

    protected: // END OF CHUNKS FILES
    bool IsEndOfChunks() { // Checks if all the input files reached the end of their current chunk
        for(int i = 0;i<m_iNumWays;i++) {
            if(m_vMergeArray[i]!=SentinelValue) return false;
        }

        return true;
    } 
    bool IsEndOfFiles() { // Checks if all the input files reached their end
        for(int i = 0;i<m_iNumWays;i++) {
            if(!m_vInput[i].eof()) return false;
        }
        return true;
    } 

    protected: // NEXT CHUNKS
    void GoToNextChunk() { // Makes all the input files go to their next chunks
        auto val = SentinelValue;
        for(int i = 0;i<m_iNumWays;i++) {
            if(!m_vInput[i].eof()) {
                auto cursor = m_vInput[i].tellg();
                m_vInput[i].read((char*)&val, sizeof(val));
                if(val!=SentinelValue) m_vInput[i].seekg(cursor);
            }
        }
    } 

    protected: // OPEN INIT CLOSE FILES
    void InitFileArrays() { // Initializes input and output arrays of files 
        m_vInput = std::vector<std::fstream>(m_iNumWays);
        m_vOutput = std::vector<std::fstream>(m_iNumWays);
    } 
    void OpenFiles(bool isOne) { // Opens input and output files
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
    void CloseFiles() { // Closes input and output files
        for(int i = 0;i<m_iNumWays;i++) {
            m_vInput[i].close();
            m_vOutput[i].close();
        }
    } 

    protected:
    std::string m_sFileName = "";           // Name of the input file
    int m_iRunSize = 0;                     // Size of the run
    int m_iNumWays = 0;                     // Number of ways

    protected:
    int m_iNumRunsPerOutputFile = 0;        // Number of times chunks are stacked on each other
    std::vector<std::fstream> m_vInput;     // Input array
    std::vector<std::fstream> m_vOutput;    // Output array
    std::vector<T> m_vMergeArray;           // Merge array
    bool m_bIsText = false;                 // Whether convert binary result file to text or not

    public:
    static T SentinelValue;           // Sentinel value

    protected:
    static std::string s_sScratchDirOne;    // Scratch directory one
    static std::string s_sScratchDirTwo;    // Scratch directory two
    static std::string s_sType;             // Type of the input file
    static std::string s_sResultBinary;     // Name of the result binary file
    static std::string s_sResultText;       // Name of the text result file

    
};
template<typename T> T BalancedMultiwayMergeSort<T>::SentinelValue = T();
template<typename T> std::string BalancedMultiwayMergeSort<T>::s_sScratchDirOne = "One";
template<typename T> std::string BalancedMultiwayMergeSort<T>::s_sScratchDirTwo = "Two";
template<typename T> std::string BalancedMultiwayMergeSort<T>::s_sType          = ".txt";
template<typename T> std::string BalancedMultiwayMergeSort<T>::s_sResultBinary  = "result_binary";
template<typename T> std::string BalancedMultiwayMergeSort<T>::s_sResultText    = "result_text";

