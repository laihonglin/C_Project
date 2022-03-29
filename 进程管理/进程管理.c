#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// 最大内存大小
#define MAX_SIZE 20

// 线程状态：运行(就绪) 阻塞
enum process_type
{
    process_type_running = 1000,
    process_type_block = -1000,
    process_type_ready = 500
};

// 进程控制块结构体
typedef struct PCB_Type
{
    // 进程PID
    int pid;
    // 进程的优先级
    int priority;
    // 进程大小(执行时间)
    int size;
    // 进程内容
    char content[20];
    // 进程状态 执行 阻塞
    int state;
    // 下一个要执行的进程
    struct PCB_Type *next;
} PCB;

//创建新的进程
void create(PCB *running_list, PCB *block_list, int *size);
//查看运行进程
void show_running(PCB *running_list);
//换出某个进程
void change(PCB *running_list, PCB *block_list, int *size);
//杀死运行进程
void killed(PCB *running_list, int *size);
//唤醒某个进程
void wake_up(PCB *running_list, PCB *block_list, int *size);
//判断在运行(就绪)队列中是否存在有该pid值的进程 0->不存在 1->存在
int exist_in_running(PCB *running_list, int pid);
//判断在阻塞队列中是否存在有该pid值的进程 0->不存在 1->存在
int exist_in_block(PCB *block_list, int pid);
//通过pid寻找进程的位置(返回其前一个结点的地址
PCB *find(PCB *list, int pid);
//展示菜单
void showLine();

void create(PCB *running_list, PCB *block_list, int *size)
{
    PCB *p, *s;
    int pid; //设置该控制块的id值
    if (*size >= MAX_SIZE)
    {
        //内存不足，无法创建
        printf("内存不足,无法创建新的进程\n");
        return;
    }
    //申请一个内存控制块的空间
    p = (PCB *)malloc(sizeof(PCB));
    assert(p != NULL);
    //设置该控制块的id值
    printf("请输入新进程的pid:\n");
    scanf("%d", &pid);
    //判断pid值是否重复
    if (exist_in_running(running_list, pid))
    {
        printf("该值已存在!\n");
        return;
    }
    if (exist_in_block(block_list, pid))
    {
        printf("该值已存在!\n");
        return;
    }
    //没重复，保存
    p->pid = pid;
    //设置该控制块的其他值
    printf("请输入新进程的优先级:\n");
    scanf("%d", &p->priority);
    printf("请输入新进程的大小:\n");
    scanf("%d", &p->size);
    printf("请输入新进程的内容:\n");
    scanf("%s", p->content);
    p->state = process_type_running;
    p->next = NULL;
    //放入就绪队列中
    s = running_list;
    while (s->next != NULL)
    {
        s = s->next;
    }
    s->next = p;
    //进程数量+1
    *size = *size + 1;
}

//查看运行进程
void show_running(PCB *running_list)
{
    PCB *s = running_list->next;
    if (s == NULL)
    {
        printf("没有正在运行的进程\n");
        return;
    }
    while (s != NULL)
    {
        printf("进程id:%d\n", s->pid);
        printf("进程优先级:%d\n", s->priority);
        printf("进程大小:%d\n", s->size);
        printf("进程内容:%s\n", s->content);
        printf("___________\n");
        s = s->next;
    }
}

//换出某个进程
void change(PCB *running_list, PCB *block_list, int *size)
{
    int pid;
    PCB *p;
    if (*size == 0)
    {
        printf("无可换出的进程\n");
        return;
    }
    printf("请输入需要换出进程的id:\n");
    scanf("%d", &pid);
    //查看该线程是否在就绪队列中
    if (exist_in_running(running_list, pid))
    {
        //存在，找到该线程的位置（返回的是前一个结点的地址
        PCB *s = find(running_list, pid);
        //修改线程状态
        s->next->state = process_type_block;
        //将该线程存入阻塞队列中
        //寻找插入位置
        p = block_list;
        while (p->next != NULL)
        {
            p = p->next;
        }
        //插入
        p->next = s->next;
        //将该线程从就绪队列中移出
        s->next = s->next->next;
        //将该线程的next重置为空
        p->next->next = NULL;
        *size = *size - 1;
        printf("成功换出\n");
    }
    else
    {
        printf("该线程不存在或已处于阻塞状态\n");
    }
}

//杀死运行进程
void killed(PCB *running_list, int *size)
{
    int pid;
    if (*size == 0)
    {
        printf("无可杀死的进程\n");
        return;
    }
    printf("请输入要杀死的进程id:\n");
    scanf("%d", &pid);
    //判断该进程是否存在
    if (exist_in_running(running_list, pid))
    {
        //存在,找到该线程的位置(返回的是前一个结点的地址
        PCB *s = find(running_list, pid);
        //保存该线程的地址，用于释放
        PCB *thisThread = s->next;
        //将该线程从就绪队列移出
        s->next = s->next->next;
        *size = *size - 1;
        //直接释放掉该线程
        free(thisThread);
        printf("成功杀死\n");
    }
    else
    {
        printf("该线程不存在或已处于阻塞状态\n");
    }
}

//唤醒某个进程
void wake_up(PCB *running_list, PCB *block_list, int *size)
{
    PCB *s = block_list;
    PCB *p;
    int pid;
    if (s->next == NULL)
    {
        printf("没有可唤醒的线程\n");
        return;
    }
    printf("请输入要唤醒的进程id:\n");
    scanf("%d", &pid);
    //判断该进程是否存在
    if (exist_in_block(block_list, pid))
    {
        //存在，查找到该线程的位置(返回的是前一个结点的地址
        s = find(block_list, pid);
        //修改线程状态
        s->next->state = process_type_running;
        //将该线程存入就绪队列中
        //寻找插入位置(最后一位)
        p = running_list;
        while (p->next != NULL)
        {
            p = p->next;
        }
        //插入
        p->next = s->next;
        //将该线程从阻塞队列中取出
        s->next = s->next->next;
        //将该线程的next值重置为null
        p->next->next = NULL;
        *size = *size + 1;
        printf("唤醒成功\n");
    }
    else
    {
        printf("该线程不存在\n");
    }
}

//判断在就绪队列中是否存在有该pid值的进程 0->不存在 1->存在
int exist_in_running(PCB *running_list, int pid)
{
    int result = 0;
    PCB *s = running_list->next;
    //遍历执行状态的链表
    while (s != NULL)
    {
        if (s->pid == pid)
        {
            //存在，直接返回
            result = 1;
            break;
        }
        s = s->next;
    }
    return result;
}

//判断在阻塞队列中是否存在有该pid值的进程 0->不存在 1->存在
int exist_in_block(PCB *block_list, int pid)
{
    int result = 0;
    //遍历阻塞状态的链表
    PCB *s = block_list;
    while (s != NULL)
    {
        if (s->pid == pid)
        {
            //存在，直接返回
            result = 1;
            break;
        }
        s = s->next;
    }
    return result;
}

//通过pid寻找进程的位置(返回其前一个结点的地址
PCB *find(PCB *list, int pid)
{
    PCB *s = list;
    while (s->next != NULL)
    {
        if (s->next->pid == pid)
        {
            return s;
        }
        s = s->next;
    }
    return NULL;
}

void showLine()
{
    printf("**************************\n");
}

int main(void)
{
    PCB *running_list, *block_list;
    int choose;
    //当前运行的线程数量
    int storage_number = 0;
    //运行(就绪)队列(头结点不储存信息)
    running_list = (PCB *)malloc(sizeof(PCB));
    running_list->next = NULL;
    //阻塞队列(头结点不储存信息)
    block_list = (PCB *)malloc(sizeof(PCB));
    block_list->next = NULL;
    choose = 1;
    while (choose)
    {
        //展示菜单
        showLine();
        printf("*     进程演示系统      *\n");
        showLine();
        printf("1.创建新的进程  2.查看运行进程\n");
        printf("3.换出某个进程  4.杀死运行进程\n");
        printf("5.唤醒某个进程  6.退出程序   \n");
        showLine();
        printf("请选择(1~6):\n");
        scanf("%d", &choose);
        switch (choose)
        {
        case 1:
            //创建新的进程
            create(running_list, block_list, &storage_number);
            break;
        case 2:
            //查看运行进程
            show_running(running_list);
            break;
        case 3:
            //换出某个进程
            change(running_list, block_list, &storage_number);
            break;
        case 4:
            //杀死运行进程
            killed(running_list, &storage_number);
            break;
        case 5:
            //唤醒某个进程
            wake_up(running_list, block_list, &storage_number);
            break;
        case 6:
            return 0;
        default: 
            printf("没有这个选项!\n");
            break;
        }
    }
    return 0;
}
