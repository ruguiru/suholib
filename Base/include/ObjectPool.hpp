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
		// ���ο��� ����
		// INDEXING true �̸� user ������Ʈ�� int �� �޸� �߰��ǰ� Ǯ���� ������ �ε��� ���� ������ �� �ִ�
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
			int GetIndex( T* t );				// INDEXING true �� ��츸

		private:
			void Free( ObjType* startptr );

			ObjectPool( const this_type& ) = delete;
			this_type& operator=( const this_type& ) = delete;

		private:
			std::queue<ObjType*>	_pool;
			std::vector<ObjType*>	_array_ptrs;			// Create �� ������ �� �迭�� ����������(delete[] ��)

			int						_expandsize = 0;		// �޸� ������ Ȯ�� ��ų ������Ʈ ��
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

			// �޸� ���� expandsize ��ŭ �߰�
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

			ObjType* startptr = new ObjType[ size ];					// �迭�� ���� (�������� ����)
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

		// �ܺο��� �޸� ����, ArrayAlloc �� new T[] �� ������ delete[] �� �����ϱ� ����
		template<typename T, bool ArrayAlloc = false>
		class ManualObjectPool
		{
		public:
			typedef typename T											    value_Type;
			typedef typename ManualObjectPool<T>		                    this_type;

		public:
			ManualObjectPool();
			~ManualObjectPool();

			void SetArrayStartPtr( T* ptr );      // new T[] �� ���� �� ���ϵ� ���� �����͸� �־��ش�

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