#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <conio.h>
#include <stdlib.h>

class TData {
    public:
    struct SRecord;
    struct SIndex;

    public:
    TData()=default;
    virtual void Display();
    virtual bool Update(int id, const char data[30]);
    virtual bool Delete(int id);
    virtual void Append(int id, const char data[30]);
    virtual bool Search(int id, const char data[30]);

    protected:
    virtual int GetBlockStart(int corPosition);
    virtual std::vector<SRecord> ReadBlock(int blockStart);
    virtual bool SearchInIndexFile(int id, const char data[30], int& position);
    virtual bool IsCorrespondingIDExists(int id, int& corID, int& lastID);
    virtual bool SearchInRecordsFile(int id, int corPosition, const char data[30], int& position);
    virtual void RebuildIndexes(int corID, int lastID);
    virtual bool SharrahSearch(int id, const char data[30], const std::vector<SRecord>& records, int& position);
    
    public:
    struct SRecord {
        char Data[30] = "";
        int Id = -1;
    };
    struct SIndex {
        int Id = -1;
        int Position = -1;
        bool operator==(const SIndex& other) {
            return Id==other.Id;
        }
        bool operator!=(const SIndex& other) {
            return Id!=other.Id;
        }
        bool operator>=(const SIndex& other) {
            return Id>=other.Id;
        }
        bool operator>(const SIndex& other) {
            return Id>other.Id;
        }
        bool operator<=(const SIndex& other) {
            return Id<=other.Id;
        }
        bool operator<(const SIndex& other) {
            return Id<other.Id;
        }
    };

    protected:
    int m_iIndexSize = 10;

    protected:
    int m_iSize = 0;

    protected:
    const std::string m_sRecordsFile = "recordsFile.txt";
    const std::string m_sIndexFile = "indexFile.txt";
};