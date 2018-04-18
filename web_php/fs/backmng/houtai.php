
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>后台操作界面</title>



<script language="javascript" type="text/javascript" src="../jquery-1.7.2/jquery.min.js"></script>
<script type="text/javascript">



$(document).ready(function() { 
// put all your jQuery goodness in here. 

		$("#btn1").click(function(){
			var dev_id=$("#dev_id").val().trim();
			if(dev_id.length>0)
			{
		
				$.ajax({

					type: "GET",
					 url: "tcp_api.php",
					 data:{"args":1,"dev_id":dev_id},
					 success: function(data){

						 $("#logout").html(data);
					 }
					});
			}
		});

		$("#btn2").click(function(){
			

		
				$.ajax({

					type: "GET",
					 url: "tcp_api.php",
					 data:{"args":2},
					 success: function(data){

						 $("#runstatus").html(data);
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
			<td colspan="2" style="text-align: center;font-size: 20px;">www.waqrs.com</td>
		</tr>
		<tr>
			<td width="100px">设备状态:</td>
			<td >
			<label>id:</label>
			<input id="dev_id"
				size="100" value=""> <br>
			
		
			
				
				<button id="btn1" >查看</button>
				<br>
				<div id="logout"></div>
				</td>
		</tr>
		<tr>
			<td width="100px">设备控制:</td>
			<td >
			状态:<input type="checkbox" id ='status'> 
				
				<br>
				<div id="ctrl_dev" ></div>
				</td>

		</tr>
	<tr>
			<td width="100px">服务器状态:</td>
			<td >
				<button id="btn2" >查看</button>
				
				<br>
				<div id="runstatus" ></div>
				</td>

		</tr>
	</table>
	
</body>

</html>