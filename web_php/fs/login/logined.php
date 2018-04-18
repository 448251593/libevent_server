<?php
//应用会话内存储的变量值之前必须先开启会话
session_start();
if(!isset($_SESSION["username"])){
	echo  "登录错误<a href='"."/web_bcg/fs/main/mainindex.php'"."> 重新登录 </a>";
	exit();
}

//若是会话没有被设置，查看是否设置了cookie
/* if(!isset($_SESSION["user_pwd"])){
    if(isset($_COOKIE["username"])){
        //用cookie给session赋值    
        $_SESSION["username"]=$_COOKIE["username"];
    }
    else 
    {
    
    	//echo $_SERVER['DOCUMENT_ROOT']."/fs/login/login.php";
    	echo  "登录错误<a href='"."/web_bcg/fs/login/login.php'"."> 重新登录 </a>";

    	exit();
    }
} */


//应用一个会话变量搜检登录状况
/* if(isset($_SESSION["username"]))
{
   // echo "You are Logged as ".$_SESSION["username"]."<br/>";
   // echo "<a href=\"logout.php\"> Log Out(".   $_SESSION["username"].    ")</a>";
   
	
} */
?>

