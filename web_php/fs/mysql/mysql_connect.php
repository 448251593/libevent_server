
<?php


	$con = mysql_connect("localhost:3396","root","root");
	if (!$con)
	{	
		die('Could not connect: '. mysql_error());
	}
	mysql_select_db("wlwdb", $con);
	
	mysql_query("set names 'utf8' ");
	mysql_query("set character_set_client=utf8");
	mysql_query("set character_set_results=utf8");
	
?>
