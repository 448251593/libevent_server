<?php
//Ӧ�ûỰ�ڴ洢�ı���ֵ֮ǰ�����ȿ����Ự
session_start();
if(!isset($_SESSION["username"])){
	echo  "��¼����<a href='"."/web_bcg/fs/main/mainindex.php'"."> ���µ�¼ </a>";
	exit();
}

//���ǻỰû�б����ã��鿴�Ƿ�������cookie
/* if(!isset($_SESSION["user_pwd"])){
    if(isset($_COOKIE["username"])){
        //��cookie��session��ֵ    
        $_SESSION["username"]=$_COOKIE["username"];
    }
    else 
    {
    
    	//echo $_SERVER['DOCUMENT_ROOT']."/fs/login/login.php";
    	echo  "��¼����<a href='"."/web_bcg/fs/login/login.php'"."> ���µ�¼ </a>";

    	exit();
    }
} */


//Ӧ��һ���Ự�����Ѽ��¼״��
/* if(isset($_SESSION["username"]))
{
   // echo "You are Logged as ".$_SESSION["username"]."<br/>";
   // echo "<a href=\"logout.php\"> Log Out(".   $_SESSION["username"].    ")</a>";
   
	
} */
?>

