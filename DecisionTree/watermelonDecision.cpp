/* 
    �����ھ����ҵ-��ѡ����(����������)
    Garker-gan
    2020-12-5
 */
#include<iostream>
#include<algorithm>
#include<sstream>      
#include<string>
#include<vector>
#include<map>
#include<set>
#include<math.h>
#include <fstream>
#include <sstream>
using namespace std;

//ȫ�ֱ���
// ��������
int N;
//�����������������
int attribute_num = 0;
//�����������飬��ſ��ܵ����ԣ��������
vector<string> data_Attributes;//���ڱ����ݼ���˵���ǣ�ɫ�� ���� ���� ���� �겿 ���� ���
//��������Զ�Ӧ������ֵ
map<string, vector<string>> data_AttValues;//���磺ɫ��={���� �ں� ǳ��}
//����ʣ�����ԣ���������������Ҫ���ں����㷨�ĵݹ飩
vector<string> remain_Attributes;//ɫ�� ���� ���� ���� �겿 ����
//�������ݱ�,����-����ֵ��ȫ�����ݵ�����ֵ����ͬһ�����飩
map<string, vector<string>>data_Table;//���ű�
 
class Tree_Node
{
public:
	//���캯������������Ϊ�����ݼ����������ݱ����������е����԰������ÿ�����Կ��ܵ�ȡֵ���ɵı�ʣ���δ�����ֵ�����
	Tree_Node(map<string, vector<string>> temp_Table, vector<string> temp_Attribute,map<string, vector<string>> data_AttValues, vector<string> temp_remain);
	//�����ӽڵ�
	void generate_ChildNode();
	//������Ϣ���� Ѱ�����Ż�������
	string findBestAttribute();
	//������Ϣ��
	double calc_Entropy(map<string, vector<string>> temp_Table);
	//���ýڵ������
	void set_NodeAttribute(string atttribute);
	//�����������ԣ������ݽ��з���
	string findClass(vector<string> attributes);
	// virtual ~Tree_Node();
private:
	//���Ը��������������
	int attribute_Num;
	//���ڵ������
	string node_Attribute;
	//���ݼ�����
	vector<string> data_Attribute;
	//���ڵ����������ֵ
	vector<string> node_AttValues;
	//ʣ�����Լ�
	vector<string>remain_Attributes;
	//�ӽڵ�,���ڵ����Զ�Ӧ������ֵ���ӽڵ��ַ����һһӳ��
	//Ϊ��˵���ýڵ�ΪҶ�ڵ�
	map<string, Tree_Node *> childNode;
	//�������ϱ�
	map<string, vector<string>> MyDateTable;
	//��������Զ�Ӧ������ֵ
	map<string, vector<string>> data_AttValues;
};

Tree_Node::Tree_Node(map<string, vector<string>> temp_Table,vector<string> temp_Attribute, map<string, vector<string>> data_AttValues, vector<string> temp_remain)
{
	//ȫ������,�������
	data_Attribute = temp_Attribute;
	//���Ը��������������
	attribute_Num = (int)temp_Attribute.size() - 1;
	//�����Զ�Ӧ������ֵ
	this->data_AttValues = data_AttValues;
	//���Ա�
	MyDateTable = temp_Table;
	//ʣ�����Լ�
	remain_Attributes = temp_remain;
}

//������Ϣ��
double Tree_Node::calc_Entropy(map<string, vector<string>> temp_Table)
{
	map<string, vector<string>> table = temp_Table;
	//���ݼ�������������
	int sample_Num = (int)temp_Table[data_Attribute[0]].size();
	//�������ݼ��е��������
	map<string, int> class_Map;
	for (int i = 0; i < sample_Num; i++)
	{
		//data_Attribute[attribute_Num]��Ӧ�ľ������ݼ������
		string class_String = table[data_Attribute[attribute_Num]][i];
		class_Map[class_String]++;
	}
 
	map<string, int>::iterator it = class_Map.begin();
	//���������Ӧ������
	//vector<string> m_Class;
	vector<int> n_Class;
	
	for (; it != class_Map.end(); it++)
	{
		//m_Class.push_back(it->first);
		n_Class.push_back(it->second);
	}
	//������Ϣ��
	double Ent = 0;
	for (int k = 0; k < class_Map.size(); k++)
	{
		//����
		double p = (double) n_Class[k] / sample_Num;
		if (p == 0)
		{
			//�涨��p=0ʱ��plogp=0
			continue;
		}
		//c++��ֻ��log��ln�������ҪӦ�û��׹�ʽ
		Ent -= p * (log(p) / log(2));//��Ϣ��
	}
	
	return Ent;
}

	//Ѱ�����Ż���
	string Tree_Node::findBestAttribute()
	{
		//��������
		int N = (int)MyDateTable[data_Attribute[0]].size();
		//�������ڴ����������
		string best_Attribute;
		//��Ϣ����
		double gain = 0;
		//��ÿ��ʣ������
		for (int i = 0; i < remain_Attributes.size(); i++)
		{
			//������Ϣ���棬ѡȡ�����������������ּ�Ϊ���Ż���
			double temp_Gain = calc_Entropy(MyDateTable);//���ݹ�ʽ�Ƚ����ڵ����Ϣ�س�ʼ��������
			//�Ը����Ե����ݼ����з��ࣨ��ȡ������ֵ�������Ӽ���
			string temp_Att = remain_Attributes[i];//����ѡȡ������
			vector<string> remain_AttValues;//���Կ��ܵ�ȡֵ
			for (int j = 0; j < data_AttValues[temp_Att].size(); j++)
			{
				remain_AttValues.push_back(data_AttValues[temp_Att][j]);
			}
			
			//��ÿ������ֵ����Ϣ��
			for (int k = 0; k < remain_AttValues.size(); k++)
			{
				//����ֵ
				string temp_AttValues = remain_AttValues[k];
				int sample_Num = 0;//������ֵ��Ӧ��������
				//����map������Ÿ�����ֵ�µ������Ӽ�
				map<string, vector<string>>sub_DataTable;
				for (int l = 0; l < MyDateTable[temp_Att].size(); l++)
				{
					if (temp_AttValues == MyDateTable[temp_Att][l])
					{
						sample_Num++;
						//������������ѵ��������sub_DataTable
						for (int m = 0; m < data_Attribute.size(); m++)
						{
							sub_DataTable[data_Attribute[m]].push_back(MyDateTable[data_Attribute[m]][l]);
						}
					}
				}
				//�ۼ�ÿ����ֵ����Ϣ��
				temp_Gain -= (double)sample_Num / N * calc_Entropy(sub_DataTable);
			}
			//�Ƚ�Ѱ�����Ż�������
			if (temp_Gain > gain)
			{
				gain = temp_Gain;
				best_Attribute = temp_Att;
			}		
		}
	
		return best_Attribute;
}

void Tree_Node::set_NodeAttribute(string attribute)
{
	//���ýڵ������
	this->node_Attribute = attribute;
}

void Tree_Node::generate_ChildNode()
{
	//��������
	int N = (int)MyDateTable[data_Attribute[0]].size();
	
	//�����ݼ�������������������map���棬ֻ���ж����һ�м���
	map<string,int> category;
	for (int i = 0; i < N; i++)
	{	
		vector<string> temp_Class;
		temp_Class = MyDateTable[data_Attribute[attribute_Num]];
		category[temp_Class[i]]++;
	}
 
	//��һ�����
	//ֻ��һ����𣬱��ΪҶ�ڵ�
	if (1 == category.size())
	{
		map<string, int>::iterator it = category.begin();
		node_Attribute = it->first;
		return;
	}
	//�ڶ������
	//���ж����������Ƿ�ȡ��ֵͬ
	bool isAllSame = false;
	for (int i = 0; i < remain_Attributes.size(); i++)
	{
		isAllSame = true;
		vector<string> temp;
		temp = MyDateTable[remain_Attributes[i]];
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
	if (remain_Attributes.empty()||isAllSame)
	{
		//�ҳ����������������ֵĸ����������ýڵ���Ϊ����
		map<string, int>::iterator it = category.begin();
		node_Attribute = it->first;
		int max = it->second;
		it++;
		for (; it != category.end(); it++)
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
	//���������
	//��remian_attributes�л�����������
	string best_Attribute = findBestAttribute();
	//�����ڵ�����Ϊ��������
	node_Attribute = best_Attribute;
	//���������Ե�ÿ������ֵ
	for (int i = 0; i < data_AttValues[best_Attribute].size(); i++)
	{
		string best_AttValues = data_AttValues[best_Attribute][i];
		//�������Զ�Ӧ�����ݼ�D
		//����map������Ÿ�����ֵ�µ������Ӽ�
		map<string, vector<string>> sub_DataTable;
		for (int j = 0; j < MyDateTable[best_Attribute].size(); j++)
		{
			//Ѱ���������������ݼ�������ֵ��ͬ����������
			if (best_AttValues == MyDateTable[best_Attribute][j])
			{
				//�ҵ���Ӧ�����ݼ��������Ӽ���sub_DataTable����������ȫ�����Զ�Ҫ���룩
				for (int k = 0; k < data_Attribute.size(); k++)
				{
					sub_DataTable[data_Attribute[k]].push_back(MyDateTable[data_Attribute[k]][j]);
				}
			}
		}
		//���Ӽ�Ϊ�գ�����֧�ڵ㣨�ӽڵ㣩���ΪҶ�ڵ㣬���ΪMyDateTable����������
		if (sub_DataTable.empty())
		{
			//�����ӽڵ�
			Tree_Node * p = new Tree_Node(sub_DataTable, data_Attribute, data_AttValues, remain_Attributes);
			//�ҳ����������࣬��Ϊ�ӽڵ������
			map<string, int>::iterator it = category.begin();
			string childNode_Attribute = it->first;
			int max_Num = it->second;
			it++;
			for (; it != category.end(); it++)
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
			childNode[best_AttValues] = p;
		}
		else//����Ϊ�գ����ʣ������ֵ�޳������ԣ�����generate_ChildNode��������ϸ��
		{
			vector<string> child_RemainAtt;
			child_RemainAtt = remain_Attributes;
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
			Tree_Node * pt = new Tree_Node(sub_DataTable, data_Attribute, data_AttValues, child_RemainAtt);
			//���ӽڵ����childNode
			childNode[best_AttValues] = pt;
			//�ӽڵ��ٵ���generate_ChildNode����
			pt->generate_ChildNode();
		}
	}
 
}

//����Ϊ��Ԥ���������������Լ���
string Tree_Node::findClass(vector<string> attributes)
{
	//�������ӽڵ�
	if (childNode.size() != 0)
	{
		//�ҳ�������������뱾�ڵ����Զ�Ӧ������ֵ���Ա�Ѱ���¸��ڵ㣬ֱ���ҵ�Ҷ�ڵ�
		string attribute_Value;
		for (int i = 0; i < data_AttValues[node_Attribute].size(); i++)
		{
			for (int j = 0; j < attributes.size(); j++)
			{
				//data_AttValues[node_Attribute]Ϊ����node_Attribute��Ӧ�����п��ܵ�ȡֵ����
				if (attributes[j] == data_AttValues[node_Attribute][i])
				{
					//�ҵ���������Ӧ������ֵ
					attribute_Value = attributes[j];
					break;
				}
			}
			//�ҵ����û��Ҫ����ѭ����
			if (!attribute_Value.empty())
			{
				break;
			}
		}
		//�ҳ�������ֵ��Ӧ���ӽڵ�ĵ�ַ���Ա���з���
		Tree_Node *p = childNode[attribute_Value];
		return p->findClass(attributes);//�ݹ�Ѱ�ң�ֱ���ҵ�Ҷ�ڵ�Ϊֹ
	}
	else//�������ӽڵ�˵���Ѿ��ҵ����࣬���Ϊ���ڵ��node_Attribute
	{
		return node_Attribute;
	}
}



// ��������txt�ļ��е����ݼ������ڶ���ı�����
void get_txt()
{
	// ��������
	int count = 0;
	//��ȡ�ļ�data_set.txt
	ifstream inFile("data_set.txt", ios::in);
	// �����ַ�������txt�ļ���ÿ������
	string lineStr;
	// �������Ը���
	int attribute_count = 0;
	// ���λ�ȡÿ������
	while (getline(inFile, lineStr))
	{
		// ��ȡ��һ�����֣�����������������
		if(count == 0)
		{
			// ��ȡ��һ������
			stringstream get_Attrubute(lineStr);
			// ���յ���������
			string str_Attribute;
			// �ո�Ϊ��϶����������
			while (getline(get_Attrubute,str_Attribute,' '))
			{
				// �������֣��ֱ��Կո�Ϊ�ָ�����������������
				data_Attributes.push_back(str_Attribute);
			}
		}
		// ��������Ϊ����ֵ�������ڱ���
		else
		{
			// ����������������
			attribute_num = data_Attributes.size();
			stringstream get_AttValue(lineStr);
			string str_AttValue;
			while (getline(get_AttValue, str_AttValue,' '))
			{
				data_Table[data_Attributes[attribute_count]].push_back(str_AttValue);
				attribute_count++;
				// ���ͳ��ֵ�ﵽ��������ֵ,�����¿�ʼ����
				if(attribute_count == attribute_num)
					attribute_count = 0;
			}
		}
		count++;		
	}
	// ������������
	N = count;
	// ��������ǽ��,���ڽ���ģ�͵Ĺ����в������,���½�һ������,���������������
	remain_Attributes = data_Attributes;
	remain_Attributes.pop_back();


	//���ɸ����Զ�Ӧ������ֵ����ӳ��data_AttValues
	for (int i = 0; i < data_Attributes.size(); i++)
	{
        //ͨ��set�ṹ��ͳ�����������и����Զ�Ӧ������ֵ�����п��ܵ�ȡֵ
        //�磺��ɫ�󡱵Ŀ���ȡֵΪ������ �ں� ǳ��
		set<string> attValues;
		for (int j = 0; j < count; j++)
		{
            //ע�⣺data_Attributes[i]����ĳ������
            //��data_Table[data_Attributes[i]]��һ������
			string temp = data_Table[data_Attributes[i]][j];
			//�����ظ�����ֵ��set�ǲ�������
			// cout<<temp<<",";	
			attValues.insert(temp);
		}
		for (set<string>::iterator it = attValues.begin(); it != attValues.end(); it++)
		{
			// cout<<*it<<" ";
            //�����п��ܵ�����ֵ����data_AttValues[data_Attributes[i]]
			data_AttValues[data_Attributes[i]].push_back(*it);
		}
	}
}

// ��ӡ����ֵ��
void print_Table()
{
	for(int i = 0; i < N;i++)
	{
		for(int j = 0; j < data_Attributes.size();j++)
		{
			cout<<data_Table[data_Attributes[j]][i]<<" ";
		}
		cout<<endl;
	}
}

int main()
{
	get_txt();
	// print_Table();

	Tree_Node myTree(data_Table, data_Attributes, data_AttValues, remain_Attributes);
	//����ѵ��
	myTree.generate_ChildNode();
	//����Ԥ������������Ԥ��
	vector<string> predict_Sample;
	string input_Line, temp;
	cout << "���������Խ���Ԥ��:" << endl;
	getline(cin, input_Line);
	istringstream input_Sample(input_Line);
	while (input_Sample >> temp)
	{
		//������Ԥ�����������Զ�����predict_Sample���Ա㴫��
		predict_Sample.push_back(temp);
	}
	cout << endl;
	//Ԥ��
	cout << "������Ϊ��" << myTree.findClass(predict_Sample) << endl;
	system("pause");
	return 0;
}