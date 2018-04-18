

<?php include '../mysql/mysql_connect.php';?>
<?php

	$username= $_POST["username"]; 

	
	$sql_inst="SELECT a.username FROM user_reg_tb a where a.username='$username'";
	//echo  $sql_inst;
	$result = mysql_query($sql_inst,$con);
	
	if (!$result)
	{
		die('Error: ' . mysql_error());
	}
	else 		
	{
		if(mysql_num_rows($result)==1)
		{
		 	echo "1";
		}
		else {
			echo "0";
		}
	}
	
	
	
?>
