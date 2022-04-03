#pragma once
#include "block.h"
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <utility>
class CParse
{
	std::vector<CBlock> m_vData;
public:
	std::string NEWLINE;
	char delimiter;
	CParse(std::ifstream &inp, const char *inputSRT);
	void SetLineEnding(const char *flename, std::string &newline, char &delimiter);
	std::vector<CBlock> GetData();
	std::vector<std::string> Split(std::string sToSplit, std::string delimiter);
	int ConvertToMillisecond(std::string sHours, std::string sMinutes, std::string sSeconds, std::string sMilliseconds);
	std::pair<int, int> TimeCalculator(std::string sTime);
	std::string TagRemover(std::string sText);
	std::string PositionFinder(std::string sTime);
	std::string PositionCalculation(int nPositionY);
	std::string PositionTagHelper(char n);
	std::string PositionTagHandler(std::string &sText);
	void FormattedText(std::vector<CFormattedText> &vec, std::string str, int start, int end, bool bold = false, bool italic = false, bool underline = false, std::string fColor = "#000000", std::string bgColor = "#00FFFFFF");
	void AddData(std::string sTime, std::string sText);
	void ShowData(std::ofstream &fout);
};