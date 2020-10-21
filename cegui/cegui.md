# CE GUI
CE GUI 笔记

## 初始化 CEGUI
为了 CEGUI 初始化和渲染需要三个步骤：
- 创建基于 CEGUI::Renderer 对象实例
- 创建 CEGUI::System 对象（传入上面创建的 renderer）
- 每一帧调用函数 CEGUI::System::renderAllGUIContexts 进行渲染。

当然也需要加载一些数据及执行其他基础的初始化，这些在后面提及。

## 使用 ResourceProviders 加载数据
### 什么是 ResourceProviders 
CEGUI 使用一个 ResourceProvider 对象在 CEGUI library 和 外部文件加载系统（可能是当前机器的文件系统，或其他由）之间提供一个桥梁。