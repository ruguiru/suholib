#pragma once

#include "LogHelper.h"

// ����� �α� Ÿ�� ���Ƴְ� �ʿ��� ������ include
BUILD_LOGTYPE(ClientLog, "C:/SuholibLog", cycle::DAILY)
#define ClientLog ClientLog::GetInstance()->Write

BUILD_LOGTYPE(TimeServerLog, "C:/SuholibLog", cycle::DAILY)
#define TimeServerLog TimeServerLog::GetInstance()->Write

BUILD_LOGTYPE(StatisticLog, "C:/SuholibLog", cycle::HOURLY)
#define StatisticLog StatisticLog::GetInstance()->Write