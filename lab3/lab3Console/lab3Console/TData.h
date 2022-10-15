#pragma once
#include "../../../lab1/lab1/BalancedMultiwayMergeSort.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <memory>

class TData {
    public:
    TData();
    virtual void Create();
    virtual void Display();
    virtual bool Update(int id, const char record[30]);
    virtual bool Delete(int id);
    virtual void Append(int id, const char record[30]);
    virtual bool Search(int id);
    virtual bool SharrahSearch(int id);
    virtual void Sort();


    protected:
    virtual void RebuildIndexes();

    public:
    struct SRec {
        SRec()=default;
        SRec(int id, const char record[30]) {
            Id = id;
            memcpy(Record, record, 30*sizeof(char));
        }
        char Record[30] = "";
        int Id = -1;
        bool operator==(const SRec& other) {
            return Id==other.Id;
        }
        bool operator!=(const SRec& other) {
            return Id!=other.Id;
        }
        bool operator>=(const SRec& other) {
            return Id>=other.Id;
        }
        bool operator>(const SRec& other) {
            return Id>other.Id;
        }
        bool operator<=(const SRec& other) {
            return Id<=other.Id;
        }
        bool operator<(const SRec& other) {
            return Id<other.Id;
        }
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
    int m_iIndexSize = 8;

    protected:
    SRec m_pSentinelRec = SRec(-1, "SENTINEL");

    protected:
    SRec m_xRecord;
    SIndex m_xIndex;

    protected:
    const std::string m_sRecordsFile = "recordsFile.txt";
    const std::string m_sIndexFile = "indexFile.txt";

    protected:
    std::shared_ptr<BalancedMultiwayMergeSort<SRec>> m_xRecSort = nullptr;;
};