/* 
    �������㷨ʵ��
    Garker-gan
    2020-11-24 �������ʵ��
 */
#include<iostream>
#include<algorithm>
#include<sstream>       //ostringstream����������� istringstream����������� stringstream���������������
#include<string>
#include<vector>
#include<map>
#include<set>
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
    decisionTree(/* args */);
    ~decisionTree();
};

decisionTree::decisionTree(/* args */)
{

}

decisionTree::~decisionTree()
{
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
    for(int i = 0;i < N;i++)
    {
        for(int j = 0;j < data_attribute.size();j++)
        {
            cout<<data_table[data_attribute[j]][i]<<"  ";
        }
        cout<<endl;
    }
}

int main()
{
    date_input();
    data_print();
    return 0;
} 