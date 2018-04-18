<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>帮助</title>

 <script type="text/javascript" src="../easyui/jquery.min.js"></script>


<!-- <LINK rel="stylesheet" type="text/css" href="./css/neiye_xiaohua1.css?v0.1">
<link href="./css/albumxxkxy1.css?v0.1" rel="stylesheet" media="screen" type="text/css"> -->

</head>
<style>
.p_info{
margin-left: 10px;
font-family: 楷体;
font-size: 19px;
color:rgb(17,57,101);
background-color: rgb(180,205,230);
}
.p_info_sub{
margin-left: 50px;
font-family: 楷体;
font-size: 17px;
color:rgb(17,57,101);
}

</style>
<body style="background-color: #F0F0F0;">
 <?php
 if(isset($_GET["srcflg"]))
 {
 	require  './nav_bar_nolog.php';
 }
 else {
 	include '../login/logined.php';
 	require  '../nav_bar.php';
 	
 }
 ?>  


 <div class="body_div">
<p style="width:100%; height: 30px;line-height: 30px;background-color: rgb(17,57,101);
color: #fff;text-align: center;display: block;margin-top:0px;margin-bottom:0px;">帮助</p>
<div id="help_div" style="border:solid rgb(17,57,101) 1px; width:100%; height:400px; overflow:auto">
<p class="p_info">如何接入:</p>
	<p class="p_info_sub">只要你的设备能进行TCP连接,那就可以接入,比如:esp8266 mtk7681等</p>
	<p class="p_info">流程:</p>
	<p class="p_info_sub">1.在网站注册一个用户;</p>
	<p class="p_info_sub">2.登录进去'设备->生成设备ID',添加到我的设备里面;</p>
	<p class="p_info_sub">3.进入 '设备->设备管理'添加设备的名称和类型;</p>
	<p class="p_info_sub">4.把激活的一个ID写到设备的发送消息(
			<a style="color: red;text-decoration:underline;cursor:pointer;" onclick="gotomsg()">消息格式</a>
			)程序里面;设备就已经连接上互联网了!</p>
	<p class="p_info_sub">5.进入 '我的设备就能看到设备的状态并控制'</p>
	<p class="p_info">控制设备:</p>
	<p class="p_info_sub">可以通过网站我的设备页面进行控制命令发送.</p>
	<p id="msg_note" class="p_info">消息格式:</p>
	<p class="p_info_sub">
	<b>设备到服务器:</b><br>
	{"ver":"1.0","dev_id":"bcg57ef6bfe6365e","cmdtype":"1","data":"1"}
	<br>
	ver:协议版本固定1.0<br>
	dev_id:设备ID,注册后可以生成ID<br>
	cmdtype:1 上报状态 ,data :0/1<br>
	cmdtype:2 上报温度,data:温度<br>
	cmdtype:3 上报自定义数据,data:自定义数据 ,最大长度30byte<br>
	<b>服务器到设备:</b><br><br>
	{"ver":"1.0","cmdtype":"1","data":"1"}<br>
	ver:协议版本固定1.0<br>
	cmdtype:1 设置状态,data区域为 0 或 1<br>
	cmdtype:2 获取状态,data区域 无<br>
	cmdtype:3 获取温度,data区域 无<br>
	cmdtype:4 自定义数据,data区域自定义数据,最大长度30byte<br>
	
	</p>
</div>
</div> 

</body>
<script type="text/javascript">

function gotomsg()
{

	var container = $('#help_div'),scrollTo = $('#msg_note'); 
	container.scrollTop( 
			scrollTo.offset().top - container.offset().top + container.scrollTop() 
			); 
	setTimeout(() => {
		scrollTo.css({"background-color":"rgb(180,205,230)"});
	}, 200);
	scrollTo.css({"background-color":"rgb(149,226,62)"});
}

</script>
</html>