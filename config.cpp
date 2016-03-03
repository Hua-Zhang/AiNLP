
#include"config.h"

Config::Config(){
}

Config::~Config(){
}

//初始化
void Config::init(){
    analyse_of_config_file(config_file);
}

//解析配置文件
void Config::analyse_of_config_file(string configfile){
    ifstream in(configfile.c_str());

    if( !in ){
        cerr<<"对不起，Config::analyse_of_config_file 打开配置文件失败！\n";
        exit(1);
    }

    string line, word;
    while(getline(in,line)){
        istringstream inword(line);
        inword>>word;
        if(word=="point-num"){
            inword>>word;
            num_of_point = atoi(word.c_str());
            continue;
        }else if(word=="data-dimen"){
            inword>>word;
            dimension_of_data = atoi(word.c_str());
            continue;
        }else if(word=="point-dis"){
            inword>>word;
            type_of_distance = atoi(word.c_str());
            continue;
        }else if(word=="center_update"){
            inword>>word;
            type_of_center_update = atoi(word.c_str());
            continue;
        }else if(word=="init_point_choice"){
            inword>>word;
            type_of_init_point_choice = atoi(word.c_str());
            continue;
        }else if(word=="iter_limit"){   
            inword>>word;
            iter_limit = atoi(word.c_str());
            continue;
        }else if(word=="error_limit"){   
            inword>>word;
            error_limit = atof(word.c_str());
            continue;
        }
    }

    if(num_of_point == -1 || dimension_of_data == -1){
        acquire_info_of_data(input_file, num_of_point, dimension_of_data);
    }
	 
    in.close();
}

//当用户没有给出数据点的个数或数据的
void Config::acquire_info_of_data(string input_file, int & num_of_point, int & dimension_of_data){
    ifstream in(input_file.c_str());

    if( !in ){       
        cerr<<"对不起，Config::acquire_info_of_data 打开输入文件失败！\n";
        exit(1);
    }
    
    if(num_of_point == -1 && dimension_of_data == -1){
        num_of_point = 0;
        dimension_of_data = -1;   //数据的第一个位置默认为产品名，后面是数据的的向量
	
        string line, word;
        getline(in,line);
        num_of_point++;
	
        istringstream inword(line);
        while(inword>>word){       //特征向量一定要正确，否则会出错	    
            dimension_of_data++;
        }

        while(getline(in,line)){   //数据不要有空行，否则会出错		    
            num_of_point++;
        }
    }else if(num_of_point == -1){
        num_of_point++;
        string line;
        while(getline(in,line)){		
            num_of_point++;
        }
    }else if(dimension_of_data == -1){
        string line, word;
        getline(in,line);
        istringstream inword(line);
        while(inword>>word){
            dimension_of_data++;
        }
    }
    //总之，数据一定要正确
	
    in.close();
}


void Config::show(){
    cout<<"\n";
    cout<<"系统的配置情况...:\n";
    cout<<"输入文件为："<<input_file<<"\n";
    cout<<"结果文件为："<<result_file<<"\n";
    cout<<"配置文件为："<<config_file<<"\n";
    cout<<"数据点的个数为："<<num_of_point<<"\n";
    cout<<"类别个数为："<<num_of_class<<"\n";
    cout<<"数据维度为："<<dimension_of_data<<"\n";
    cout<<"数据点距离的计算方法为："<<type_of_distance<<"\n";
    cout<<"初始中心点选择方法为："<<type_of_init_point_choice<<"\n";
    cout<<"类别中心点更新方法为："<<type_of_center_update<<"\n";
    cout<<"误差限制为："<<error_limit<<"\n";
    cout<<"迭代次数限制为："<<iter_limit<<"\n";
    cout<<"\n";
}
