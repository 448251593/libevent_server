<?php
/**ͬʱ�˳�session��cookie��ҳ��*/
//��ʹ���˳�ʱ��Ҳ�������׿��˻Ự�ŸɽӼ��Ự����
session_start();
//Ӧ��һ���Ự�����Ѽ��¼״��
if(isset($_SESSION["username"])){
    //Ҫ����Ự��������$_SESSION����ȫ�ֱ�������Ϊһ��������
    $_SESSION = array();
    //���Ǵ���һ���Ựcookie�����ɹ��̽�����ʱ������Ϊ֮ǰ1��Сʱ�Ӷ�����ɾ��
    //if(isset($_COOKIE[session_name()])){
     //   setcookie(session_name(),"",time()-3600);
    //}
    //Ӧ������session_destroy()�������ó����Ự
    session_destroy();
}
//ͬʱ������cookie�ĵ���ʱ����Ϊ��ȥ��ĳ��ʱ��ʹ��������ϵɾ����ʱ������Ϊ��λ
//setcookie("username","",time()-3600);
//location�ײ�ʹ������ض�����һ��ҳ��
$home_url = "/web_bcg/fs/main/mainindex.php";
header("Location:".$home_url);
?>