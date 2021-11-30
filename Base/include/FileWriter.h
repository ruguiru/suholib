#pragma once

#include <iostream>
#include <fstream>

#include "NonCopyable.h"

namespace suho
{
	namespace file
	{
		const struct CloseType {} close;
		const struct ClearType {} clear;

		class FileWriter : public suho::idioms::NonCopyable
		{
		public:
			FileWriter();
			FileWriter( const std::string& filename );
			FileWriter( const std::string& filename, const std::string& path );
			FileWriter( const std::string& filename, const std::string& path, unsigned int openmode );
			~FileWriter();

			bool Open( const std::string& filename, const std::string& path = ".\\", unsigned int openmode = std::ios::app );
			void Close();

			template<typename T>
			inline FileWriter& operator<<( const T& data )
			{
				_outstream << data;
				return *this;
			}

			inline void operator<<( const CloseType& )
			{
				_outstream.close();
			}

			inline void operator<<( const ClearType& )
			{
				_outstream.clear();
			}

		private:
			std::ofstream           _outstream;
		};
	}   // end file
}   // end namespace suho