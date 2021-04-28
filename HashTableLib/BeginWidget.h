#pragma once

#include <QWidget>
#include <QStringList>
#include <QFont>

class QLabel;
class QPushButton;


class BeginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BeginWidget(const QString &initalText, QWidget *parent = nullptr);

private slots:
    void backwardButtonHandler();
    void forwardButtonHandler();

private:
    QLabel *_textLabel;
    QPushButton *_backwardButton;
    QPushButton *_forwardButton;
    QStringList _textList;
    int _currentTextIndex;

    void addText(const QString &newText);
    void updateButtonStates();
    void updateText();

    QFont buildTitleFont() const;
    QFont buildNormalFont() const;
};

