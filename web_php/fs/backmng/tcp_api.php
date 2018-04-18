
<?php 
 $ipstr = "www.waqrs.com:1212";
// $ipstr = "103.229.28.233:1212";
// $ipstr = "127.0.0.1:1212";
 //$ipstr = "192.168.110.131:1212";
// http://www.waqrs.com:1212/api.ctrl?dev_id=bcg0
if(isset($_GET["args"]))
{
	if($_GET["args"] == 1)
	{
		$urlpic= "http://".$ipstr."/api.ctrl?dev_id=".$_GET["dev_id"];
		$html = file_get_contents($urlpic);
		echo $html;
		
	}
	else if($_GET["args"] == 2)
	{
		$urlpic= "http://".$ipstr."/api.ctrl";
		$html = file_get_contents($urlpic);
		echo $html;
		
	}
	else if($_GET["args"] == 3)
	{
		//$_GET["dev_id"];
		//$_GET["set_status"];
		
		$urlpic= "http://".$ipstr."/api.ctrl?dev_id=".$_GET["dev_id"]."&ctrl=".$_GET["set_status"];
		$html = file_get_contents($urlpic);
		echo $html;
	
	}
}


/* $html = file_get_contents($url);

echo $html; 

 echo "<img src=\"pic/$filename\"></img>"; */
?>


