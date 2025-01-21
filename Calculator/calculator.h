#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>

class Calculator : public QWidget
{
    Q_OBJECT
public:
    Calculator(QWidget* parent = nullptr);

private slots:
    void onDigitClicked();
    void onOperationClicked();
    void onEqualsClicked();
    void onClearClicked();

private:
    QLineEdit* display;
    QString currentOperation;
    double firstOperand;
    bool isOperationClicked;

    void createButtons(QGridLayout* layout);
};

#endif // CALCULATOR_H
