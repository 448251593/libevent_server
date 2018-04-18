
<?php include '../mysql/mysql_connect.php';?>
<?php

$username= $_POST["username"]; 
$password= md5($_POST["password"]); 
//$repassword= $_POST["repassword"]; 
$email= $_POST["email"];
$qq_num= $_POST["qqnum"]; 
$phone_num= $_POST["phonenum"]; 

//echo $username."<br>".$password."<br>".$repassword."<br>".$qq_num."<br>".$phone_num."<br>";
	
	$sql_inst="INSERT INTO user_reg_tb(username, pwd, qqnum,telephone,email,enable,role,timestamp)	values ('$username', '$password' ,'$qq_num',$phone_num,'$email','1','1',CURRENT_TIMESTAMP)";
/* 	$sql_inst="INSERT INTO user_reg_tb(username, pwd, qqnum,telephone,email,enable,role,timestamp) 
	SELECT '$username', '$password' ,'$qq_num',$phone_num,'$email','1','1',CURRENT_TIMESTAMP
	FROM DUAL 
	WHERE NOT EXISTS(SELECT username FROM user_reg_tb WHERE username = '$username')"; */
	//echo  $sql_inst;
	if (!mysql_query($sql_inst,$con))
	{
		die('Error: ' . mysql_error());
	}
	else
	{		
		echo "registok";
	}
?>

