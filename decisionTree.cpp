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

vector<string>data_Attribute;       //属性 色泽,根蒂,敲声,纹理 
vector<string>full_Attribute;       //属性 色泽,根蒂,敲声,纹理 类别
map<string,vector<string>>data_AttrValues;     //每个属性对应的属性值,如 色泽={青绿,乌黑,浅白}
map<string,vector<string>>data_Table;            //建立训练集的表格
int N;      //样本数量

class decisionTree
{
private:
    int Attribute_num;
    string node_Attribute;
    vector<string>full_Attribute;
    vector<string>node_AttrValues;
    vector<string>data_Attribute;
    map<string,decisionTree *>childNode;
    map<string,vector<string>>data_Table;
    map<string,vector<string>>data_AttrValues;
public:
    decisionTree(map<string,vector<string>>data_table,vector<string>full_attribute,map<string,vector<string>>data_attrValues,vector<string>data_attribute);
    ~decisionTree();

    // 计算信息熵
    double cal_Entropy(map<string,vector<string>>temp_table);

    // 计算信息增益值寻找最佳属性
    string findBestAttribute();

    // 生成子节点
    void generate_ChildNode();

    string findClass(vector<string>attribute);

    void set_NodeAttribute(string atttribute)
    {
        this->node_Attribute = atttribute;
    }
};

decisionTree::decisionTree(map<string,vector<string>>data_table,vector<string>full_attribute,map<string,vector<string>>data_attrValues,vector<string>data_attribute)
{
    Attribute_num = full_attribute.size() - 1;
    data_Table = data_table;
    data_Attribute = data_attribute;
    data_AttrValues = data_attrValues;
    full_Attribute = full_attribute;
}

decisionTree::~decisionTree()
{
}

// 计算信息熵
double decisionTree::cal_Entropy(map<string,vector<string>>temp_table)
{
    map<string,int>class_map;
    int n = temp_table[data_Attribute[0]].size();
    for(int i = 0;i < data_Attribute.size();i++)
    {
        for (int j = 0; j < n; j++)
        {
            string attr_value_name = temp_table[data_Attribute[i]][j];
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
    double temp_gain = cal_Entropy(data_Table);
    // 创建一个子表
    map<string,vector<string>>sub_data_table;
    for(int i = 0; i < data_Attribute.size();i++)
    {
        // cout<<"enter circle 1 "<<i<<endl;
        string temp_attr = data_Attribute[i];
        for (int j = 0; j < data_AttrValues[data_Attribute[i]].size() ; j++)
        {
            // cout<<"enter circle 2 "<<j<<endl;
            int sample_num = 0;     //样本数量
            string temp_attrValues = data_Table[data_Attribute[i]][j];
            for(int k = 0;k < N;k++)
            {
                // cout<<"enter circle 3 "<<k<<endl;
                 if(temp_attrValues == data_Table[data_Attribute[i]][k])
                 {
                     sample_num++;
                     for(int m = 0;m < data_Attribute.size();m++)
                     {
                        //  cout<<"enter circle 4 "<<m<<endl;
                         sub_data_table[data_Attribute[m]].push_back(data_Table[data_Attribute[m]][k]);
                     }
                 }
            }

            temp_gain -= (double)sample_num/N * cal_Entropy(sub_data_table);
            // cout<<temp_gain<<endl;
        }
        if(temp_gain > gain)
        {
            gain = temp_gain;
            best_attribute = temp_attr;
            // cout<<gain<<endl; 
        }
    }
    return best_attribute;
}

// 生成子节点
void decisionTree::generate_ChildNode()
{
    map<string,int>class_count;
    // 统计最后一列的类别数目，即好瓜，坏瓜的数目
    for(int i = 0; i < N;i++)
    {   
        class_count[data_Table[full_Attribute[Attribute_num-1]][i]]++;
    }

    //第一种情况
    cout<<"case 1"<<endl;
    if(class_count.size() == 1)
    {
        map<string,int>::iterator it;
        node_Attribute = it->first;
        return;
    }

    // 第二种情况
    cout<<"case 2"<<endl;
    bool isAllSame = false;
	for (int i = 0; i < data_Attribute.size(); i++)
	{
		isAllSame = true;
		vector<string> temp;
		temp = data_Table[data_Attribute[i]];
		for (int j = 1; j < temp.size(); j++)
		{
			//只要有一个不同，即可退出
			if (temp[0] != temp[j])
			{
				isAllSame = false;
				break;
			}
		}
		if (isAllSame == false)
		{
			break;
		}
	}
	//若属性集为空或者样本中的全部属性取值相同
	if (data_Attribute.empty()||isAllSame)
	{
		//找出数量最多的类别及其出现的个数，并将该节点标记为该类
		map<string, int>::iterator it = class_count.begin();
		node_Attribute = it->first;
		int max = it->second;
		it++;
		for (; it != class_count.end(); it++)
		{
			int num = it->second;
			if (num > max)
			{
				node_Attribute = it->first;
				max = num;
			}
		}
		return;
	}


    // 第三种情况
    cout<<"case 3"<<endl;
    string best_Attribute = findBestAttribute();
    node_Attribute = best_Attribute;
    map<string,vector<string>>sub_Table;
    for(int i = 0 ; i < data_AttrValues[best_Attribute].size();i++)
    {
        string best_AttValue = data_AttrValues[best_Attribute][i];
        for(int j = 0;j < N;j++)
        {
            if(best_AttValue == data_Table[best_Attribute][j])
            {
                for(int k = 0; k < data_Attribute.size();k++)
                {
                    sub_Table[data_Attribute[k]].push_back(data_Table[data_Attribute[k]][j]);
                }
            }
        }

        //若子集为空，将分支节点（子节点）标记为叶节点，类别为MyDateTable样本最多的类
		if (sub_Table.empty())
		{
            cout<<"case 3-1"<<endl;
			//生成子节点
			decisionTree *p = new decisionTree(sub_Table,full_Attribute,data_AttrValues,data_Attribute);
			//找出样本最多的类，作为子节点的属性
			map<string, int>::iterator it = class_count.begin();
			string childNode_Attribute = it->first;
			int max_Num = it->second;
			it++;
			for (; it != class_count.end(); it++)
			{
				if (it->second > max_Num)
				{
					max_Num = it->second;
					childNode_Attribute = it->first;
				}
			}
			//设置子叶节点属性
			p->set_NodeAttribute(childNode_Attribute);
			//将子节点存入childNode，预测样本的时候会用到
			childNode[best_AttValue] = p;
		}

        else
        {
            cout<<"case 3-2"<<endl;
            vector<string> child_RemainAtt;
			child_RemainAtt = data_Attribute;
			//找出child_RemainAtt中的与该最佳属性相等的属性
			vector<string>::iterator it = child_RemainAtt.begin();
			for (; it != child_RemainAtt.end(); it++)
			{
				if (*it == best_Attribute)
				{
					break;
				}
			}
			//删除
			child_RemainAtt.erase(it);
			//生成子节点
			decisionTree * pt = new decisionTree(sub_Table,full_Attribute,data_AttrValues,data_Attribute);
			//将子节点存入childNode
			childNode[best_AttValue] = pt;
            cout<<"test"<<endl;
			//子节点再调用generate_ChildNode函数
			// pt->generate_ChildNode();
            node_Attribute = best_Attribute;
            cout<<"end case 3-2"<<endl;
        }
        
    }
}

string decisionTree::findClass(vector<string>attribute)
{
    if(childNode.size())
    {
        string attribute_value;
        for(int i = 0; i < data_AttrValues[node_Attribute].size();i++)
        {
            for(int j = 0;j < attribute.size();j++)
            {
                if(attribute[j] == data_AttrValues[node_Attribute][i])
                {
                    attribute_value = attribute[j];
                    break;
                }
            }
            if(!attribute_value.empty())
                break;
        }
        decisionTree *p = childNode[attribute_value];
        return p->findClass(attribute);
    }
    else
    {
        return node_Attribute;
    }
}

//输入函数
void  data_input()
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
        full_Attribute.push_back(attribute);
    }

    //将类别这个属性去除
    data_Attribute = full_Attribute;
    data_Attribute.pop_back();

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
        for (int j = 0; j < data_Attribute.size(); j++)
        {
            get_attribute_value>>attribute_value_temp;
            // 将各个属性值对应于属性存入表格
            data_Table[data_Attribute[j]].push_back(attribute_value_temp);
        }
    }  
        // 使用set对每个属性对应的几种属性值进行绑定
    for(int i = 0;i < data_Attribute.size();i++)
    {
        set<string>attributeValue;
        for(int j = 0; j < N;j++)
        {
            string temp = data_Table[data_Attribute[i]][j];
            // 将对应的几种属性值保存,且不重复
            attributeValue.insert(temp);
        }
        // 将每个属性对应的几种属性值保存在map中
        for (set<string>::iterator it = attributeValue.begin();it != attributeValue.end();it++)
        {
            data_AttrValues[data_Attribute[i]].push_back(*it);
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
    for(int i = 0;i < data_Attribute.size();i++)
    {
        for(int j = 0; j < data_Table[data_Attribute[i]].size();j++)
        {
            cout<<data_AttrValues[data_Attribute[i]][j]<<" ";
        }
        cout<<endl;
    }
}



int main()
{
    data_input();
    decisionTree d(data_Table,full_Attribute,data_AttrValues,data_Attribute);

    // data_print();
    // cout<<"信息熵为："<<endl;
    // cout<<d.cal_Entropy(data_Table)<<endl;
    // cout<<"最佳属性为："<<endl;
    // cout<<d.findBestAttribute()<<endl;

    d.generate_ChildNode();

    vector<string>test_set;
    string input_Line,temp;
    cout<<endl;
    cout<<"请输入一行测试集："<<endl;
    getline(cin,input_Line);
    istringstream input_sample(input_Line);
    while (input_sample>>temp)
    {
        test_set.push_back(temp);
    }
    cout<<endl;
    cout<<"测试结果为："<<d.findClass(test_set)<<endl;
    return 0; 
} 