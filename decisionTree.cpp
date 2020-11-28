/* 
    决策树算法实现
    Garker-gan
    2020-11-24 输入输出实现
    2020-11-27 增加信息熵的计算
    2020-11-28 增加信息增益的计算
 */
#include<iostream>
#include<algorithm>
#include<sstream>       //ostringstream串流输出操作 istringstream串流输入操作 stringstream串流输入输出操作
#include<string>
#include<vector>
#include<map>
#include<set>
#include<math.h>
using namespace std;

vector<string>data_attribute;       //属性 色泽,根蒂,敲声,纹理
map<string,vector<string>>data_attribute_value;     //每个属性对应的属性值,如 色泽={青绿,乌黑,浅白}
map<string,vector<string>>data_table;            //建立训练集的表格
int N;      //样本数量
class decisionTree
{
private:
    /* data */
public:
    // decisionTree(map<string,vector<string>>data_table,vector<string>data_attribute);
    decisionTree();
    ~decisionTree();

    // 计算信息熵
    double cal_Entropy(map<string,vector<string>>temp_table);

    // 计算信息增益值寻找最佳属性
    string findBestAttribute();
};

decisionTree::decisionTree(/* args */)
{

}

decisionTree::~decisionTree()
{
}

// 计算信息熵
double decisionTree::cal_Entropy(map<string,vector<string>>temp_table)
{
    map<string,int>class_map;
    int n = temp_table[data_attribute[0]].size();
    for(int i = 0;i < data_attribute.size();i++)
    {
        for (int j = 0; j < n; j++)
        {
            string attr_value_name = temp_table[data_attribute[i]][j];
            class_map[attr_value_name]++;
        }
    }
    map<string,int>::iterator it = class_map.begin();
    double Ent = 0;
    for(;it != class_map.end();it++)
    {
        double p = (double)it->second/N;
        Ent -= (double)p * (log(p)/log(2));
    }
    return Ent;
}   

// 计算信息增益值寻找最佳属性
string decisionTree::findBestAttribute()
{
    string best_attribute;
    double gain = 0;
    double temp_gain = cal_Entropy(data_table);
    // 创建一个子表
    map<string,vector<string>>sub_data_table;
    for(int i = 0; i < data_attribute.size();i++)
    {
        // cout<<"enter circle 1 "<<i<<endl;
        string temp_attr;
        for (int j = 0; j < data_attribute_value[data_attribute[i]].size() ; j++)
        {
            // cout<<"enter circle 2 "<<j<<endl;
            int sample_num = 0;     //样本数量
            temp_attr = data_attribute_value[data_attribute[i]][j];
            for(int k = 0;k < N;k++)
            {
                // cout<<"enter circle 3 "<<k<<endl;
                 if(temp_attr == data_table[data_attribute[i]][k])
                 {
                     sample_num++;
                     for(int m = 0;m < data_attribute.size();m++)
                     {
                        //  cout<<"enter circle 4 "<<m<<endl;
                         sub_data_table[data_attribute[m]].push_back(data_table[data_attribute[m]][k]);
                     }
                 }
            }

            // for(int i = 0;i < sub_data_table[data_attribute[i]].size();i++)
            // {
            //     for(int j = 0;j < data_attribute.size();j++)
            //     {
            //         cout<<sub_data_table[data_attribute[j]][i]<<"  ";
            //     }
            //     cout<<endl;
            // }

            // cout<<cal_Entropy(sub_data_table)<<endl;
            temp_gain -= (double)sample_num/N * cal_Entropy(sub_data_table);
            // cout<<temp_gain<<endl;
        }
        if(temp_gain > gain)
        {
            gain = temp_gain;
            best_attribute = temp_attr;
        }
    }
    return best_attribute;
}

//输入函数
void date_input()
{
    //输入属性
    string attrubute_line;      //保存输入的整行属性
    string attribute;           //从整行的属性中保存单个属性
    cout<<"请输入属性:"<<endl;
    getline(cin,attrubute_line);
    istringstream get_attribute(attrubute_line);
    while(get_attribute>>attribute)
    {
        // 将属性压入数组中
        data_attribute.push_back(attribute);
    }

    //将类别这个属性去除
    data_attribute.pop_back();

    //输入训练集数量
    // int N;
    cout<<"请输入样本数量:"<<endl;
    cin>>N;
    cin.ignore();

    cout<<"请输入样本:"<<endl;
    for(int i = 0;i < N;i++)
    {
        string attribute_value_line;
        string attribute_value_temp;
        getline(cin,attribute_value_line);
        istringstream get_attribute_value(attribute_value_line);
        for (int j = 0; j < data_attribute.size(); j++)
        {
            get_attribute_value>>attribute_value_temp;
            // 将各个属性值对应于属性存入表格
            data_table[data_attribute[j]].push_back(attribute_value_temp);
        }
    }  
        // 使用set对每个属性对应的几种属性值进行绑定
    for(int i = 0;i < data_attribute.size();i++)
    {
        set<string>attributeValue;
        for(int j = 0; j < N;j++)
        {
            string temp = data_table[data_attribute[i]][j];
            // 将对应的几种属性值保存,且不重复
            attributeValue.insert(temp);
        }
        // 将每个属性对应的几种属性值保存在map中
        for (set<string>::iterator it = attributeValue.begin();it != attributeValue.end();it++)
        {
            data_attribute_value[data_attribute[i]].push_back(*it);
        }
    }
}

// 输出函数 
void data_print()
{
    // for(int i = 0;i < N;i++)
    // {
    //     for(int j = 0;j < data_attribute.size();j++)
    //     {
    //         cout<<data_table[data_attribute[j]][i]<<"  ";
    //     }
    //     cout<<endl;
    // }
    for(int i = 0;i < data_attribute.size();i++)
    {
        for(int j = 0; j < data_attribute_value[data_attribute[i]].size();j++)
        {
            cout<<data_attribute_value[data_attribute[i]][j]<<" ";
        }
        cout<<endl;
    }
}



int main()
{
    date_input();
    data_print();
    decisionTree d;
    cout<<"信息熵为："<<endl;
    cout<<d.cal_Entropy(data_table)<<endl;
    cout<<"最佳属性为："<<endl;
    cout<<d.findBestAttribute()<<endl;
    return 0; 
} 