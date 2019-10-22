## 转型操作符
1. static_cast
2. const_cast
3. dynamic_cast
4. reinterpret_cast

### 使用转型操作符的好处
使用转型操作符可以使转型操作意图更准确，便于诸如grep之类的工具区分。

#### 1. static_cast
static_cast可以将int转型成double，但不能将struct转型成int，或者将double转型成指针。static_cast不能够移除表达式的常量性(constness)

#### 2. const_cast
const_cast用于改变表达式中的常量性(constness)或易变性(volatileness)

#### 3. dynamic_cast
dynamic_cast用来执行继承体系中“安全的向下转型或跨系转型动作”。可以用dynamic_cast，将“指向base class objects的pointers或references”转型为指向“derived(或sibling base)class objects
的pointers或references”，并得知转型是否成功。如果转型失败，会以一个null指针(当转型对象是指针)或一个exception(当转型对象是reference)表现出来。

#### 4. reinterpret_cast
reinterpret_cast转型结果与编译平台相关，所以reinterpret_cast不具移植性。reinterpret_cast的最常用用途是转换“函数指针”类型。
```c
typedef void(*FuncPtr)();
FuncPtr funcPtrArray[10];
int doSomething(); // 将该函数指针放入funcPtrArray中，注意返回类型为int
// 如果没有转型，是不能实现的，因为doSomething的类型与funcPtrArray所能接受的不同。
// 通过使用reinterpret_cast可以强制编译器了解你的意图
funcPtrArray[0] = reinterpret_cast<FuncPtr>(&doSomething);
```

# algorithm
### sort
template< class RandomIt >
constexpr void sort( RandomIt first, RandomIt last );

### upper_bound  lower_bound 
Description:
Returns an iterator pointing to the first element in the range [first, last) that is greater than value, or last if no such element is found.

Implementation:
```c
template<typename ForwardIt, typename T>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value)
{
  ForwardIt it;
  typename std::iterator_traits<ForwardIt>::difference_type count, step;
  count = std::distance(first, last);
  while(count > 0)
  {
    it = first;
    step = count / 2;
    std::advance(it, step);
    if(!(value < *it))
    {
      first = ++it;
      count -= step + 1;
    }
    else
    {
      count = step;
    }
  }
  return first;
}
```
Prototype:
template< class ForwardIt, class T >
constexpr ForwardIt lower_bound( ForwardIt first, ForwardIt last, const T& value );

Description:
Returns an iterator pointing to the first element in the range [first, last) that is not less than (i.e. greater or equal to) value, or last if no such element is found.

Implementation:
```c
template<typename ForwardIt first, ForwardIt last, const T& value)
{
  typename iterator_traits<ForwardIt>::diffrence_type count, step;
  Forward it;
  count = std::distance(first, last);
  while(count > 0)
  {
    it = first;
    step = count / 2;
    std::advance(it, step);
    if(*it < value)
    {
      first = ++it;
      count -= step + 1;
    }
    else
    {
      count = step;
    }
  }
  return first;
}
```




# iterator
### advance
Prototype: 
template< class InputIt, class Distance >
constexpr void advance( InputIt& it, Distance n );

Description:
Increments given iterator it by n elements.
If n is negative, the iterator is decremented.
