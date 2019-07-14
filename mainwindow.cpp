#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableWidgetItem>
#include <QFileDialog>
#include <fstream>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(2);
    QStringList header;
    header << "key" << "value";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->verticalHeader()->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addRow(const std::string& keyStr, const std::string& valStr)
{
    auto rows = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rows);

    auto key = new QTableWidgetItem();
    ui->tableWidget->setItem(rows, 0, key);
    key->setText(QString::fromStdString(keyStr));

    auto val = new QTableWidgetItem();
    ui->tableWidget->setItem(rows, 1, val);
    val->setText(QString::fromStdString(valStr));
}

void MainWindow::on_addButton_clicked()
{
    addRow("", "");
}

void MainWindow::on_saveButton_clicked()
{
    QString txtFilter("Text files (*.txt)");
    auto filename = QFileDialog::getSaveFileName(0, "Save file", QDir::currentPath(),
                                                 txtFilter, &txtFilter);
    if (!filename.isEmpty())
    {
        std::fstream fs;
        fs.open (filename.toStdString(), std::fstream::out);
        auto rows = ui->tableWidget->rowCount();
        for (int i=0; i < rows; ++i)
        {
            fs << ui->tableWidget->item(i, 0)->text().toStdString() << std::endl;
            fs << ui->tableWidget->item(i, 1)->text().toStdString() << std::endl;
        }
        fs.close();
    }
}

void MainWindow::on_loadButton_clicked()
{
    QString txtFilter("Text files (*.txt)");
    auto filename = QFileDialog::getOpenFileName(0, "Load file", QDir::currentPath(),
                                                 txtFilter, &txtFilter);
    if (!filename.isEmpty())
    {
        //clear contents
        ui->tableWidget->setRowCount(0);

        std::fstream fs;
        fs.open (filename.toStdString(), std::fstream::in);

        bool isReadyToAdd = false;
        std::string prior;
        std::string line;
        while (std::getline(fs, line))
        {
            if (isReadyToAdd)
            {
                addRow(prior, line);
            }
            isReadyToAdd = !isReadyToAdd;
            prior = line;
        }

        fs.close();
    }
}
