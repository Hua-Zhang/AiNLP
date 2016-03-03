/*
 * kmeans 聚类工具
 */
#include"config.h"
#include"kmeans.h"

using namespace std;

void Usage();
void Head();
void ParserCommandLine(int argc, char** argv, Config& conf);

int main(int argc, char** argv){
    Head();

    //初始化系统的参数配置
    Config conf;
    ParserCommandLine(argc, argv, conf);
    conf.init();
    conf.show();

    //开始执行
    Kmeans K(conf);
    K.clustering();

    return 0;
}

void ParserCommandLine(int argc, char** argv, Config& conf){
    if(argc != 9){
        Usage();
        exit(1);
    }

    string tmp;
    for(int i=1; i<argc; i++){
        tmp = argv[i];
        if(tmp=="-conf"){
            conf.config_file = argv[i+1];
            i++;
        }else if(tmp=="-input-file"){
            conf.input_file = argv[i+1];
            i++;
        }else if(tmp=="-result-file"){
            conf.result_file = argv[i+1];
            i++;
        }else if(tmp=="-class-num"){
            conf.num_of_class = atoi(argv[i+1]);
            i++;
        }else{
            Usage();
        //  cout<<"I am here!\n";
            exit(1);
        }
    }
}

void Head(){
    cout<<"\n";
    cout<<"*****************************************\n";
    cout<<"K-means 聚类工具                           \n";
    cout<<"*****************************************\n";
    cout<<"\n";
}

void Usage(){
    cout<<"\n";
    cout<<"K-means 聚类工具命令指南...:\n\n";
    cout<<"-conf filepath  指定配置文件的路径。\n\n";
    cout<<"-input-file filepath 指定待处理的文件路径。\n\n";
    cout<<"-result-file filepath 指定聚类结果的文件路径。\n\n";
    cout<<"-class-num  number  指定类别个数。\n\n";
    cout<<"其他参数可在配置文件中指定。\n\n";
    cout<<"\n";
}
