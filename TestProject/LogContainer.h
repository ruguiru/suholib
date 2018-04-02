#pragma once

#include "LogHelper.h"

// 여기다 로그 타입 몰아넣고 필요한 곳에서 include
BUILD_LOGTYPE(ClientLog, "C:/SuholibLog", cycle::DAILY)
#define ClientLog ClientLog::GetInstance()->Write

BUILD_LOGTYPE(TimeServerLog, "C:/SuholibLog", cycle::DAILY)
#define TimeServerLog TimeServerLog::GetInstance()->Write

BUILD_LOGTYPE(StatisticLog, "C:/SuholibLog", cycle::HOURLY)
#define StatisticLog StatisticLog::GetInstance()->Write