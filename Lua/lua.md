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

要调用一个函数请遵循以下协议：首先，要调用的函数应该被压入栈；接着，把需要传递给这个函数的参数按正序压入；这是指第一个参数首先压栈。最后调用一下 lua_call；nargs 是你压入栈的参数个数。当函数调用完毕后，所有的参数以及函数本身都会出栈。而函数的返回值这时则被压栈。返回值的个数将被调整为 nresults 个，除非 nresults 被设置成 LUA_MULTRET。在这种情况下，所有的返回值都被压入堆栈中。Lua 会保证返回值都放入栈空间中。函数返回值将按正序压栈（第一个返回值首先压栈），因此在调用结束后，最后一个返回值将被放在栈顶。被调用函数内发生的错误（通过 longjump）一直上抛。

下面例子中，这行 Lua 代码等价于在宿主程序中用 C 代码做一些工作：
a = f("how", t.x, 14)

这里是 C 的代码：
```c
     lua_getglobal(L, "f");                  /* function to be called */
     lua_pushliteral(L, "how");                       /* 1st argument */
     lua_getglobal(L, "t");                    /* table to be indexed */
     lua_getfield(L, -1, "x");        /* push result of t.x (2nd arg) */
     lua_remove(L, -2);                  /* remove 't' from the stack */
     lua_pushinteger(L, 14);                          /* 3rd argument */
     lua_call(L, 3, 1);     /* call 'f' with 3 arguments and 1 result */
     lua_setglobal(L, "a");                         /* set global 'a' */
```
注意上面这段代码是 平衡 的： 到了最后，堆栈恢复成原有的配置。 这是一种良好的编程习惯。
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


### lua_getglobal
int lua_getglobal (lua_State *L, const char *name);

把全局变量 name 里的值压栈，返回该值的类型。

### lua_pushliteral
const char *lua_pushliteral (lua_State *L, const char *s);

这个宏等价于 lua_pushstring， 区别仅在于只能在 s 是一个字面量时才能用它。 它会自动给出字符串的长度。

### lua_pushstring
const char *lua_pushstring (lua_State *L, const char *s);

将指针 s 指向的零结尾的字符串压栈。Lua 对这个字符串做一个内部副本（或是复用一个副本）， 因此 s 处的内存在函数返回后，可以释放掉或是立刻重用于其它用途。