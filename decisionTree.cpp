/* 
    �������㷨ʵ��
    Garker-gan
    2020-11-24 �������ʵ��
    2020-11-27 ������Ϣ�صļ���
    2020-11-28 ������Ϣ����ļ���
 */
#include<iostream>
#include<algorithm>
#include<sstream>       //ostringstream����������� istringstream����������� stringstream���������������
#include<string>
#include<vector>
#include<map>
#include<set>
#include<math.h>
using namespace std;

vector<string>data_attribute;       //���� ɫ��,����,����,����
map<string,vector<string>>data_attribute_value;     //ÿ�����Զ�Ӧ������ֵ,�� ɫ��={����,�ں�,ǳ��}
map<string,vector<string>>data_table;            //����ѵ�����ı��
int N;      //��������
class decisionTree
{
private:
    /* data */
public:
    // decisionTree(map<string,vector<string>>data_table,vector<string>data_attribute);
    decisionTree();
    ~decisionTree();

    // ������Ϣ��
    double cal_Entropy(map<string,vector<string>>temp_table);

    // ������Ϣ����ֵѰ���������
    string findBestAttribute();
};

decisionTree::decisionTree(/* args */)
{

}

decisionTree::~decisionTree()
{
}

// ������Ϣ��
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

// ������Ϣ����ֵѰ���������
string decisionTree::findBestAttribute()
{
    string best_attribute;
    double gain = 0;
    double temp_gain = cal_Entropy(data_table);
    // ����һ���ӱ�
    map<string,vector<string>>sub_data_table;
    for(int i = 0; i < data_attribute.size();i++)
    {
        // cout<<"enter circle 1 "<<i<<endl;
        string temp_attr;
        for (int j = 0; j < data_attribute_value[data_attribute[i]].size() ; j++)
        {
            // cout<<"enter circle 2 "<<j<<endl;
            int sample_num = 0;     //��������
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

//���뺯��
void date_input()
{
    //��������
    string attrubute_line;      //�����������������
    string attribute;           //�����е������б��浥������
    cout<<"����������:"<<endl;
    getline(cin,attrubute_line);
    istringstream get_attribute(attrubute_line);
    while(get_attribute>>attribute)
    {
        // ������ѹ��������
        data_attribute.push_back(attribute);
    }

    //������������ȥ��
    data_attribute.pop_back();

    //����ѵ��������
    // int N;
    cout<<"��������������:"<<endl;
    cin>>N;
    cin.ignore();

    cout<<"����������:"<<endl;
    for(int i = 0;i < N;i++)
    {
        string attribute_value_line;
        string attribute_value_temp;
        getline(cin,attribute_value_line);
        istringstream get_attribute_value(attribute_value_line);
        for (int j = 0; j < data_attribute.size(); j++)
        {
            get_attribute_value>>attribute_value_temp;
            // ����������ֵ��Ӧ�����Դ�����
            data_table[data_attribute[j]].push_back(attribute_value_temp);
        }
    }  
        // ʹ��set��ÿ�����Զ�Ӧ�ļ�������ֵ���а�
    for(int i = 0;i < data_attribute.size();i++)
    {
        set<string>attributeValue;
        for(int j = 0; j < N;j++)
        {
            string temp = data_table[data_attribute[i]][j];
            // ����Ӧ�ļ�������ֵ����,�Ҳ��ظ�
            attributeValue.insert(temp);
        }
        // ��ÿ�����Զ�Ӧ�ļ�������ֵ������map��
        for (set<string>::iterator it = attributeValue.begin();it != attributeValue.end();it++)
        {
            data_attribute_value[data_attribute[i]].push_back(*it);
        }
    }
}

// ������� 
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
    cout<<"��Ϣ��Ϊ��"<<endl;
    cout<<d.cal_Entropy(data_table)<<endl;
    cout<<"�������Ϊ��"<<endl;
    cout<<d.findBestAttribute()<<endl;
    return 0; 
} 