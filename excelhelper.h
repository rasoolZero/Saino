#ifndef EXCELHELPER_H
#define EXCELHELPER_H

#include <QString>
#include <QException>
#include "xlsxdocument.h"

class EmptyStorage : public QException{
    // QException interface
public:
    void raise() const override
    {
        throw *this;
    }
    QException *clone() const override
    {
        return new EmptyStorage(*this);
    }
};

class ExcelHelper
{
public:
    ExcelHelper(const ExcelHelper& other) = delete;
    ExcelHelper& operator=(const ExcelHelper& other) = delete;

    ExcelHelper();
    void save(const QString& fileName);
private:
    QXlsx::Document xlsx;
    int col;
    QMap<QString,int> columns;
};

#endif // EXCELHELPER_H
