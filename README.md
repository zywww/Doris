# Doris
一个扩展的正则表达式引擎

##分支
|

##限定符
\* 匹配上一个元素零次或多次。
\+ 匹配上一个元素一次或多次。
? 匹配上一个元素零次或一次。
{n} 匹配上一个元素恰好 n 次。
{n,} 匹配上一个元素至少 n 次。
{n,m} 匹配上一个元素至少 n 次，但不多于 m 次。
? 非贪婪匹配

##字符转义
\f	换页符
\n	换行符。	\x0a 
\r	回车符。	\x0d 
\t	Tab 字符。	\x09 
\v	垂直制表符。\x0b 
\xnn 使用十六进制表示形式指定字符（nn 恰好由两位数字组成）

##字符类
[charset] 匹配 charset 中的任何单个字符。 默认情况下，匹配区分大小写。
[^charset] 求反：与不在 charset 中的任何单个字符匹配。 默认情况下，charset中的字符区分大小写。
. 通配符：与除 \n 之外的任何单个字符匹配。
\w 与任何单词字符匹配。
\W 与任何非单词字符匹配。
\s 与任何空白字符匹配。
\S 与任何非空白字符匹配。
\d 与任何十进制数字匹配。
\D 匹配不是十进制数的任意字符。

##定位点
^ 匹配必须从字符串或一行的开头开始。
$ 匹配必须出现在字符串的末尾或出现在行或字符串末尾的 \n 之前。
\b 匹配必须出现在 \w（字母数字）和 \W（非字母数字）字符之间的边界上。
\B 匹配不得出现在 \b 边界上。

##分组构造
(子表达式) 捕获匹配的子表达式并将其分配到一个从 1 开始的序号中。
(?:子表达式) 定义非捕获组。
(?<名称>子表达式) 将匹配的子表达式捕获到一个命名组中 (?<double>\w)\k<double>
(?=纯正则表达式) 正向预查	
(?!纯正则表达式)  反向预查

##反向引用
\number 后向引用。 匹配编号子表达式的值。如果大于9，可以用命名分组
\k\<name\> 命名后向引用。 匹配命名表达式的值。

