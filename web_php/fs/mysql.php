<html>
<body>
<div style="width: 50%;height: 50px;margin-left: 25%;margin-top: 100px;text-align: center;">
<?php

$username= $_POST["username"]; 
$password= $_POST["password"]; 
$repassword= $_POST["repassword"]; 
$email= $_POST["email"];
$qq_num= $_POST["qqnum"]; 
$phone_num= $_POST["phonenum"]; 

//echo $username."<br>".$password."<br>".$repassword."<br>".$qq_num."<br>".$phone_num."<br>";

	$con = mysql_connect("localhost:3396","root","root");
	if (!$con)
	{	
		die('Could not connect: ' . mysql_error());
	}
	mysql_select_db("wlwdb", $con);
	$sql_inst="INSERT INTO user_reg_tb(username, pwd, qqnum,telephone,email,enable,role,timestamp) 
	SELECT '$username', '$password' ,'$qq_num',$phone_num,'$email','1','1',CURRENT_TIMESTAMP
	FROM DUAL 
	WHERE NOT EXISTS(SELECT username FROM user_reg_tb WHERE username = '$username')";
	//echo  $sql_inst;
	if (!mysql_query($sql_inst,$con))
	{
		die('Error: ' . mysql_error());
	}
	
	echo "注册成功!";
	echo "<a style=\"text-align: center;\" href=\"#\">现在去登陆</a>"
	
?>

</div>



</body>

</html>