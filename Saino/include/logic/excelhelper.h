#ifndef EXCELHELPER_H
#define EXCELHELPER_H

#include <QException>
#include <QString>
#include "xlsxdocument.h"

class EmptyStorage : public QException
{
    // QException interface
public:
    void raise() const override { throw *this; }
    QException *clone() const override { return new EmptyStorage(*this); }
};

// used to export saved data in storage into an excel file
class ExcelHelper
{
public:
    ExcelHelper(const ExcelHelper &other) = delete;
    ExcelHelper &operator=(const ExcelHelper &other) = delete;

    // if data storage is empty, throws EmptyStorage exception
    ExcelHelper();
    bool save(const QString &fileName);

private:
    QXlsx::Document xlsx;
    int col;

    // used to store which column is for which data ID
    QMap<QString, int> columns;
};

#endif // EXCELHELPER_H
