#pragma once

#include "LogHelper.h"

// ����� �α� Ÿ�� ���Ƴְ� �ʿ��� ������ include
BUILD_LOGTYPE(ClientLog, "C:/SuholibLog", cycle::FileCreateCycle::DAILY)
#define ClientLog ClientLog::GetInstance()->Write

BUILD_LOGTYPE(TimeServerLog, "C:/SuholibLog", cycle::FileCreateCycle::DAILY)
#define TimeServerLog TimeServerLog::GetInstance()->Write

BUILD_LOGTYPE(StatisticLog, "C:/SuholibLog", cycle::FileCreateCycle::HOURLY)
#define StatisticLog StatisticLog::GetInstance()->Write