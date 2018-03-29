#include "test_common.h"
#include "Tokenizer.h"
#include "FileReader.h"
#include "json.hpp"

using namespace std;
using namespace nlohmann;

class TestClass
{
public:
	std::string GetClassName() const
	{
		std::string fullname(typeid(*this).name());

		suho::util::Tokenizer parser(fullname, ' ');
		parser.GetToken();
		if (parser.HasNext())
		{
			return parser.GetToken();
		}

		return "";
	}
};

int main()
{
	TestClass t;
	cout << t.GetClassName() << "\n";

	suho::file::FileReader fr("../jsontest.txt");
	int filesize = fr.GetFileSize();
	
	char* buf = new char[filesize];
	memset(buf, 0, filesize);

	fr.Read(buf, filesize);
	

	json root = json::parse(buf);

	delete[] buf;
	
	json acc = root["AcceptConnections"];
	json client = acc["Client"];
	short port = client["Port"];
	int maxaccept = client["MaxAccepts"];

	
	json conn = root["ConnectConnections"];	
	json timesvr = conn["TimeServer"];
	int interval = timesvr["RetryIntervalmsec"];

	json dest_array = timesvr["Destinations"];
	int arrcnt = dest_array.size();

	for (int i = 0; i < arrcnt; i++)
	{
		json dest = dest_array[i];
		int id = dest["ID"];
		std::string ip = dest["IP"];
	}	
}