#include "JsonParser.h"
#include <fstream>
#include <string>

void parse(const char* input)
{
	IParser* parser = new JsonParser();;
	
	parser->parse(input);
	delete parser;
}

int main()
{
	fstream fs;
	fs.open("json_test.json", fstream::in);
	string content;
	string line;
	bool exists = fs.good();
	system("echo %cd%");
	if (exists)
	{
		while (getline(fs, line))
		{
			content += line;
		}
	}
	
	const char* json =content.c_str();
	fs.close();
	parse(json);
	return 0;
}