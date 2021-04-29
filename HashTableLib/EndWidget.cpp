#include "EndWidget.h"

#include <QPushButton>
#include <QHBoxLayout>


EndWidget::EndWidget(QWidget *parent)
    : QWidget(parent)
    , _firstConclusionButton(nullptr)
    , _secondConclusionButton(nullptr)
    , _thirdConclusionButton(nullptr)
{
   constexpr int minumumLength = 500;
   _firstConclusionButton = new QPushButton("???");
   _firstConclusionButton->setStyleSheet("font: bold;background-color: grey;font-size: 60px");
   _firstConclusionButton->setMinimumSize(minumumLength, minumumLength);

   _secondConclusionButton = new QPushButton("???");
   _secondConclusionButton->setStyleSheet("font: bold;background-color: grey;font-size: 60px");
   _secondConclusionButton->setMinimumSize(minumumLength, minumumLength);

   _thirdConclusionButton = new QPushButton("???");
   _thirdConclusionButton->setStyleSheet("font: bold;background-color: grey;font-size: 60px");
   _thirdConclusionButton->setMinimumSize(minumumLength, minumumLength);

   constexpr int strechSize = 25;
   QHBoxLayout *mainLayout = new QHBoxLayout;
   mainLayout->addStretch(strechSize);
   mainLayout->addWidget(_firstConclusionButton);
   mainLayout->addStretch(strechSize);
   mainLayout->addWidget(_secondConclusionButton);
   mainLayout->addStretch(strechSize);
   mainLayout->addWidget(_thirdConclusionButton);
   mainLayout->addStretch(strechSize);

   setLayout(mainLayout);

   connect(_firstConclusionButton, &QPushButton::released, this, &EndWidget::firstConclusionHandler);
   connect(_secondConclusionButton, &QPushButton::released, this, &EndWidget::secondConclusionHandler);
   connect(_thirdConclusionButton, &QPushButton::released, this, &EndWidget::thirdConclusionHandler);
}


void EndWidget::firstConclusionHandler()
{
    static int counter = 0;
    static const QString initialMessage = "???";
    static const QString finalMessage = "UNORDERED\nELEMENTS";
    counter++;
    if (counter % 2 == 0)
    {
        _firstConclusionButton->setText(initialMessage);
        _firstConclusionButton->setStyleSheet("font: bold;background-color: grey;font-size: 60px");
    }
    else
    {
        _firstConclusionButton->setText(finalMessage);
        _firstConclusionButton->setStyleSheet("font: bold;background-color: yellow;font-size: 60px");
    }
}


void EndWidget::secondConclusionHandler()
{
    static int counter = 0;
    static const QString initialMessage = "???";
    static const QString finalMessage = "COOL\nHASH FUNCTION";
    counter++;
    if (counter % 2 == 0)
    {
        _secondConclusionButton->setText(initialMessage);
        _secondConclusionButton->setStyleSheet("font: bold;background-color: grey;font-size: 60px");
    }
    else
    {
        _secondConclusionButton->setText(finalMessage);
        _secondConclusionButton->setStyleSheet("font: bold;background-color: red;font-size: 60px");
    }
}


void EndWidget::thirdConclusionHandler()
{
    static int counter = 0;
    static const QString initialMessage = "???";
    static const QString jokeMessage = "NEVER\nUNDERSTIMATE\nA\nPRIME NUMBER";
    static const QString finalMessage = "MEASURE\nTIME AND\nMEMORY";
    counter++;
    if (counter % 3 == 0)
    {
        _thirdConclusionButton->setText(initialMessage);
        _thirdConclusionButton->setStyleSheet("font: bold;background-color: grey;font-size: 60px");
    }
    else if (counter % 3 == 1)
    {
        _thirdConclusionButton->setText(jokeMessage);
        _thirdConclusionButton->setStyleSheet("font: bold;background-color: blue;font-size: 60px");
    }
    else
    {
        _thirdConclusionButton->setText(finalMessage);
        _thirdConclusionButton->setStyleSheet("font: bold;background-color: green;font-size: 60px");
    }
}
