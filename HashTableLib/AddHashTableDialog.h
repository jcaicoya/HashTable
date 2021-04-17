#pragma once

#include <QDialog>
#include <QList>

class HashTableDefinition;
class QRadioButton;
class QLineEdit;
class QIntValidator;


class AddHashTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddHashTableDialog(QWidget *parent = nullptr);

private slots:
    void verify();
    void updateValidator();

signals:
    void addHashTable(const HashTableDefinition &description);

private:
    QList<QRadioButton *> _hashButtonList;
    QList<QRadioButton *> _rehashButtonList;
    QLineEdit *_sizeLineEdit;
    QIntValidator *_sizeValidator;

};

