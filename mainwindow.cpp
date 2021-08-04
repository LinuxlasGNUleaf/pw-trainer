#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "passwordquery.h"
#include "sha256.h"

#include "QFile"
#include "QTextStream"
#include "QVector"
#include "QDebug"
#include "QCloseEvent"
#include "QMessageBox"
#include "QApplication"
#include "QStandardItem"
#include "QAbstractItemModel"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    read_hashes_from_file();
    if (ui->passwordList->model()->rowCount() > 0)
        ui->passwordList->selectRow(0);
}

MainWindow::~MainWindow()
{
    write_hashes_to_file();
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    ui->newPWLabel->setText("");
    if(ui->labelInput->text().isEmpty()){
        ui->newPWLabel->setText("Please enter a password label!");
        return;
    }
    if(ui->pwInput->text().isEmpty()  || ui->pwInputRepeat->text().isEmpty()){
        ui->newPWLabel->setText("Please enter a password and repeat it!");
        return;
    }
    if(ui->pwInput->text() != ui->pwInputRepeat->text()){
        ui->newPWLabel->setText("Passwords do not match!");
        return;
    }
    if(ui->pwInput->text() == ui->labelInput->text()){
        ui->newPWLabel->setText("LABEL SHOULD NOT BE THE PASSWORD, DUMMY! IT'S SAVED AS CLEAR TEXT!");
        ui->labelInput->setText("");
        return;
    }

    int ind;
    if (ui->passwordList->selectionModel()->selectedRows().isEmpty()){
        ind = model->rowCount();
    } else {
        ind = ui->passwordList->selectionModel()->selectedRows().at(0).row();
    }
    model->insertRow(ind);

    QStandardItem *label = new QStandardItem(ui->labelInput->text());
    QStandardItem *hash = new QStandardItem(sha256(ui->pwInput->text().toStdString()));
    model->setItem(ind,0,label);
    model->setItem(ind,1,hash);

    ui->passwordList->setModel(model);
}

void MainWindow::read_hashes_from_file()
{
    qDebug() << "Reading from hashes.txt!\n";
    QFile file("hashes.txt");
    QVector<QStringList> hashes;
    if (!file.exists()){
        qDebug() << "hashes.txt not found, creating empty file.\n";
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.close();
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        std::cout << "File could not be read!\n";
        exit(1);
    }

    QTextStream in(&file);
    while (!in.atEnd())
        hashes.append(in.readLine().split(','));
    file.close();

    model = new QStandardItemModel(hashes.length(), 2, this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Label")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Hash")));

    for (int i = 0; i < hashes.length(); i++){
        QStandardItem *label = new QStandardItem(hashes[i][0]);
        QStandardItem *hash = new QStandardItem(hashes[i][1]);
        model->setItem(i,0,label);
        model->setItem(i,1,hash);
    }
    ui->passwordList->setModel(model);
}

void MainWindow::on_deleteButton_clicked()
{
    QModelIndexList selection = ui->passwordList->selectionModel()->selectedRows();
    if (selection.isEmpty())
        return;
    int selection_ind = selection.at(0).row();
    model->removeRow(selection_ind);
    ui->passwordList->setModel(model);
    ui->passwordList->selectRow(std::min(ui->passwordList->model()->rowCount(),std::max(0,selection_ind)));
}

void MainWindow::write_hashes_to_file()
{
    qDebug() << "Writing to hashes.txt!\n";
    QFile file("hashes.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        std::cout << "File could not be written to!\n";
        exit(1);
    }

    QTextStream out(&file);
    for (int i = 0; i < model->rowCount(); i++){
        out << model->index(i,0).data().toString() << "," << model->index(i,1).data().toString() << "\n";
    }
    file.close();
}

void MainWindow::on_trainingButton_clicked()
{
    QList<QStringList> *trainingSet = new QList<QStringList>;
    for (int i = 0; i < model->rowCount(); i++){
        trainingSet->append({model->index(i,0).data().toString(),model->index(i,1).data().toString()});
    }
    PasswordQuery training(*trainingSet);
    training.setModal(true);
    training.exec();
}

