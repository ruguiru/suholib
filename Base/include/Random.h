#pragma once

#include <random>
#include <type_traits>

namespace suho {
namespace util
{
	namespace random
	{
		// 균등 분포
		inline int GetUniformInt(const int min, const int max)
		{
			std::random_device rd;
			std::mt19937 mt_engine(rd());
			std::uniform_int_distribution<int> range(min, max);
			return range(mt_engine);
		}

		inline double GetUniformReal(const double min, const double max)
		{
			std::random_device rd;
			std::mt19937 mt_engine(rd());
			std::uniform_real_distribution<double> range(min, max);
			return range(mt_engine);
		}

		// 정규분포 (평균, 표준편차)
		inline double GetNormal(const int average, const int deviation)
		{
			std::random_device rd;
			std::mt19937 mt_engine(rd());
			std::normal_distribution<> dist(average, deviation);
			return dist(mt_engine);
		}

		// 확률 지정
		inline bool GetTry(const double rate)
		{
			std::random_device rd;
			std::mt19937 mt_engine(rd());
			std::bernoulli_distribution result(rate);

			return result(mt_engine);
		}
		
		inline int GetMultiTry(const int try_cnt, const double rate)
		{
			std::random_device rd;
			std::mt19937 mt_engine(rd());
			std::binomial_distribution<> result(try_cnt, rate);

			return result(mt_engine);
		}
	};
}
}