#include<queue>
#include<iostream>  
using namespace std;
 
struct Tree{  
    int val;            //结点数据  
    struct Tree *lchild;        //左孩子  
    struct Tree *rchild;        //右孩子  
};  
 
void addTree(Tree *T,Tree *p)  //创造二叉树  
{  
    queue<Tree*>q;
    q.push(T);
    Tree * tem=q.front();
    while (!q.empty())
    {
        q.pop();
        if (tem->lchild==NULL)
        {
            tem->lchild=p;
            break;
        }
        if (tem->rchild==NULL)
        {
            tem->rchild=p;
            break;
        }
        q.push(tem->lchild);
        q.push(tem->rchild);
        tem=q.front();
    }
 
}  
 
void front(Tree* T)  //前序遍历  
{  
    if(T){  
        cout<<T->val<<"  ";  
        front(T->lchild);  
        front(T->rchild);  
    }  
}  
 
void middle(Tree* T)  //中序遍历  
{  
    if(T){  
        middle(T->lchild);  
        cout<<T->val<<"  ";  
        middle(T->rchild);  
    }  
}  
 
void back(Tree* T)  //后序遍历 
{  
    if(T){  
        back(T->lchild);  
        back(T->rchild);  
        cout<<T->val<<"  ";   
    }  
}  
void print(Tree* treeRoot )  
{  
    if (!treeRoot)
    return ;
    cout<<"按层打印:"<<endl;
    queue<Tree* >q;
    q.push(treeRoot);
 
    while ( !q.empty() )
    {
        Tree * tem=q.front();
        q.pop();
        cout<<tem->val<<"  ";
        if (tem->lchild!=NULL)
            q.push(tem->lchild);
        if (tem->rchild!=NULL)
            q.push(tem->rchild);
 
    }
    cout<<endl;
}  
 
int main()  
{   
    printf("请输入结点内容(以0作为结束标识，不读入):\n");
 
    Tree* treeRoot=NULL;
    int val; 
    cin>>val;  
    while(val!=0){    //判断输入  
        Tree *node = new Tree ;        //创建新结点  
        node->val = val;  
        node->lchild = NULL;  
        node->rchild = NULL;  
        if(treeRoot==NULL)  
            treeRoot= node;  
        else  
            addTree(treeRoot,node);  
        cin>>val;//读入用户输入  
    }  
     print(treeRoot);
     cout<<"前序遍历："<<endl;  
     front(treeRoot);  
     cout<<"\n中序遍历："<<endl;  
     middle(treeRoot);  
     cout<<"\n后序遍历："<<endl;  
     back(treeRoot);  
 
}
