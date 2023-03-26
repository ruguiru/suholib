#pragma once

#include <vector>
#include <queue>
#include <atomic>
#include <mutex>
#include <type_traits>

namespace suho
{
	namespace pool
	{
		// 내부에서 생성
		// INDEXING true 이면 user 오브젝트에 int 형 메모리 추가되고 풀에서 생성된 인덱스 값을 가져올 수 있다
		template<typename T, bool INDEXING = false>
		class ObjectPool
		{
		public:
			class IndexingT : public T
			{
				int GetIndex() const { return _index; }
				void SetIndex( int index ) { _index = index; }

				int _index = 0;

				friend class ObjectPool;
			};

			template<typename T, bool> struct TypeSelect;
			template<typename T> struct TypeSelect<T, true>
			{
				typedef IndexingT ResultType;
			};

			template<typename T> struct TypeSelect<T, false>
			{
				typedef T ResultType;
			};

			typedef typename T											value_Type;
			typedef typename ObjectPool<T, INDEXING>					this_type;

			typedef typename TypeSelect<T, INDEXING>::ResultType		ObjType;

		public:
			ObjectPool();
			~ObjectPool();

			void Create( int size );

			T* Borrow();
			void Return( T* obj );
			int GetIndex( T* t );				// INDEXING true 인 경우만

		private:
			void Free( ObjType* startptr );

			ObjectPool( const this_type& ) = delete;
			this_type& operator=( const this_type& ) = delete;

		private:
			std::queue<ObjType*>	_pool;
			std::vector<ObjType*>	_array_ptrs;			// Create 로 생성한 각 배열의 시작포인터(delete[] 용)

			int						_expandsize = 0;		// 메모리 부족시 확장 시킬 오브젝트 수
			std::recursive_mutex	_mutex;
			std::atomic<int>		_index = 0;
		};

		template<typename T, bool INDEXING>
		inline ObjectPool<T, INDEXING>::ObjectPool()
		{
			static_assert( !std::is_pointer<T>::value, "Pointer type forbidden!" );

			_array_ptrs.reserve( 10 );
		}

		template<typename T, bool INDEXING>
		inline ObjectPool<T, INDEXING>::~ObjectPool()
		{
			for ( ObjType* arrayptr : _array_ptrs )
			{
				if ( arrayptr )
				{
					delete[] arrayptr;
				}
			}

			_array_ptrs.clear();

			[this]()
			{
				std::queue<ObjType*> empty;
				std::swap( _pool, empty );
			}( );
		}

		template<typename T, bool INDEXING>
		inline T* ObjectPool<T, INDEXING>::Borrow()
		{
			std::lock_guard<std::recursive_mutex> lock( _mutex );

			// 메모리 고갈시 expandsize 만큼 추가
			if ( _pool.empty() )
			{
				Create( _expandsize );
			}

			ObjType* obj = _pool.front();
			_pool.pop();

			return obj;
		}

		template<typename T, bool INDEXING>
		inline void ObjectPool<T, INDEXING>::Return( T* obj )
		{
			std::lock_guard<std::recursive_mutex> lock( _mutex );

			_pool.push( static_cast<ObjType*>( obj ) );
		}

		template<typename T, bool INDEXING>
		inline int ObjectPool<T, INDEXING>::GetIndex( T* t )
		{
			if ( INDEXING )
				return static_cast<IndexingT*>( t )->GetIndex();

			return 0;
		}

		template<typename T, bool INDEXING>
		inline void ObjectPool<T, INDEXING>::Create( int size )
		{
			std::lock_guard<std::recursive_mutex> lock( _mutex );

			ObjType* startptr = new ObjType[ size ];					// 배열로 생성 (지역성의 원리)
			_array_ptrs.push_back( startptr );

			for ( int i = 0; i < size; i++ )
			{
				if ( INDEXING )
				{
					IndexingT* obj = static_cast<IndexingT*>( startptr + i );
					obj->SetIndex( _index++ );
				}

				_pool.push( startptr + i );
			}

			_expandsize = static_cast<int>(_pool.size() * 2);
		}

		template<typename T, bool INDEXING>
		inline void ObjectPool<T, INDEXING>::Free( ObjType* startptr )
		{
			std::lock_guard<std::mutex> lock( _mutex );

			if ( startptr )
			{
				delete[] t_array;
				t_array = nullptr;
			}
		}

		// 외부에서 메모리 생성, ArrayAlloc 은 new T[] 로 생성시 delete[] 로 삭제하기 위함
		template<typename T, bool ArrayAlloc = false>
		class ManualObjectPool
		{
		public:
			typedef typename T											    value_Type;
			typedef typename ManualObjectPool<T>		                    this_type;

		public:
			ManualObjectPool();
			~ManualObjectPool();

			void SetArrayStartPtr( T* ptr );      // new T[] 로 생성 후 리턴된 시작 포인터를 넣어준다

			void Add( T* obj );
			T* Borrow();
			void Return( T* obj );

		private:
			ManualObjectPool( const this_type& ) = delete;
			this_type& operator=( const this_type& ) = delete;

		private:
			std::queue<T*>	        _pool;
			std::vector<T*>	        _array_ptrs;

			std::mutex				_mutex;
		};

		template<typename T, bool ArrayAlloc>
		inline ManualObjectPool<T, ArrayAlloc>::ManualObjectPool()
		{
			static_assert( !std::is_pointer<T>::value, "Pointer type forbidden!" );

			_array_ptrs.reserve( 10 );
		}

		template<typename T, bool ArrayAlloc>
		inline ManualObjectPool<T, ArrayAlloc>::~ManualObjectPool()
		{
			if ( ArrayAlloc )
			{
				for ( T* arrayptr : _array_ptrs )
				{
					if ( arrayptr )
					{
						delete[] arrayptr;
					}
				}

				_array_ptrs.clear();

				[this]()
				{
					std::queue<T*> empty;
					std::swap( _pool, empty );
				}( );
			}
			else
			{
				while ( !_pool.empty() )
				{
					T* obj = _pool.front();
					_pool.pop();

					if ( obj )
					{
						delete obj;
						obj = nullptr;
					}
				}
			}
		}

		template<typename T, bool ArrayAlloc>
		inline void ManualObjectPool<T, ArrayAlloc>::SetArrayStartPtr( T* ptr )
		{
			std::lock_guard<std::mutex> lock( _mutex );

			_array_ptrs.push_back( ptr );
		}

		template<typename T, bool ArrayAlloc>
		inline void ManualObjectPool<T, ArrayAlloc>::Add( T* obj )
		{
			std::lock_guard<std::mutex> lock( _mutex );

			_pool.push( obj );
		}

		template<typename T, bool ArrayAlloc>
		inline T* ManualObjectPool<T, ArrayAlloc>::Borrow()
		{
			std::lock_guard<std::mutex> lock( _mutex );

			if ( _pool.empty() )
				return nullptr;

			T* obj = _pool.front();
			_pool.pop();

			return obj;
		}

		template<typename T, bool ArrayAlloc>
		inline void ManualObjectPool<T, ArrayAlloc>::Return( T* obj )
		{
			std::lock_guard<std::mutex> lock( _mutex );

			_pool.push( obj );
		}

	}	// end namespace suho
}	// end namespace pool