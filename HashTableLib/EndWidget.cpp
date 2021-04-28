#include "EndWidget.h"

#include <QPushButton>
#include <QHBoxLayout>


EndWidget::EndWidget(QWidget *parent)
    : QWidget(parent)
    , _firstConclusionButton(nullptr)
    , _secondConclusionButton(nullptr)
    , _thirdConclusionButton(nullptr)
    , _conclusionList()
{
    _conclusionList.append("firstConclusion");
    _conclusionList.append("secondConclusion");
    _conclusionList.append("jokeConclusion");
    _conclusionList.append("thirdConclusion");

   _firstConclusionButton = new QPushButton("???");
   _firstConclusionButton->setStyleSheet("font: bold;background-color: red;font-size: 36px;height: 48px;width: 120px;");

   _secondConclusionButton = new QPushButton("???");
   _thirdConclusionButton = new QPushButton("???");

   constexpr int strechSize = 100;
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
    static const QString finalMessage = _conclusionList.at(0);
    counter++;
    if (counter % 2 == 0)
    {
        _firstConclusionButton->setText(initialMessage);
    }
    else
    {
        _firstConclusionButton->setText(finalMessage);
    }
}


void EndWidget::secondConclusionHandler()
{
    static int counter = 0;
    static const QString initialMessage = "???";
    static const QString finalMessage = _conclusionList.at(1);
    counter++;
    if (counter % 2 == 0)
    {
        _secondConclusionButton->setText(initialMessage);
    }
    else
    {
        _secondConclusionButton->setText(finalMessage);
    }
}


void EndWidget::thirdConclusionHandler()
{
    static int counter = 0;
    static const QString initialMessage = "???";
    static const QString jokeMessage = _conclusionList.at(2);
    static const QString finalMessage = _conclusionList.at(3);
    counter++;
    if (counter % 3 == 0)
    {
        _thirdConclusionButton->setText(initialMessage);
    }
    else if (counter % 3 == 1)
    {
        _thirdConclusionButton->setText(jokeMessage);
    }
    else
    {
        _thirdConclusionButton->setText(finalMessage);
    }
}
