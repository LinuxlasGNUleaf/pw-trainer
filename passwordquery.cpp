#include "passwordquery.h"
#include "ui_passwordquery.h"
#include "sha256.h"
#include <iostream>

#include "QString"

PasswordQuery::PasswordQuery(QList<QStringList> set, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordQuery)
{
    ui->setupUi(this);
    std::cout << "";
    trainingSet = &set;
    index = 0;
    updateUI();
}

PasswordQuery::~PasswordQuery()
{
    delete ui;
}

void PasswordQuery::on_verifyButton_clicked()
{
    if (sha256(ui->passwordInput->text().toStdString()) == trainingSet->at(index)[1]){
        ui->statusLabel->setText(QString("Input for \"%0\" valid!").arg(trainingSet->at(index)[0]));
        index++;
        if (index >= trainingSet->length())
        {
            this->close();
            return;
        }
    } else {
        ui->statusLabel->setText(QString("Input for \"%0\" **not** valid!").arg(trainingSet->at(index)[0]));
    }
    ui->passwordInput->setText("");
    updateUI();
}

void PasswordQuery::updateUI(){
    QStringList temp = trainingSet->at(index);
    ui->pwLabel->setText(temp[0]);
    ui->indexLabel->setText(QString("%0 of %1").arg(QString::number(index+1), QString::number(trainingSet->length())));
}

