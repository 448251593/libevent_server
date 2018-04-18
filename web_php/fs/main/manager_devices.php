<?php //include '../mysql/mysql_connect.php';?>
<?php include '../login/logined.php';?>

<html>
<meta charset="UTF-8">
<title>设备管理</title>
<head>
<link href="/web_bcg/fs/css/style.css" rel="stylesheet">
<script type="text/javascript" src="../easyui/jquery.min.js"></script>
</head>

<script type="text/javascript">
var   max_items = 0;
function save_name(name,id,idtype)
{
	console.log(name+"->"+id+'->'+idtype);
	 $.ajax({
		type : "post",
		url : "manager_devices_back.php",
		data:{"arg":2,"name":name,"id":id,"idtype":idtype},
		success:function(data){
			console.log(data);
			 var obj = eval('(' + data + ')');
			
			 if(obj.rslt == -1)
			 {
				alert(obj.info);
			 }
			 else
			 {
				 alert(obj.info);
				
				
			 }
		}
	}); 
	
}
function delete_by_id(id)
{
	console.log("->"+id);
	 $.ajax({
		type : "post",
		url : "manager_devices_back.php",
		data:{"arg":3,"id":id},
		success:function(data){
			console.log(data);
			 var obj = eval('(' + data + ')');
			
			 if(obj.rslt == -1)
			 {
				alert(obj.info);
			 }
			 else
			 {
				 alert(obj.info);
			 }
		}
	}); 
	
}
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
					 tmp+="<td><input type='checkbox' id='item'/></td>";
					 tmp+="<td><input type='text'  value='" +obj.data_arr[i][0] +"'/>";
					 tmp+='</td>'
					 tmp+='<td>'+ obj.data_arr[i][1]+'</td>';


						 if(obj.data_arr[i][2]=='1')
						 {
							 tmp+="<td>"+
							 "<select>"+
	 					  	"<option value ='1' selected='selected'>开关</option>"+
						  	"<option value ='2'>温度</option>"+
						  	"<option value ='3'>自定义</option>"+
							"</select>"+
							 "</td>";
						 }
						 else if(obj.data_arr[i][2]=='2')
						 {
							 tmp+="<td>"+
							 "<select>"+
	 					  	"<option value ='1' >开关</option>"+
						  	"<option value ='2' selected='selected'>温度</option>"+
						  	"<option value ='3'>自定义</option>"+
							"</select>"+
							 "</td>";
						 }
						 else
						 {
							 tmp+="<td>"+
							 "<select>"+
	 					  	"<option value ='1' >开关</option>"+
						  	"<option value ='2' >温度</option>"+
						  	"<option value ='3' selected='selected'>自定义</option>"+
							"</select>"+
							 "</td>";
						 }
					 tmp+="<td><button id=\"delete_btn\">删除</button>";
					 tmp+="<button id=\"save_btn\">保存</button>"+'</td>';

					 tmp+= '</tr>';
						//alert(tmp);
				     $("#alternatecolor tbody").append(tmp);
				} 
				//绑定input的
					  $("input:text").change(function(){
					//	alert($(this).parent().next().next().next().children().next().text());
						$(this).parent().prev().find("input").prop({"checked":true});
					});  
					
					$("button").click(function(){
				
						
						if($(this).attr('id')=="save_btn"){
							save_name($(this).parent().prev().prev().prev().find("input").val()
									,$(this).parent().prev().prev().text().trim()
									,$(this).parent().prev().find("select").val());

						}
						else if($(this).attr('id')=="delete_btn"){

							if(confirm("确定删除?"))
							{
								delete_by_id($(this).parent().prev().prev().text().trim());
							}
						}
					});
					
					
			 }  
		
		}
	});
	}
$(function(){
	init_id_list();
	
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
	padding: 4px;
	border-style: solid;
	border-color: #a9c6c9;
}
table.altrowstable td {
	border-width: 1px;
	padding: 1px;
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
	<th width="50px" style="text-align: center;">选择</th>
	<th width="200px">名称</th>
	<th width="200px">ID</th>
	<th width="200px">设备类型</th>
	<th>操作</th>
	
	
</tr>
</tbody>

</table>

</div >

<table class="altrowstable" style="width:100%;margin-top:1px;">
<tr>
<td width="100px">批量操作:</td>
<td><input type='checkbox' id="select_all"/>全选
 <button  onclick="deal_all();">批量修改</button>
  <button  onclick="delete_all();">批量删除</button>
 </td>
</tr>
</table>
</div>
<!-- edit 对话框 -->
<script type="text/javascript">

function  delete_all(){

	var  idarr = new Array();
	$("input:checkbox").each(function (i,e){
		//console.log($(this).prop("checked"));
		
		if($(this).prop("checked"))
		{
			$(this).prop({"checked":true});
			var tmp = $(this).parent().next().next().text().trim();
			if(tmp.length > 0)
			{
				idarr.push(tmp);
				
			}
		}
	});	
	

	 var arrString=JSON.stringify(idarr);    //将myArray对象转化为字符串
	 

	 console.log(arrString);
	
	if( arrString.length==2)
	{
		return;
	}
	$.ajax({
		type : "post",
		url : "manager_devices_back.php",
		data:{"arg":5,"idarr":arrString},
		success : function(data) {
			console.log("dd->"+data);
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
function  deal_all()
{
	var  idname = new Array();
	var  idarr = new Array();
	var  idarrtype = new Array();
	
	$("input:checkbox").each(function (i,e){
		//console.log($(this).prop("checked"));
		//console.log($(this).prop("id"));
		
		if($(this).prop("checked") && $(this).prop("id")=='item')
		{
			//$(this).prop({"checked":true});
			var tmp = $(this).parent().next().find("input").val();
// 			if(tmp.length > 0)
// 			{
				idname.push(tmp);
				idarr.push($(this).parent().next().next().text().trim());
				idarrtype.push($(this).parent().next().next().next().find("select").val());
// 			}
		}
	});	
	
	 var arrStringname=JSON.stringify(idname);    //将myArray对象转化为字符串
	 var arrString=JSON.stringify(idarr);    //将myArray对象转化为字符串
	 var arrStringtype=JSON.stringify(idarrtype);    //将myArray对象转化为字符串
	 
	 
	 console.log(arrStringname);
	 console.log(arrString);
	 console.log(arrStringtype);


	if(arrStringname.length ==2 || arrString.length==2)
	{
		return;
	}
	$.ajax({
		type : "post",
		url : "manager_devices_back.php",
		data:{"arg":4,"idname":arrStringname,"idarr":arrString,"idarrtype":arrStringtype},
		success : function(data) {
			console.log("ddd->"+data);
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
<div id = "dialog_id" style="border:solid rgb(17,57,101) 1px;
	  position:absolute;
	  top:50%;
	  left: 50%;
	  width:400px;
	  height:300px;	
	  margin-top:-150px;
	  margin-left: -200px;
	  background-color:#f0f0f0;
	  text-align:center;
	  display:none;">
	  <p style="width:100%; height: 30px;line-height: 30px;background-color: rgb(17,57,101);
color: #fff;text-align: center;display: block;margin-top:0px;margin-bottom:0px;">编辑</p>

		
	  </div>
</body>
</html>