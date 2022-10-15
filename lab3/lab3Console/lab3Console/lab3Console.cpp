#include "TData.h"

void main() {
	srand(time(NULL));
	/*std::filesystem::remove("indexFile.txt");
	std::filesystem::remove("recordsFile.txt");
	auto seqFile = std::fstream("recordsFile.txt", std::ios::out);
	seqFile.close();
	auto indexFile = std::fstream("indexFile.txt", std::ios::out);
	indexFile.close();*/
    TData List = TData();
	/*for(int i=0;i<6;i++) {
		auto val = rand()%30000;
		char str[30] = "";
		for(int j=0;j<29;j++) {
			str[j] = static_cast<char>(rand()%60+20);
		}
		str[29] = '\0';
		List.Append(val, str);
	}*/
	/*auto file = std::ifstream("recordsFile.txt", std::ios::in|std::ios::binary);
	TData::SRec val;
	while(file.read(reinterpret_cast<char*>(&val), sizeof(val))) {
		std::cout<<std::endl;
		std::cout<<val.Record<<std::endl;
		std::cout<<val.Id;
	}
	std::cout<<std::endl<<"==================="<<std::endl;*/
	List.Display();
	auto val = TData::SRec();
	List.Search(215, val);
	
}