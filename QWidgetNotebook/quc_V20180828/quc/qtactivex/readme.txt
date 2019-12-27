将生成好的qtactivex.exe放到qt运行库的bin目录，然后cmd命令执行 idc.exe qtactivex.exe /regserver,执行成功会打印Server registered successfully!

新建文本文件，写入内容，保存成html文件。html文件可以在任意位置，如果环境变量中设置了qt运行库的话。
<html>

<head>
<title>Qt ActiveX Demo</title>
</head>

<body>
<object id="QtActiveX" width="100%" height="100%" classid="clsid:4dc3f340-a6f7-44e4-a79b-3e9217695fb8"></object>
</body>

</html>

用IE浏览器打开html文件即可预览。
