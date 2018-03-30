#pragma once

#include "LogHelper.h"

// ����� �α� Ÿ�� ���Ƴְ� �ʿ��� ������ include
BUILD_LOGTYPE(ClientLog, "C:/Log", cycle::DAILY)
#define ClientLog ClientLog::GetInstance()->Write

BUILD_LOGTYPE(TimeServerLog, "C:/Log", cycle::DAILY)
#define TimeServerLog TimeServerLog::GetInstance()->Write

BUILD_LOGTYPE(StatisticLog, "C:/Log", cycle::DAILY)
#define StatisticLog StatisticLog::GetInstance()->Write