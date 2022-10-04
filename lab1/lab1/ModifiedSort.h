#pragma once
#include <string>
#include <fstream>
#include <vector>
class ModifiedSort {
    public:
    ModifiedSort() = delete;
    ModifiedSort(const std::string& fileName, int runSize, int numWays, bool isText=false);
    virtual ~ModifiedSort() = default;

    protected: // MAIN FUNCTIONS
    virtual void Sort(); // Main procedure 
    virtual void SplitIntoFiles(); // Splits big file into smaller ones
    virtual void SortMerge(); // Merges files into bigger ones Procedure

    protected: // CONVERT BINARY TO TEXT
    virtual void FromBinaryToText(); // Converts binary result into the text one if caller wants

    protected: //CLEAR GARBAGE
    virtual void ClearScratchFiles(); // Clears directories of scratch files

    protected: // HELP FUNCTIONS
    virtual int GetDirectotyNumberOfFiles(const std::string& dirName); // Returns number of files in specified directory
    virtual void Sentinel(std::fstream& outFile); // Adds sentinel value to the end of chunk of the output file

    protected: // RUNS
    virtual void PrepareRun(int i); // Prepares runs controlling what file is the output one to be merged into
    virtual void Run(std::fstream& outFile); // Creates a run to merge one chunk from each of the input files into the single chunk of the output file
    virtual void LastRun(); // Creates last run where all the files are merger into the resulting one

    protected: // MERGE ARRAY
    virtual void InitMergeArray(); // Initializes merge array of size of the number of ways
    virtual void UpdateMergeArray(int index); // Updates merge array with the index of the smallest element
    virtual int FindMinElement(); // Returns the index of the input file where the smallest element is

    protected: // END OF CHUNKS FILES
    virtual bool IsEndOfChunks(); // Checks if all the input files reached the end of their current chunk
    virtual bool IsEndOfFiles(); // Checks if all the input files reached their end

    protected: // NEXT CHUNKS
    virtual void GoToNextChunk(); // Makes all the input files go to their next chunks

    protected: // OPEN INIT CLOSE FILES
    virtual void InitFileArrays(); // Initializes input and output arrays of files 
    virtual void OpenFiles(bool isOne); // Opens input and output files
    virtual void CloseFiles(); // Closes input and output files

    protected:
    std::string m_sFileName = ""; // Name of the input file
    int m_iRunSize = 0; // Size of the run
    int m_iNumWays = 0; // Number of ways

    protected:
    int m_iNumRunsPerOutputFile = 0; // Number of times chunks are stacked on each other
    std::vector<std::fstream> m_vInput; // Input array
    std::vector<std::fstream> m_vOutput; // Output array
    std::vector<double> m_vMergeArray; // Merge array
    bool m_bIsText = false; // Whether convert binary result file to text or not

    protected:
    static double s_dSentinel; // Sentinel value
    static std::string s_sScratchDirOne; // Scratch directory one
    static std::string s_sScratchDirTwo; // Scratch directory two
    static std::string s_sType; // Type of the input file
    static std::string s_sResultBinary; // Name of the result binary file
    static std::string s_sResultText; // Name of the text result file

};
