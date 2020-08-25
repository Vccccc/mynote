# BlockMango 源码笔记

## common
### item_class
主要提供访问 item 配置属性的一些接口，如 id()、full_name()。

### item
提供操作 item 的公共接口（最上层接口）。如创建 item，消耗 item 等。主要是做逻辑处理。

### entity
初始化 entity 属性和配置。分别从 C++、Lua、json 中定义，其中 json 文件是由客户定制，
可覆盖 C++ 和 Lua 中的定义。配置加载后不可改动，属性可以动态变化。

### tray_class_base
tray 的基类，用于初始化（构造）一个 tray。
## server
### item_data
item_data 继承于 item_class。用于创建一个物品（关联一张表），设置一些动态属性（如物品个数），提供序列，逆序列化方法。

### item_manager
item_manager 用于创建、序列化、逆序列化 item。主要是调用 item_data 的接口。

## skill
技能系统分析笔记

释放技能场景可划分为几个阶段：
1. 播放抬手动作（可被中断）
2. 播放技能特效