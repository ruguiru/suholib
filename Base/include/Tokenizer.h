#pragma once

// ���� : ��ü ���� �� Parsing �Լ��� ���������Ϳ� �����ڸ� ������ �� HasNext() �� �˻��Ͽ� true �̸� GetToken() ���� ������ ���

#include <vector>

namespace suho {
namespace util
{
    class Tokenizer
    {
    public:
        Tokenizer(const std::string& source_str, const char delimeter);	// �Ľ��Ͽ� ���� �����̳ʿ� ����
        ~Tokenizer();

        void Parse(const std::string& source_str, const char delimeter);
        bool HasNext() const;
        const std::string& GetToken();									            // �����̳ʿ��� �ϳ��� ��ū�� ������, ������� iterator�� ��ȭ �ϹǷ� ����
        int GetTokenCount() const { return (int)_inner_vec.size(); }
        bool IsEmpty() const;

        const std::vector<std::string>& GetContainer() const;				// ���� �����̳� ���۷��� ��������
        void Reset();

    private:
        std::vector<std::string> _inner_vec;

        std::vector<std::string>::iterator _head_iter;
        std::vector<std::string>::iterator _tail_iter;
    };
}   // end namespace util
}   // end 

