
<!DOCTYPE html>

<html>
<head>
<meta charset="UTF-8">

		<link rel="stylesheet" type="text/css" href="../easyui/themes/default/easyui.css">
		<link rel="stylesheet" type="text/css" href="../easyui/themes/icon.css">
		<script type="text/javascript" src="../easyui/jquery.min.js"></script>
		<script type="text/javascript" src="../easyui/jquery.easyui.min.js"></script>
<title>我爱嵌入式物联网</title>

<meta name="keywords" content="我爱嵌入式 www.waqrs.cn 单片机  51单片机  ucos ucosII ucosIII 嵌入式 Linux wifi模块  esp8266 mtk7681" />
<meta name="description" content="免费物联网平台 mtk7681 esp8266等,单片机  51单片机  ucos ucosII ucosIII 嵌入式 Linux wifi模块  各种嵌入式相关代码资料等免费下载" />



</head>
<style>
.p_info{
margin-left: 10px;
font-family: 楷体;
font-size: 19px;
color:rgb(17,57,101);
}
.div_class{
//float: left;
width: 100%;
//margin-left: 1px;
height: 205px;
border:solid rgb(17,57,101) 1px;
}
</style>
<body style="background-color: #F0F0F0;">
 <?php require  './nav_bar_nolog.php';?>  
  
 <div class="body_div">
 <!-- <table style="width: 100%;border-collapse: collapse;"> -->
 <table style="width: 100%;border-collapse: collapse;">
 	<tr >
 	<td width="270px" valign="top">
 	<div id="logform" style="border: solid 0px red; height: 50%; width: 100%; margin-left: 0px;
	 margin-top: 0px; text-align: center;float:left;">
		
	 	<p style="width:100%; height: 30px;line-height: 30px;background-color: rgb(17,57,101);
color: #fff;text-align: center;	margin-top:0px;margin-bottom:0px;border:solid rgb(17,57,101) 1px;">登录</p> 

	
		<div class ="div_class">
		<form method="POST" action="../login/login.php">
		
			
				<div style="margin-bottom: 10px;margin-top: 20px;">
					<input class="easyui-textbox"
						style="width: 70%; height: 35px; padding: 12px;" id="username"
						name="username"
						data-options="prompt:'用户名',iconCls:'icon-man',iconWidth:38">
				</div>
				<div style="margin-bottom: 20px">
					<input class="easyui-textbox" type="password"
						style="width: 70%; height: 35px; padding: 12px" id="password"
						name="password"
						data-options="prompt:'密码',iconCls:'icon-lock',iconWidth:38">
				</div>

				<input class="easyui-linkbutton" iconCls="icon-ok" type="submit"
					value="登录" name="submit" style="width: 70%; height: 32px;" />
					
				<div style="display: block;text-align: left;width: 100%;margin-top:10px;">
				<a href="/web_bcg/fs/regist/regist.html" 
			style="margin-left:15%;width:70%;font-size: 13px;color:rgb(17,57,101);">
				注册用户
			</a>
			  <?php
				if (isset ( $_GET ["err"] )) {
					echo "<p style=\"margin-left:15%;color:red;font-weight:bold;font-size: 13px;\" class=\"error\">".$_GET ["err"]."</p>";
				}
		?>
				</div>
			</form>
		</div>		
		</div>
	</td>
 	 	<td >
 	 <p style="width:100%; height: 30px;line-height: 30px;background-color: rgb(17,57,101);
color: #fff;text-align: center;	margin-top:0px;margin-bottom:0px;border:solid rgb(17,57,101) 1px;">登录</p>
 			<div class="div_class">
 				
 			<p>sssssssssss</p>
 			</div>
 		
 		</td>
 	</tr>



<!-- ddddddddddddddddddddddddddddddddddddddddddddddddddddddd -->
 	<tr >
 		<td>
 		<div style="float: left;width: 100%;;margin-left: 1px;height: 100%;">
			<div style="border:solid rgb(17,57,101) 1px; width:100%; height:200px;">
		<marquee direction="up">
			<p class="p_info">&nbsp;&nbsp;&nbsp;&nbsp;欢迎来到程序员物联网DIY,
			如果你有一个闲置wifi模块或者其他可以连接网络的嵌入式设备,那你可以用它接入网络来实现远程控制,突破局域网限制!</p>
		<p class="p_info">&nbsp;&nbsp;&nbsp;&nbsp;欢迎加群:423982829共同讨论!</p>
		
		</marpuee>

			</div>
	
		</div>
 		</td>
 	<td >
 			<div class="div_class">
 				<p style="width:100%; height: 30px;line-height: 30px;background-color: rgb(17,57,101);
color: #fff;text-align: center;	margin-top:0px;margin-bottom:0px;border:solid rgb(17,57,101) 1px;">登录</p>
 			<p>sssssssssss</p>
 			</div>
 		
 		</td>
 	</tr>
 	
 
 </table>
 
 
 
		
	

</div>  
</body>
</html>