#include "calculator.h"

Calculator::Calculator(QWidget* parent) : QWidget(parent), isOperationClicked(false) {
    display = new QLineEdit("0");
    display->setReadOnly(true); // Только чтение
    display->setAlignment(Qt::AlignRight);  // Выравнивание текста (с правого края)

    QGridLayout* layout = new QGridLayout;
    // display - добавляемый виджет, 0 и 0 - номера строки и столбца в сетке
    // 1 и 4 - занимаемое количество строк и столбцов
    layout->addWidget(display, 0, 0, 1, 4);

    createButtons(layout);

    setLayout(layout); // Устанавливаем сетку с кнопками для окна калькулятора
    setWindowTitle("Qt Calculator");
    setWindowIcon(QIcon("CalculatorIcon.png"));
    setMinimumSize(300, 400);
}

void Calculator::createButtons(QGridLayout* layout) {
    const QStringList buttonLables = {
        "√", "x²", "C", "⌫",
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "0", ".", "=", "+"
    };

    for (int i = 0; i < buttonLables.size(); ++i) {
        // Получаем кнопку, которая отрпавила сигнал (вызвала текущий слот)
        QPushButton* button = new QPushButton(buttonLables[i]);
        connect(button, &QPushButton::clicked, this, [this, button]() {
            if (button->text() == "=") {
                onEqualsClicked();
            } else if (button->text() == "C") {
                onClearClicked();
            } else if (button->text() == "+" || button->text() == "-" ||
                       button->text() == "*" || button->text() == "/") {
                onOperationClicked();
            } else if (button->text() == "√") {
                onSquareRootClicked();
            } else if (button->text() == "x²") {
                onSquareClicked();
            } else if (button->text() == "⌫") {
                onBackspaceClicked();
            } else {
                onDigitClicked();
            }
        });

        // button - добавляемый виджет,
        // 1 + i / 4 - номер строки, i % 4 - номер столбца
        layout->addWidget(button, 1 + i / 4, i % 4);
    }
}

void Calculator::onDigitClicked() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString digit = button->text();

    // Если нажата кнопка "."
    if (digit == ".") {
        // Проверяем, есть ли уже точка в текущем числе
        if (!display->text().contains(".")) {
            display->setText(display->text() + ".");
        }
    } else if (digit == "-") {  // Если нажата кнопка "-"
        QString currentText = display->text();

        // Если текст пуст или равено нулю, то добавляем минус
        if (currentText == "0" || currentText.isEmpty()) {
            display->setText("-");
        } else if (currentText.startsWith("-")) {    // Если минус уже есть, то удаляем его
            display->setText(currentText.mid(1));   // Убираем первый символ (минус)
        } else {    // Если минуса нет, добавляем его
            display->setText("-" + currentText);
        }
    } else {    // Если нажата цифра
        // Если в поле ввода 0 или была выбрана операция (+, -, *, /)
        if (display->text() == "0" || isOperationClicked) {
            display->setText(digit); // Сбрасываем текст
            isOperationClicked = false;
        } else {    // Если в поле уже есть цифры или не нажата кнопка операции
            display->setText(display->text() + digit); // Добавляем цифру к текущему тексту
        }
    }
}

void Calculator::onOperationClicked() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    firstOperand = display->text().toDouble();
    currentOperation = button->text();
    isOperationClicked = true;
}

void Calculator::onEqualsClicked() {
    double secondOperand = display->text().toDouble();
    double result = 0;

    if (currentOperation == "+") {
        result = firstOperand + secondOperand;
    } else if (currentOperation == "-") {
        result = firstOperand - secondOperand;
    } else if (currentOperation == "/") {
        if (secondOperand == 0) {
            QMessageBox::warning(this, "Error", "Division by zero is prohibited");
            return;
        }
        result = firstOperand / secondOperand;
    } else if (currentOperation == "*") {
        result = firstOperand * secondOperand;
    }

    display->setText(QString::number(result));  // Преобразуем число в строку
    isOperationClicked = true;
}

void Calculator::onClearClicked() {
    display->setText("0");
    firstOperand = 0;
    currentOperation = "";
    isOperationClicked = false;
}

void Calculator::onSquareRootClicked() {
    double number = display->text().toDouble();
    if (number < 0) {
        QMessageBox::warning(this, "Error", "Square root of a negative number is not defined");
        return;
    }

    double result = std::sqrt(number);
    display->setText(QString::number(result));
}

void Calculator::onSquareClicked() {
    double number = display->text().toDouble();
    double result = std::pow(number, 2);
    display->setText(QString::number(result));
}

void Calculator::onBackspaceClicked() {
    QString text = display->text();
    if (text.length() > 1) {
        text.chop(1);   // Удаляем последний символ
    } else {
        text = "0"; // Если строка пустая, то устанавливаем ноль
    }

    display->setText(text);
}
