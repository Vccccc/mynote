# Algorithms
### sort
template< class RandomIt >
constexpr void sort( RandomIt first, RandomIt last );

### upper_bound 
```c
Description:
Returns an iterator pointing to the first element in the range [first, last) that is greater than value, or last if no such element is found.

template<typename ForwardIt, typename T>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value)
{
  ForwardIt it;
  typename std::iterator_traits<ForwardIt>::difference_type count, step;
  count = std::distance(first, last);
  while(count > 0)
  {
    it = first;
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