<?php include '../mysql/mysql_connect.php';?>
<?php include '../login/logined.php';?>




<?php
$args="";
if(isset($_GET["arg"]))
{
		$args = $_GET["arg"];
}
else if(isset($_POST["arg"]))
{
	
	$args = $_POST["arg"];
}
else 
{
	echo "arg is not find!";
	exit();
}

if ($args == 1) {//初始化设备列表

	$sql_inst="SELECT a.dev_name,a.deviceid,a.dev_type,a.timestamp FROM user_dev_tb a
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
			/* echo  "<tr style=\"height: 10px;\">";
				
			echo  "<td>".$row['dev_name'] . "</td>";
			echo  "<td>".$row['deviceid'] . "</td>";
			echo  "<td>".$row['enable'] . "</td>";
			echo  "<td>".$row['timestamp'] . "</td>";							
			echo  "</tr>"; */
			$datastr['data_arr'][]=array($row['dev_name'],$row['deviceid'],$row['dev_type']);
		}
		echo "{rslt:1,info:'成功222!',".substr(json_encode($datastr), 1,-1)."}";

	}

} else if ($args == 2) {//更新单条记录
	
	if(isset($_POST["name"])&& isset($_POST["id"]) && isset($_POST["idtype"]))
	{
		$name=$_POST["name"];
		$id=$_POST["id"];
		echo update_id_item($con, $name,$id,$_POST["idtype"]);
	}
	else{
		echo "{rslt:-1,info:'参数错误!'}";
	}
	
	
} else if ($args == 3) {//删除单条记录
	if(isset($_POST["id"]))
	{
		//$name=$_POST["name"];
		$id=$_POST["id"];
		$sql_inst="delete from user_dev_tb where  deviceid='$id'";
		//	echo $sql_inst;
		$result = mysql_query($sql_inst,$con);
	
		if (!$result)
		{
			die('Error: ' . mysql_error());
			echo "{rslt:-1,info:'失败!'}";
		}
		else
		{
			echo "{rslt:1,info:'成功!'}";
		}
	}
	else{
		echo "{rslt:-1,info:'参数错误!'}";
	}
} else if ($args == 4) {//批量更新

	if(isset($_POST["idarr"]) && isset($_POST["idname"]))
	{
		$idname =json_decode($_POST["idname"]);
		$arrdata =json_decode($_POST["idarr"]);
		$arrdatatype =json_decode($_POST["idarrtype"]);
		//echo generate_id($con,$_POST["id"]);
		//print $arrdata[0];
		//echo $arrdata[0];
		//echo "{rslt:1,info:'dddddddd!',data:".$arrdata[0]."}";
		//echo  "{rslt:1,info:'ttttttttttt!'}";
		echo	update_id_all($con,$idname,$arrdata,$arrdatatype);
	}
	else
	{
		echo  "{rslt:-1,info:'获取id参数失败!'}";
	}
} else if ($args == 5) {//批量删除
	if(isset($_POST["idarr"]) )
	{
	
		$arrdata =json_decode($_POST["idarr"]);
		echo	delete_id_all($con,$arrdata);
	}
	else
	{
		echo  "{rslt:-1,info:'获取参数失败!'}";
	}
}
function delete_id_all($con, $idarr)
{
	$sql_inst="delete from user_dev_tb where ";
	
	$sql_t = "";
	for( $i = 0; $i < count($idarr); $i++)
	{
		if(strlen($idarr[$i]) > 0)
		{
			$sql_t = $sql_t."deviceid='$idarr[$i]' or ";
		}
	
	}
	$sql_inst =$sql_inst.substr($sql_t, 0, -4) ;
	
	$result = mysql_query($sql_inst,$con);
	
	if (!$result)
	{
		die('Error: ' . mysql_error());
		return  "{rslt:-1,info:'失败!'}";
	}
	else
	{
		return  "{rslt:1,info:'成功!'}";
	}
	
}
function update_id_item($con,$idname, $idarr,$arrdatatype)
{
	$sql_inst="update user_dev_tb a set  a.dev_name ='$idname',a.dev_type='$arrdatatype'
	where a.deviceid='$idarr'";
	//	echo $sql_inst;
	$result = mysql_query($sql_inst,$con);
	
	if (!$result)
	{
		die('Error: ' . mysql_error());
		return "{rslt:-1,info:'保存失败!'}";
	}
	else
	{
		return  "{rslt:1,info:'成功!'}";
	}
	
}	

function update_id_all($con,$idname, $idarr,$arrdatatype)
{
	//插入新的数据
	for( $i = 0; $i < count($idarr); $i++)
	{

		$rslt = update_id_item($con, $idname[$i], $idarr[$i],$arrdatatype[$i]);
		
		$arrdata =json_decode($rslt);
		if($arrdata["rslt"] == -1)
		{
			return "{rslt:-1,info:'保存失败!'}";
		} 
	}
	return "{rslt:1,info:'保存成功!'}";
	
}


?>

