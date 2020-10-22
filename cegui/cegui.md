# CE GUI
CE GUI 笔记

## 1 - 初始化 CEGUI
为了 CEGUI 初始化和渲染需要三个步骤：
- 创建基于 CEGUI::Renderer 对象实例
- 创建 CEGUI::System 对象（传入上面创建的 renderer）
- 每一帧调用函数 CEGUI::System::renderAllGUIContexts 进行渲染。

当然也需要加载一些数据及执行其他基础的初始化，这些在后面提及。

## 2 - 使用 ResourceProviders 加载数据
### 什么是 ResourceProviders 
CEGUI 使用一个 ResourceProvider 对象在 CEGUI library 和 外部文件加载系统（可能是当前机器的文件系统，或其他如 Ogre3D 和 Irrlicht 引擎提供的资源管理子系统）之间提供一个桥梁。通过提供 ResourceProvider 接口的自定义实现，可以为 CEGUI 无缝地集成所有这些系统。

### DefaultResourceProvider
 DefaultResourceProvider 为 CEGUI 的默认 resource provider，为那些还没有或不需要更复杂的功能的人提供一些基本功能。除了提供 CEGUI 实际加载文件数据所需的功能外，DefaultResourceProvider 还对 “资源组” 提供了基本支持。资源组基本上是给系统上的目录位置作标签。这样一来，便可以对目录中的文件进行逻辑分组，然后可以通过简单的标签而不是硬编码的路径来引用这些位置。这意味着如果您需要更改某些数据文件的位置，则只需要更新资源组的位置，而不是更新整个代码和XML文件中的路径信息。

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
最后要考虑的一件事是 ResourceProvider 类也有一个默认资源组。 这应该被视为全局或默认值；每当特定的资源加载类没有默认值时就使用它。 如果将所有数据都放在一个目录中，这将很有用。

### ImageManager 默认资源组
由于没有额外的 Imageset 类（一个 Imageset 现在只是从 xml 文件加载在一起的一组图像），因此用于加载这些文件的默认组是在 CEGUI::ImageManager 类上设置的，并且使用与上面提到的“标准”，您需要的功能被命名为：
```c
const String& ImageManager::getImagesetDefaultResourceGroup();
void ImageManager::setImagesetDefaultResourceGroup(const String& resourceGroup);
```

指定的资源组将用作加载图像集 XML 文件和图像纹理文件的默认资源组。

### XercesParser 默认资源组
对于 XercesParser，有一个特殊的资源组设置，用于指定在哪里可以找到用于 XML 模式验证的 .xsd 模式文件。 对于这种特殊情况，您可以使用 PropertySet 接口并访问一个名为SchemaDefaultResourceGroup 的属性。 使用属性接口主要是为了避免直接与基于 xerces xml 的解析器模块链接以设置默认模式资源组的需要。

因为您可能无法提前知道实际使用的是哪个 XML 解析器模块，因此可能不知道该属性是否存在，所以在设置属性之前应检查属性是否存在。 这比检查 XML 分析器 ID 字符串中的 “Xerces” 更好，因为它使您可以与任何将来也可以提供验证的分析器模块无缝地工作（只要它公开相同的属性）。

比如：
```c
// setup default group for validation schemas
CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
    parser->setProperty("SchemaDefaultResourceGroup", "schemas");
```

### 完整例子
最后，我们将展示如何执行资源组及其目标目录的初始化，以访问CEGUI 随附的 datafiles 目录中的数据文件，以及如何分配用于所有资源类型的默认组。在初始化核心 CEGUI::System 对象之后，我们指定一组资源组及其目标目录（假设工作目录是 CEGUI
 包内的 “bin” 目录：
```c
// initialise the required dirs for the DefaultResourceProvider
CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
    (CEGUI::System::getSingleton().getResourceProvider());
rp->setResourceGroupDirectory("schemes", "../datafiles/schemes/");
rp->setResourceGroupDirectory("imagesets", "../datafiles/imagesets/");
rp->setResourceGroupDirectory("fonts", "../datafiles/fonts/");
rp->setResourceGroupDirectory("layouts", "../datafiles/layouts/");
rp->setResourceGroupDirectory("looknfeels", "../datafiles/looknfeel/");
rp->setResourceGroupDirectory("lua_scripts", "../datafiles/lua_scripts/");
// This is only really needed if you are using Xerces and need to
// specify the schemas location
rp->setResourceGroupDirectory("schemas", "../datafiles/xml_schemas/");
```

现在，我们有了一组资源组，并为其设置了目标目录。 最后，要使系统使用这些目录，我们设置要使用的默认资源组：
```c
// set the default resource groups to be used
CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
CEGUI::Font::setDefaultResourceGroup("fonts");
CEGUI::Scheme::setDefaultResourceGroup("schemes");
CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
CEGUI::WindowManager::setDefaultResourceGroup("layouts");
CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
// setup default group for validation schemas
CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
    parser->setProperty("SchemaDefaultResourceGroup", "schemas");
```

### 总结
这是 CEGUI 使用的 ResouceProvider 系统的简要介绍。 您已经了解了如何通过 CEGUI 的 DefaultResourceProvider 创建和使用资源组，以及如何为 CEGUI 使用的每种资源类型指定默认资源组。

## 3 - Data Files and Defaults Initialisation
CEGUI 使用各种不同的数据文件，在最初可能对所有这些文件的确切含义以及它们之间的相互关系有些困惑。 此外，还有一些系统范围的默认值-尽管在技术上不需要设置，但是您几乎肯定会希望使用
 CEGUI 为大多数应用程序设置。

### Font, Imageset, LookNFeel, Scheme, XSD? It's all XML!
除图形图像文件，脚本语言文件和可加载模块（.dll /.so文件等）外，CEGUI 所使用的所有数据文件均基于 XML。 实际上，这使我们遇到了您可能遇到的第一个潜在障碍：.xsd模式文件。

#### Schema Validation with Xerces-C++
使用此特定解析器的优点是它提供架构验证。模式验证是一种可以检查传入 XML 数据以确保其格式正确并包含所需信息的方法。为了使此验证有效，需要一些其他文件-称为 scheme 文件（不要与下面描述的 CEGUI 自己的 scheme 文件混淆）。可以在 CEGUI 发行版的 datafiles/xml_schema/ 目录中找到用于验证 CEGUI XML 的 scheme 文件，文件扩展名为.xsd。

现在，您需要知道的主要事情是，在使用基于 Xerces-C++ 的 XML
 解析器时，这些 .xsd 文件必须可用于 ResourceProvider 系统；例如，最好通过在包含 scheme 文件的目录设置资源组， 并将该组设置为加载 scheme 文件时 CEGUI::XercesParser 使用的默认值。（有关如何执行此操作的详细信息，请参见 2 - 使用 ResourceProviders 加载数据 ）

### The Data Files
如前所述，CEGUI 中的大多数数据文件都是基于 XML 的。 数据文件通常不使用通用的 “ .xml” 文件扩展名，而是根据文件实际表示的名称来命名。 例如，.imageset（用于 Imageset ）和.font（用于 Font ）（但是，该命名约定不是系统的要求-您可以随意使用任何方式命名文件）。 现在对每种文件类型的用途进行非常简短的概述。

#### Imageset
图像集是某些源表面上的命名区域的集合，例如图像文件或纹理。 在CEGUI术语中，每个命名区域都是一个 Image，并且是 CEGUI 使用的基本图像级别。 通过修改源图像文件或纹理内容，或通过修改Imageset 文件中已定义区域的区域，您可以轻松地更改CEGUI绘制内容的外观。

#### Font
字体文件定义了在 CEGUI 中使用的字体。 当前可以定义两种类型的字体：
- FreeTypeFont：这是一种基于标准字体文件的字体（例如 True-type '.ttf'字体）。通过.font XML 文件中的属性Type =“FreeType” 指定 FreeTypeFont。
- PixmapFont：这种类型的字体基于 Imageset，该 Imageset 为字体字形定义了 Image，并且最好将其视为位图字体。通过.font XML 文件中的属性 Type =“Pixmap” 指定一个PixmapFont。

#### LookNFeel
LookNFeel 文件构成 CEGUI 外观系统的主要部分。 每个文件都将包含一个或多个 WidgetLook 定义，这些定义向系统描述了各种图像和子窗口小部件组件，它们的位置，其他属性值等等。 这些WidgetLook 规范随后绑定到系统中的具体元素，最终形成 CEGUI中的窗口和小部件。

#### Scheme
Scheme 文件在很大程度上是将其他数据文件和资源分组在一起，并定义它们之间的某些相互作用，并形成一个单一点来加载 CEGUI 窗口和小部件的“外观”的一种手段。 Scheme 可以包含以下一项或多项：
- Imageset 指定一个 Imageset 文件，作为 Scheme 的一部分被加载。
-  Font 指定一个 Font文件，作为 Scheme 的一部分被加载。
- LookNFeel 指定一个 LookNFeel 文件，作为 Scheme 的一部分被加载。
- WindowSet 指定要加载的模块的名称，以及您希望在系统中注册的该模块中包含的一组小部件名称（如果未列出名称，则将注册该模块中的所有可用类型）。这是将其他核心窗口小部件类型添加到CEGUI 的便捷方法（特别是如果模块是由第三方提供的话）。
- WindowRendererSet 指定要加载模块的名称，以及您希望在系统中注册的该模块中包含的一组窗口渲染器名称（如果未列出名称，则注册该模块中的所有可用类型）。 “窗口渲染器”是一个可以控制某些基本窗口类型的渲染的对象，CEGUI 随附的所有窗口渲染器对象都通过使用 “Falagard” 换肤系统执行渲染（尽管这不是严格的要求）。
- WindowAlias 提供了一种通过备用名称引用窗口/窗口小部件类型的方法，它也可以用来“隐藏”已注册的窗口小部件类型和另一个窗口小部件类型（以便代替使用其他窗口小部件类型）。
- FalagardMapping 通过链接具体的窗口/小部件类型，窗口渲染器类型，小部件外观定义以及（可选）渲染效果来创建新的可用窗口类型。

#### Layout
Layout 文件包含窗口布局的 XML 表示形式。 每个嵌套的“ Window” 元素定义要创建的窗口或小部件，“Property” 元素定义每个定义的窗口的所需设置和属性值。

#### Config
CEGUI 的 Config 文件支持使您可以指定所有系统默认值，要加载的初始资源以及要执行的初始化和终止脚本文件（使用ScriptModule 时），而无需编写超出创建 Renderer 和 System
 对象的代码之外的 C++ 代码行。

### Loading the Basic Files
为了使一切正常运行，您需要加载一些文件。所需的典型文件集为：
- Imageset
- Font
- LookNFeel
- Scheme

从上面的描述中可能已经了解到，Scheme 文件的好处是它可以自动为您加载其他文件。 在本教程中，我们将加载一个 Scheme 文件和一个 Font 文件，假定该 Scheme 自动为我们加载了 Imageset 和 LookNFeel。 加载 Scheme 和 Font 文件的操作如下：
```c
// create (load) the TaharezLook scheme file
// (this auto-loads the TaharezLook looknfeel and imageset files)
CEGUI::SchemeManager::getSingleton().createFromFile( "TaharezLook.scheme" );
// create (load) a font.
// The first font loaded automatically becomes the default font, but note
// that the scheme might have already loaded a font, so there may already
// be a default set - if we want the "DejaVuSans-10" font to definitely
// be the default, we should set the default explicitly afterwards.
CEGUI::FontManager::getSingleton().createFromFile( "DejaVuSans-10.font" );
```

在上面的代码（和 CEGUI 示例中）中，假定资源组位置和默认组均已设置。默认情况下，CEGUI 的源包，SDK 和其他分发形式随附了TaharezLook.scheme 文件（本教程中用作示例）以及相应的
 imageset，font 和 looknfeel 文件（包括DejaVuSans-10.font）。 因此，取决于安装 CEGUI 的方式，这些文件可能已在您的系统上可用（例如，在Linux系统上的/usr/share/cegui/datafiles中）。 如果您下载了源软件包或从存储库中获取了 CEGUI，则这些文件应位于 CEGUI 文件夹的datafiles/ 文件夹中，该文件夹是默认文件夹以及 CEGUI SampleBrowser 在 Windows 上用于示例的结构。

### System Defaults Initialisation
最后，您需要为系统指定一些默认选项。这样可以确保当窗口或窗口小部件未指定其自身的首选项时，系统始终具有可用的字体和鼠标光标。
```c
CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont( "DejaVuSans-10" );
```
通常将设置的另一个默认对象是鼠标光标。这样可以确保当您将鼠标移到没有设置其自己的光标的任何元素上时，光标不会消失。为初始的默认 GUIContext 设置默认鼠标光标的代码如下：
```c
CEGUI::System::getSingleton().getDefaultGUIContext().
    getMouseCursor().setDefaultImage( "TaharezLook/MouseArrow" );
```

## Creating a CEGUI Window
本教程的目的是向您展示如何创建一个简单的窗口并将其显示在屏幕上。

### Introduction to window and widget concepts
在开始学习本教程的内容之前，您必须首先考虑一些基本概念。

#### Every widget is a window
这是使用 CEGUI 时必须掌握的最核心的概念。系统中的每个小部件都从相同的 Window 基类派生。因此，出于本教程的目的，每当我提到一个窗口时，相同的想法都可以轻松地应用于按钮或滚动条小部件。

#### Many settings are inherited
CEGUI 中可用于窗口的许多设置和属性都沿窗口层次结构传递。例如，如果将特定窗口的 Alpha 透明度设置为 0.5，则默认情况下，所有附加到该窗口的窗口和小部件也将受到较高级别应用的更改的影响；尽管如此，但请注意子窗口上的实际设置保持不变-最终值和（或）设置通常是从层次结构中所有窗口到当前窗口的设置值的某种组合。这也适用于诸如窗口销毁之类的事情。默认情况下，窗口在销毁时会销毁附加的子窗口和小部件。这种安排的主要优点是，您可以通过更改根窗口设置（例如 Alpha，可见性，启用/禁用状态）来轻松影响整个GUI，并可以通过简单地销毁根窗口来轻松“清理”整个 GUI 布局。在需要更多细粒度控制或要使用某些高级管理技术的情况下，可以在每个窗口的基础上覆盖默认的“继承”行为。

### Creating the windows
让我们创建一个窗口。
可以通过 C++代码 和 XML 布局文件两种方法来实现。每种方法将在下面讨论。

#### GUI Creation via C++ code
CEGUI 中的所有窗口都是由 WindowManager 单例对象创建的。 您可以通过 WindowManager::getSingleton 函数访问此对象：
```c
using namespace CEGUI;
WindowManager& wmgr = WindowManager::getSingleton();
```

通常，您将在 GUI 布局中使用 DefaultWindow 的作为 “root” 窗口。 这不是必需的，但强烈建议这样做，因为它在输入处理方面有两个有用的好处，并且符合CEGUI的公认用法。 此外，一旦开始添加更多顶级窗口，它就可以帮助简化布局。

对于任何 CEGUI 应用程序（以及您的布局文件），建议的 root 窗口都是 DefaultWindow，它覆盖了整个应用程序窗口。 只需将窗口的大小设置为 1.0 的相对宽度和高度，并将位置设置为原点（0,0），即可使其覆盖整个窗口。 这是新创建的 DefaultWindows 的默认设置，因此不必专门设置此类窗口。创建窗口后，可以随时将其设置为 root 窗口。始终只能有一个 root
 窗口，所有其他窗口都应附加到该窗口。

我们将创建一个 DefaultWindow 并将其设置为初始的默认GUIContext 的 root 窗口：
```c
Window* myRoot = wmgr.createWindow( "DefaultWindow", "root" );
System::getSingleton().getDefaultGUIContext().setRootWindow( myRoot );
```

WindowManager::createWindow 函数采用两个字符串作为其参数。在本示例中，第一个“DefaultWindow”告诉系统您要创建的窗口的类型或类。通常，可用窗口是在加载 scheme 文件时注册的窗口，尽管某些窗口（例如 DefaultWindow）是全局类型，并且始终可用。在此示例中，第二个参数“root”是将分配给窗口的名称。请注意，将 root 窗口命名为“root”不是必需的，而是常见的约定。

GUIContext::setRootWindow 函数用于指定一个窗口，以用作给定 GUIContext 的 GUI 的 root 窗口。这将替换任何当前的 root 窗口，尽管要注意，先前的窗口层次实际上并没有被销毁，它只是从 GUIContext 中分离出来的。您可以通过使用 GUIContext::setRootWindow 函数在 GUI “页面” 之间进行轻松地切换。

现在，您已经创建了第一个窗口并将其附加到 GUIContext。系统在绘制 GUI 时将使用此窗口作为 GUI 的 root。但是，如果您要使用此代码编译一个简单的程序，您仍然看不到任何内容。是什么呢？您的应用程序没有任何问题，我们上面创建的 DefaultWindow
 完全不可见！这就是使 DefaultWindow 非常适合用作 root 窗口的原因。它用作空白画布，可以在其上附加其他窗口和小部件。 

在这里，我们将创建一个 frame 窗口；这是一个窗口，其功能类似于桌面 UI 上的窗口，它具有标题栏并且可以移动和调整大小。
```c
FrameWindow* fWnd = static_cast<FrameWindow*>(
    wmgr.createWindow( "TaharezLook/FrameWindow", "testWindow" ));
```
在这里，我们正在创建一个 “TaharezLook/FrameWindow” 窗口。 该名称使用整个系统中的另一种约定，即窗口类型以小部件集的名称为前缀（如果要加载 WindowsLook 方案，则可以创建“ WindowsLook/FrameWindow”对象）。我们为新窗口赋予了简单的测试名称 “testWindow”。 最后需要注意的是强制转换的使用，这是必需的，因为 WindowManager::createWindow 函数始终返回Window 类型。在这种情况下（以及许多其他情况下），基本的Window指针就足够了，但是有时您需要访问子类中引入的函数，因此在使用 CEGUI 时，如所示的那样进行强制转换很常见。

为了使系统能够在新窗口中执行有用的操作，我们必须执行一些附加步骤。
首先，我们必须将窗口附加到上面建立的 root 窗口中：
```c
myRoot->addChild( fWnd );
```

现在，我们可以为窗口设置初始位置和大小。CEGUI 使用“统一”坐标系统，可以同时使用相对（比例）和绝对（偏移）分量。这就是为什么您将看到的每个坐标都有两个部分的原因。
```c
// position a quarter of the way in from the top-left of parent.
fWnd->setPosition( UVector2( UDim( 0.25f, 0.0f ), UDim( 0.25f, 0.0f ) ) );
// set size to be half the size of the parent
fWnd->setSize( USize( UDim( 0.5f, 0.0f ), UDim( 0.5f, 0.0f ) ) );
```

最后，我们为 frame 窗口的标题栏设置标题：
```c
fWnd->setText( "Hello World!" );
```

编译到应用程序中后，您现在将在显示中间看到一个简单的 frame
 窗口。

请注意，现在您已将 FrameWindow “testWindow” 附加到DefaultWindow “root”。因此，现在将是提及 CEGUI 的名称路径的好时机。随着布局变得更加复杂，您通常需要在给定已知起点（通常是 root 窗口）的情况下访问某些子窗口，这就是 CEGUI 的名称路径的亮点。每个窗口都有一个名称路径，该路径基本上是所有由斜杠分隔的祖先窗口的名称。在我们的示例中，FrameWindow 的名称路径为 “root/testWindow”。您可以通过其名称路径和 CEGUI::Window::getChild 函数访问层次结构中的任何窗口。请求子窗口时，请注意，您指定的路径不包含起点，因此要从 “root” 访问 FrameWindow，您需要访问 “testWindow” 而不是 “root/testWindow”，但是如果 FrameWindow 有，例如，附加到它的名为 “myButton” 的按钮可以从根目录访问该按钮，您将使用名称路径 “testWindow/myButton” 调用 Window::getChild。

#### XML layouts
上面所有这些都很好，但是有一个主要缺点。每当您希望调整 GUI
 布局时，都需要编辑代码并重新编译。这很快就会变旧，所以您真正想要的是能够在外部指定 GUI 布局，并使代码通过文件加载该布局。这是 CEGUI 布局 XML 文件的目的。

系统支持XML布局文件，可以通过 WindowManager::loadWindowLayout 函数加载该文件。此函数为您创建所有窗口，并返回指向已加载层次结构的 root 窗口的指针。

因此，首先我们需要一个布局文件。以下保存为文本文件的 XML 是等效于我们上面讨论的代码的布局文件：
```c
<?xml version="1.0" ?>
<GUILayout version="4">
    <Window type="DefaultWindow" name="root">
        <Window type="TaharezLook/FrameWindow" name="testWindow">
            <Property name="Position" value="{ {0.25, 0}, {0.25, 0} }" />
            <Property name="Size" value="{ {0.5, 0}, {0.5, 0} }" />
            <Property name="Text" value="Hello World!" />
        </Window>
    </Window>
</GUILayout>
```

Window 元素显示了到 WindowManager::createWindow 函数的明显映射，它们具有直接对应于前面讨论的参数的类型和名称。

Window 元素的嵌套用于将某些窗口附加到其他窗口。请注意，布局文件中可能只有一个 “root” 级窗口，这是另一个原因，您通常会看到 DefaultWindow 用作放置其他窗口和小部件的画布。

Property 元素用于在要定义的 Window 上设置属性。每个窗口/小部件类都有许多可用的属性，并且每个类还从其父类继承所有属性。

如果保存为名为 “test.layout” 的文件，则可以加载此布局并将其设置为 GUIContext 的 root，如下所示：
```c
using namespace CEGUI;
Window* myRoot = WindowManager::getSingleton().loadLayoutFromFile( "test.layout" );
System::getSingleton().getDefaultGUIContext().setRootWindow( myRoot );
```
最终结果与之前的 C++ 代码完全相同，不同之处在于，现在您可以修改和增强GUI布局，而无需不断编辑和重新编译应用程序代码。

## Layout XML files
布局XML文件定义了要创建的基于窗口的对象的层次结构，其中包含每个窗口的属性和事件绑定信息。

### Layout Element List:
- \<AutoWindow> Element
- \<Event> Element
- \<GUILayout> Element
- \<LayoutImport> Element
- \<Property> Element
- \<Window> Element

#### \<GUILayout> Element
- Purpose：
	- \<GUILayout> 元素是 XML 布局文件中的根元素
- Attributes：
	- version：应该在所有源文件指定版本，当前版本为：4
- Usage：
	- \<GUILayout> 元素是 XML 布局文件中的根元素
	- \<GUILayout> 元素必须至少包含一个 /<Window> 元素
	- \<GUILayout> 元素不能作为其他元素的子元素


#### \<Window> Element
- Purpose：
	- \<Window> 元素用于指定要创建的新窗口对象
- Attributes：
	- type：指定要创建的窗口的类型。这可能是指具体的窗口类型，别名或 falagard 映射类型。必填属性。
	- name：指定要提供窗口的名称。 窗口名称在系统中必须唯一。 可选属性，如果未指定该属性，则会自动生成一个名称。
- Usage：
	- \<Window> 可作为以下内容的子元素
		- \<GUILayout> 元素
		- \<Window> 元素
		- \<AutoWindow> 元素
	- \<Window> 元素可能包含以下子元素：
		- 任意数量的 <LayoutImport> 元素。Followed by,
		- 任意数量的 <Property> 元素。Followed by,
		- 任意数量的 <Event> 元素。Followed by,
		- 任意数量的 <Window> 元素
		- 任意数量的 <AutoWindow> 元素
		- 最后，任意数量的 <Property> 元素（again）

#### \<Property> Element
- Purpose：
	- \<Property> 元素用于设置由包含 <Window> 元素创建的Window 的属性。
- Attributes：
	- name：指定要设置的属性的名称。 必填属性
	- value：指定要在属性上设置的值。 可选属性，如果未指定该属性，则应通过元素文本内容给出要设置的值。
- Usage：
	- \<Property> 元素可以作为 \<Window> 和 \<AutoWindow> 元素的子元素
	- \<Property> 元素可能不包含任何子元素
	- \<Property>元素可能包含要用作值的文本内容。

#### \<Event> Element
- Purpose：
	- Event 元素用于在 GUI 元素和脚本函数之间创建绑定。
- Attributes：
	- name：指定将由 Function 属性指定的处理程序订阅的事件的名称。 必填属性。
	- function：指定要订阅由 Name 属性指定的事件的脚本函数的名称。 必填属性。
- Usage：
	-  \<Event> 元素可以作为 \<Window> 和 \<AutoWindow> 元素的子元素
	- \<Event> 元素可能不包含任何子元素


#### \<LayoutImport> Element
- Purpose：
	- \<LayoutImport> 元素用于引用（导入）另一个布局文件。导入布局的根窗口将附加到发生导入的窗口。
- Attributes：
	- filename：指定要导入的 xml 布局文件的路径。 必填属性。
	- resourceGroup：指定在加载“filename”属性中给定的文件时传递给 ResourceProvider 的资源组标识符。 可选属性，默认为 WindowManager 默认组。
- Usage：
	- \<LayoutImport> 元素可以作为 \<Window> 和 \<AutoWindow> 元素的子元素
	- \<LayoutImport> 元素可能不包含任何子元素

#### \<AutoWindow> Element
- Purpose：
	- \<AutoWindow>元素用于将属性设置，事件和其他内容定位到当前窗口（或 AutoWindow）的自动创建的子窗口上。
- Attributes：
	- namePath：指定目标自动窗口的名称路径。 必填属性。
- Usage：


