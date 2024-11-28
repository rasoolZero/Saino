#include "common.h"

const SPL::infomap &SPL::getInfo()
{
    using namespace SPL;
    static infomap allInfo;
    if (allInfo.size())
        return allInfo;

    allInfo[OIL_PRESSURE] = {false, "OIL PRESSURE", 0, 1000};
    allInfo[OIL_TEMPERATURE] = {false, "OIL TEMPERATURE", 0, 400};
    allInfo[FUEL_FLOW] = {false, "FUEL FLOW", 0, 800};
    allInfo[FUEL] = {false, "FUEL", 0, 800};
    allInfo[EGT] = {false, "EGT", 0, 400};
    allInfo[TORQUE] = {false, "TORQUE", 0, 400};
    allInfo[INDICATED_POWER] = {false, "INDICATED POWER", 0, 400};
    allInfo[FRICTIONAL_POWER] = {false, "Frictional Power", 0, 400};
    allInfo[THERMAL_EFFICIENCY] = {false, "Thermal efficiency", 0, 100};
    allInfo[AIR_FUEL_RATIO] = {false, "Air-Fuel ratio", 0, 20};
    allInfo[MOTOR_SPEED] = {false, "MOTOR SPEED", 0, 1000};
    allInfo[OUTPUT_AIR_SPEED] = {false, "OUTPUT AIR SPEED", 0, 1000};
    allInfo[VIBRATION] = {false, "VIBRATION", 0, 100};
    allInfo[BODY_TEMP] = {false, "BODY TEMP", 0, 400};
    allInfo[AIR_TEMP] = {false, "AIR TEMP", 0, 400};

    allInfo[OIL_PRESSURE_SENSOR_ERROR] = {true, "OIL PRESSURE SENSOR ERROR", 0, 1};
    allInfo[OIL_TEMPERATURE_SENSOR_ERROR] = {true, "OIL TEMPERATURE SENSOR ERROR", 0, 1};
    allInfo[FUEL_FLOW_SENSOR_ERROR] = {true, "FUEL FLOW SENSOR ERROR", 0, 1};
    allInfo[FUEL_SENSOR_ERROR] = {true, "FUEL SENSOR ERROR", 0, 1};
    allInfo[EGT_SENSOR_ERROR] = {true, "EGT SENSOR ERROR", 0, 1};
    allInfo[TORQUE_SENSOR_ERROR] = {true, "TORQUE SENSOR ERROR", 0, 1};
    allInfo[INDICATED_POWER_SENSOR_ERROR] = {true, "INDICATED POWER SENSOR ERROR", 0, 1};
    allInfo[FRICTIONAL_POWER_SENSOR_ERROR] = {true, "Frictional Power SENSOR ERROR", 0, 1};
    allInfo[THERMAL_EFFICIENCY_SENSOR_ERROR] = {true, "Thermal efficiency SENSOR ERROR", 0, 1};
    allInfo[AIR_FUEL_RATIO_SENSOR_ERROR] = {true, "Air-Fuel ratio SENSOR ERROR", 0, 1};
    allInfo[MOTOR_SPEED_SENSOR_ERROR] = {true, "MOTOR SPEED SENSOR ERROR", 0, 1};
    allInfo[OUTPUT_AIR_SPEED_SENSOR_ERROR] = {true, "OUTPUT AIR SPEED SENSOR ERROR", 0, 1};
    allInfo[VIBRATION_SENSOR_ERROR] = {true, "VIBRATION SENSOR ERROR", 0, 1};
    allInfo[BODY_TEMP_SENSOR_ERROR] = {true, "BODY TEMP SENSOR ERROR", 0, 1};
    allInfo[AIR_TEMP_SENSOR_ERROR] = {true, "AIR TEMP SENSOR ERROR", 0, 1};

    return allInfo;
}

QVector<SPL::DataID> SPL::allErrorCodes()
{
    QVector<DataID> result;
    for (DataID i = DataID::OIL_PRESSURE_SENSOR_ERROR; i <= DataID::AIR_TEMP_SENSOR_ERROR;
         i = static_cast<DataID>(static_cast<int>(i) + 1))
        result.push_back(i);
    return result;
}

QVector<SPL::DataID> SPL::allDataCodes()
{
    QVector<DataID> result;
    for (DataID i = DataID::OIL_PRESSURE; i <= DataID::AIR_TEMP;
         i = static_cast<DataID>(static_cast<int>(i) + 1))
        result.push_back(i);
    return result;
}
