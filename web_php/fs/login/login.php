<?php 
include '../mysql/mysql_connect.php';
session_start();
$error_msg = "";
//若是用户未登录，即未设置$_SESSION[""user_id""]时，履行以下代码
if(!isset($_SESSION["username"])){
    if(isset($_POST["submit"])){//用户提交登录表单时履行如下代码
    	
      //$dbc = mysqli_connect(DB_HOST��DB_USER��DB_PASSWORD��DB_NAME);    	
    	//$dbc =$con;
    	if(strlen(trim($_POST["username"]))==0)
    	{
    		$error_msg = "用户名为空!";
    		$home_url = "/web_bcg/fs/main/mainindex.php?err=用户名空";
	            	header("Location: ".$home_url);
    	}
    	else if(strlen(trim($_POST["password"]))==0)
    	{
    		$error_msg = "密码为空!";
    		$home_url = "/web_bcg/fs/main/mainindex.php?err=密码为空";
	            	header("Location: ".$home_url);
    	}
    	else{
    		
    	//	include '/web_bcg/fs/mysql/mysql_connect.php';
	        $user_username = mysql_real_escape_string(trim($_POST["username"]));
	        $user_password = mysql_real_escape_string(trim($_POST["password"]));
			$user_password = md5( $user_password);
	        if(!empty($user_username)&&!empty($user_password)){
	            //MySql中的SHA()函数用于对字符串进行单向加密
	        	$sql_inst="SELECT a.username FROM user_reg_tb a where 
	        	a.username='$user_username' and a.pwd ='$user_password'";
	        	$data = mysql_query($sql_inst,$con);
	           
	           // $query = "SELECT user_id�� username FROM mismatch_user WHERE username = "$user_username AND "."password = SHA(""$user_password"")";
	           // $data = mysqli_query($dbc��$query);
	             //用用户名和暗码进行查询，若查到的记录正好为一条，则设置SESSION和COOKIE，同时进行页面重定向
	            if(mysql_num_rows($data)==1){
	                $row = mysql_fetch_array($data);
	                //$_SESSION["user_pwd"]=$row["pwd"];
	                $_SESSION["username"]=$row["username"];
	                //setcookie("user_pwd",$row["pwd"],time()+(60*60*24*30));
	                //setcookie("username",$row["username"],time()+(60*60*24*30));
	                
	                $home_url = "/web_bcg/fs/main/main_devices.php";
	                header("Location: ".$home_url);
	            }else{//若查到的记录不合错误，则设置错误信息
	            	
	            	$home_url = "/web_bcg/fs/main/mainindex.php?err=用户名或密码错误";
	            	header("Location: ".$home_url);
	            	
	                $error_msg = "用户名或密码错误";
	            }
	        }else{
	            $error_msg = "用户名或密码错误!";
	            $home_url = "/web_bcg/fs/main/mainindex.php?err=用户名或密码错误";
	            header("Location: ".$home_url);
	        }
    	}
    }
}else{//若是用户已经登录，则直接跳转到已经登录页面
    $home_url = "/web_bcg/fs/main/main_devices.php";
    header("Location: ".$home_url);
}


?>
