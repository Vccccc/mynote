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
DefaultResourceProvider 可以使你定义任意数量命名的资源组和每个组指定一个访问目录。你可以创建一个资源组为 “imagesets”，然后分配一个目录给它，比如“./mygame/datafiles/gui/imagesets/”。当通过 ImageManager 加载一个 Imageset 时，你需要指定一个资源组如 “imagesets”，然后系统会在前面定义的位置“./mygame/datafiles/gui/imagesets/”查找。注意，当前资源组只支持一个目录与其关联。

下面是一个例子。
为了避免通过一个显式路径加载资源，如：
```c
ImageManager::getSingleton().loadImageset(
  "./mygame/datafiles/gui/imagesets/WindowsLook.imageset");
```

在初始化时，你需要在默认 resource provider 中设置一个资源组，如：
```c
DefaultResourceProvider* rp = static_cast<DefaultResourceProvider*>(
    CEGUI::System::getSingleton().getResourceProvider());
rp->setResourceGroupDirectory("imagesets", "./mygame/datafiles/gui/imagesets/");
```

在后续的代码中，当你需要加载一个 imageset 时，只需要做：
```c
ImageManager::getSingleton().loadImageset(
    "WindowsLook.imageset", "imagesets");
```

稍后将向您展示如何为每种资源类型设置默认资源组，然后在加载资源时不必指定组。

需要考虑的重要一点是，在使用这种资源组方法时，包含对其他数据文件的引用的数据文件不应包含相对路径信息。通常，它们应仅具有所引用文件的实际文件名。这种方式 资源组系统可以得到更好的利用，并且还使以后移动文件变得更加容易。因为不必“修复”所有相对路径，而只需更新资源组路径即可。

### 指定默认资源组
表示可加载资源的每个核心系统类都有静态成员来设置和获取默认资源组。 加载给定类所需的特定数据文件时，将使用此资源组。

对于每个 resource consuming classe，静态成员的名称都相同（特殊例外是 ImageManager 和基于 xerces-c 的 XML paser-参见下文）：
```c
const String& getDefaultResourceGroup();
void setDefaultResourceGroup(const String& groupname);
```

以下是核心资源加载类及其加载的资源的列表：
- CEGUI::Font : Font xml and freetype loadable font files.
- CEGUI::Scheme : Scheme xml files
- CEGUI::WindowManager : Window layout xml files.
- CEGUI::WidgetLookManager : LookNFeel xml files
- CEGUI::ScriptModule : Script files in whichever scripted langauge.


> 注意
最后要考虑的一件事是 ResourceProvider 类也有一个默认资源组。 这应该被视为全局或默认值每当特定的资源加载类没有默认值时就使用它。 如果将所有数据都放在一个目录中，这将很有用。
