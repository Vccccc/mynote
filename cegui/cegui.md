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
CEGUI 使用一个 ResourceProvider 对象在 CEGUI library 和 外部文件加载系统（可能是当前机器的文件系统，或其他如 Ogre3D 和 Irrlicht 引擎提供的资源管理子系统）之间提供一个桥梁。通过提供 ResourceProvider 接口的自定义实现，可以为 CEGUI 无缝地集成所有这些系统。

### DefaultResourceProvider
 DefaultResourceProvider 为 CEGUI 的默认 resource provider，为那些还没有或不需要更复杂的功能的人提供一些基本功能。除了提供 CEGUI 实际加载文件数据所需的功能外，DefaultResourceProvider 还对 “资源组” 提供了基本支持。资源组基本上是给系统上的目录位置标签。这样一来，便可以对目录中的文件进行逻辑分组，然后可以通过简单的标签而不是硬编码的路径来引用这些位置。这意味着如果您需要更改某些数据文件的位置，则只需要更新资源组的位置，而不是更新整个代码和XML文件中的路径信息。

### 指定资源组和目录
DefaultResourceProvider 可以使你定义任意数量命名的资源组和每个组指定一个访问目录。你可以创建一个资源组为 “imagesets”，然后分配一个目录给它，比如“./mygame/datafiles/gui/imagesets/”。当通过 ImageManager 加载一个 Imageset 时，你需要指定一个资源组如 “imagesets”，然后系统会查找前面定义的位置“./mygame/datafiles/gui/imagesets/”。当前资源组只支持一个目录与其关联。