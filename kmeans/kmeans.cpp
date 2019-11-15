#include"kmeans.h"

Kmeans::Kmeans(Config config){
    num_of_class = config.num_of_class;
    num_of_point = config.num_of_point;;
    dimension_of_data = config.dimension_of_data;
    type_of_distance = config.type_of_distance;
    type_of_center_update = config.type_of_center_update;
    type_of_init_point_choice = config.type_of_init_point_choice;
    input_file = config.input_file;
    result_file = config.result_file;
    error_limit = config.error_limit;
    iter_limit = config.iter_limit;

    all_data = new Point[num_of_point];
    name_of_all_data = new string[num_of_point];
    cluster.clu_cent = new Point[num_of_class];
    for(int k=0; k<num_of_class; k++){       
        cluster.clu_cent[k].p = new double[dimension_of_data];
    }
	
    cluster.num_of_each_clu = new int[num_of_class];
    for(int i=0; i<num_of_class; i++){
        cluster.num_of_each_clu[i] = 0;
    }

    cluster.clu = new int*[num_of_class];
    for(int j=0; j<num_of_class; j++){
        cluster.clu[j] = new int[num_of_point];    //每一个类都预留了点数的空间，这个有点浪费啊，以后再改吧
    }

    cluster.fitness = 0;
    cluster.old_fitness = 0;
    cluster.Je = 0;
}

Kmeans::~Kmeans(){
    int i,j,k;

    for(i=0; i<num_of_point; i++){
        delete [] all_data[i].p;
        delete [] all_data[i].distance;
    }
    
    delete [] all_data;
    delete [] name_of_all_data;

    for(j=0; j<num_of_class; j++){
        delete [] cluster.clu_cent[j].p;
 //     delete [] cluster.clu_cent[j].distance;
    }
    delete [] cluster.clu_cent;

    delete [] cluster.num_of_each_clu;
    
    for(k=0; k<num_of_class; k++){
        delete [] cluster.clu[k];
    }
    delete [] cluster.clu;
}


/* 接口 */
void Kmeans::clustering(){ 
    int i = 0;          //记录迭代的次数
    double differ = 1;  //适应度差值
    int flag = 0;       //用来标示是显示初始数据还是聚类后的数据
    input_data();       //导入数据
    Init_center();      //初始化质心

    ofstream output(result_file.c_str());
   
    for(i = 0; (i < iter_limit) && (differ > error_limit); i++){  //iter_limit用来限制迭代的次数，error_limit为结束条件，即超过迭代次数或满足结束条件就结束
        calculate_distance();                  //计算欧几里德距离
        Make_new_cluster();                    //产生新的聚类

        if(flag == 0){        
            output_info(flag,output);                 //输出第一次聚类后的结果
            flag = 1;
        }

        Make_new_center();                     //对新的聚类产生新的质心
        differ = cluster.old_fitness - cluster.fitness;//判断条件
        differ = fabs(differ);         //fabs(a)是求浮点数的绝对值，abs是求整数的绝对值

        cout<<"fitness = "<<cluster.fitness<<"\n";
		cout<<"differ = "<<differ<<"\n\n";
    }

//  cout<<"***************************************************\n";
    output<<"*****************************************************\n";
    output_info(flag, output);  //聚类后显示结果

    output.close();
}


/* 从外部文件导入数据，对于没有数据文件将报错， */   
void Kmeans::input_data(){
    int i=0, j=0;

    ifstream infile(input_file.c_str());

    if( !infile ){  //检查文件是否成功读入;
        cerr<<"对不起，Kmeans::input_data 输入文件打开错误，无法导入数据\n";
        exit(1);
    }


    //先把要聚类的数据点存储
    string line, word;

    for(i = 0; i < num_of_point; i++){  //num_of_point用来指示种群的大小
        getline(infile,line);

        all_data[i].p = new double[dimension_of_data];
        all_data[i].distance = new double[num_of_class];

        istringstream inword(line);
        inword>>word;                   //第一个是商品的名称，不是数据
        name_of_all_data[i] = word;

        for(j = 0; j < dimension_of_data; j++){  //dimension_of_data用来指定维数
            inword>>word;
            all_data[i].p[j] = atof(word.c_str());
         // cout<<i<<"--"<<j<<": "<< all_data[i].p[j]<<"\n";
        }
     // cout<<"\n";
    }

    infile.close();    //关闭文件
}


/* 初始化聚类中心总接口 */
void Kmeans::Init_center(){
    if(type_of_init_point_choice == 1){
        Init_center_rand();
    }
}


/* 随机初始化聚类中心，当随机数有相同时跳过继续执行 */
void Kmeans::Init_center_rand(){
    int i=0, j=0;
    int num = 0;
    int rand_num;
    int *rand_num_tmp = new int[num_of_class];

    // 随机产生num_of_class个0～num_of_point的数
    while(num < num_of_class){  //k是聚类的数目
        rand_num = rand() % num_of_point;
        if(!Is_equal(rand_num_tmp, num, rand_num)){
            rand_num_tmp[num] = rand_num;
            num++;
        }
    }

    for(i = 0; i < num_of_class; i++){  //该循环将所有初始化的质心进行输出
    //  cout<<"初始化中心"<<i + 1<<":\n";
        for(j = 0; j < dimension_of_data; j++){
            cluster.clu_cent[i].p[j] = all_data[rand_num_tmp[i]].p[j]; //存储质心
         // cout<<cluster.clu_cent[i].p[j];
        }
     // cout<<"\n";
    }
    
    delete [] rand_num_tmp;
}


/* 检查数据是否有相等,相等返回1 */
int Kmeans::Is_equal(int a[], int n, int b){
    int i;
    for(i = 0; i < n; i++){
        if(a[i] == b){
            return 1;
        }
    }

    return 0;
}


/* 计算num_of_point组数据到num_of_class个质心的欧几里德距离*/
void Kmeans::calculate_distance(){
    int i, j;
    for(i = 0; i < num_of_point; i++){
        for(j = 0; j < num_of_class; j++){
            all_data[i].distance[j] = Distance_of_point(all_data[i].p, cluster.clu_cent[j].p);
        //  cout<<i<<"---"<<j<<all_data[i].distance[j]<<"\n";
        }
    }
}


/* 计算两点间距离的总接口 */
double Kmeans::Distance_of_point(double *x, double *y){
    if(type_of_distance == 1){
        return  Euclid(x,y);
    }else{
        cerr<<"Kmean:;Distance_of_point 点的计算方法指定有问题。\n";
        exit(1);
    }
}


/* 此函数为欧几里得距离公式函数,此处用于计算*
  * 一组数据到对应簇中心的欧几里得距离。   */
double Kmeans::Euclid(double *x, double *y){
    int i;
    double distance = 0;

    for(i = 0; i < dimension_of_data; i++){
        distance += pow(x[i] - y[i], 2);   //pow(a,b)求a的b次方
    }

    distance = sqrt(distance);    //sqrt(a)求a的平方根

    return distance;
}


/* 将数据进行簇集归类 */
void Kmeans::Make_new_cluster(){
    int i, j;
    double min;

    for(i = 0; i < num_of_class; i++){      //初始化编号
        cluster.num_of_each_clu[i] = 0;
    }

    for(i = 0; i < num_of_point; i++){
        int index = 0;
        min = all_data[i].distance[0];

        for(j = 1; j < num_of_class; j++){     //筛选到簇心欧几里德最小的
            if(all_data[i].distance[j] < min){
                min = all_data[i].distance[j];
                index = j;
            }
        }

        //划分簇集
        cluster.clu[index][ cluster.num_of_each_clu[index] ] = i;
        cluster.num_of_each_clu[index]++;
    }

    //计算所有样本的平方误差和
    cluster.Je = 0;
    for(i = 0; i < num_of_class; i++){
        for(j = 0; j < cluster.num_of_each_clu[i]; j++){
            //样本到簇心的值即为其欧几里德距离
            cluster.Je +=pow(all_data[cluster.clu[i][j]].distance[i],2);
        }
    }

    cluster.old_fitness = cluster.fitness; //前一次迭代适应度值
    cout<<"old_fitness = "<<cluster.old_fitness<<"\n";
    cluster.fitness = cluster.Je;   //所有样本平方误差和即为适应度值
}

/* 类中心更新总接口 */
void Kmeans::Make_new_center(){
    if(type_of_center_update == 1){
        Make_new_center_aver();
    }
}

/* 更新类中心，即求其类的平均距离为新的类中心  */
void Kmeans::Make_new_center_aver(){
    int i, j, n;
    double tmp_sum;

    for(i = 0; i < num_of_class; i++){
        for(j = 0; j < dimension_of_data; j++){
            tmp_sum = 0;
            for(n = 0; n < cluster.num_of_each_clu[i]; n++){
                //第i个簇的第j维数的所有数据和
                tmp_sum += all_data[cluster.clu[i][n]].p[j];
            }
            //取平均数得到新的簇中心
            cluster.clu_cent[i].p[j] = tmp_sum / cluster.num_of_each_clu[i];
        }
    }
}


/*  输出信息函数       *
  * 显示格式为：       *
  * 质心num_of_class:             *
  * dimension_of_data维的质心数据     *
  * 簇类num_of_class              *
  * dimension_of_data维属于簇类num_of_class的数据*/
void Kmeans::output_info(int flag, ofstream &output){
    int i, j, n;

    for(i = 0; i < num_of_class; i++){
        if(flag == 0){
            output<<"初始化类别中心"<<i+1<<":\n";
            for(n = 0; n < dimension_of_data; n++){
                output<<cluster.clu_cent[i].p[n]<<"\t";
            }
        }else{
            if(flag == 1){
                output<<"最终类别中心"<<i+1<<":\n";
                for(n = 0; n < dimension_of_data; n++){
                    output<<cluster.clu_cent[i].p[n]<<"\t";
                }
            }
        }

        output<<"\n类别"<<i+1<<":\n";
        for(j = 0; j < cluster.num_of_each_clu[i]; j++){
            output<<name_of_all_data[cluster.clu[i][j]]<<"\t";
            for(n = 0; n < dimension_of_data; n++){
                output<<all_data[cluster.clu[i][j]].p[n]<<"\t";
            }
            output<<"\n";
        }
    }
}
