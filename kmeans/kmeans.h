/* k-means 聚类 */
#ifndef _KMEANS_H_
#define _KMEANS_H_

#include<fstream>
#include<sstream>
#include<iostream>
#include<vector>
#include<string>
#include<stdlib.h>
#include<cmath>

#include"config.h"

using namespace std;

typedef struct{
    double *p;
    double *distance;
}Point;

typedef struct{
    Point *clu_cent;        //即cluster_center 类别中心
    int **clu;              //类别数组,将指向一个二维数组
    int *num_of_each_clu;   //每一类中数据的个数吧
    double fitness;         //样本适应度值,用于判断结束条件
    double old_fitness;     //前一次迭代的适应度值
    double Je;              //所有样本的平方误差和
}Cluster;

class Kmeans{
public:
    Kmeans(Config config);
    ~Kmeans();

    void clustering();
private:
    //数据成员
    int  num_of_class;
    int  num_of_point;
    int  dimension_of_data;
    int  type_of_distance;
    int  type_of_center_update;
    int  type_of_init_point_choice;
    string input_file;
    string result_file;
    double error_limit;
    int   iter_limit;    

    Point *all_data;    //所有数据点
    Cluster cluster;    //聚类信息
    string *name_of_all_data;   //所有的商品名，就是数据的第一列

    //方法成员
    void calculate_distance();   //计算距离函数
    double Distance_of_point(double *x, double *y); //两点间距离总接口函数
    double Euclid(double *x, double *y);  //两点间距离方法1:欧氏距离
        
    void input_data();           //文件加载函数

    void Init_center();          //初始中心总接口函数
    void Init_center_rand();     //初始中心方法1:random
    int Is_equal(int a[], int n, int b);
   
    void Make_new_cluster();     //归类函数
    
    void Make_new_center();      //更新类中心函数总接口
    void Make_new_center_aver(); //更新类中心方法1: 均值
    
    void output_info(int flag, ofstream &output );  //输出信息函数
};

#endif
