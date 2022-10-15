#include "TData.h"
TData::TData() {
    
}
void TData::Create() {
    auto i = 0;
    auto ch = 'y';
    auto seqfile = std::fstream(m_sRecordsFile,std::ios::out|std::ios::binary);
    auto indexfile = std::fstream(m_sIndexFile, std::ios::out|std::ios::binary);
    do {
        std::cout<<"\n Enter Record: ";
        std::cin>>m_xRecord.Record;
        std::cout<<"\n Enter Id: ";
        std::cin>>m_xRecord.Id;
        seqfile.write(reinterpret_cast<char*>(&m_xRecord), sizeof(m_xRecord))<<std::flush;;
        m_xIndex.Id=m_xRecord.Id;
        m_xIndex.Position=i;
        indexfile.write(reinterpret_cast<char*>(&m_xIndex), sizeof(m_xIndex))<<std::flush;;
        i++;
        std::cout<<"\nDo you want to add more records?";
        std::cin>>ch;
     } while(ch=='y');
}
void TData::Display() {
    auto i = 0;
    auto seqfile = std::fstream(m_sRecordsFile,std::ios::in|std::ios::binary);
    auto indexfile = std::fstream(m_sIndexFile,std::ios::in|std::ios::binary);
    while(indexfile.read(reinterpret_cast<char*>(&m_xIndex), sizeof(m_xIndex))) {
        i = m_xIndex.Position*sizeof(SRec);//getting pos from index file
        seqfile.seekg(i,std::ios::beg);//seeking record of that pos from seq.file
        seqfile.read(reinterpret_cast<char*>(&m_xRecord),sizeof(m_xRecord));//reading record
        //if rec. is not deleted logically
        if(m_xRecord.Id!=-1) {   //then display it
            std::cout<<"\nRecord: "<<m_xRecord.Record<<std::flush;
            std::cout<<"\nId: "<<m_xRecord.Id;
            std::cout<<"\n";
        }
    }
}
void TData::RebuildIndexes() {
    auto i = 0;
    auto seqfile = std::fstream(m_sRecordsFile,std::ios::in|std::ios::binary);
    auto indexfile = std::fstream(m_sIndexFile,std::ios::out|std::ios::binary);
	auto val = TData::SRec();
    auto index = TData::SIndex();
    auto pos = seqfile.tellg()/sizeof(SRec);
    while(seqfile.read(reinterpret_cast<char*>(&val), sizeof(val))) {
        if(i!=0 && i%m_iIndexSize==0) {
            index.Id = val.Id;
            index.Position = pos;
            indexfile.write(reinterpret_cast<char*>(&index), sizeof(index));
        }
        pos = seqfile.tellg()/sizeof(SRec);
        i++;
	}

}
bool TData::Update(int id, const char record[30]) {
    auto seqfile = std::fstream(m_sRecordsFile,std::ios::in|std::ios::out|std::ios::binary);
    auto indexfile = std::fstream(m_sIndexFile,std::ios::in|std::ios::out|std::ios::binary);
    auto pos = -1;
    //reading index file for getting the index
    while(indexfile.read(reinterpret_cast<char*>(&m_xIndex),sizeof(m_xIndex))) {
        //the desired record is found
        if(id==m_xIndex.Id) {
            pos=m_xIndex.Position;//getting the Position
            break;
        }
    }
    if(pos==-1) {
        return false;
    } 
    //calculating the Position of record in seq. file using the pos of ind. file
    auto offset = pos*sizeof(SRec);
    seqfile.seekp(offset);//seeking the desired record for modification
    memcpy(m_xRecord.Record, record, 30*sizeof(char));//can be updated
    m_xRecord.Id = id;//It's unique id,so don't change
    seqfile.write(reinterpret_cast<char*>(&m_xRecord),sizeof(m_xRecord))<<std::flush;
    return true;
}
bool TData::Delete(int id) {
    auto seqfile = std::fstream(m_sRecordsFile,std::ios::in|std::ios::out|std::ios::binary);
    auto indexfile = std::fstream(m_sIndexFile,std::ios::in|std::ios::out|std::ios::binary);
    auto pos = -1;
    //reading index file for getting the index
    while(indexfile.read(reinterpret_cast<char*>(&m_xIndex),sizeof(m_xIndex))) {
        //desired record is found
        if(id==m_xIndex.Id) {
            pos = m_xIndex.Position;
            m_xIndex.Id = -1;
            break;
        }
    }
    if(pos==-1) {
        return false;
    }
    //calculating the Position of record in seq. file using the pos of ind. file
    auto offset = pos*sizeof(SRec);
    seqfile.seekp(offset);//seeking the desired record for deletion
    memcpy(m_xRecord.Record,"", 30*sizeof(char));
    m_xRecord.Id = -1; //logical deletion
    seqfile.write(reinterpret_cast<char*>(&m_xRecord), sizeof(m_xRecord))<<std::flush;//writing deleted status 
    //From index file also the desired record gets deleted as follows
    offset = pos*sizeof(SIndex);//getting Position in index file
    indexfile.seekp(offset); //seeking that record
    m_xIndex.Id=-1; //logical deletion of Id
    m_xIndex.Position = pos;//Position remain unchanged
    indexfile.write(reinterpret_cast<char*>(&m_xIndex), sizeof(m_xIndex))<<std::flush;
    return true;
}
void TData::Append(int id, const char record[30]) {
    auto indexfile = std::fstream(m_sIndexFile,std::ios::in|std::ios::binary);
    indexfile.seekg(0,std::ios::end);
    auto pos=indexfile.tellg()/sizeof(m_xIndex);
    indexfile.close();

    indexfile.open(m_sIndexFile,std::ios::app|std::ios::binary);
    auto seqfile = std::fstream(m_sRecordsFile,std::ios::app|std::ios::binary);

    memcpy(m_xRecord.Record, record, 30*sizeof(char));
    m_xRecord.Id = id;

    seqfile.write(reinterpret_cast<char*>(&m_xRecord), sizeof(m_xRecord));//inserting rec at end in seq. file
    seqfile.close();

    
    Sort();
    //RebuildIndexes();
    
}
bool TData::Search(int id) {
    auto indexfile = std::fstream(m_sIndexFile,std::ios::in|std::ios::binary);
    auto offset = 0;
    auto pos = -1;
    //reading index file to obtain the index of desired record
    while(indexfile.read(reinterpret_cast<char*>(&m_xIndex), sizeof(m_xIndex))) {
        //desired record found
        if(id==m_xIndex.Id) {
            pos=m_xIndex.Position;//seeking the Position
            break;
        }
    }
    if(pos==-1) {
        return false;
    }
    //calculate offset using Position obtained from ind. file
    offset = pos*sizeof(m_xRecord);
    auto seqfile = std::fstream(m_sRecordsFile,std::ios::in|std::ios::binary);
    //seeking the record from seq. file using calculated offset
    seqfile.seekg(offset,std::ios::beg);//seeking for reading purpose
    seqfile.read(reinterpret_cast<char*>(&m_xRecord), sizeof(m_xRecord));
    if(m_xRecord.Id==-1) {
        return false;
        //Id=desired record’s id
    } else {
        std::cout<<"\n The Record is present in the file and it is...";
        std::cout<<"\n Record: "<<m_xRecord.Record;
        std::cout<<"\n Id: "<<m_xRecord.Id;
    }
    return true;
}

bool TData::SharrahSearch(int id) {
    return false;
}

void TData::Sort() {
    m_xRecSort = nullptr;
    m_xRecSort = std::make_shared<BalancedMultiwayMergeSort<SRec>>(m_sRecordsFile, 100, 10);
    m_xRecSort->SentinelValue = m_pSentinelRec;
    m_xRecSort->Sort();
    //m_xIndexSort.Sort();
}
