#ifndef PASSWORDQUERY_H
#define PASSWORDQUERY_H

#include <QDialog>

namespace Ui {
class PasswordQuery;
}

class PasswordQuery : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordQuery(QList<QStringList>, QWidget *parent = nullptr);
    QList<QStringList> *trainingSet;
    ~PasswordQuery();

private slots:
    void on_verifyButton_clicked();
    void updateUI();

private:
    Ui::PasswordQuery *ui;
    int index;
};

#endif // PASSWORDQUERY_H
