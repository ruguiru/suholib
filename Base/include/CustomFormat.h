#pragma once

#include "Format.h"
#include "Common.h"

namespace suho
{
	namespace datetime
	{
		namespace format
		{
			BUILDFORMAT(CustomFormat1,
				[] ( const std::string& date ) -> decltype( std::tuple<int, int, int, int, int, int>() )
				{
					int year = 0;
					int month = 0;
					int day = 0;
					int hour = 0;
					int minute = 0;
					int second = 0;

					if ( date.size() < 27 )
						return std::tie( year, month, day, hour, minute, second );

					year = static_cast<int>( atoi( date.substr( 0, 4 ).c_str() ) );
					month = static_cast<int>( atoi( date.substr( 6, 2 ).c_str() ) );
					day = static_cast<int>( atoi( date.substr( 10, 2 ).c_str() ) );
					hour = static_cast<int>( atoi( date.substr( 16, 2 ).c_str() ) );
					minute = static_cast<int>( atoi( date.substr( 20, 2 ).c_str() ) );
					second = static_cast<int>( atoi( date.substr( 24, 2 ).c_str() ) );

					return std::tie( year, month, day, hour, minute, second );
				},
				[] ( int year, int month, int day, int hour, int minute, int second ) -> decltype( std::string() )
				{
					char date[ 32 ] = {};

					sprintf_s( date, "%04dy %02dm %02dy - %02dh %02dm %02ds", year, month, day, hour, minute, second );

					return date;
				} )	// end BUILDFORMAT
		}
	}   // end namespace datetime 
}   // end namespace suho