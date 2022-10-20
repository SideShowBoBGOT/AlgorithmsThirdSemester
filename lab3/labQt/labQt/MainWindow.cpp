#include "MainWindow.h"
#include <filesystem>
#include <fstream>
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent) {
	ui.setupUi(this);
	Display();
	connect(ui.updateButton, SIGNAL(released()), this, SLOT(Update()));
	connect(ui.deleteButton, SIGNAL(released()), this, SLOT(Delete()));
	connect(ui.searchButton, SIGNAL(released()), this, SLOT(Search()));
	connect(ui.clearButton, SIGNAL(released()), this, SLOT(Clear()));
	connect(ui.generateButton, SIGNAL(released()), this, SLOT(Generate()));
	connect(ui.displayButton, SIGNAL(released()), this, SLOT(Display()));
	
}

MainWindow::~MainWindow() {

}

void MainWindow::Update() {
	ui.label_4->setText("");

	auto id = ui.idEdit->text().toInt();
	auto rawData = ui.dataEdit->text().toStdString();
	while(rawData.size()<20) {
		rawData += ' ';
	}
	auto cleanData = rawData.c_str();
	char data[30] = { 0 };
	for(auto i=0;i<30;++i) {
		data[i] = '\0';
	}
	for(auto i=0;i<20;++i) {
		data[i] = cleanData[i];
	}
	table.Update(id, data);
	Display();
}
void MainWindow::Delete() {
	ui.label_4->setText("");

	auto id = ui.idEdit->text().toInt();
	if(!table.Delete(id)) ui.label_4->setText("No such ID");
	Display();
}

void MainWindow::Clear() {
	ui.label_4->setText("");

	std::filesystem::remove("indexFile.txt");
	std::filesystem::remove("recordsFile.txt");
	auto seqFile = std::fstream("recordsFile.txt", std::ios::out);
	seqFile.close();
	auto indexFile = std::fstream("indexFile.txt", std::ios::out);
	indexFile.close();
	Display();
}

void MainWindow::Generate() {
	ui.label_4->setText("");

	srand(time(NULL));
	for(int i=0;i<1000;i++) {
		auto val =  i;
		char str[30] = "";
		for(int j=0;j<29;j++) {
			str[j] = static_cast<char>(rand()%60+20);
		}
		str[29] = '\0';
		table.Update(val, str);
	}
	Display();
}

void MainWindow::Search() {
	ui.label_4->setText("");

	for(const auto& label : labels) {
		layout->removeWidget(label);
		delete label;
	}
	
	labels.clear();

	auto id = ui.idEdit->text().toInt();
	char data[30] = { 0 };
	for(auto i=0;i<30;++i) {
		data[i] = '\0';
	}
	auto comparisions = 0;
	if(table.Search(id, data, comparisions)) {
		auto qlabel = new QLabel();
		
		auto s = QString();
		s += QString::number(id);
		s += '\t';
		s += data;

		qlabel->setText(s);
		labels.push_back(qlabel);
		layout->addWidget(qlabel);
		auto se = QString();
		se += "Comparisions: ";
		auto num = QString::number(comparisions);
		se += num;
		ui.label_4->setText(se);
		ui.scrollAreaWidgetContents->setLayout(layout);
	} else {
		ui.label_4->setText("No such ID");
	}
}

void MainWindow::Display() {
	ui.label_4->setText("");

	for(const auto& label : labels) {
		layout->removeWidget(label);
		delete label;
	}
	
	labels.clear();

	
	auto recs = table.Display();
	for(const auto& r : recs) {
		auto qlabel = new QLabel();
		
		auto s = QString();
		s += QString::number(r.first);
		s += '\t';
		s += r.second;

		qlabel->setText(s);
		labels.push_back(qlabel);
		layout->addWidget(qlabel);
	}
	ui.scrollAreaWidgetContents->setLayout(layout);
}
