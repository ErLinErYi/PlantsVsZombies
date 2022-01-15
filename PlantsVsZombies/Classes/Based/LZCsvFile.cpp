//
//  CSVFile.cpp
//
//  Created by quasi_lee on 15-7-7.
//  Modify by lz on 22-1-14
//

#include "LZCSVFile.h"

CSVFile::CSVFile():
    fieldsep(","),
    cols(0)
{

}

//析构函数,释放内存
CSVFile::~CSVFile()
{
    for (int i = 0; i < data.size(); i++)
    {
        data[i].clear();
    }
    data.clear();
}

//获取指定行列的数据
const char* CSVFile::getData(int rows, int cols)
{
    if (rows < 0
        || rows >= data.size()
        || cols < 0
        || cols >= data[rows].size())
    {
        return "";
    }
    return data[rows][cols].c_str();
}

//获取指定数据的列下标
int CSVFile::findColsData(int cols, const char* value)
{
    for (int i = 0; i < data.size(); i++)
    {
        if (strcmp(getData(i, cols), value) == 0)
        {
            return i;
        }
    }
    return -1;
}

//解析CSV文件
bool CSVFile::openFile(const char* fileName)
{
    string pathKey = CCFileUtils::getInstance()->fullPathForFilename(fileName);
    unsigned char* pBuffer = NULL;
    ssize_t bufferSize = 0;
    pBuffer = CCFileUtils::getInstance()->getFileData(pathKey.c_str(), "r", &bufferSize);

    string s = (char*)pBuffer;
    string str = s.substr(0, bufferSize);

    vector<string> line;
    StringSplit(str, line, '\n');
    for (int i = 0; i < line.size(); i++)
    {
        vector<string> field;
        split(field, line[i]);
        data.push_back(field);
        cols = max(cols, (int)field.size());
    }
   
    return true;
}

bool CSVFile::openFile(string& str)
{
    vector<string> line;
    StringSplit(str, line, '\n');
    for (int i = 0; i < line.size(); i++)
    {
        vector<string> field;
        split(field, line[i]);
        data.push_back(field);
        cols = max(cols, (int)field.size());
    }
    return true;
}

void CSVFile::addNewData(vector<string>& newData)
{
    data.push_back(newData);
}

void CSVFile::StringSplit(const std::string& str, vector<string>& tokens, const char& delimiters)
{
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos
        || string::npos != lastPos) {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void CSVFile::split(vector<string>& field, string line)
{
    string fld;
    int i = 0;
    int j = 0;

    if (line.length() == 0)
    {
        return;
    }

    do {
        if (i < line.length()
            && line[i] == '"')
        {
            j = advquoted(line, fld, ++i);
        }
        else
        {
            j = advplain(line, fld, i);
        }
        field.push_back(fld);
        i = j + 1;
    } while (j < line.length());
}

int CSVFile::advquoted(const string& line, string& fld, int i)
{
    int j = 0;
    fld = "";
    for (j = i; j < line.length(); j++)
    {
        if (line[j] == '"'
            && line[++j] != '"')
        {
            int k = line.find_first_of(fieldsep, j);
            if (k > line.length())
            {
                k = line.length();
            }
            for (k -= j; k-- > 0; )
            {
                fld += line[j++];
            }
            break;
        }
        fld += line[j];
    }
    return j;
}

void CSVFile::sortData(int pos, bool cmp)
{
    sort(data.begin(), data.end(), 
        [cmp,pos](vector<string>& a, vector<string>& b)
        {
            if (a[pos].size() == b[pos].size())
            {
                return !cmp ? a[pos] > b[pos]:a[pos] < b[pos];
            }
            else
            {
                return !cmp ? a[pos].size() > b[pos].size() : a[pos].size() < b[pos].size();
            }
        });
}

int CSVFile::advplain(const string& line, string& fld, int i)
{
    int j = 0;
    j = line.find_first_of(fieldsep, i);
    if (j > line.length())
    {
        j = line.length();
    }
    fld = string(line, i, j - i);
    return j;
}
