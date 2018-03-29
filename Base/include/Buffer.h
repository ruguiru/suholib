#pragma once

namespace suho {
namespace buffer
{
	class Buffer
	{
	public:
        Buffer() {}
        Buffer(int capacity);
        Buffer(const Buffer& other);
		Buffer(Buffer&& other);
        ~Buffer();

        Buffer& operator=(const Buffer& other);
        Buffer& operator=(Buffer&& other);

        void Create(int capacity);
        void Assign(const char* buf, int size);        
        void Append(const char* buf, int size);
        void Append(const Buffer& buf) { Append(buf.Begin(), buf.GetSize()); }
        void ReSize(int newsize, bool is_keepdata = true);

        void Clear();
        int GetCapacity() const { return _capacity; }
        int GetSize() const { return _filled_size; }
        void SetSize(int size) { _filled_size = size; }
		void AddSize(int size) { _filled_size += size; }
        char* Begin() { return _begin; }
        const char* Begin() const { return _begin; }
        char* End() { return _begin + _filled_size; }
        const char* End() const { return _begin + _filled_size; }
        bool Empty() const { return _filled_size == 0; }

    private:
        void Swap(Buffer& other);

	private:
		int			_capacity = 0;
        int         _filled_size = 0;
		char*		_begin = 0;
	};
}	// end namespace suho
}	// end namespace buffer