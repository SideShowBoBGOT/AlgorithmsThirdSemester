#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "TData.h"

class MainWindow : public QMainWindow {
	Q_OBJECT

	public:
	MainWindow(QWidget *parent = nullptr);
	virtual ~MainWindow();

	protected slots:
	virtual void Update();
	virtual void Delete();
	virtual void Search();
	virtual void Display();
	virtual void Clear();
	virtual void Generate();

	protected:
	std::vector<QLabel*> labels;
	QVBoxLayout* layout = new QVBoxLayout(this);
	protected:
	TData table;
	Ui::MainWindowClass ui;
};
