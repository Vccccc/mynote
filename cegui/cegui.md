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
现在，您需要知道的主要事情是，在使用基于 Xerces-C++ 的 XML
 解析器时，这些 .xsd 文件必须可用于 ResourceProvider 系统；例如，最好通过在包含模式文件的目录设置资源组， 并将该组设置为加载模式文件时 CEGUI::XercesParser 使用的默认值。（有关如何执行此操作的详细信息，请参见 2 - 使用 ResourceProviders 加载数据 ）

### The Data Files
如前所述，CEGUI 中的大多数数据文件都是基于 XML 的。 数据文件通常不使用通用的 “ .xml” 文件扩展名，而是根据文件实际表示的名称来命名。 例如，.imageset（用于 Imageset ）和.font（用于 Font ）（但是，该命名约定不是系统的要求-您可以随意使用任何方式命名文件）。 现在对每种文件类型的用途进行非常简短的概述。

#### Imageset
图像集是某些源表面上的命名区域的集合，例如图像文件或纹理。 在CEGUI术语中，每个命名区域都是一个 Image，并且是 CEGUI 使用的基本图像级别。 通过修改源图像文件或纹理内容，或通过修改Imageset 文件中已定义区域的区域，您可以轻松地更改CEGUI绘制内容的外观。

#### Font
字体文件定义了在 CEGUI 中使用的字体。 当前可以定义两种类型的字体：
- FreeTypeFont：这是一种基于标准字体文件的字体（例如 True-type '.ttf'字体）。通过.font XML 文件中的属性Type =“FreeType” 指定 FreeTypeFont。
- PixmapFont：这种类型的字体基于 Imageset，该 Imageset 为字体字形定义了 Image，并且最好将其视为位图字体。通过.font XML 文件中的属性 Type =“Pixmap” 指定一个PixmapFont。

#### LookNFeel
LookNFeel 文件构成 CEGUI 外观系统的主要部分。 每个文件都将包含一个或多个 WidgetLook 定义，这些定义向系统描述了各种图像和子窗口小部件组件，它们的位置，其他属性值等等。 这些WidgetLook 规范随后绑定到系统中的具体元素，以形成最终表示为 CEGUI中 的窗口和小部件的东西。
