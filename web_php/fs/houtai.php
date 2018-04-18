
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>后台操作界面</title>



<script language="javascript" type="text/javascript" src="jquery-1.7.2/jquery.min.js"></script>
<script type="text/javascript">



$(document).ready(function() { 
// put all your jQuery goodness in here. 

		$("#btn1").click(function(){
			var url=$("#wangzhi").val().trim();
			var deep_ct=$("#deep_ct").val().trim();
		
				$.ajax({

					type: "GET",
					 url: "get_pic_url.php",
					 data:{url:url,deep_ct:deep_ct},
					 success: function(data){

						 $("#logout").html(data);
					 }
					});
		});
}); 
</script>
<style type="text/css">
table
  {
    	 font-size:15px;
  	 font-family:宋体;
  border-collapse:collapse;
  }

table, td, th
  {

	  border:1px solid blue;

	  padding: 5px;
  }
  button{
  width: 80px;
  margin-top: 9px;
  
  }
</style>
</head>
<body>

	<table style="width: 80%; margin-left: 10%; margin-top: 30px;">
		<tr>
			<td colspan="2" style="text-align: center;font-size: 20px;">sdf</td>
		</tr>
		<tr>
			<td width="100px">图片:</td>
			<td >
			<label>网址:</label>
			<input id="wangzhi"
				size="100" value="http://imgxiao.39yst.com/uploads/allimg/160907/16-160ZF94922.jpg"> <br>
			
			<div style="margin-top: 5px;">
			<label>深度:&nbsp</label><input id="deep_ct"	size="5" value="5"> <br>
			</div>
			
				
				<button id="btn1" >执行</button></td>
		</tr>
		<tr>
			<td width="100px">段子:</td>
			<td ><label style="font-size: 18">网址:</label><input id="wangzhi"
				size="100" value=""> <br>
				<button id="btn2" >执行</button></td>

		</tr>
	
	</table>
	<div id="logout" style="border: solid 1px blue;width: 80%;margin-left: 10%;height: 30%;"></div>
</body>

</html>