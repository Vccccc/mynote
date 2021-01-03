# 配置
## Ijson
### what
一个接口类，提供接口将文本解析到内存，或将 json 转换成文本
### how
使用 rapidjson 库
### why
配置文件以 json 格式存储，读取配置文件时需要将配置文本解析到内存中。保存配置文件时需要转换成文本。
### implementation
