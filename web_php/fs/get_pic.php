
<?php
function make_dir($path){ 
    if(!file_exists($path)){//不存在则建立 
        $mk=@mkdir($path,0777); //权限 
        @chmod($path,0777); 
    } 
    return true; 
} 

function read_filetext($filepath){ 
	$string="";
    $filepath=trim($filepath); 
    $htmlfp=@fopen($filepath,"r"); 
    //远程 
    if(strstr($filepath,"://")){ 
        while($data=@fread($htmlfp,500000)){ 
            $string.=$data; 
        } 
    } 
    //本地 
    else{ 
        $string=@fread($htmlfp,@filesize($filepath)); 
    } 
    @fclose($htmlfp); 
    return $string; 
}

function write_filetext($filepath,$string){ 
    //$string=stripSlashes($string); 
    $fp=@fopen($filepath,"w"); 
    @fputs($fp,$string); 
    @fclose($fp); 
}

function get_filename($filepath){ 
    $fr=explode("/",$filepath); 
    $count=count($fr)-1; 
    return $fr[$count]; 
} 

function save_pic($url,$savepath=''){ 
    //处理地址 
    $url=trim($url); 
    $url=str_replace(" ","%20",$url); 
    //读文件 
    $string=read_filetext($url); 
    if(empty($string)){ 
        echo '读取不了文件';exit; 
    } 
    //文件名 
    $filename = get_filename($url); 
    //存放目录 
    make_dir($savepath); //建立存放目录 
    //文件地址 
    $filepath = $savepath.$filename; 
    //写文件 
    write_filetext($filepath,$string); 
    return $filepath; 
}

?>
<?php 
$urlpic= $_GET["url"]; 
$filename=substr($urlpic, strrpos($urlpic, "/")+1);
save_pic($urlpic,"pic/");

 echo "<img src=\"pic/$filename\"></img>"; 
?>




<!-- 
 $urlpic='http://tj.pengfu.com/uploads/allimg/160901/6-160Z1145216.jpg';
save_pic($urlpic,"D:/xampp/htdocs/");
echo  "get img test";  

//$url='http://www.baidu.com/';  
//$html = file_get_contents($url);  
//echo $html; -->

<!-- <img src="http://tj.pengfu.com/uploads/allimg/160901/6-160Z1145216.jpg"></img> -->
