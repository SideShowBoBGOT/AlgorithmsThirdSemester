#pragma once
#include <vector>
#include <string>
class TData {
    public:
    TData()=default;
    virtual void Display();
    virtual void Update(int id, const char data[30]);
    virtual bool Delete(int id);
    virtual bool Search(int id, const char data[30]);

    protected:
    struct SRecord;
    struct SIndex;

    protected:
    virtual int GetBlockStart(int corPosition);
    virtual int GetCorPosition(int corID);
    virtual std::vector<SRecord> ReadBlock(int blockStart);
    virtual void WriteBlock(int blockStart, std::vector<SRecord>& block);
    virtual bool SearchInIndexFile(int id, const char data[30], int& position);
    virtual bool IsCorrespondingIDExists(int id, int& corID, int& lastID);
    virtual bool SearchInRecordsFile(int id, int corPosition, const char data[30], int& position);
    virtual void RebuildIndexes(int corID, int lastID);
    virtual bool SharrahSearch(int id, const char data[30], const std::vector<SRecord>& records, int& position);
    
    protected:
    struct SRecord {
        char Data[30] = "";
        int Id = -1;
        bool operator<(const SRecord& other) {
            return Id<other.Id;
        }
    };
    struct SIndex {
        int Id = -1;
        int Position = -1;
    };

    protected:
    int m_iIndexSize = 1000;

    protected:
    int m_iSize = 0;

    protected:
    const std::string m_sRecordsFile = "recordsFile.txt";
    const std::string m_sIndexFile = "indexFile.txt";
};