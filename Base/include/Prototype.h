#pragma once

namespace suho
{
	namespace pattern
	{

		template<typename T>
		class ProtoType
		{
		public:
			ProtoType() {}
			virtual ~ProtoType() {}

			virtual T* Clone() = 0;
		};
	}   // end namespace pattern
}   // end namespace suho