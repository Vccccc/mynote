# c++沉思录
## chapter 4 类设计者的核查表
### 类需要一个构造函数吗？
**类需要一个构造函数吗？** 有些类太简单，它们的结构就是它们的接口，所以不需要构造函数。但复杂的类则需要构造函数来隐藏它们的内部工作方式。
### 数据成员是私有的吗？
**数据成员是私有的吗？** 通常使用使用公有的数据成员不是什么好事，因为类设计者无法控制何时访问这些成员。例如，下面一个支持可变长矢量类：
```c
template<typename T> class Vector
{
public: 
  int length;
};
```
如果类设计者将矢量的长度当作一个成员变量，那么设计者就必须保证这个成员变量在任何时候都能够正确反应实际矢量的长度，因为没有办法知道类的使用者什么时候会来访问这个信息。如果长度在函数中是像这样实现的：
```c
template<typename T> class Vector
{
public: 
  int length() const;
};
```
那么除非用户调用函数 length，否则类 Vecotr 都不必计算长度。

### 类需要一个无参的构造函数吗？
如果一个类已经有了构造函数，而想声明该类的对象可以不必显式地初始化它们，则必须显示地写一个无参构造函数。例如:
```c
class Point
{
public:
  Point(int p, int q): x(p), y(q) {}
private:
  int x, y;
};
```
这里定义了一个有一个构造函数的类。除非这个类有一个不需要参数的构造函数，否则下面的语句就是非法的：
Point p;  //错误：没有初始化

如果一个类需要一个显式构造函数，如上面的 Point 类一般，则试图生成该类对象的数组是非法的：
Point pa[100]; // 错误
即使想要把类的所有实例都初始化，也应该考虑所付出的代价，是否值得为此禁用数组对象。

### 是不是每个构造函数初始化了所有的数据成员？
构造函数的用途就是用一种明确定义的状态来设置对象。对象的状态由对象的数据成员进行反映。因此，每个构造函数都要负责为所有的数据成员设置经过明确定义的值。当然，有时，类会有一些数据成员，它们只在它们的对象存在了一定时间之后才有意义。所以要多思考。

### 类需要析构函数吗？
并非所有有构造函数的类都需要有析构函数。如果深入思考一个类要做些什么，那么类是否需要析构函数的问题就十分明显了。应该想一想该类是否分配了资源，而这些资源又不会由成员函数自动释放。特别是那些构造函数里包含了 new 表达式的类，通常要在析构函数中加上相应的 delete 表达式，所以会需要一个析构函数。

### 类需要一个虚析构函数吗？
有些类需要虚析构函数只是为了声明它们的析构函数是虚的。当然，决不会用作基类的类是不需要虚析构函数的：任何虚函数只在继承的情况下才有用。但是，如果有一个类 B，派生了一个类 D，那么 B 何时需要一个虚析构函数？只要有人可能会对实际指向 D 类型对象的 B* 指针执行 delete 表达式，就需要给 B 加上一个虚析构函数。
即使 B 和 D 都没有虚函数，这也是需要的，例如：
```c
struct B
{
  string s;
};

struct D : B
{
  string t;
};

int main()
{
  B* bp = new D; // 这里没有问题
  delete dp; 	// 除非 B 有一个虚析构函数，否则将调用错误的析构函数！
}
```
这里，即使 B 没有虚成员函数，甚至根本没有任何成员函数，也必须有一个虚析构函数，否则 delete 会出错：
```c
struct B
{
  string s;
  virtual ~B(){}
};
```
虚析构函数通常是空的。

### 类需要复制构造函数吗？
很多时候答案是“不”，但有时候答案是“是”。关键在于复制该类的对象是否就相当于复制其数据成员和基类对象。如果并不相当，就需要复制构造函数。

如果类在构造函数内分配资源，则可能需要一个显式的复制构造函数来管理资源。有析构函数（除了空的虚析构函数外）的类通常是用析构函数来释放构造函数分配的资源，这通常也需要一个复制构造函数。一个典型的例子是类 String:
```c
class String
{
public:
  String();
  String(const char* s);
  // 其他成员函数
private: 
  char* data;
};
```
它需要一个析构函数，因为它的数据成员指向了必须由对应的对象释放的被动态分配的内存。出于同样的原因，它还需要一个显式的复制构造函数：没有的话，复制 String 对象就会以复制它的 data 成员的形式隐式地定义。复制完后，两个对象的 data 成员将指向同样的内存：当两个对象被销毁时，这个内存会被释放两次。

如果不想用户能复制该类，就使用 delete 修饰复制构造函数（可能还有赋值操作符）：
```c
class Thing
{
public:
  // ...
  Thing(const Thing&) = delete;
  Thing& operator=(const Thing&) = delete;
};
```

### 类需要一个赋值操作符吗
如果需要复制构造函数，同理多半也会需要一个赋值操作符。
类 X 的赋值操作符由 X::operator= 来定义。通常，operator= 应该返回一个 X&，并且由
return *this;
结束以保证与内建的赋值操作符一致。
#### 操作符能正确地将对象赋给对象本身吗
如果原对象和目标对象是同一个，而我们又奉行“先释放旧值，再赋值”的行事规程，那么就有可能还没有实施复制之前就把原对象销毁了。例如类String:
```
class String
{
public:
  String& operator=(const String& s);
private:
  char* data
};
```
很容易就用下面的方法来实现赋值：
```c
// 很明显但不正确的实现
String& String::operator=(const String& s)
{
  delete [] data;
  data = new char[strlen(s.data)+1];
  strcpy(data, s.data);
  return *this;
}
```
一旦把一个 String 对象赋值给它本身，这个方法就会彻底失败，因为 s 和 this 同指向同样的对象。避免这个问题可以显式地加以预防：
```c
// 正确的实现方法1
String& String::operator=(const String& s)
{
  if(&s != this)
  {
    delete [] data;
    data = new char[strlen(s.data)+1];
    strcpy(data, s.data);
  }
  return *this;
}
```
另一种可行的方法是将旧值保存起来，直到将源值复制完成：
```c
// 正确的实现方法2
String& String::operator=(const String& s)
{
  char* newdata = new char[strlen(s.data)+1];
  strcpy(newdata, s.data);
  delete [] data;
  data = newdata;
  return *this;
}
```

### 类需要定义关系操作符吗
通用库的容器类提供了诸如列表、集合和图等数据结构的泛型定义。这些容器依赖于它们所包含的元素类型的操作。通常要求容器能够判断两个值是否相等。还常常需要容器具有判断一个值是否大于或者小于另一个值的能力。
如果类逻辑上支持相等操作，那么提供 operator== 和 operator!= 就可能会有好处。类似地，如果类的值有某种排序关系，那么可能会想提供余下的关系操作符。即使不希望用户直接使用关系操作符，也可能需要这些关系操作符。只要想创建类型的有序集合，就必须提供关系操作符。


### 记得在复制构造函数和赋值操作符的参数类型中加上const了吗
赋值构造函数应该像 X::X(const X&) 这样。赋值操作符应该像 X::operator=(const X&)。

### 参数有引用参数时应该是 const 引用吗
只有当函数想改变参数时，它才应该有不用 const 声明的引用参数。

### 记得适当声明成员函数为 const 了吗
如果确信一个成员函数不用修改它的对象，就可以声明它为 const，这样就把它用于 const 对象了。
取回长度值不改变该值的函数应被声明为 const:
```c
template<class T> Vector
{
public:
  int length() const;
  int length(int);  
}
```
否则，会遇到类似的问题：
```c
template<typename T>
int padded_length(const Vector<T>& v, int n)
{
  int k = v.length();  // oops!
  return k > n ? k : n;
}
```
除非对 length 的声明中出现了前面所说的 const，否则标注了 oops！的行不会编译，因为 v 是 const 引用。

## chapter 5 代理类
怎样才能设计一个 C++ 容器，使它有能力包含类型不同而彼此相关的对象呢？容器通常只能包含一种类型的对象，所以很难在容器中存储对象本身。存储指向对象的指针，虽然允许通过继承来处理类型不同的问题，但是也增加了内存分配的额外负担。

有一种方法，通过定义名为代理（surrogate)的对象来解决该问题。代理运行起来和它所代表的对象基本相同，但是允许将整个派生层次压缩在一个对象类型中。surrogate 是 handle 类中最简单的一种。

### 5.1 问题
```c
class Vehicle
{
public:
    virtual double weight() const = 0;
    virtual void start() = 0;
};
class RoadVehicle : public Vehicle { };
class AutoVehicle : public RoadVehicle { };
class Aircraft : public Vehicle { };
class Helicopter : public Aircraft { };
```
所有 Vehicle 都有一些类 Vehicle 中的成员声明的公共属性。但是，有的 Vehicle 具有一些其他 Vehicle 所没有的属性。例如，只有 Aircraft 能飞，也只有 Helicopter 能盘旋。

假设要跟踪处理一系列不同类型的 Vehicle。在实际中，可能会要用到某种容器类；这里使用数组来实现：
Vehicle parking_lot[1000];
没有产生预期的效果，为什么？
表明上看是由于 Vehicle 是一个抽象基类，所以类 Vehicle 本身不会有对象。既然 Vehicle 对象不存在，当然也就不可能有其对象数组了。
但还有更深层次的原因。即便是存在类 Vehicle 对象，写类似于下面的语句会导致什么结果呢？
```c
Automobile x = /* ... */
Parking_lot[num_vehicles++] = x;
```
答案是：把 x 赋给 parking_lot 的元素，会把 x  转换成一个 Vehicle 对象，同时会丢失所有在 Vehicle 类中没有的成员。该赋值语句还会把这个被剪裁了的对象复制到 parking_lot 数组中去。这样子 parking_lot 是 Vehicles 的集合，而不是所有继承自 Vehicle 的对象的集合。

### 5.2 经典解决方案
这种情况下，实现灵活性的常见做法是提供一个间接层（indirection）。最早的合适的间接层形式就是存储指针，而不是对象本身：
Vehicle* parking_lot[1000];
然后，输入类似
```c
Automobile x = /* ... */
Parking_lot[num_vehicles++] = &x;
```
的语句。这种方法解决了迫切的问题，但也带来了两个新的问题。

首先，存储在 parking_lot 中的是指向 x 的指针，在本例中是一个局部变量。这样，一旦变量 x 没有了，parking_lot 就不知道指向什么东西了。

我们可以变通一下，放入 parking_lot 中的值，不是指向原对象的指针，而是指向它们的副本的指针。然后，可以采用一个约定，就是当释放 parking_lot 时，也释放其中所指向的全部对象。因此，可以把前面的例子改成：
```c
Automobile x = /* ... */
Parking_lot[num_vehicles++] = new Automobile(x);
```
尽管这样修改可以不用存储指向本地对象的指针，它也带来了动态内存管理的负担。另外只有当我们知道要放到 parking_lot 中的对象是静态类型后，这种方法才能起作用。如果不知道又会怎么样？例如，假设我们想让 parking_lot[p] 指向一个新建的 Vehicle，这个 Vehicle 的类型和值与由 parking_lot[q] 指向的对象相同，情况会怎样？我们不能使用这样的语句
```c
if(p != q)
{
  delete parking_lot[p];
  parking_lot[p] = parking_lot[q];
}
```
因为接下来 parking_lot[p] 和 parking_lot[q] 将指向相同的对象。我们也不能使用这样的语句
```c
if(p != q)
{
  delete parking_lot[p];
  parking_lot[p] = new Vehicle(parking_lot[q]);
}
```
因为这样又会回到前面的问题：没有 Vehicle 类型的对象，即使有，也不是我们想要的！

### 5.3 虚复制函数
想一个办法来复制编译时类型未知的对象。C++ 中处理未知类型的对象的方法就是使用虚函数。

由于想能够复制任何类型的 Vehicle，所以应该在 Vehicle 类中增加一个合适的虚函数：
```c
class Vehicle
{
public:
    virtual double weight() const = 0;
    virtual void start() = 0;
    virtual Vehicle* copy() = 0;
```
接下来，在每个派生自 Vehicle 的类中添加一个新的成员函数 copy。指导思想就是，如果 vp 指向某个继承自 Vehicle 的不确定类的对象，则 vp->copy() 会获得一个指针，该指针指向该对象的一个新建的副本。例如，如果 Truck 继承自（直接或间接地）类 Vehicle，则它的 copy 函数就类似于：
```c
Vehicle* Trunk::copy() const
{
    return new Trunk(*this);
}
```
当然，处理完一个对象后，则需要清楚该对象。要做到这一点，就必须确保类 Vehicle 有一个虚析构函数：
```c
class Vehicle
{
public:
    virtual double weight() const = 0;
    virtual void start() = 0;
    virtual Vehicle* copy() = 0;
    vritual ~Vehicle() {}
```


### 5.4 定义代理类
有没有一种方法既能够避免显式地处理内存分配，又能够保持类 Vehicle 在运行时绑定的属性呢？

解决这个问题的关键是要**用类来表示概念**，这在 C++ 中是很常见的。在复制对象的过程中运用这个设计原则，就是定义一个行为和 Vehicle 对象类似、而又潜在地表示了所有继承自 Vehicle 类的对象的东西。这种类的对象叫做代理（surrogate)。

每个 Vehicle 代理都代表某个继承自 Vehicle 类的对象。只要该代理关联着这个对象，该对象就存在。因此，复制代理就会复制相对应的对象，而给代理赋值新值也会先删除就对象、在复制新对象。所幸的是，我们在类 Vehicle 中已经有了虚函数 copy 来完成这些复制工作。所以，我们可以开始定义自己的代理了：
```c
class VehicleSurrogate
{
public:
    VehicleSurrogate();
    VehicleSurrogate(const Vehicle&):
    ~VehicleSurrogate();
    VehicleSurrogate(const VehicleSurrogate&);
    VehicleSurrogate& operator=(const VehicleSurrogate&);

private:
    Vehicle* vp;
};
```
上述代理类有一个以 const Vehicle& 为参数的构造函数，这样就能为任意继承自 Vehicle 的类的对象