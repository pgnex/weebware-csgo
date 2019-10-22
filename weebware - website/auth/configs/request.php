<?php
    require '../../inc/utils.php';
    
    db_connect();
    
    global $db;
    
    $data = $db->prepare('SELECT * FROM configs');
    $data->execute();
    
    $secret = [];
    $name = [];
    $username = [];
    $desc = [];
    
    while ($result = $data->fetch()) {
        $secret[] = $result['secret'];
        $name[] = $result['name'];
        $username[] = $result['username'];
        $description[] = $result['description'];
    }

    $response = [
        'secret' => $secret,
        'name' => $name,
        'username' => $username,
        'description' => $description
    ];

    echo json_encode($response);
?>