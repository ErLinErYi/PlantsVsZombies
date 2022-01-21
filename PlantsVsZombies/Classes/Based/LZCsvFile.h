//
//  CSVFile.h
//
//  Created by quasi_lee on 15-7-7.
//  Modify by lz on 22-1-14
//

#ifndef __CSVFile__
#define __CSVFile__

#include"cocos2d.h"

using namespace cocos2d;
using namespace std;

class CSVFile {
public:
    CSVFile();
    ~CSVFile();

    //用以存储数据
    vector<vector<string> > data;

public:
    //打开CSV文件
    bool openFile(const char* fileName);
    bool openFile(string& str);

    //根据行列获取数据
    const char* getData(unsigned int rows, unsigned int cols);

    //获取指定数据的列下标
    int findColsData(unsigned int cols, const char* value);

    //得到总列数
    inline int getCols() { return cols; }

    //得到总行数
    inline int getRows() { return data.size(); }

    //得到某列排除指定数据后的总行数
    int getRows(int i, const string& exclude);

    //添加新数据
    void addNewData(vector<string>& newData);

    //数据排序,pos为排序时以pos参数位置为基准，cmp默认降序
    void sortData(int pos, bool cmp = false);

    //删除pos列指定分隔符的前缀信息
    void deletePrefix(const char* separator, int pos);

    //删除pos列指定分隔符的后缀信息
    void deleteSuffix(const char* separator, int pos);

    //去除pos列重复的值，且pos列的其他值保留最大的
    void removeDuplicationReservedMaximum(int pos);

    //将csvdata转换成字符串
    string csvDataToString();

private:
    void StringSplit(const string& str, vector<string>& tokens, const char& delimiters);
    void split(vector<string>& field, string line);
    int advplain(const string& line, string& fld, int);
    int advquoted(const string& line, string& fld, int);

private:
    string fieldsep;
    int cols;
};

#endif /* defined(__CSVFile__) */