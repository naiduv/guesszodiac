<?php 

echo "test";
echo $_POST['comment'];

$url = '127.0.0.1:5001';
$ch = curl_init();
curl_setopt($ch,CURLOPT_URL,$url);
curl_setopt($ch,CURLOPT_POST,count($_POST['comment']));
curl_setopt($ch,CURLOPT_POSTFIELDS,($_POST['comment']));
$result = curl_exec($ch);

//close connection
curl_close($ch);

?>