#include "Tokenizer.h"

using namespace suho::util;

Tokenizer::Tokenizer(const std::string& source_str, const char delimeter)
{
	_inner_vec.reserve(32);
    Parse(source_str, delimeter);
}

Tokenizer::~Tokenizer()
{
    _inner_vec.clear();
}

void Tokenizer::Parse(const std::string & source_str, const char delimeter)
{
    Reset();

    std::string::size_type head_pos = 0;
    std::string::size_type next_pos = 0;
    std::string::size_type retval = 0;

    while (retval != std::string::npos)
    {
        retval = source_str.find_first_of(delimeter, head_pos);				// find_first_of( 구분자, 검색시작 인덱스 ) HeadPos 에서부터 delimeter 가 나오는 첫번째 인덱스 값 리턴
        if (std::string::npos == retval)       // 찾지 못함
        {
            next_pos = source_str.size();       // 마지막 인덱스
        }
        else
        {
            next_pos = retval;
        }

        std::string strtemp(source_str.substr(head_pos, next_pos - head_pos));				// substr( 시작위치, 사이즈 )

        head_pos = retval + 1;

        if (strtemp.empty())
            continue;

        _inner_vec.push_back(strtemp);
    }

    _head_iter = _inner_vec.begin();
    _tail_iter = _inner_vec.end();
}

bool Tokenizer::HasNext() const
{
    return (_head_iter != _tail_iter) ? true : false;
}

const std::string& Tokenizer::GetToken()
{
    return *(_head_iter++);
}

const std::vector<std::string>& Tokenizer::GetContainer() const
{
    return _inner_vec;
}

void Tokenizer::Reset()
{
    _inner_vec.clear();
    _head_iter = _inner_vec.begin();
    _tail_iter = _inner_vec.end();
}

bool Tokenizer::IsEmpty() const
{
    return _inner_vec.empty();
}
