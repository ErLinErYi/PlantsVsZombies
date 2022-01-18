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
    //添加新数据
    void addNewData(vector<string>& newData);
    //数据排序,pos为排序时以pos参数位置为基准，cmp默认降序
    void sortData(int pos, bool cmp = false);

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