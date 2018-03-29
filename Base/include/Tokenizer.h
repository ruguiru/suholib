#pragma once

// 사용법 : 객체 생성 후 Parsing 함수에 원본데이터와 구분자를 전달한 후 HasNext() 로 검사하여 true 이면 GetToken() 으로 꺼내어 사용

#include <vector>

namespace suho {
namespace util
{
    class Tokenizer
    {
    public:
        Tokenizer(const std::string& source_str, const char delimeter);	// 파싱하여 내부 컨테이너에 저장
        ~Tokenizer();

        void Parse(const std::string& source_str, const char delimeter);
        bool HasNext() const;
        const std::string& GetToken();									            // 컨테이너에서 하나씩 토큰을 꺼내옴, 멤버변수 iterator도 변화 하므로 주의
        int GetTokenCount() const { return (int)_inner_vec.size(); }
        bool IsEmpty() const;

        const std::vector<std::string>& GetContainer() const;				// 내부 컨테이너 레퍼런스 가져오기
        void Reset();

    private:
        std::vector<std::string> _inner_vec;

        std::vector<std::string>::iterator _head_iter;
        std::vector<std::string>::iterator _tail_iter;
    };
}   // end namespace util
}   // end 

