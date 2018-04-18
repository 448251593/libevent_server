<?php include '../mysql/mysql_connect.php';?>
<?php include '../login/logined.php';?>
<style>
.table_mng{
	border-collapse: collapse;
	width:100%;
	height:80%;
	text-align: center;
}
.table_mng th{
	background-color:#ADD8E6;
	border: solid 1px red;
	vertical-align: middle;	
} 
.table_mng td{

	border: solid 1px red;
	vertical-align: middle;
		
} 
.table_mng  tr{
	vertical-align: middle;
	border: solid 1px red;
	height: 30px;
} 
</style>


<table style="width: 100%;" >
	
		<tr style="background-color: #ADD8E6">
		<th >名称</th>
		<th >设备id</th>
		<th >状态</th>
		<th >绑定时间</th>
		
		
		</tr>
<?php

$args = $_POST ["arg"];
if ($args == 1) {
	
	$sql_inst="SELECT a.dev_name,a.deviceid,a.enable,a.timestamp FROM user_to_dev_tb a
				where a.username='".$_SESSION["username"]."'";
	//echo  $sql_inst;
	$result = mysql_query($sql_inst,$con);
	
	if (!$result)
	{
		die('Error: ' . mysql_error());
	}
	else
	{

		while($row = mysql_fetch_array($result))
		{
			echo  "<tr style=\"height: 10px;\">";
			
			echo  "<td>".$row['dev_name'] . "</td>";
			echo  "<td>".$row['deviceid'] . "</td>";
			echo  "<td>".$row['enable'] . "</td>";
			echo  "<td>".$row['timestamp'] . "</td>";
			
			
			echo  "</tr>";
		}
		
	
	}
	
} else if ($args == 2) {
	echo "22";
} else if ($args == 3) {
	echo "33";
} else if ($args == 4) {
	echo "44";
}

?>
</table>
