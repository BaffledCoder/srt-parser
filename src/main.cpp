#pragma once
#include "parser.h"
#include "block.h"
#include "ftext.h"
#include "xmlconverter.h"

int main()
{
	std::string filename;
	std::cin>>filename;

	std::string outputPathTXT;
	outputPathTXT = "./Output_Files/" + filename + ".txt";

	std::string outputPathXML;
	outputPathXML = "./Output_Files/" + filename + ".xml";

	std::string inputSRT;
	inputSRT= "./SRT_Files/" + filename + ".srt";
	std::ifstream fin;
	fin.open(inputSRT.c_str(),std::ios::in);
	if(!fin.is_open())
	{
		std::cout<<"File not opened";
		return 0;
	}
	CParse parseSrt(fin, inputSRT.c_str());

	std::ofstream fout(outputPathTXT, std::ios::out);
	parseSrt.ShowData(fout);
	fout.close();

	srt_xml(parseSrt.GetData(), outputPathXML.c_str());
	return 0;
}