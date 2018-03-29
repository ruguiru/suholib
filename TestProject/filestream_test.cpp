#include "test_common.h"
#include "FileReader.h"
#include "FileWriter.h"

using namespace suho::file;

void main()
{
	
	FileWriter f("test.txt");
	f << 123 << "\n";
	f << "abc\n";
	f << 1.232323f << "\n";
	f.Close();

	std::string b;
	FileReader fr("test.txt");

	if (fr.Eof())
	{
	    printf("eof\n");
	}

	int filesize = fr.GetFileSize();
	printf("filesize : %d\n", filesize);

	char* buf = new char[filesize];
	memset(buf, 0, filesize);

	fr.Read(buf, filesize);
	delete[] buf;

	FileReader fr2("test.txt");

	while (fr2.Good())
	{
	    //char buf[1024] = {};
	    //fr.ReadLine(buf, 1024);
	    //printf("%s\n", buf);

	    char ch;
	    fr2.ReadChar(ch);
	    printf("%c", ch);
	}
}