<?php include '../mysql/mysql_connect.php';?>
<?php include '../login/logined.php';?>
<?php 
	$username = $_SESSION["username"];

	
	
	if(isset($_POST["args"]))
	{
		if($_POST["args"] == 0)//
		{
			//echo  $_POST["args"];
			echo get_id_list($con);
		}
		 else if($_POST["args"] == 1)
		{
			if(get_nums($con,$username) >= 50)
			{
				echo   "{rslt:-1,info:'已经达到上限'}";
			}
			else {
			 	echo	create_new_id($con,$username);
			}
		} 
		else if($_POST["args"] == 2)//单条添加id到用户
		{
			if(isset($_POST["id"]))
			{
				echo generate_id($con,$_POST["id"]);
			}
			else 
			{
				return "{rslt:-1,info:'获取id参数失败!'}";
			}
		}
		else if($_POST["args"] == 3)
		{
			if(isset($_POST["idarr"]))
			{	
				$arrdata =json_decode($_POST["idarr"]);
				
				//echo generate_id($con,$_POST["id"]);
				//print $arrdata[0];
				//echo $arrdata[0];
				//echo "{rslt:1,info:'dddddddd!',data:".$arrdata[0]."}";
				echo	generate_id_all($con,$arrdata);
				
			}
			else
			{
				echo  "{rslt:-1,info:'获取id参数失败!'}";
			}
		}
		
	}
	exit();

function generate_id_all($con, $idarr)
{
	//插入新的数据
	$sql_inst="INSERT INTO user_dev_tb
	select create_user,dev_id,'' name, 1 enable,timestamp,'1' dev_type from creat_dev_id_tb where ";
	
	$sql_t = "";
	for( $i = 0; $i < count($idarr); $i++)
	{
		if(strlen($idarr[$i]) > 0)
		{
			$sql_t = $sql_t."dev_id='$idarr[$i]' or ";			
		}
		
	}
	$sql_inst =$sql_inst.substr($sql_t, 0, -4) ;
//	echo  $sql_inst;
	
	 if (!mysql_query($sql_inst,$con))
	{
		die('Error: ' . mysql_error());
		return "{rslt:-1,info:'插入用户使用表错误!'}";
	}
	return "{rslt:1,info:'操作成功!'}"; 
}

//添加单条id记录到用户下面
function generate_id($con, $id)
{
/* 	//更新创建id的数据库
	$sql_inst="UPDATE creat_dev_id_tb SET bindflag='1' WHERE DEV_ID='$id'";
	if (!mysql_query($sql_inst,$con))
	{
		die('Error: ' . mysql_error());
		 return "{rslt:-1,info:'更新创建表错误!'}"; 
	} */
	//查询是否已经存在
	$sql_inst="select count(1) ct from user_dev_tb WHERE deviceid='$id'";
	$result = mysql_query($sql_inst,$con);
	if (!$result)
	{
		die('Error: ' . mysql_error());
		return "{rslt:-1,info:'查询用户使用表错误!'}";
	}
	else
	{
		$row = mysql_fetch_array($result);	
		if( $row['ct'] >=1)
		{
			return "{rslt:1,info:'已经操作成功!'}";
		}
	}
	
	//插入新的数据
	$sql_inst="INSERT INTO user_dev_tb
	select create_user,dev_id,'' name, 1 enable,timestamp,'1' dev_type from creat_dev_id_tb where dev_id='$id'";
	
	if (!mysql_query($sql_inst,$con))
	{
		die('Error: ' . mysql_error());
		return "{rslt:-1,info:'插入用户使用表错误!'}";
	}
	return "{rslt:1,info:'操作成功!'}";
	
}

function get_nums($con,$username)
{
	//查询当前用户已经生成id数量
	//根据数量检查是否可以继续生成
	$sql_inst="SELECT count(1) ct  FROM creat_dev_id_tb a where a.create_user='$username'";
	$result = mysql_query($sql_inst,$con);
	if(!$result)
	{
		die('Error: ' . mysql_error());
	}
	else {
		$row = mysql_fetch_array($result);
		return $row['ct'];		
	}	
	
}

function get_id_list($con)
{
		
	//查询出来已经生成的id
	$sql_inst="SELECT a.dev_id,
					(
					case when  a.dev_id in (select b.deviceid from user_dev_tb b) 
					then '1'
					else
						'0'
					end
					)   bindflag
					,a.create_user  
				FROM creat_dev_id_tb a	
				where a.create_user='".$_SESSION["username"]."'"."order by bindflag" ;
	$result = mysql_query($sql_inst, $con);
	if(!$result)
	{
		die('Error: ' . mysql_error());
	}
	else {
		while($row = mysql_fetch_array($result))
		{
			$datastr['data_arr'][]=array($row['dev_id'],$row['bindflag'],$row['create_user']);
		}
		return 	"{rslt:1,info:'成功!',".substr(json_encode($datastr), 1,-1)."}";
	}	
	
}

 function  create_new_id($con,$username)
 {
 	$newdevid = uniqid($_SESSION["username"]);//当然你可以加上前缀
 	//插入新的数据
 	$sql_inst="INSERT INTO creat_dev_id_tb(dev_id, bindflag, create_user,timestamp)	values ('$newdevid', '0' ,'$username',CURRENT_TIMESTAMP)";
 	
 	if (!mysql_query($sql_inst,$con))
 	{
 		die('Error: ' . mysql_error());
 	}
 	else
 	{
 		return  "{data_arr:[".json_encode(array($newdevid,'0',$username))."]}" ;
 	}	
 	
 	
 }
 


?>