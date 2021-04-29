#include "BeginWidget.h"

#include <QLabel>
#include <QPushButton>
#include <QGridLayout>


BeginWidget::BeginWidget(const QString &initalText, QWidget *parent)
    : QWidget(parent)
    , _textLabel(nullptr)
    , _backwardButton(nullptr)
    , _forwardButton(nullptr)
    , _textList()
    , _currentTextIndex(0)
{
    _textList.append(initalText);

    _textLabel = new QLabel(initalText);
    _textLabel->setAlignment(Qt::AlignCenter);
    updateText();

    _backwardButton = new QPushButton("<<");
    _forwardButton = new QPushButton(">>");
    updateButtonStates();

    connect(_backwardButton, &QPushButton::released, this, &BeginWidget::backwardButtonHandler);
    connect(_forwardButton, &QPushButton::released, this, &BeginWidget::forwardButtonHandler);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(_textLabel, 0, 0, 1, 2, Qt::AlignCenter);
    mainLayout->addWidget(_backwardButton, 1, 0);
    mainLayout->addWidget(_forwardButton, 1, 1);

    setLayout(mainLayout);

    addText("PERFORMANCE MATTERS");
    addText("HASH TABLES\nSEARCH TREES\nMIND THE CACHE\nPREFETCHING\nOPTIMIZATIONS");
    addText("HASH TABLES");
}


void BeginWidget::backwardButtonHandler()
{
    if (_currentTextIndex == 0)
    {
        return;
    }

    _currentTextIndex--;
    updateText();
    updateButtonStates();
}


void BeginWidget::forwardButtonHandler()
{
    if (_currentTextIndex >= _textList.size())
    {
        return;
    }

    _currentTextIndex++;
    updateText();
    updateButtonStates();
}


void BeginWidget::addText(const QString &newText)
{
    _textList.append(newText);
    updateButtonStates();
}


void BeginWidget::updateButtonStates()
{
    _backwardButton->setEnabled(_currentTextIndex > 0);
    _forwardButton->setEnabled(_currentTextIndex < _textList.size() - 1);
}


void BeginWidget::updateText()
{
    if (_currentTextIndex == 0)
    {
        _textLabel->setFont(buildTitleFont());
    }
    else
    {
        _textLabel->setFont(buildNormalFont());
    }

    _textLabel->setText(_textList.at(_currentTextIndex));
}



QFont BeginWidget::buildTitleFont() const
{
    QFont labelFont;
    labelFont.setBold(true);
    labelFont.setPointSize(150);
    return labelFont;
}


QFont BeginWidget::buildNormalFont() const
{
    QFont labelFont;
    labelFont.setBold(true);
    labelFont.setPointSize(100);
    return labelFont;
}
