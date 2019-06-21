# algorithm
### sort
template< class RandomIt >
constexpr void sort( RandomIt first, RandomIt last );

### upper_bound  lower_bound 
```c
Description:
Returns an iterator pointing to the first element in the range [first, last) that is greater than value, or last if no such element is found.

Implementation:
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
    if(value < *it)
    {
      first = ++it;
      
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
