[toc]

## 0.前言
C++ 标准模板库 STL（Standard Template Library） 一共给我们提供了四种智能指针：auto_ptr、unique_ptr、shared_ptr 和 weak_ptr，其中 auto_ptr 是 C++98 提出的，C++11 已将其摒弃（deprecated），并提出了 unique_ptr 作为 auto_ptr 替代方案。虽然 auto_ptr 已被摒弃，但在实际项目中仍可使用，但建议使用较新的 unique_ptr，因为 unique_ptr 比 auto_ptr 更加安全，后文会详细叙述。shared_ptr 和 weak_ptr 则是 C+11 从准标准库 Boost 中引入的两种智能指针。此外，Boost 库还提出了 boost::scoped_ptr、boost::scoped_array、boost::intrusive_ptr 等智能指针，虽然尚未得到 C++ 标准采纳，但是在开发实践中可以使用。

## 1. unique_ptr
unique_ptr 由 C++11 引入，旨在替代不安全的 auto_ptr。unique_ptr 是一种定义在头文件<memory>中的智能指针。它持有对对象的独有权——两个unique_ptr不能指向一个对象，即 unique_ptr 不共享它所管理的对象。它无法复制到其他 unique_ptr，无法通过值传递到函数，也无法用于需要副本的任何标准模板库 （STL）算法。只能移动 unique_ptr，即对资源管理权限可以实现转移。这意味着，内存资源所有权可以转移到另一个 unique_ptr，并且原始 unique_ptr 不再拥有此资源。实际使用中，建议将对象限制为由一个所有者所有，因为多个所有权会使程序逻辑变得复杂。因此，当需要智能指针用于存 C++ 对象时，可使用 unique_ptr，构造 unique_ptr 时，可使用 make_unique Helper 函数。

下图演示了两个 unique_ptr 实例之间的所有权转换。

unique_ptr 与原始指针一样有效，并可用于 STL 容器。将 unique_ptr 实例添加到 STL 容器运行效率很高，因为通过 unique_ptr 的移动构造函数，不再需要进行复制操作。unique_ptr 指针与其所指对象的关系：在智能指针生命周期内，可以改变智能指针所指对象，如创建智能指针时通过构造函数指定、通过 reset 方法重新指定、通过 release 方法释放所有权、通过移动语义转移所有权，unique_ptr 还可能没有对象，这种情况被称为 empty。

unique_ptr的基本操作有：
```
//智能指针的创建  
unique_ptr<int> u_i;     //创建空智能指针
u_i.reset(new int(3));     //绑定动态对象  
unique_ptr<int> u_i2(new int(4));//创建时指定动态对象
unique_ptr<T,D> u(d);    //创建空 unique_ptr，执行类型为 T 的对象，用类型为 D 的对象 d 来替代默认的删除器 delete

//所有权的变化  
int *p_i = u_i2.release();    //释放所有权  
unique_ptr<string> u_s(new string("abc"));  
unique_ptr<string> u_s2 = std::move(u_s); //所有权转移(通过移动语义)，u_s所有权转移后，变成“空指针” 
u_s2.reset(u_s.release());    //所有权转移
u_s2=nullptr;//显式销毁所指对象，同时智能指针变为空指针。与u_s2.reset()等价
```
## 2.auto_ptr
auto_ptr 同样是 STL 智能指针家族的成员之一，由 C++98 引入，定义在头文件<memory>。其功能和用法类似于 unique_ptr，由 new expression 获得对象，在 auto_ptr 对象销毁时，他所管理的对象也会自动被 delete 掉。

auto_ptr 从 C++98 使用至今，为何从 C++11 开始，引入unique_ptr 来替代 auto_ptr 呢？原因主要有如下几点：

（1）基于安全考虑。
先来看下面的赋值语句:
```
auto_ptr< string> ps (new string ("I reigned lonely as a cloud.”）;
auto_ptr<string> vocation; 
vocaticn = ps;
```
上述赋值语句将完成什么工作呢？如果 ps 和 vocation 是常规指针，则两个指针将指向同一个 string 对象。这是不能接受的，因为程序将试图删除同一个对象两次，一次是 ps 过期时，另一次是 vocation 过期时。要避免这种问题，方法有多种：
（a）定义陚值运算符，使之执行深复制。这样两个指针将指向不同的对象，其中的一个对象是另一个对象的副本，缺点是浪费空间，所以智能指针都未采用此方案。
（b）建立所有权（ownership）概念。对于特定的对象，只能有一个智能指针可拥有，这样只有拥有对象的智能指针的析构函数会删除该对象。然后让赋值操作转让所有权。这就是用于 auto_ptr 和 unique_ptr 的策略，但 unique_ptr 的策略更严格。
（c）创建智能更高的指针，跟踪引用特定对象的智能指针数。这称为引用计数。例如，赋值时，计数将加 1，而指针过期时，计数将减 1,。当减为 0 时才调用 delete。这是 shared_ptr 采用的策略。

当然，同样的策略也适用于复制构造函数，即auto_ptr<string> vocation(ps)时也需要上面的策略。每种方法都有其用途，但为何要摒弃 auto_ptr 呢？

下面举个例子来说明。
```
#include <iostream>
#include <string>
#include <memory>
using namespace std;

int main()
{
    auto_ptr<string> films[5] ={
    auto_ptr<string> (new string("Fowl Balls")),
    auto_ptr<string> (new string("Duck Walks")),
    auto_ptr<string> (new string("Chicken Runs")),
    auto_ptr<string> (new string("Turkey Errors")),
    auto_ptr<string> (new string("Goose Eggs"))
    };
    auto_ptr<string> pwin;
    pwin = films[2]; // films[2] loses ownership. 将所有权从films[2]转让给pwin，此时films[2]不再引用该字符串从而变成空指针

    cout << "The nominees for best avian baseballl film are\n";
    for(int i = 0; i < 5; ++i)
    {
        cout << *films[i] << endl;
    }
     cout << "The winner is " << *pwin << endl;
    return 0;
}
```

运行下发现程序崩溃了，原因在上面注释已经说的很清楚，films[2] 已经是空指针了，下面输出访问空指针当然会崩溃了。但这里如果把 auto_ptr 换成 shared_ptr 或 unique_ptr 后，程序就不会崩溃，原因如下：

使用 shared_ptr 时运行正常，因为 shared_ptr 采用引用计数，pwin 和 films[2] 都指向同一块内存，在释放空间时因为事先要判断引用计数值的大小因此不会出现多次删除一个对象的错误。

使用 unique_ptr 时编译出错，与 auto_ptr 一样，unique_ptr 也采用所有权模型，但在使用 unique_ptr 时，程序不会等到运行阶段崩溃，而在编译期因下述代码行出现错误：
```
unique_ptr<string> pwin;
pwin = films[2];                     //films[2] loses ownership
```
指导你发现潜在的内存错误。这就是为何要摒弃 auto_ptr 的原因，一句话总结就是：避免因潜在的内存问题导致程序崩溃。

从上面可见，unique_ptr 比 auto_ptr 更加安全，因为 auto_ptr 有拷贝语义，拷贝后原对象变得无效，再次访问原对象时会导致程序崩溃；unique_ptr 则禁止了拷贝语义，但提供了移动语义，即可以使用std::move() 进行控制权限的转移，如下代码所示：
```
unique_ptr<string> upt(new string("lvlv"));
unique_ptr<string> upt1(upt);    //编译出错，已禁止拷贝
unique_ptr<string> upt1=upt;    //编译出错，已禁止拷贝
unique_ptr<string> upt1=std::move(upt);  //控制权限转移

auto_ptr<string> apt(new string("lvlv"));
auto_ptr<string> apt1(apt);    //编译通过
auto_ptr<string> apt1=apt;    //编译通过
```

这里要注意，在使用std::move将unique_ptr的控制权限转移后，不能够再通过unique_ptr来访问和控制资源了，否则同样会出现程序崩溃。我们可以在使用unique_ptr访问资源前，使用成员函数get()进行判空操作。
```
unique_ptr<string> upt1=std::move(upt);                              //控制权限转移
if(upt.get()!=nullptr)                                                                //判空操作更安全
{
    //do something
}
```
（2）unique_ptr 不仅安全，而且灵活。
如果 unique_ptr 是个临时右值，编译器允许拷贝语义。参考如下代码：
```
unique_ptr<string> demo(const char* s)
{
    unique_ptr<string> temp (new string(s))； 
    return temp；
}
````
//假设编写了如下代码：
```
unique_ptr<string> ps;
ps = demo('Uniquely special")；
```
demo() 返回一个临时 unique_ptr，然后 ps 接管了临时对象 unique_ptr 所管理的资源，而返回时临时的 unique_ptr 被销毁，也就是说没有机会使用 unique_ptr 来访问无效的数据，换句话来说，这种赋值是不会出现任何问题的，即没有理由禁止这种赋值。实际上，编译器确实允许这种赋值。相对于 auto_ptr 任何情况下都允许拷贝语义，这正是 unique_ptr 更加灵活聪明的地方。

（3）扩展 auto_ptr 不能完成的功能。
（a）unique_ptr 可放在容器中，弥补了 auto_ptr 不能作为容器元素的缺点。
```
//方式一：
vector<unique_ptr<string>> vs { new string{“Doug”}, new string{“Adams”} };  

//方式二：
vector<unique_ptr<string>>v;  
unique_ptr<string> p1(new string("abc"));  
```
（b）管理动态数组，因为 unique_ptr 有 unique_ptr<X[]> 重载版本，销毁动态对象时调用 delete[]。

```
unique_ptr<int[]> p (new int[3]{1,2,3});  
p[0] = 0;// 重载了operator[]
```
（c）自定义资源删除操作（Deleter）。unique_ptr 默认的资源删除操作是 delete/delete[]，若需要，可以进行自定义：

```
void end_connection(connection *p) { disconnect(*p); } //资源清理函数  
//资源清理器的“类型” 
unique_ptr<connection, decltype(end_connection)*> p(&c, end_connection);
// 传入函数名，会自动转换为函数指针  
```
综上所述，基于 unique_ptr 的安全性和扩充的功能，unique_ptr 成功的将 auto_ptr 取而代之。

## 3.shared_ptr
### 3.1 shared_ptr 简介
shared_ptr 是一个标准的共享所有权的智能指针，允许多个指针指向同一个对象，定义在 memory 文件中，命名空间为 std。shared_ptr最初实现于Boost库中，后由 C++11 引入到 C++ STL。shared_ptr 利用引用计数的方式实现了对所管理的对象的所有权的分享，即允许多个 shared_ptr 共同管理同一个对象。像 shared_ptr 这种智能指针，《Effective C++》称之为“引用计数型智能指针”（reference-counting smart pointer，RCSP）。

shared_ptr 是为了解决 auto_ptr 在对象所有权上的局限性（auto_ptr 是独占的），在使用引用计数的机制上提供了可以共享所有权的智能指针，当然这需要额外的开销：
（1）shared_ptr 对象除了包括一个所拥有对象的指针外，还必须包括一个引用计数代理对象的指针；
（2）时间上的开销主要在初始化和拷贝操作上， * 和 -> 操作符重载的开销跟 auto_ptr 是一样；
（3）开销并不是我们不使用 shared_ptr 的理由,，永远不要进行不成熟的优化，直到性能分析器告诉你这一点。

### 3.2 通过辅助类模拟实现 shared_ptr
（1）基础对象类
首先，我们来定义一个基础对象类 Point 类，为了方便后面我们验证智能指针是否有效，我们为 Point 类创建如下接口：
```
class Point
{
private:
    int x, y;
public:
    Point(int xVal = 0, int yVal = 0) :x(xVal), y(yVal) {}
    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int xVal) { x = xVal; }
    void setY(int yVal) { y = yVal; }
};
```

（2）辅助类
在创建智能指针类之前，我们先创建一个辅助类。这个类的所有成员皆为私有类型，因为它不被普通用户所使用。为了只为智能指针使用，还需要把智能指针类声明为辅助类的友元。这个辅助类含有两个数据成员：计数count 与基础对象指针。也即辅助类用以封装使用计数与基础对象指针。

```
class RefPtr
{
private:
    friend class SmartPtr;      
    RefPtr(Point *ptr):p(ptr),count(1){ }
    ~RefPtr(){delete p;}
    
    int count;   
    Point *p;                                                      
};
```
（3）为基础对象类实现智能指针类
引用计数是实现智能指针的一种通用方法。智能指针将一个计数器与类指向的对象相关联，引用计数跟踪共有多少个类对象共享同一指针。它的具体做法如下：
（a）当创建智能指针类的新对象时，初始化指针，并将引用计数设置为1；
（b）当能智能指针类对象作为另一个对象的副本时，拷贝构造函数复制副本的指向辅助类对象的指针，并增加辅助类对象对基础类对象的引用计数（加1）；
（c）使用赋值操作符对一个智能指针类对象进行赋值时，处理复杂一点：先使左操作数的引用计数减 1（为何减 1：因为指针已经指向别的地方），如果减1后引用计数为 0，则释放指针所指对象内存。然后增加右操作数所指对象的引用计数（为何增加：因为此时做操作数指向对象即右操作数指向对象）；
（d）完成析构函数：调用析构函数时，析构函数先使引用计数减 1，如果减至 0 则 delete 对象。

做好前面的准备后，我们可以为基础对象类 Point 书写一个智能指针类了。根据引用计数实现关键点，我们可以写出如下智能指针类：

```
class SmartPtr
{
public:
    
    //构造函数
    SmartPtr() { rp = nullptr; }
    SmartPtr(Point *ptr):rp(new RefPtr(ptr)) {}
    SmartPtr(const SmartPtr &sp):rp(sp.rp)
    { 
        ++rp->count;
        cout << "in copy constructor" <<endl;
    }
    
    //重载赋值运算符
    SmartPtr& operator=(const SmartPtr& rhs)
    {
        ++rhs.rp->count;
        if (rp != nullptr && --rp->count == 0)
        {
            delete rp;
        }
        rp = rhs.rp;
        cout << "in assignment operator" << endl;
        return *this;
    }
    
    //重载->操作符
    Point* operator->()
    {
        return rp->p;
    }
    
    //重载*操作符
    Point& operator*()
    {
        return *(rp->p);
    }

    ~SmartPtr()
    {
        if (--rp->count == 0)
            delete rp;
        else
            cout << "还有" << rp->count << "个指针指向基础对象" << endl;
    }

private:
    RefPtr* rp;
};

```

（4）智能指针类的使用与测试
至此，我们的智能指针类就完成了，我们可以来看看如何使用。


```
int main()
{
    //定义一个基础对象类指针
    Point *pa = new Point(10, 20);

    //定义三个智能指针类对象，对象都指向基础类对象 pa
    //使用花括号控制三个智能指针的生命周期，观察计数的变化
    {
        SmartPtr sptr1(pa);//此时计数 count=1
        cout <<"sptr1:"<<sptr1->getX()<<","<<sptr1->getY()<<endl;
        {
            SmartPtr sptr2(sptr1); //调用拷贝构造函数，此时计数为 count=2
            cout<<"sptr2:" <<sptr2->getX()<<","<<sptr2->getY()<<endl;
            {
                SmartPtr sptr3;
                SmartPtr sptr3=sptr1; //调用赋值操作符，此时计数为 conut=3
                cout<<"sptr3:"<<(*sptr3).getX()<<","<<(*sptr3).getY()<<endl;
            }
            //此时count=2
        }
        //此时count=1；
    }
    //此时count=0；对象 pa 被 delete 掉
    cout << pa->getX() << endl;
    return 0;
}
```
运行结果：

sptr1:10,20
in copy constructor
sptr2:10,20
in assignment operator
sptr3:10,20
还有2个指针指向基础对象
还有1个指针指向基础对象
-572662307
如期，在离开大括号后，共享基础对象的指针从 3->2->1->0 变换，最后计数为 0 时，pa 对象被 delete，此时使用 getX() 已经获取不到原来的值。

（5）对智能指针的改进
目前这个智能指针只能用于管理 Point 类的基础对象，如果此时定义了个矩阵的基础对象类，那不是还得重新写一个属于矩阵类的智能指针类吗？但是矩阵类的智能指针类设计思想和 Poin t类一样啊，就不能借用吗？答案当然是能，那就是使用模板技术。为了使我们的智能指针适用于更多的基础对象类，我们有必要把智能指针类通过模板来实现。这里贴上上面的智能指针类的模板版本：
```
//模板类作为友元时要先有声明
template <typename T> class SmartPtr;
   
//辅助类
template <typename T> class RefPtr
{
private:
    //该类成员访问权限全部为private，因为不想让用户直接使用该类
    friend class SmartPtr<T>;      //定义智能指针类为友元，因为智能指针类需要直接操纵辅助类
    
    //构造函数的参数为基础对象的指针
    RefPtr(T *ptr):p(ptr), count(1)
    {}
    
    //析构函数
    ~RefPtr()
    {
        delete p;
    }
    
    //引用计数
    int count;   
    
    //基础对象指针
    T *p;                                                      
};

//智能指针类
template <typename T> class SmartPtr
{
public:
    //构造函数
    SmartPtr(T *ptr) :rp(new RefPtr<T>(ptr))
    {}
    //拷贝构造函数
    SmartPtr(const SmartPtr<T> &sp):rp(sp.rp)
    {
        ++rp->count;
    }
    //重载赋值操作符
    SmartPtr& operator=(const SmartPtr<T>& rhs)
    {
        ++rhs.rp->count;        //首先将右操作数引用计数加1，
        if (--rp->count == 0)   //然后将引用计数减1，可以应对自赋值
            delete rp;
        rp = rhs.rp;
        return *this;
    }
    //重载*操作符
    T & operator *()
    {
        return *(rp->p);
    }
    //重载->操作符
    T* operator ->()
    {
        return rp->p;
    }
    //析构函数
    ~SmartPtr()
    {
        if (--rp->count == 0)    //当引用计数减为0时，删除辅助类对象指针，从而删除基础对象
        {
            delete rp;
        }
        else
        {
            cout << "还有" << rp->count << "个指针指向基础对象" << endl;
        }
    }
private:
    RefPtr<T> *rp;  //辅助类对象指针
};
```
现在使用智能指针类模板来共享其它类型的基础对象，以int为例：

```
int main()
{
    //定义一个基础对象类指针
    int* ia = new int(10);
    {
        SmartPtr<int> sptr1(ia);
        cout <<"sptr1:"<<*sptr1<<endl;
        {
            SmartPtr<int> sptr2(sptr1); 
            cout <<"sptr2:"<<*sptr2<<endl;
            *sptr2=5;
            {
                SmartPtr<int> sptr3=sptr1; 
                cout <<"sptr3:"<<*sptr3<<endl;
            }
        }
    }
    //此时count=0；pa对象被delete掉
    cout << *ia << endl;
    return 0;
}
```
测试结果如下：

sptr1:10
sptr2:10
sptr3:5
还有2个指针指向基础对象
还有1个指针指向基础对象
3968064

## 4.weak_ptr
### 4.1 weak_ptr 简介
weak_ptr 被设计为与 shared_ptr 共同工作，可以从一个 shared_ptr 或者另一个 weak_ptr 对象构造而来。weak_ptr 是为了配合 shared_ptr 而引入的一种智能指针，它更像是 shared_ptr 的一个助手而不是智能指针，因为它不具有普通指针的行为，没有重载 operator* 和 operator-> ，因此取名为 weak，表明其是功能较弱的智能指针。它的最大作用在于协助 shared_ptr 工作，可获得资源的观测权，像旁观者那样观测资源的使用情况。观察者意味着 weak_ptr 只对 shared_ptr 进行引用，而不改变其引用计数，当被观察的 shared_ptr 失效后，相应的 weak_ptr 也相应失效。

### 4.2 weak_ptr 用法
使用 weak_ptr 的成员函数 use_count() 可以观测资源的引用计数，另一个成员函数 expired() 的功能等价于 use_count()==0，但更快，表示被观测的资源（也就是 shared_ptr 管理的资源）已经不复存在。weak_ptr 可以使用一个非常重要的成员函数lock()从被观测的 shared_ptr 获得一个可用的 shared_ptr 管理的对象， 从而操作资源。但当 expired()==true 的时候，lock() 函数将返回一个存储空指针的 shared_ptr。总的来说，weak_ptr 的基本用法总结如下：
```
weak_ptr<T> w;         //创建空 weak_ptr，可以指向类型为 T 的对象
weak_ptr<T> w(sp);    //与 shared_ptr 指向相同的对象，shared_ptr 引用计数不变。T必须能转换为 sp 指向的类型
w=p;                //p 可以是 shared_ptr 或 weak_ptr，赋值后 w 与 p 共享对象
w.reset();            //将 w 置空
w.use_count();        //返回与 w 共享对象的 shared_ptr 的数量
w.expired();        //若 w.use_count() 为 0，返回 true，否则返回 false
w.lock();            //如果 expired() 为 true，返回一个空 shared_ptr，否则返回非空 shared_ptr
```
下面是一个简单的使用示例：
```
#include < assert.h>

#include <iostream>
#include <memory>
#include <string>
using namespace std;

int main()
{
    shared_ptr<int> sp(new int(10));
    assert(sp.use_count() == 1);
    weak_ptr<int> wp(sp);     //从 shared_ptr 创建 weak_ptr
    assert(wp.use_count() == 1);
    if (!wp.expired())        //判断 weak_ptr 观察的对象是否失效
    {
        shared_ptr<int> sp2 = wp.lock();//获得一个shared_ptr
        *sp2 = 100;
        assert(wp.use_count() == 2);
    }
    assert(wp.use_count() == 1);
    cout << "int:" << *sp << endl;
    return 0;
}
```
程序输出：

int：100
从上面可以看到，尽管以 shared_ptr 来构造 weak_ptr，但是 weak_ptr内部的引用计数并没有什么变化。

### 4.3 weak_ptr 的作用
现在要说的问题是，weak_ptr 到底有什么作用呢？从上面那个例子看来，似乎没有任何作用。其实 weak_ptr 可用于打破循环引用。引用计数是一种便利的内存管理机制，但它有一个很大的缺点，那就是不能管理循环引用的对象。一个简单的例子如下：
```
#include <iostream>
#include <memory>
  
class Woman;  
class Man
{  
private:  
    //std::weak_ptr<Woman> _wife;  
    std::shared_ptr<Woman> _wife;  
public:  
    void setWife(std::shared_ptr<Woman> woman)
    {  
        _wife = woman;  
    }  
  
    void doSomthing()
    {  
        if(_wife.lock())
        {}  
    }  
  
    ~Man()
    {
        std::cout << "kill man\n";  
    }  
};  
  
class Woman
{  
private:  
    //std::weak_ptr<Man> _husband;  
    std::shared_ptr<Man> _husband;  
public:  
    void setHusband(std::shared_ptr<Man> man)
    {  
        _husband = man;  
    }  
    ~Woman()
    {  
        std::cout <<"kill woman\n";  
    }  
};

int main(int argc, char** argv)
{  
    std::shared_ptr<Man> m(new Man());  
    std::shared_ptr<Woman> w(new Woman());  
    if(m && w)
    {  
        m->setWife(w);  
        w->setHusband(m);  
    }  
    return 0;  
}  
```
在 Man 类内部会引用一个 Woman，Woman 类内部也引用一个 Man。当一个 man 和一个 woman 是夫妻的时候，他们直接就存在了相互引用问题。man 内部有个用于管理wife生命期的 shared_ptr 变量，也就是说 wife 必定是在 husband 去世之后才能去世。同样的，woman 内部也有一个管理 husband 生命期的 shared_ptr 变量，也就是说 husband 必须在 wife 去世之后才能去世。这就是循环引用存在的问题：husband 的生命期由 wife 的生命期决定，wife 的生命期由 husband 的生命期决定，最后两人都死不掉，违反了自然规律，导致了内存泄漏。

一般来讲，解除这种循环引用有下面三种可行的方法：
（1）当只剩下最后一个引用的时候需要手动打破循环引用释放对象。
（2）当 parent 的生存期超过 children 的生存期的时候，children 改为使用一个普通指针指向 parent。
（3）使用弱引用的智能指针打破这种循环引用。
虽然这三种方法都可行，但方法 1 和方法 2 都需要程序员手动控制，麻烦且容易出错。这里主要介绍一下第三种方法，使用弱引用的智能指针std:weak_ptr 来打破循环引用。

weak_ptr 对象引用资源时不会增加引用计数，但是它能够通过 lock() 方法来判断它所管理的资源是否被释放。做法就是上面的代码注释的地方取消注释，取消 Woman 类或者 Man 类的任意一个即可，也可同时取消注释，全部换成弱引用 weak_ptr。

另外很自然地一个问题是：既然 weak_ptr 不增加资源的引用计数，那么在使用 weak_ptr 对象的时候，资源被突然释放了怎么办呢？不用担心，因为不能直接通过 weak_ptr 来访问资源。那么如何通过 weak_ptr 来间接访问资源呢？答案是在需要访问资源的时候 weak_ptr 为你生成一个shared_ptr，shared_ptr 能够保证在 shared_ptr 没有被释放之前，其所管理的资源是不会被释放的。创建 shared_ptr 的方法就是 lock() 成员函数。

注意： shared_ptr 实现了 operator bool() const 方法来判断被管理的资源是否已被释放。

## 5.如何选择智能指针
上文简单地介绍了 C++ STL 的四种智能指针。当然，除了 STL 的智能指针，C++ 准标准库 Boost 的智能指针，比如 boost::scoped_ptr、boost::shared_array、boost::intrusive_ptr 也可以在编程实践中拿来使用，但这里不做进一步的介绍，有兴趣的读者可以参考：C++ 智能指针详解。

在了解 STL 的四种智能指针后，大家可能会想另一个问题：在实际应用中，应使用哪种智能指针呢？

下面给出几个使用指南。
1. 如果程序要使用多个指向同一个对象的指针，应选择 shared_ptr。这样的情况包括：

+ （a）将指针作为参数或者函数的返回值进行传递的话，应该使用 shared_ptr；
+ （b）两个对象都包含指向第三个对象的指针，此时应该使用 shared_ptr 来管理第三个对象；
+ （c）STL 容器包含指针。很多 STL 算法都支持复制和赋值操作，这些操作可用于 shared_ptr，但不能用于 unique_ptr（编译器发出warning）和 auto_ptr（行为不确定）。

2. 如果程序不需要多个指向同一个对象的指针，则可使用 unique_ptr。如果函数使用 new 分配内存，并返还指向该内存的指针，将其返回类型声明为 unique_ptr 是不错的选择。这样，所有权转让给接受返回值的 unique_ptr，而该智能指针将负责调用 delete。可将 unique_ptr 存储到 STL 容器中，只要不调用将一个 unique_ptr 复制或赋值给另一个的算法（如 sort()）。例如，可在程序中使用类似于下面的代码段。
```
unique_ptr<int> make_int(int n)
{
    return unique_ptr<int>(new int(n));
}

void show(unique_ptr<int>& p1)
{
    cout << *p1 << ' ';
}

int main()
{
    //...
    vector<unique_ptr<int>> vp(size);
    for(int i = 0; i < vp.size(); i++)
    {
        vp[i] = make_int(rand() % 1000);       //copy temporary unique_ptr
    }
    vp.push_back(make_int(rand() % 1000));     //ok because arg is temporary
    for_each(vp.begin(), vp.end(), show);      //use for_each()
    //...
}
```
其中 push_back 调用没有问题，因为它返回一个临时 unique_ptr，该 unique_ptr 被赋给 vp 中的一个 unique_ptr。另外，如果按值而不是按引用给 show() 传递对象，for_each() 将非法，因为这将导致使用一个来自 vp 的非临时 unique_ptr 初始化 pi，而这是不允许的。前面说过，编译器将发现错误使用 unique_ptr 的企图。

在 unique_ptr 为右值时，可将其赋给 shared_ptr，这与将一个 unique_ptr 赋给另一个 unique_ptr 需要满足的条件相同，即 unique_ptr 必须是一个临时对象。与前面一样，在下面的代码中，make_int() 的返回类型为 unique_ptr<int>：
```
unique_ptr<int> pup(make_int(rand() % 1000));       // ok
shared_ptr<int> spp(pup);                        // not allowed, pup as lvalue
shared_ptr<int> spr(make_int(rand() % 1000));        // ok
```
模板 shared_ptr 包含一个显式构造函数，可用于将右值 unique_ptr 转换为 shared_ptr。shared_ptr 将接管原来归 unique_ptr 所有的对象。

在满足 unique_ptr 要求的条件时，也可使用 auto_ptr，但 unique_ptr 是更好的选择。如果你的编译器没有unique_ptr，可考虑使用 Boost 库提供的 scoped_ptr，它与 unique_ptr 类似。

（3）虽然说在满足 unique_ptr 要求的条件时，使用 auto_ptr 也可以完成对内存资源的管理，但是因为 auto_ ptr 不够安全，不提倡使用，即任何情况下都不应该使用 auto_ptr。

（4）为了解决 shared_ptr 的循环引用问题，我们可以祭出 weak_ptr。

（5）在局部作用域（例如函数内部或类内部），且不需要将指针作为参数或返回值进行传递的情况下，如果对性能要求严格，使用 scoped_ptr 的开销较 shared_ptr 会小一些。

## 参考文献
[1] Stanley B.Lippman著,王刚,杨巨峰译.C++ Primer(第五版).2013.P400-422
[2] Scott Meyers著，侯捷译.Effective C++中文版(第三版).2011.P61-77
[3] C++智能指针简单剖析
[4] shared_ptr基于引用计数智能指针实现
[5] C++中智能指针的设计和使用
[6] C++11智能指针之unique_ptr
[7] Boost智能指针——weak_ptr
[8] std::shared_ptr 和 std::weak_ptr的用法以及引用计数的循环引用问题
————————————————
版权声明：本文为CSDN博主「Dablelv」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。

原文链接： https://blog.csdn.net/k346k346/article/details/81478223
