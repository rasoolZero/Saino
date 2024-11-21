#include "excelhelper.h"
#include "datastorage.h"
#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;


ExcelHelper::ExcelHelper()
{
    const auto & allPackets = DataStorage::getInstance().getPackets();
    if(allPackets.size() == 0)
        EmptyStorage().raise();
    const auto & allInfo = DataStorage::getInstance().getInfo();
    const auto & allErrorCodes = DataStorage::getInstance().allErrorCodes();
    const auto & allDataCodes = DataStorage::getInstance().allDataCodes();
    col = 1;
    xlsx.write(1, col++, "Message Counter");
    foreach(auto code,allDataCodes){
        auto name = allInfo[code].name;
        xlsx.write(1,col,name);
        columns[name] = col++;
    }

    foreach(auto code,allErrorCodes){
        auto name = allInfo[code].name;
        xlsx.write(1,col,name);
        columns[name] = col++;
    }
}

void ExcelHelper::save(const QString &filename)
{
    const auto & allPackets = DataStorage::getInstance().getPackets();
    const auto & allInfo = DataStorage::getInstance().getInfo();


    int row = 2;
    foreach(auto packet,allPackets){
        auto msgCounter = packet.getMsgCounter();
        const auto & allData = packet.getAllPackets();
        xlsx.write(row,1,msgCounter);
        foreach(auto packetData,allData){
            auto name = allInfo[packetData.getId()].name;
            auto value = packetData.getValue();
            auto col = columns[name];
            xlsx.write(row,col,value);
        }
        row++;
    }
    xlsx.autosizeColumnWidth(1,col);
    xlsx.saveAs(filename);
}

