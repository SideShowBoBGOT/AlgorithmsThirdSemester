#include "TData.h"
#include <fstream>
#include <iostream>
#include <algorithm>
std::vector<std::pair<int, const char[30]>> TData::Display() {
    auto seqfile = std::fstream(m_sRecordsFile, std::ios::out|std::ios::in|std::ios::binary);
    auto indexfile = std::fstream(m_sIndexFile, std::ios::out|std::ios::in|std::ios::binary);
    auto index = SIndex();
    auto records = std::vector<std::pair<int, const char[30]>>();
    while(indexfile.read(reinterpret_cast<char*>(&index), sizeof(index))) {
        auto corPosition = GetCorPosition(index.Id);
        auto blockStart = GetBlockStart(corPosition);
        auto block = ReadBlock(blockStart);
        for(const auto& record : block) {
            if(record.Id!=-1) {
                auto p = std::pair<int, const char[30]>();
                p.first = record.Id;
                memcpy((void*)p.second, record.Data, 30*sizeof(char));
                records.push_back(p);
            }
        }
    }
    return records;
}
void TData::RebuildIndexes(int corIndex, int lastIndex) {
    auto indexFile = std::fstream(m_sIndexFile, std::ios::out|std::ios::in|std::ios::binary);
    auto recordsFile = std::fstream(m_sRecordsFile, std::ios::out|std::ios::in|std::ios::binary);
    indexFile.seekp(0, std::ios::end);
    recordsFile.seekp(0, std::ios::end);
    auto index = SIndex();
    
    index.Id = lastIndex + m_iIndexSize;
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
    lastID = -1;
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

bool TData::Search(int id, const char data[30], int& comparisions) {
    auto position = 0;
    return SearchInIndexFile(id, data, position, comparisions);
}


bool TData::SearchInIndexFile(int id, const char data[30], int& position, int& comparisions) {
    auto corID = 0;
    auto lastID = 0;
    position = 0;

    if(!IsCorrespondingIDExists(id, corID, lastID))
        return false;

    auto corPosition = GetCorPosition(corID);

    return SearchInRecordsFile(id, corPosition, data, position, comparisions);
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

bool TData::SearchInRecordsFile(int id, int corPosition, const char data[30], int& position, int& comparisions) {
    position = GetBlockStart(corPosition);
    auto rawblock = ReadBlock(position);
    auto block = std::vector<SRecord>();
    for(const auto& r : rawblock) {
        if(r.Id!=-1) {
            block.push_back(r);
        }
    }

    return SharrahSearch(id, data, block, comparisions);
}

bool TData::SharrahSearch(int id, const char data[30], const std::vector<SRecord>& block, int& comparisions) {
    auto flag = true;
    auto num = int(block.size());
    int i = num/2 + 1;
    int b = num/2;
    int counter = 0;
    while(counter < 2) {
        if(b<=0) {
            counter++;
        }
        if(i>=num) {
            i -= b/2 + 1;
            b /= 2;
            comparisions++;

        } else if(i<=0) {
            i += b/2 + 1;
            b /= 2;
            comparisions++;

        } else if(block[i].Id< id) {
            i += b/2 + 1;
            b /= 2;
            comparisions++;

        } else if(block[i].Id > id) {
            i -= b/2 + 1;
            b /= 2;
            comparisions++;
        } else if(block[i].Id == id) {
            memcpy((void*)data, block[int(i)].Data, sizeof(char)*30);
            return true;
        }
    }
    return false;
}