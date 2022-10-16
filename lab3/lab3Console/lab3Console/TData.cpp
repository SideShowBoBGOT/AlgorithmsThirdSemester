#include "TData.h"
#include <fstream>
#include <iostream>
#include <algorithm>
void TData::Display() {
    auto seqfile = std::fstream(m_sRecordsFile, std::ios::out|std::ios::in|std::ios::binary);
    auto indexfile = std::fstream(m_sIndexFile, std::ios::out|std::ios::in|std::ios::binary);
    auto index = SIndex();
    while(indexfile.read(reinterpret_cast<char*>(&index), sizeof(index))) {
        auto corPosition = GetCorPosition(index.Id);
        auto blockStart = GetBlockStart(corPosition);
        auto block = ReadBlock(blockStart);
        for(const auto& record : block) {
            if(record.Id!=-1) {
                std::cout<<"ID: "<<record.Id<<"\n";
                std::cout<<"Data: "<<record.Data<<"\n";
            }
        }
    }
}
void TData::RebuildIndexes(int corIndex, int lastIndex) {
    auto indexFile = std::fstream(m_sIndexFile, std::ios::out|std::ios::in|std::ios::binary);
    auto recordsFile = std::fstream(m_sRecordsFile, std::ios::out|std::ios::in|std::ios::binary);
    indexFile.seekp(0, std::ios::end);
    recordsFile.seekp(0, std::ios::end);
    auto index = SIndex();
    
    index.Id = lastIndex + m_iIndexSize - 1;
    for(;index.Id<=corIndex;index.Id+=m_iIndexSize) {
        auto record = SRecord();
        for(auto i=0;i<m_iIndexSize-1;++i) {
            recordsFile.write(reinterpret_cast<char*>(&record), sizeof(record));
        }
        record.Id = index.Id;
        index.Position = recordsFile.tellp();
        recordsFile.write(reinterpret_cast<char*>(&record), sizeof(record));
        indexFile.write(reinterpret_cast<char*>(&index), sizeof(index));
    }
}
bool TData::Delete(int id) {
    auto corID = 0;
    auto lastID = 0;
    if(!IsCorrespondingIDExists(id, corID, lastID))
        return false;
    
    auto corPosition = GetCorPosition(corID);
    auto blockStart = GetBlockStart(corPosition);

    auto position = blockStart + (id%m_iIndexSize)*sizeof(SRecord);
    auto recordsFile = std::fstream(m_sRecordsFile, std::ios::out|std::ios::in|std::ios::binary);
    recordsFile.seekg(position, std::ios::beg);
    auto record = SRecord();
    recordsFile.read(reinterpret_cast<char*>(&record), sizeof(record));

    if(record.Id==-1)
        return false;
    
    record.Id = -1;
    const char data[30] = {0};
    memcpy(record.Data, data, 30*sizeof(char));
    recordsFile.seekp(position, std::ios::beg);
    recordsFile.write(reinterpret_cast<char*>(&record), sizeof(record));

    return true;
}
bool TData::IsCorrespondingIDExists(int id, int& corID, int& lastID) {
    auto indexFile = std::fstream(m_sIndexFile, std::ios::out|std::ios::in|std::ios::binary);
    corID = (id/m_iIndexSize + 1)*m_iIndexSize - 1;
    auto index = SIndex();
    while(indexFile.read(reinterpret_cast<char*>(&index), sizeof(index))) {
        lastID = index.Id;
        if(index.Id==corID) {
            return true;
        }
    }

    return false;
}

std::vector<TData::SRecord> TData::ReadBlock(int blockStart) {
    auto block = std::vector<SRecord>(m_iIndexSize);
    auto recordsFile = std::fstream(m_sRecordsFile, std::ios::out|std::ios::in|std::ios::binary);
    recordsFile.seekg(blockStart, std::ios::beg);
    auto record = SRecord();
    for(auto i=0;i<m_iIndexSize;++i) {
        recordsFile.read(reinterpret_cast<char*>(&record), sizeof(record));
        block[i] = record;
    }
    return block;
}

void TData::WriteBlock(int blockStart, std::vector<SRecord>& block) {
    auto recordsFile = std::fstream(m_sRecordsFile, std::ios::out|std::ios::in|std::ios::binary);
    recordsFile.seekp(blockStart, std::ios::beg);
    for(auto i=0;i<m_iIndexSize;++i) {
        recordsFile.write(reinterpret_cast<char*>(&block[i]), sizeof(block[i]));
    }
}

void TData::Update(int id, const char data[30]) {
    auto corID = 0;
    auto lastID = 0;
    if(!IsCorrespondingIDExists(id, corID, lastID))
        RebuildIndexes(corID, lastID); 
    
    auto corPosition = GetCorPosition(corID);
    auto blockStart = GetBlockStart(corPosition);

    auto writePosition = blockStart + (id%m_iIndexSize)*sizeof(SRecord);
    auto record = SRecord();
    record.Id = id;
    memcpy(record.Data, data, 30*sizeof(char));
    auto recordsFile = std::fstream(m_sRecordsFile, std::ios::out|std::ios::in|std::ios::binary);
    recordsFile.seekp(writePosition, std::ios::beg);
    recordsFile.write(reinterpret_cast<char*>(&record), sizeof(record));
}

bool TData::Search(int id, const char data[30]) {
    auto position = 0;
    return SearchInIndexFile(id, data, position);
}


bool TData::SearchInIndexFile(int id, const char data[30], int& position) {
    auto corID = 0;
    auto lastID = 0;
    position = 0;

    if(!IsCorrespondingIDExists(id, corID, lastID))
        return false;

    auto corPosition = GetCorPosition(corID);

    return SearchInRecordsFile(id, corPosition, data, position);
}

int TData::GetBlockStart(int corPosition) {
    return corPosition - (m_iIndexSize - 1)*sizeof(SRecord);
}

int TData::GetCorPosition(int corID) {
    auto indexFile = std::fstream(m_sIndexFile, std::ios::out|std::ios::in|std::ios::binary);
    auto indexPosition = (corID/m_iIndexSize)*sizeof(SIndex);
    indexFile.seekg(indexPosition, std::ios::beg);
    auto index = SIndex();
    indexFile.read(reinterpret_cast<char*>(&index), sizeof(index));
    auto corPosition = index.Position;
    return corPosition;
}

bool TData::SearchInRecordsFile(int id, int corPosition, const char data[30], int& position) {
    position = GetBlockStart(corPosition);
    auto block = ReadBlock(position);
    return SharrahSearch(id, data, block, position);
}

bool TData::SharrahSearch(int id, const char data[30], const std::vector<SRecord>& block, int& position) {
    auto flag = true;
    auto isFound = false;
    auto num = block.size();
    auto k = int(std::log2(num));
    auto i = std::pow(2, k) - 1;
    auto b = 0.0;
    if(id<block[int(i)].Id) {
        b = i + 1;
    } else {
        auto l = std::log2(num - i);
        i = num - std::pow(2, l);
        b = std::pow(2, l);
    }
    while(flag) {
        if(b>=0) {
            if(i>=num || block[int(i)].Id>=id) {
                i -= (int(b)/2) + 1;
                b = int(b)/2;
            } else if(i<0 || block[int(i)].Id<id) {
                i += (int(b)/2) + 1;
                b = int(b)/2;
            } else if(block[int(i)].Id==id) {
                memcpy((void*)data, block[int(i)].Data, sizeof(char)*30);
                position += int(i)*sizeof(SRecord);
                isFound = true;
                flag = false;
            }
        } else {
            flag = false;
        }
    }
    return isFound;
}
