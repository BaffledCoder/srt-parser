#pragma once
#include "parser.h"

void CParse::SetLineEnding(const char *filename, std::string &newline, char &delimiter)
{
	std::string str;
	std::ifstream chk(filename,std::ios::binary);
	if(getline(chk, str))
	{
		if(str.size() && str[str.size()-1] == '\r') 
		{
			if(getline(chk, str))
			{
				delimiter = '\n';
				newline = "\\r\\n";
			}
			else
			{
				delimiter = '\r';
				newline = "\\r";
			}
		}
		else 
		{
			delimiter = '\n';
			newline = "\\n";
		}
	}
}
CParse::CParse(std::ifstream &inp, const char *inputSRT)
{
	std::string str;
    std::string time;
    std::string block;


	//Setting newline and delimiter acc. to line ending
	SetLineEnding(inputSRT,NEWLINE,delimiter);
	int i=0;
    while (!inp.eof())
    {
        i++;
        getline(inp, str, delimiter);
        // a blank line
        if (str.length()==0 )
        {
            // send block
            AddData(time, block);
            block="";
            i=0;
            continue;
        }
        if (i==2) time = str;
        else if(i>2)
        {
            if(block!="") block=block + NEWLINE + str;
            else block = str;
        }
    }
	inp.close();
}

std::vector<CBlock> CParse::GetData() {return m_vData;}

int CParse::ConvertToMillisecond(std::string sHours, std::string sMinutes, std::string sSeconds, std::string sMilliseconds)
{
	int nHours = std::stoi(sHours), nMinutes = std::stoi(sMinutes), nSeconds = std::stoi(sSeconds), nMilliseconds = std::stoi(sMilliseconds);
	return nHours * 3600000 + nMinutes * 60000 + nSeconds * 1000 + nMilliseconds;
}

std::pair<int, int> CParse::TimeCalculator(std::string sTime)
{
				//00:02:58,325 --> 00:03:00,633
	std::vector<std::string> sStartEnd = Split(sTime, " --> ");
	std::string sStart, sEnd;
	sStart = sStartEnd[0];
	sEnd = sStartEnd[1];
	std::string sStartHour, sStartMin, sStartSec, sStartMil, sEndHour, sEndMin, sEndSec, sEndMil;
				//00:02:58,325
	std::vector<std::string> vStartStrip = Split(sStart, ":");
	sStartHour = vStartStrip[0];
	sStartMin = vStartStrip[1];
	sStartSec.push_back(vStartStrip[2][0]);
	sStartSec.push_back(vStartStrip[2][1]);
	sStartMil.push_back(vStartStrip[2][3]);
	sStartMil.push_back(vStartStrip[2][4]);
	sStartMil.push_back(vStartStrip[2][5]);
				//00:03:00,633
	std::vector<std::string> vEndStrip = Split(sEnd, ":");
	sEndHour = vEndStrip[0];
	sEndMin = vEndStrip[1];
	sEndSec.push_back(vEndStrip[2][0]);
	sEndSec.push_back(vEndStrip[2][1]);
	sEndMil.push_back(vEndStrip[2][3]);
	sEndMil.push_back(vEndStrip[2][4]);
	sEndMil.push_back(vEndStrip[2][5]);
	int nStartMS = ConvertToMillisecond(sStartHour, sStartMin, sStartSec, sStartMil);
	int nEndMS = ConvertToMillisecond(sEndHour, sEndMin, sEndSec, sEndMil);
	std::pair<int, int> p(nStartMS, nEndMS);
	return p;
}

std::string CParse::TagRemover(std::string sText)
{
	int left = 0;
    for(int i=0;i<sText.size();i++) // replacing <...> with ~~~~
    {
        if(sText[i]=='<')
        {
            left++;
            sText[i] = '~';
        }
        else
        {
            if(left!=0)
            {
				if(sText[i] == '>') left--;
				sText[i] = '~';
            }
        }
	}
	//std::cout<<"Shubham"<<text<<std::endl;
	sText.erase(std::remove(sText.begin(), sText.end(), '~'), sText.end());
	return sText;
}

std::string CParse::PositionCalculation(int nPositionY)
{
	if(nPositionY<=333) return "Top";
	else if(nPositionY>333 && nPositionY<=664) return "Middle";
	return "Bottom";
	
}

std::string CParse::PositionFinder(std::string sTime)
{
	size_t foundY1 = sTime.find("Y1:");
    if (foundY1 != std::string::npos)
	{
		size_t foundY2 = sTime.find("Y2:");
		std::string sStr;
		sStr.push_back(sTime[foundY1+3]);
		sStr.push_back(sTime[foundY1+4]);
		sStr.push_back(sTime[foundY1+5]);
		return PositionCalculation(stoi(sStr));
	}
	return "Bottom";
}

std::string CParse::PositionTagHelper(char n)
{
	if(n == '4' || n == '5' || n == '6') return "Middle";
	else if(n == '7' || n == '8' || n == '9') return "Top";
	else return "Bottom";
}

std::string CParse::PositionTagHandler(std::string &sText)
{
	int found1 = int (sText.find('{'));
	std::string sResult;
	if(found1 == -1) return "Bottom";
	else
	{
		if(sText[found1+3] == 'n') sResult = PositionTagHelper(sText[found1+4]);
		else sResult = PositionTagHelper(sText[found1+3]);
		sText = sText.substr(sText.find('}')+1, sText.size()-1);
	}
	return sResult;
}

void CParse::FormattedText(std::vector<CFormattedText> &vec, std::string str, int start, int end, bool bold, bool italic, bool underline, std::string fColor, std::string bgColor)
{   std::string instr = "";
    int count=0; 
    for(int i=start;i<=end;i++)
    {
        if(str[i]=='<'&&str[i+1]=='b')
        {
            if(instr.size()!=0)
            {
				std::string sLineEnd = NEWLINE;
				if(instr.size()!=sLineEnd.size()) 
				{
					if(instr.size()>sLineEnd.size() && sLineEnd==instr.substr(instr.size()-sLineEnd.size(), sLineEnd.size()))
					{
						instr.erase(instr.begin()+instr.size()-sLineEnd.size(),instr.end());
					}
					CFormattedText ft(instr,bold,italic,underline,fColor,bgColor);
					vec.push_back(ft);
				}
            }
            count++;
            for(;i<=end;)
            {
                if(str[i]=='>') 
                {
                    i++;
                    break;
                }
                else i++;
            }
            int j, k=i;
            for(i;i<=end;i++)
            {
                if(str[i]=='<'&&str[i+1]=='/'&&str[i+2]=='b')
                {
                    j=i+2;
                    while(str[j]!='>') j++;
                    break;
                }
            }
            FormattedText(vec,str,k,i-1,true,italic,underline,fColor,bgColor);
            FormattedText(vec,str,j+1,end,bold,italic,underline,fColor,bgColor);
            return;
        }
        else if(str[i]=='<'&&str[i+1]=='i')
        {
            if(instr.size()!=0)
            {
				std::string sLineEnd = NEWLINE;
				if(instr.size()!=sLineEnd.size()) 
				{
					if(instr.size()>sLineEnd.size() && sLineEnd==instr.substr(instr.size()-sLineEnd.size(), sLineEnd.size()))
					{
						instr.erase(instr.begin()+instr.size()-sLineEnd.size(),instr.end());
					}
					CFormattedText ft(instr,bold,italic,underline,fColor,bgColor);
					vec.push_back(ft);
				}
            }
            count++;
            for(;i<=end;)
            {
                if(str[i]=='>') 
                {
                    i++;
                    break;
                }
                else i++;
            }
            int j, k=i;
            for(i;i<=end;i++)
            {
                if(str[i]=='<'&&str[i+1]=='/'&&str[i+2]=='i')
                {
                    j=i+2;
                    while(str[j]!='>') j++;
                    break;
                }
            }
            FormattedText(vec,str,k,i-1,bold,true,underline,fColor,bgColor);
            FormattedText(vec,str,j+1,end,bold,italic,underline,fColor,bgColor);
            return;
        }
		else if(str[i]=='<'&&str[i+1]=='u')
        {
            if(instr.size()!=0)
            {
				std::string sLineEnd = NEWLINE;
				if(instr.size()!=sLineEnd.size()) 
				{
					if(instr.size()>sLineEnd.size() && sLineEnd==instr.substr(instr.size()-sLineEnd.size(), sLineEnd.size()))
					{
						instr.erase(instr.begin()+instr.size()-sLineEnd.size(),instr.end());
					}
					CFormattedText ft(instr,bold,italic,underline,fColor,bgColor);
					vec.push_back(ft);
				}
            }
            count++;
            for(;i<=end;)
            {
                if(str[i]=='>') 
                {
                    i++;
                    break;
                }
                else i++;
            }
            int j, k=i;
            for(i;i<=end;i++)
            {
                if(str[i]=='<'&&str[i+1]=='/'&&str[i+2]=='u')
                {
                    j=i+2;
                    while(str[j]!='>') j++;
                    break;
                }
            }
            FormattedText(vec,str,k,i-1,bold,italic,true,fColor,bgColor);
            FormattedText(vec,str,j+1,end,bold,italic,underline,fColor,bgColor);
            return;
        }
        else if(str[i]=='<'&&str[i+1]=='f')
        {
            if(instr.size()!=0)
            {
				std::string sLineEnd = NEWLINE;
				if(instr.size()!=sLineEnd.size()) 
				{
					if(instr.size()>sLineEnd.size() && sLineEnd==instr.substr(instr.size()-sLineEnd.size(), sLineEnd.size()))
					{
						instr.erase(instr.begin()+instr.size()-sLineEnd.size(),instr.end());
					}
					CFormattedText ft(instr,bold,italic,underline,fColor,bgColor);
					vec.push_back(ft);
				}
            }
            count++;
            std::string fc = "";
            int found1 = int (str.find('"',i));
            int found2 = int (str.find('"',found1+1));
            for(int f=found1+1;f<found2;f++) fc+=str[f];
            for(;i<=end;)
            {
                if(str[i]=='>') 
                {
                    i++;
                    break;
                }
                else i++;
            }
            int j, k=i;
            for(i;i<=end;i++)
            {
                if(str[i]=='<'&&str[i+1]=='/'&&str[i+2]=='f')
                {
                    j=i+2;
                    while(str[j]!='>') j++;
                    break;
                }
            }
            FormattedText(vec,str,k,i-1,bold,italic,underline,fc,bgColor);
            FormattedText(vec,str,j+1,end,bold,italic,underline,fColor,bgColor);
            return;
        }
        else
        {
            instr+=str[i];
        }
    }
    if(count==0&&instr.size()!=0)
    {
        CFormattedText ft(instr,bold,italic,underline,fColor,bgColor);
        vec.push_back(ft);
    }
}

void CParse::AddData(std::string sTime, std::string sText)
{
	CBlock *b = new CBlock;

	std::pair<int, int> pairSE = TimeCalculator(sTime);
	b->SetStartTime(pairSE.first);
	b->SetEndTime(pairSE.second);

	b->SetPosition(PositionFinder(sTime));
	if(b->GetPosition() == "Bottom") b->SetPosition(PositionTagHandler(sText));
	
	std::pair<std::string, std::vector<CFormattedText>> PlainText;
	//Plain Text
	std::string sPlainText = TagRemover(sText);

	//Formatted Text
	std::vector<CFormattedText> vFormattedText;
	FormattedText(vFormattedText, sText, 0, int (sText.size()-1), false, false, false, "#000000", "#00FFFFFF");
	PlainText = std::make_pair(sPlainText, vFormattedText);
	b->SetText(PlainText);

	//Adding block to vector<Block> data
	m_vData.push_back(*b);
}

std::vector<std::string> CParse::Split(std::string sToSplit, std::string sDelimiter) {
    size_t pos = 0;
    std::vector<std::string> matches;
    do {
        pos = sToSplit.find(sDelimiter);
        int take_elemAt_pos;
        if (pos == std::string::npos) {
            pos = sToSplit.length() - 1;
            take_elemAt_pos = 1;
        }
        else {
            take_elemAt_pos = 0;
        }
		//std::cout<<delimiter.size()<<std::endl;
        matches.push_back(sToSplit.substr(0, pos+take_elemAt_pos));
        sToSplit.erase(0, pos+sDelimiter.size());

    }
    while (!sToSplit.empty());
    return matches;
}

void CParse::ShowData(std::ofstream &fout)
{
	
	for(int i=0;i<m_vData.size();i++)
	{
		fout<<"Start Time(in millisecond): ";
		fout<<m_vData[i].GetStartTime()<<"ms"<<std::endl;
		fout<<"End Time(in millisecond): ";
		fout<<m_vData[i].GetEndTime()<<"ms"<<std::endl;
		fout<<"Position: ";
		fout<<m_vData[i].GetPosition()<<std::endl;
		fout<<"Plain Text: ";
		fout<<m_vData[i].	GetPlainText()<<std::endl;
		fout<<"Formatted Text: "<<std::endl;
		std::vector<CFormattedText> fText = m_vData[i].GetFormattedText();
		for(int i=0;i<fText.size();i++)
		{
			fout<<"Text: "<<fText[i].GetText()<<std::endl;
			fout<<"Is Bold: "<<fText[i].IsBold()<<std::endl;
			fout<<"Is Italic: "<<fText[i].IsItalic()<<std::endl;
			fout<<"Is Underline: "<<fText[i].IsUnderline()<<std::endl;
			fout<<"Font Color: "<<fText[i].GetFontColor()<<std::endl;
			fout<<"Background Color: "<<fText[i].GetBackgroundColor()<<std::endl;
		}
		fout<<" ------------------------------------------------- "<<std::endl;
	}

}