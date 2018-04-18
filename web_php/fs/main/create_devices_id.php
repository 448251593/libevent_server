<?php //include '../mysql/mysql_connect.php';?>
<?php include '../login/logined.php';?>

<html>
<head>
<link href="/web_bcg/fs/css/style.css" rel="stylesheet">
<script type="text/javascript" src="../easyui/jquery.min.js"></script>
</head>

<script type="text/javascript">
var   max_items = 0;
function generate_id_add_user(self,id)
{

	$.ajax({
		type : "post",
		url : "create_devices_id_act.php",
		data:{"args":2,"id":id},
		success : function(data) {
			console.log(data);
			 var obj = eval('(' + data + ')');
			
			 if(obj.rslt == -1)
			 {
				alert(obj.info);
			 }
			 else
			 {
				alert(obj.info);
				$("#alternatecolor").find("tr").each(function(){

						if($(this).find("td:first").next().text().trim() == id)
						{
						//	console.log($(this).find("td:first").text());
						//	console.log($(this).find("td:first").next().text());
							
							$(this).find("td:first").next().next().html('是');
							$(this).find("td:first").next().next().next().find("button").css({"display":"none"});
							console.log($(this).find("td:first").next().next().text());
						}
					

				});

			 } 
		
		}
	});
}
function	init_id_list()
{
	$.ajax({
		type : "post",
		url : "create_devices_id_act.php",
		data:{"args":0},
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
				if(max_items >= 50){
					$("#generate_id").attr("disabled",true);	
				} 
				 //console.log(obj.data_arr);
				 
				 for(i=0;i<obj.data_arr.length;i++)
				 {
					
					 var tmp = '<tr>'
						 tmp+='<td>'+i+'</td>'

					 
					 if(obj.data_arr[i][1]==1)
					 {
						 tmp+='<td>--</td>'
						
						 tmp+='<td>'+obj.data_arr[i][0]+'</td>'
						 tmp +='<td>是</td>';
						 tmp +='<td></td>';
						 
					 }
					 else
					 {
						 tmp+='<td>'+"<input type='checkbox'/>"+'</td>'
						 tmp+='<td>'+obj.data_arr[i][0]+'</td>'
						 tmp +='<td>否</td>'; 
						 tmp+= "<td><button onclick=\"generate_id_add_user(this,'" + obj.data_arr[i][0] + "');\">添加到我的设备</button></td>";
					 }
						


						 tmp+='</tr>';
						 console.log(tmp);
						 $("#alternatecolor tbody").append(tmp);
				}
					
			 } 
		
		}
	});
	}
$(function(){
	init_id_list();
 
	//注册
	$("#generate_id").click(function(){
			if(max_items >= 50)
			{
				alert("达到最大值!");
			}
			$.ajax({
				type : "post",
				url : "create_devices_id_act.php",
				data:{args:1},
				success : function(data) {
					console.log(data);
					 var obj = eval('(' + data + ')');
					
					 if(obj.rslt == -1)
					 {
						alert(obj.info);
					 }
					 else
					 {
						 var tmp = '';
						 tmp +='<tr>';
						 tmp+="<td>"+($("#alternatecolor tbody tr").length )+"</td>"
						 tmp+='<td>'+"<input type='checkbox'/>"+'</td>'	
						  tmp += '<td>'+obj.data_arr[0][0]+'</td>'
						  if(obj.data_arr[0][1]==1)
							 {
								 tmp +='<td>是</td>';
								 tmp +='<td></td>';
								 
							 }
							 else
							 {
								 tmp +='<td>否</td>'; 
								 tmp+= "<td><button  onclick=\"generate_id_add_user(this,'" + obj.data_arr[0][0] + "');\">添加到我的设备</button></td>";
							 }
				
						 tmp+='</tr>';
							//alert(tmp);
						
					     $("#alternatecolor tbody").append(tmp);
					     $('#content').scrollTop( $('#content')[0].scrollHeight );
					}
					
				}
			});

	

	});
	
	
})




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
	text-align:center;
	border-width: 1px;
	padding: 2px;
	border-style: solid;
	border-color: #a9c6c9;
}
.oddrowcolor{
	background-color:#d4e3e5;
}
.evenrowcolor{
	background-color:#c3dde0;
}

.table_ctrl{
	font-size:13px;
	margin-top:1px;
	width:100%;
   border-collapse: collapse; 
}

.table_ctrl,td,th{
   border: solid  #a9c6c9 1px;  
}

</style>
<body style="background-color: #f0f0f0;">
<?php require  '../nav_bar.php';?>

<div class="body_div">
<div style="width:100%;height:30px;background-color:rgb(37,153,220);vertical-align:middle;text-align:center;display:none;">
设备ID生成
</div>
<p style="width:100%; height: 30px;line-height: 30px;background-color: rgb(17,57,101);
color: #fff;text-align: center;display: block;margin-top:0px;margin-bottom:0px;">设备ID列表</p>
<!--查询设备id,生成列表-->
<div id="content" style="border:solid rgb(17,57,101) 1px; width:100%; height:68%; overflow:auto">
<table class="altrowstable" id="alternatecolor"  style="width:100%;margin-top:0px;">
<tbody>
<tr>
	
	<th width="60px">序号</th>
	<th width="60px">选择</th>
	<th width="180px">设备ID</th>
	<th width="180px">是否在我的设备</th>
	<th>操作</th>
</tr>
</tbody>

</table>

</div >

<table  class="table_ctrl">
<tr >
	<td width="200px" >生成一个新的设备ID:</td>
	<td >
		<button  id="generate_id" style="margin-left: 60px;width:100px;">生成设备ID</button>
	</td>
</tr>
<tr >
	<td width="200px">批量添加操作:</td>
	<td >
		<input id="select_all"  type="checkbox"  />全选
		<button  id="generate_id" onclick="deal_all();" style="margin-left: 0px;width:100px;">批量添加操作</button>
	</td>
</tr>
</table>



</div>

</body>
<script type="text/javascript">
//批量添加id到用户下面去
function  deal_all()
{
	var  idarr = new Array();
	$("input:checkbox").each(function (i,e){
		//console.log($(this).prop("checked"));
		
		if($(this).prop("checked"))
		{
			$(this).prop({"checked":true});
			idarr.push( $(this).parent().next().text().trim());
		}
	});	
	
	 var arrString=JSON.stringify(idarr);    //将myArray对象转化为字符串
	 
	 console.log(arrString);
	 
	$.ajax({
		type : "post",
		url : "create_devices_id_act.php",
		data:{"args":3,"idarr":arrString},
		
		success : function(data) {
			console.log(data);
			 var obj = eval('(' + data + ')');
			
			 if(obj.rslt == -1)
			 {
				alert(obj.info);
			 }
			 else
			 {
				alert(obj.info);
				window.location.reload();
			 } 
		}
	});
	}
	
$(function(){

	$("#select_all").click(function(){
		var v =	$(this).prop("checked")
		$("input:checkbox").each(function (i,e){
			//console.log($(this).prop("checked"));
			
			if(v)
			{
				$(this).prop({"checked":true});
			}
			else
			{	
				$(this).prop({"checked":false});
			}
	});
			
		
		});
	
})


</script>
</html>