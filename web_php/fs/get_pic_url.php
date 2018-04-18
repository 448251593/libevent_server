
<?php 

$urlpic= $_GET["url"]; 
$deep_ct= $_GET["deep_ct"]; 
$urldoname=substr($urlpic, strrpos($urlpic, "http://"));
$urldoname=substr($urldoname,0, strrpos($urlpic, "com")+3);
$next_pic_url='';
for ( $i = 0; $i< $deep_ct; $i++)
{
	$html = file_get_contents($urlpic);
	
	
	
	$picut=substr($html, strpos($html, "/uploads/allimg/"));
	$picut=substr($picut,0, strpos($picut, ".jpg")+4);
	
	

//	echo  $i.'='.$picut.'<br>';
	echo  $urldoname.$picut.'<br>';
}
/* $html = file_get_contents($url);

echo $html; 

 echo "<img src=\"pic/$filename\"></img>"; */
?>




<!-- 
 $urlpic='http://tj.pengfu.com/uploads/allimg/160901/6-160Z1145216.jpg';
save_pic($urlpic,"D:/xampp/htdocs/");
echo  "get img test";  



<!-- <img src="http://tj.pengfu.com/uploads/allimg/160901/6-160Z1145216.jpg"></img> -->
