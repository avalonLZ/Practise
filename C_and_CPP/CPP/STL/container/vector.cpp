/*************************************************************************
	> File Name: vector_front.cpp
	> Author:lizhong
	> Mail:423810942@qq.com
	> Created Time: Mon 29 Oct 2018 12:18:03 AM PDT
 ************************************************************************/

#include<iostream>
#include<string>
#include<string.h>
#include<vector>
#include<algorithm>

using namespace std;


void my_print(int element)
{
    static int i = 0;
    cout << "NO." << i << "," << "element is:" << element << endl;
    ++i;
}

int main(int argc, char *argv[])
{
    vector<int> v1(10);
    vector<int>::iterator iv;
    int i = 1;
    iv = v1.begin();

    while(iv != v1.end())
    {
        *iv = i;
        iv++;
        i++;
    }
    cout << "vector front is:" << v1.front() << endl;
    cout << "vector size is:" << v1.size() << endl;
    
    cout << "push a value to vector..." << endl;
    v1.push_back(0);
    cout << "vector size is:" << v1.size() << endl;

    cout << "pop a value from vector..." << endl;
    v1.pop_back();

    //删除第5个元素6
    v1.erase(v1.begin() + 5);

    //插入值为6的10个元素
    //v1.insert(v1.begin(), 10, 6);
    cout << "vector size is:" << v1.size() << endl;
 
    //遍历打印所有元素
    for_each(v1.begin(), v1.end(), &my_print);

    cout << "capacity is " << v1.capacity() << endl;

    cout << "********************************************" << endl;
    //验证push_back是深拷贝还是浅拷贝
    int pre_copy_data[3] = {111,222,333};
    v1.push_back(pre_copy_data[0]);
    v1.push_back(pre_copy_data[1]);
    v1.push_back(pre_copy_data[2]);
    //遍历打印所有元素
    for_each(v1.begin(), v1.end(), &my_print);
    //改变源数据
    pre_copy_data[0] = 100;
    pre_copy_data[1] = 200;
    pre_copy_data[2] = 300;
    //遍历打印所有元素,不随源数据的改变而改变，说明是深拷贝
    for_each(v1.begin(), v1.end(), &my_print);

    return 0;
}
