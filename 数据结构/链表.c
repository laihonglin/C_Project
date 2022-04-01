#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int val;
    struct Node *next;
}*Node;

Node CreateList()
{
    Node head = (Node)malloc(sizeof(struct Node));
    head->val = 0;
    head->next = NULL;
    return head;
}

//尾插法
void InsertNode(Node head,int val)
{
    Node node = (Node)malloc(sizeof(struct Node));
    node->val = val;
    node->next = NULL;
    Node cur = head;
    while(cur->next != NULL)
    {
        cur = cur->next;
    }
    cur->next = node;
}

void DeleteNode(Node head,int val)
{
    Node cur = head->next;
    while(cur != NULL && cur->next != NULL)
    {
        if(cur->next->val == val)
        {
            Node node = cur->next;
            cur->next = cur->next->next;
            free(node);
            return;
        }
        cur = cur->next;
    }
    printf("%d号该结点不存在,无法删除\n",val);
}

//存在返回1，不存在返回0
int IsExist(Node head,int val)
{
    Node cur = head->next;
    while(cur != NULL)
    {
        if(cur->val == val)
        {
            printf("%d号结点存在\n",val);
            return 1;
        }
        cur = cur->next;
    }
    printf("%d号结点不存在\n",val);
    return 0;
}

Node ReverseList(Node head)
{
    Node pre = head;
    Node cur = head->next;
    while(cur != NULL)
    {
        Node next = cur->next;
        cur->next = pre;
        pre = cur;
        cur = next;
    }
    return pre;
}

void Print(Node head)
{
    Node cur = head->next;
    while(cur != NULL)
    {
        if(cur->next == NULL)
            printf("%d",cur->val);
        else
            printf("%d ",cur->val);
        cur = cur->next;
    }
}

void ReversePrint(Node rhead)
{
    Node cur = rhead;
    while(cur->val != 0)
    {
        if(cur->next->val == 0)
            printf("%d",cur->val);
        else
            printf("%d ",cur->val);
        cur = cur->next;
    }
}

int main(void)
{
    Node head = CreateList();
    InsertNode(head,1);
    InsertNode(head,2);
    InsertNode(head,3);
    DeleteNode(head,3);
    InsertNode(head,4);
    InsertNode(head,5);
    InsertNode(head,6);
    DeleteNode(head,6);
    DeleteNode(head,7);
    int flag =  IsExist(head,6);
    printf("flag = %d\n",flag);
    Print(head);
    putchar('\n');
    //Node rhead = ReverseList(head);
    //ReversePrint(rhead);
    return 0;
}
