# BlockMango 源码笔记

## common
### item_class
主要提供访问 item 配置属性的一些接口，如 id()、full_name()。

## server
### item_data
item_data 继承于 item_class。用于创建一个物品（关联一张表），设置一些动态属性（如物品个数），提供序列，逆序列化方法。

### item_manager
item_manager 用于创建、序列化、逆序列化 item。主要是调用 item_data 的接口。