/* configuration 文件解析模块 */
#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<stdlib.h>

using namespace std;

class Config{
public:
    Config();
    ~Config();

    //对外接口
    void init();
    void show();

    //数据成员
    int  num_of_class;
    int  num_of_point;
    int  dimension_of_data;
    int  type_of_distance;
    int  type_of_center_update;
    int  type_of_init_point_choice;
    string input_file;
    string result_file;
    string config_file;
    double error_limit;
    int   iter_limit;
private:
    void analyse_of_config_file(string configfile);
    void acquire_info_of_data(string input_file, int & num_of_point, int & dimension_of_data);
};

#endif
