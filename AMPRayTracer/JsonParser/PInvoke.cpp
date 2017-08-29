#include "JsonParser.h"

void parse(const char* input)
{
	IParser* parser = new JsonParser();
	parser->parse(input);
}

int main()
{
	const char* json ="{\"geometries\" :{\"spheres\":[{ \"radius\" : 5.0, \"center\" : [1, 2, 4]},{ \"radius\" : 3.0, \"center\" : [-8.2,2,4]},{ \"radius\" : 2.0, \"center\" : [-8.2,2,4.0]}]}}";
	parse(json);
	return 0;
}