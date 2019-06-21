# Algorithms
### sort
template< class RandomIt >
constexpr void sort( RandomIt first, RandomIt last );

### upper_bound 
```c
template<typename ForwardIt, typename T>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value)
{
  ForwardIt it;
  typename std::iterator_traits<ForwardIt>::difference_type count, step;
  count = std::distance(first, last);
  while(count
}
```