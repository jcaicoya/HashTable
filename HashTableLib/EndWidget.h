#pragma once

#include <QWidget>
#include <QStringList>

class QPushButton;



class EndWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EndWidget(QWidget *parent = nullptr);

private slots:
    void firstConclusionHandler();
    void secondConclusionHandler();
    void thirdConclusionHandler();

private:
    QPushButton *_firstConclusionButton;
    QPushButton *_secondConclusionButton;
    QPushButton *_thirdConclusionButton;

    QStringList _conclusionList;
};
