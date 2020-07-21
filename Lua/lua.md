## Lua 与 C 交互
### lua_CFunction
为了正确的和 Lua 通讯，C 函数必须使用下列协议。这个协议定义了参数以及返回值传递方法：C 函数通过 Lua 中的栈来接受参数，参数以正序入栈（第一个参数首先入栈）。因此，当函数开始的时候，lua_gettop(L) 可以返回函数接收到的参数个数。第一个参数（如果有的话）在索引 1 的地方，而最后一个参数在索引 lua_gettop(L) 处。当需要向 Lua 返回值的时候，C 函数只需要把它们以正序压到堆栈上（第一个返回值最先压入），然后返回这些返回值的个数。在这些返回值之下的，堆栈上的东西都会被 Lua 丢掉。和 Lua 函数一样，从 Lua 中调用 C 函数也可以有很多返回值。

下面这个例子中的函数将接收若干数字参数，并返回它们的平均数与和：
```c
     static int foo (lua_State *L) {
       int n = lua_gettop(L);    /* 参数的个数 */
       lua_Number sum = 0.0;
       int i;
       for (i = 1; i <= n; i++) {
         if (!lua_isnumber(L, i)) {
           lua_pushliteral(L, "incorrect argument");
           lua_error(L);
         }
         sum += lua_tonumber(L, i);
       }
       lua_pushnumber(L, sum/n);        /* 第一个返回值 */
       lua_pushnumber(L, sum);         /* 第二个返回值 */
       return 2;                   /* 返回值的个数 */
     }
```

### lua_call
void lua_call (lua_State *L, int nargs, int nresults);

调用一个函数。

要调用一个函数请遵循以下协议：首先，要调用的函数应该被压入栈；接着，把需要传递给这个函数的参数按正序压入；这是指

## C API
### lua_gettop
int lua_gettop (lua_State *L);
返回栈顶元素的索引。 因为索引是从 1 开始编号的， 所以这个结果等于栈上的元素个数； 特别指出，0 表示栈为空。

### lua_error
int lua_error (lua_State *L);
以栈顶的值作为错误对象，抛出一个 Lua 错误。 这个函数将做一次长跳转，所以一定不会返回 （参见 luaL_error）。

### luaL_error
int luaL_error (lua_State *L, const char *fmt, ...);

抛出一个错误。 错误消息的格式由 fmt 给出。 后面需提供若干参数， 这些参数遵循 lua_pushfstring 中的规则。 如果能获得相关信息，它还会在消息前面加上错误发生时的文件名及行号。

这个函数永远不会返回。 但是在 C 函数中通常遵循惯用法： return luaL_error(args) 。
