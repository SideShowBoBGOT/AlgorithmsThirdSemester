/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *updateButton;
    QPushButton *searchButton;
    QPushButton *deleteButton;
    QPushButton *generateButton;
    QPushButton *clearButton;
    QLineEdit *idEdit;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *dataEdit;
    QLabel *label_3;
    QLabel *label_4;
    QScrollArea *records;
    QWidget *scrollAreaWidgetContents;
    QPushButton *displayButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName("MainWindowClass");
        MainWindowClass->resize(331, 443);
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName("centralWidget");
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(220, 20, 101, 141));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        updateButton = new QPushButton(verticalLayoutWidget);
        updateButton->setObjectName("updateButton");

        verticalLayout->addWidget(updateButton);

        searchButton = new QPushButton(verticalLayoutWidget);
        searchButton->setObjectName("searchButton");

        verticalLayout->addWidget(searchButton);

        deleteButton = new QPushButton(verticalLayoutWidget);
        deleteButton->setObjectName("deleteButton");

        verticalLayout->addWidget(deleteButton);

        generateButton = new QPushButton(verticalLayoutWidget);
        generateButton->setObjectName("generateButton");

        verticalLayout->addWidget(generateButton);

        clearButton = new QPushButton(verticalLayoutWidget);
        clearButton->setObjectName("clearButton");

        verticalLayout->addWidget(clearButton);

        idEdit = new QLineEdit(centralWidget);
        idEdit->setObjectName("idEdit");
        idEdit->setGeometry(QRect(60, 20, 151, 21));
        label = new QLabel(centralWidget);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 20, 49, 16));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 50, 49, 16));
        dataEdit = new QLineEdit(centralWidget);
        dataEdit->setObjectName("dataEdit");
        dataEdit->setGeometry(QRect(60, 50, 151, 21));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 80, 51, 31));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(80, 80, 121, 31));
        records = new QScrollArea(centralWidget);
        records->setObjectName("records");
        records->setGeometry(QRect(19, 189, 301, 191));
        records->setWidgetResizable(true);
        records->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 299, 189));
        records->setWidget(scrollAreaWidgetContents);
        displayButton = new QPushButton(centralWidget);
        displayButton->setObjectName("displayButton");
        displayButton->setGeometry(QRect(20, 120, 99, 24));
        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 331, 22));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName("mainToolBar");
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName("statusBar");
        MainWindowClass->setStatusBar(statusBar);
#if QT_CONFIG(shortcut)
        label->setBuddy(idEdit);
        label_2->setBuddy(dataEdit);
#endif // QT_CONFIG(shortcut)

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QCoreApplication::translate("MainWindowClass", "MainWindow", nullptr));
        updateButton->setText(QCoreApplication::translate("MainWindowClass", "Update", nullptr));
        searchButton->setText(QCoreApplication::translate("MainWindowClass", "Search", nullptr));
        deleteButton->setText(QCoreApplication::translate("MainWindowClass", "Delete", nullptr));
        generateButton->setText(QCoreApplication::translate("MainWindowClass", "Generate", nullptr));
        clearButton->setText(QCoreApplication::translate("MainWindowClass", "Clear", nullptr));
        label->setText(QCoreApplication::translate("MainWindowClass", "ID:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindowClass", "DATA:", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindowClass", "Message:", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindowClass", "TextLabel", nullptr));
        displayButton->setText(QCoreApplication::translate("MainWindowClass", "Display", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
