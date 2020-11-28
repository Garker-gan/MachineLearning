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

vector<string>data_Attribute;       //���� ɫ��,����,����,���� 
vector<string>full_Attribute;       //���� ɫ��,����,����,���� ���
map<string,vector<string>>data_AttrValues;     //ÿ�����Զ�Ӧ������ֵ,�� ɫ��={����,�ں�,ǳ��}
map<string,vector<string>>data_Table;            //����ѵ�����ı��
int N;      //��������

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

    // ������Ϣ��
    double cal_Entropy(map<string,vector<string>>temp_table);

    // ������Ϣ����ֵѰ���������
    string findBestAttribute();

    // �����ӽڵ�
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

// ������Ϣ��
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

// ������Ϣ����ֵѰ���������
string decisionTree::findBestAttribute()
{
    string best_attribute;
    double gain = 0;
    double temp_gain = cal_Entropy(data_Table);
    // ����һ���ӱ�
    map<string,vector<string>>sub_data_table;
    for(int i = 0; i < data_Attribute.size();i++)
    {
        // cout<<"enter circle 1 "<<i<<endl;
        string temp_attr = data_Attribute[i];
        for (int j = 0; j < data_AttrValues[data_Attribute[i]].size() ; j++)
        {
            // cout<<"enter circle 2 "<<j<<endl;
            int sample_num = 0;     //��������
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

// �����ӽڵ�
void decisionTree::generate_ChildNode()
{
    map<string,int>class_count;
    // ͳ�����һ�е������Ŀ�����ùϣ����ϵ���Ŀ
    for(int i = 0; i < N;i++)
    {   
        class_count[data_Table[full_Attribute[Attribute_num-1]][i]]++;
    }

    //��һ�����
    cout<<"case 1"<<endl;
    if(class_count.size() == 1)
    {
        map<string,int>::iterator it;
        node_Attribute = it->first;
        return;
    }

    // �ڶ������
    cout<<"case 2"<<endl;
    bool isAllSame = false;
	for (int i = 0; i < data_Attribute.size(); i++)
	{
		isAllSame = true;
		vector<string> temp;
		temp = data_Table[data_Attribute[i]];
		for (int j = 1; j < temp.size(); j++)
		{
			//ֻҪ��һ����ͬ�������˳�
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
	//�����Լ�Ϊ�ջ��������е�ȫ������ȡֵ��ͬ
	if (data_Attribute.empty()||isAllSame)
	{
		//�ҳ����������������ֵĸ����������ýڵ���Ϊ����
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


    // ���������
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

        //���Ӽ�Ϊ�գ�����֧�ڵ㣨�ӽڵ㣩���ΪҶ�ڵ㣬���ΪMyDateTable����������
		if (sub_Table.empty())
		{
            cout<<"case 3-1"<<endl;
			//�����ӽڵ�
			decisionTree *p = new decisionTree(sub_Table,full_Attribute,data_AttrValues,data_Attribute);
			//�ҳ����������࣬��Ϊ�ӽڵ������
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
			//������Ҷ�ڵ�����
			p->set_NodeAttribute(childNode_Attribute);
			//���ӽڵ����childNode��Ԥ��������ʱ����õ�
			childNode[best_AttValue] = p;
		}

        else
        {
            cout<<"case 3-2"<<endl;
            vector<string> child_RemainAtt;
			child_RemainAtt = data_Attribute;
			//�ҳ�child_RemainAtt�е�������������ȵ�����
			vector<string>::iterator it = child_RemainAtt.begin();
			for (; it != child_RemainAtt.end(); it++)
			{
				if (*it == best_Attribute)
				{
					break;
				}
			}
			//ɾ��
			child_RemainAtt.erase(it);
			//�����ӽڵ�
			decisionTree * pt = new decisionTree(sub_Table,full_Attribute,data_AttrValues,data_Attribute);
			//���ӽڵ����childNode
			childNode[best_AttValue] = pt;
            cout<<"test"<<endl;
			//�ӽڵ��ٵ���generate_ChildNode����
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

//���뺯��
void  data_input()
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
        full_Attribute.push_back(attribute);
    }

    //������������ȥ��
    data_Attribute = full_Attribute;
    data_Attribute.pop_back();

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
        for (int j = 0; j < data_Attribute.size(); j++)
        {
            get_attribute_value>>attribute_value_temp;
            // ����������ֵ��Ӧ�����Դ�����
            data_Table[data_Attribute[j]].push_back(attribute_value_temp);
        }
    }  
        // ʹ��set��ÿ�����Զ�Ӧ�ļ�������ֵ���а�
    for(int i = 0;i < data_Attribute.size();i++)
    {
        set<string>attributeValue;
        for(int j = 0; j < N;j++)
        {
            string temp = data_Table[data_Attribute[i]][j];
            // ����Ӧ�ļ�������ֵ����,�Ҳ��ظ�
            attributeValue.insert(temp);
        }
        // ��ÿ�����Զ�Ӧ�ļ�������ֵ������map��
        for (set<string>::iterator it = attributeValue.begin();it != attributeValue.end();it++)
        {
            data_AttrValues[data_Attribute[i]].push_back(*it);
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
    // cout<<"��Ϣ��Ϊ��"<<endl;
    // cout<<d.cal_Entropy(data_Table)<<endl;
    // cout<<"�������Ϊ��"<<endl;
    // cout<<d.findBestAttribute()<<endl;

    d.generate_ChildNode();

    vector<string>test_set;
    string input_Line,temp;
    cout<<endl;
    cout<<"������һ�в��Լ���"<<endl;
    getline(cin,input_Line);
    istringstream input_sample(input_Line);
    while (input_sample>>temp)
    {
        test_set.push_back(temp);
    }
    cout<<endl;
    cout<<"���Խ��Ϊ��"<<d.findClass(test_set)<<endl;
    return 0; 
} 