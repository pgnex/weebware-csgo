<?php
    require '../../inc/utils.php';
    
    db_connect();
    
    global $db;
    
    $data = $db->prepare('SELECT * FROM configs');
    $data->execute();
    
    $secret = array();
    $name = array();
    $username = array();
    $desc = array();
    
    while ($result = $data->fetch()) {
        $secret[] = $result['secret'];
        $name[] = $result['name'];
        $username[] = $result['username'];
        $description[] = $result['description'];
    }
    $response = array('secret' => $secret, 'name' => $name, 'username' => $username, 'description' => $description);
    echo json_encode($response);
    
?>