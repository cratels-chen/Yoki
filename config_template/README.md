配置文件是json格式的文件，其属性结果如下图所示：
```json
{
  "program_name": "Program Name",
  "program_path": "path/to/program",
  "rules_enabled":[
    "rule1",
    "rule2",
    "rule3"
  ],
  "exclude_paths":[
    "path1",
    "path2",
    "path3"
  ]
}
```
* program_name：待检测的工程名
* program_path：待检测的工程文件所在路径
* rules_enabled：开启的规则集。若为空则默认开启所有
* exclude_paths：排出的文件路径
