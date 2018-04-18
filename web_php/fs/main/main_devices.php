<?php //include '../mysql/mysql_connect.php';?>
<?php include '../login/logined.php';?>

<html>
<meta charset="UTF-8">
<head>
<link href="/web_bcg/fs/css/style.css" rel="stylesheet">
<script type="text/javascript" src="../easyui/jquery.min.js"></script>
</head>

<script type="text/javascript">
var idarr = new Array();
var   max_items = 0;

function	init_id_list()
{
	$.ajax({
		type : "post",
		url : "manager_devices_back.php",
		data:{"arg":1},
		success : function(data) {
			console.log(data);
			 var obj = eval('(' + data + ')');
			
			 if(obj.rslt == -1)
			 {
				alert(obj.info);
			 }
			 else
			 {
				 max_items = obj.data_arr.length;				
				 //console.log(obj.data_arr);				 
				 for(i=0;i < obj.data_arr.length; i++)
				 {					
					 var tmp = '<tr>';
					 tmp+="<td style='text-align:center;'>"+($("#alternatecolor tbody tr").length)+"</td>"
					 tmp+='<td >';
					 //判断名称是否为空 空则显示未命名
					 if(obj.data_arr[i][0].length == 0)
					 {
						 tmp+="<p style=\"text-align:center;\" > "+"未命名" +"</p>";
						}
					 else
					 {
						 tmp+="<p style=\"text-align:center;\" > "+obj.data_arr[i][0] +"</p>";
						 }
						
					 tmp+='</td >';
					 
					 tmp+='<td >';
					 tmp+="<p style=\"text-align:center;\"  id='"+obj.data_arr[i][1]+"'> "+"离线" +"</p>";
					 tmp+='</td >';
					 
					 if(obj.data_arr[i][2] == '1')
					 {
						tmp+="<td>开关</td>";
						 tmp+="<td><input type='checkbox' onclick=\"ctrl_cmd(this,'"+obj.data_arr[i][1]+"')\"/></td>";
					 }
					 else if(obj.data_arr[i][2] == '2')
					 {
						 tmp+="<td>温度</td>";
						 tmp+="<td><input type='text' value='23'/>℃</td>";
					 }
					 else
					 {
						 tmp+="<td>自定义</td>";
						 tmp+="<td><input type='text' value=''/></td>";
					 }
					 
					
				
					 tmp+='<td>'+ obj.data_arr[i][1]+'</td>';
						
						tmp+='</tr>';
						
				     $("#alternatecolor tbody").append(tmp);

				     idarr.push(obj.data_arr[i][1]);
				} 
				//绑定input的
					$(":input").change(function(){

						$(this).next().css({"display":"inline","margin-left":"15px","font-size":"15px"});
					});
// 					$("button").click(function(){
// 						save_name($(this).prev().val(),$(this).parent().next().text())
// 						//$(this).css({"display":"none"});
						
// 					});
					setTimeout(function(){
						refresh_status();
					},'1000');
			 }  
		
		}
	});
	}
$(function(){
	init_id_list();

})
function  ctrl_cmd(self,id)
{
	var status =0;
	console.log(self.checked+"-->"+id);
	if( self.checked)
	{
		 status =1;
	}

	
	$.ajax({
		type: "GET",
		 url: "../backmng/tcp_api.php",
		 data:{"args":3,"dev_id":id,"set_status":status},
		 success: function(data){

			// $("#logout").html(data);
				console.log(data);
				
				
		 }
		});
}


function refresh_status()
{
	for(var i = 0;i < idarr.length; i++)
	{
	 $("p").each(function (t,e){
			if($(this).prop("id") == idarr[i])
			{
				$(this).text("离线");
				$(this).css({"background":"rgb(240,240,240)"});
				console.log(i+$(this).prop("id"));
			//	break;
			}
// 			console.log(i+$(this).prop("id"));
		});	
	}
		
	for(var i = 0;i < idarr.length; i++)
	{
		console.log(idarr[i]);

		$.ajax({
			type: "GET",
			 url: "../backmng/tcp_api.php",
			 data:{"args":1,"dev_id":idarr[i]},
			 success: function(data){

				// $("#logout").html(data);
					console.log(data);
					 var obj = eval('(' + data + ')');
						$("p").each(function (i,e){
							if($(this).prop("id") == obj.dev_id)
							{
								$(this).css({"background":"rgb(123,202,61)"});
								$(this).text("在线");
								//类型
								if(obj.cmdtype=='1')//开关
								{
									if(obj.dev_data == '1')
									{
										$(this).parent().next().next().children().prop({"checked":true});
									}
									else
									{
										$(this).parent().next().next().children().prop({"checked":false});
									}
								}
								else if(obj.cmdtype=='2')//温度
								{
									$(this).parent().next().next().children().prop({"value":obj.dev_data});
								}
								else if(obj.cmdtype=='3')//自定义
								{
									$(this).parent().next().next().children().prop({"value":obj.dev_data});
								}
																
								
							}
						
						});			
			 },
			 error:function(e){
		
				 }
			});	
	}	
}


</script>


<!-- CSS goes in the document HEAD or added to your external stylesheet -->
<style type="text/css">
table.altrowstable {
	font-family: verdana,arial,sans-serif;
	font-size:11px;
	color:#333333;
	border-width: 1px;
	border-color: #a9c6c9;
	border-collapse: collapse;
}
table.altrowstable th {
	border-width: 1px;
	padding: 8px;
	border-style: solid;
	border-color: #a9c6c9;
}
table.altrowstable td {
	border-width: 1px;
	padding: 8px;
	border-style: solid;
	border-color: #a9c6c9;
}
.oddrowcolor{
	background-color:#d4e3e5;
}
.evenrowcolor{
	background-color:#c3dde0;
}
</style>
<body style="background-color: #f0f0f0;">
<?php require  '../nav_bar.php';?>

<div class="body_div">

<p style="width:100%; height: 30px;line-height: 30px;background-color: rgb(17,57,101);
color: #fff;text-align: center;display: block;margin-top:0px;margin-bottom:0px;">设备管理</p>

<div style="border:solid rgb(17,57,101) 1px; width:100%; height:65%; overflow:auto">
<table class="altrowstable" id="alternatecolor"  style="width:100%;margin-top:0px;">
<tbody>
<tr>
	<th width="60px">序号</th>
	<th width="100px">名称</th>
	<th width="100px">状态</th>
	<th width="50px" >类型</th>

	<th width="100px">操作</th>
	
	<th>ID</th>
</tr>
</tbody>

</table>

</div >
<table class="altrowstable" style="width: 100%;">
<tr>
	<td style="width: 100px;text-align: center;">操作:</td>
	<td>
	<button style="width: 100px;" onclick="refresh_status();">刷新</button>
	</td>
</tr>
</table>

</div>
</body>
</html>