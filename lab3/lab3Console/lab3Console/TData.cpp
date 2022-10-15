#include "TData.h"
void TData::Display() {
    auto i = 0;
    auto seqfile = std::fstream(m_sRecordsFile,std::ios::in|std::ios::binary);
    auto indexfile = std::fstream(m_sIndexFile,std::ios::in|std::ios::binary);
    auto prevI = 0;
    while(indexfile.read(reinterpret_cast<char*>(&m_xIndex), sizeof(m_xIndex))) {
        i = m_xIndex.Position*sizeof(SRecord);//getting pos from index file

        seqfile.seekg(prevI,std::ios::beg);//seeking record of that pos from seq.file
        do {
            seqfile.read(reinterpret_cast<char*>(&m_xRecord), sizeof(m_xRecord));//reading record
            std::cout<<"\nRecord: "<<m_xRecord.Data<<std::flush;
            std::cout<<"\nId: "<<m_xRecord.Id;
            std::cout<<"\n";
        } while(seqfile.tellg()!=i);
        prevI = i;
    }
    seqfile.seekg(prevI,std::ios::beg);//seeking record of that pos from seq.file
    while(seqfile.read(reinterpret_cast<char*>(&m_xRecord), sizeof(m_xRecord))) {
        std::cout<<"\nRecord: "<<m_xRecord.Data<<std::flush;
        std::cout<<"\nId: "<<m_xRecord.Id;
        std::cout<<"\n";
    }
}
void TData::RebuildIndexes(int corIndex, int lastIndex) {
    auto indexFile = std::fstream(m_sIndexFile, std::ios::out|std::ios::binary);
    auto recordsFile = std::fstream(m_sRecordsFile, std::ios::out|std::ios::binary);
    indexFile.seekp(0, std::ios::end);
    recordsFile.seekp(0, std::ios::end);
    auto index = SIndex();
    auto record = SRecord();
    index.Id = lastIndex + m_iIndexSize;
    for(;index.Id<=corIndex;index.Id+=m_iIndexSize) {
        indexFile.write(reinterpret_cast<char*>(&index), sizeof(index));
        for(auto i=0;i<m_iIndexSize-1;++i) {
            recordsFile.write(reinterpret_cast<char*>(&record), sizeof(record));
        }
        record.Id = index.Id;
        recordsFile.write(reinterpret_cast<char*>(&record), sizeof(record));
    }
}
bool TData::Update(int id, const char record[30]) {
    
    return true;
}
bool TData::Delete(int id) {
   
    return true;
}
bool TData::IsCorrespondingIDExists(int id, int& corID, int& lastID) {
    auto indexFile = std::fstream(m_sIndexFile, std::ios::in|std::ios::binary);
    corID = (id/m_iIndexSize + 1)*m_iIndexSize;
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
    auto recordsFile = std::fstream(m_sRecordsFile, std::ios::in|std::ios::binary);
    recordsFile.seekg(blockStart, std::ios::beg);
    auto record = SRecord();
    for(auto i=0;i<m_iIndexSize;++i) {
        recordsFile.read(reinterpret_cast<char*>(&record), sizeof(record));
        block.push_back(record);
    }
    return block;
}


void TData::Append(int id, const char record[30]) {
    auto corID = 0;
    auto lastID = 0;
    if(!IsCorrespondingIDExists(id, corID, lastID))
        RebuildIndexes(corID, lastID); 
    

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

    auto indexFile = std::fstream(m_sIndexFile, std::ios::in|std::ios::binary);
    auto indexPosition = corID/m_iIndexSize*sizeof(SIndex);
    indexFile.seekg(indexPosition, std::ios::beg);
    auto index = SIndex();
    indexFile.read(reinterpret_cast<char*>(&index), sizeof(index));
    auto corPosition = index.Position;

    return SearchInRecordsFile(id, corPosition, data, position);
}

int TData::GetBlockStart(int corPosition) {
    return corPosition - m_iIndexSize + 1;
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
