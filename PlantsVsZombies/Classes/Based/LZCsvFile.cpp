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
    for (unsigned int i = 0; i < data.size(); i++)
    {
        data[i].clear();
    }
    data.clear();
}

//获取指定行列的数据
const char* CSVFile::getData(unsigned int rows, unsigned int cols)
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
int CSVFile::findColsData(unsigned int cols, const char* value)
{
    for (unsigned int i = 0; i < data.size(); i++)
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
    auto str = CCFileUtils::getInstance()->getStringFromFile(pathKey.c_str());

    vector<string> line;
    StringSplit(str, line, '\n');
    for (unsigned int i = 0; i < line.size(); i++)
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
    for (unsigned int i = 0; i < line.size(); i++)
    {
        vector<string> field;
        split(field, line[i]);
        data.push_back(field);
        cols = max(cols, (int)field.size());
    }
    return true;
}

int CSVFile::getRows(int i, const string& exclude)
{
    int rowNum = 0;
    for (auto& r : data)
    {
        if (r[i] != exclude)
        {
            ++rowNum;
        }
    }
    return rowNum;
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
    unsigned int i = 0;
    unsigned int j = 0;

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
    unsigned int j = 0;
    fld = "";
    for (j = i; j < line.length(); j++)
    {
        if (line[j] == '"'
            && line[++j] != '"')
        {
            unsigned int k = line.find_first_of(fieldsep, j);
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
    stable_sort(data.begin(), data.end(), 
        [cmp,pos](const vector<string>& a, const vector<string>& b)
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

void CSVFile::deletePrefix(const char* separator, int pos)
{
    for (auto& r : data)
    {
        auto n = r[pos].find(separator);
        r[pos] = r[pos].substr(n + strlen(separator));
    }
}

void CSVFile::deleteSuffix(const char* separator, int pos)
{
    for (auto& r : data)
    {
        auto n = r[pos].find(separator);
        if (n != string::npos)
        {
            r[pos] = r[pos].erase(n);
        }
    }
}

void CSVFile::removeDuplicationReservedMaximum(int pos)
{
    vector<vector<string> > tempData;
    auto rows = getRows() - 1;

    for (int i = 0; i < rows; ++i)
    {
        if (data[i][pos] == data[i + 1][pos])
        {
            for (int j = 0; j < cols; ++j)
            {
                if (j != pos)
                {
                    data[i + 1][j] = max(data[i][j], data[i + 1][j]);
                }
            }
        }
        else
        {
            tempData.emplace_back(data[i]);
        }
    }
    if (data.size() > rows)
    {
        tempData.emplace_back(data[rows]);
    }

    data.clear();
    data = tempData;
}

void CSVFile::strMatchingAndInsertStr(int id, const string& matchStr, const string& insertPosStr, const string& insertStr)
{
    for (auto& r : data)
    {
        if (r[id] == matchStr)
        {
            r[id].insert(r[id].find(insertPosStr) + 2, insertStr);
        }
    }
}

string CSVFile::csvDataToString()
{
    string str;
    for (auto& r : data)
    {
        for (int i = 0; i < cols; ++i)
        {
            str += r[i];
            if (i != cols - 1)
            {
                str += fieldsep;
            }
        }
        str += "\n";
    }

    return str;
}

int CSVFile::advplain(const string& line, string& fld, int i)
{
    unsigned int j = 0;
    j = line.find_first_of(fieldsep, i);
    if (j > line.length())
    {
        j = line.length();
    }
    fld = string(line, i, j - i);
    return j;
}
