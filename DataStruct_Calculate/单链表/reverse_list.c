/*************************************************************************
	> File Name: reverse_list.c
	> Author:lizhong
	> Mail:423810942@qq.com
	> Created Time:Wed 20 Feb 2019 05:21:00 PM PST
	> Instruction:反转单链表
 ************************************************************************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

typedef struct node
{
    int data;
    struct node *next;
}*Node, Node_t;

typedef struct signal_list
{
    int size;
    Node head;
}*List, List_t;

List List_init()
{
    List temp_list = malloc(sizeof(List_t));
    memset(temp_list, 0, sizeof(List_t));

    return temp_list;
}

void List_add_node(List list, int value)
{
    int i = 0;
    Node temp_list_node = NULL;
    Node temp_node = malloc(sizeof(Node_t));
    memset(temp_node, 0, sizeof(Node_t));
    temp_node->data = value;

    if(list->size == 0)
    {
        list->head = temp_node;
    }  
    else
    {   
        temp_list_node = list->head;
        
        for(i = 1; i < list->size; ++i)
            temp_list_node = temp_list_node->next;

        temp_list_node->next = temp_node;
    }
    list->size++;
}

void List_Print(List list)
{
    int i = 0;
    Node temp_node = NULL;

    temp_node = list->head;
    printf("list size = %d\n", list->size);

    for(i = 0; i < list->size; ++i)
    {
        printf("index = %d, value = %d\n", i, temp_node->data);
        temp_node = temp_node->next;
    }
}

void List_Reverse(List list)
{
    Node pre_node = NULL;
    Node aft_node = NULL; 
    Node index_node = list->head;

    while(index_node)
    {
        aft_node = index_node->next;
         
        if(aft_node == NULL)
            list->head = index_node;

        index_node->next = pre_node;

        //至此，当前节点已经倒置完成
        //一下工作是为倒置下一个节点做准备
        pre_node = index_node;
        index_node = aft_node;
    }
}

int main(int argc, char *argv[])
{
    int i = 0;
    List list = NULL;
    list = List_init();
    
    for(i = 0; i <= 10; ++i)
    {
        //List_add_node(list, rand() % 100);
        List_add_node(list, i);
    }
    List_Print(list);
    List_Reverse(list);
    printf("*************************\n");
    List_Print(list);

    return 0;
}
