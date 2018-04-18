<?php
 include '../login/logined.php';
?>

<!DOCTYPE html>
<html>

<head>
	<link href="/web_bcg/fs/css/style.css" rel="stylesheet">
	<link rel="stylesheet" type="text/css" href="../easyui/themes/default/easyui.css">
	<link rel="stylesheet" type="text/css" href="../easyui/themes/icon.css">
	
	<script type="text/javascript" src="../easyui/jquery.min.js"></script>
	<script type="text/javascript" src="../easyui/jquery.easyui.min.js"></script>

<meta charset="UTF-8">
<title>主页</title>
</head>
<style>
html,body{height:100%}
.p_info{
margin-left: 10px;
font-family: 楷体;
font-size: 19px;
color:rgb(17,57,101);
}
</style>

<script language="javascript" type="text/javascript" src="../jquery-1.7.2/jquery.min.js"></script>
<script type="text/javascript">

</script>

<body style="background-color: #F0F0F0;height: 100%">
<?php require  '../nav_bar.php';?>

<div class="body_div">
<p style="width:100%; height: 30px;line-height: 30px;background-color: rgb(17,57,101);
color: #fff;text-align: center;display: block;margin-top:0px;margin-bottom:0px;">欢迎</p>
<div style="border:solid rgb(17,57,101) 1px; width:100%; height:400px; overflow:auto">

<p class="p_info">&nbsp;&nbsp;&nbsp;&nbsp;欢迎来到程序员物联网DIY,
如果你有一个闲置wifi模块,那你可以用它接入网络来实现远程控制,突破局域网限制.同时可以用它来实现更多好玩的控制!</p>
<p class="p_info">&nbsp;&nbsp;&nbsp;&nbsp;物联网就是通过把单片机接入
互联网,然后通过网络来控制.中间通过外网服务器来转发控制指令的数据,实现远程控制的!</p>
<img alt="示意图" src="./shiyitu.jpg" style="width:40%;height: 200px;margin-left: 30%;">
</div>
</div>
	
</body>





</script>
</html>