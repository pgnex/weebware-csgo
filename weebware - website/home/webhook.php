<?php
    
    require '../inc/utils.php';

    if (!isset($_GET['secret'])) die();

    db_connect();

    $order_data = get_order_data($_GET['secret']);
    $user_data = get_user_data($order_data['username']);

    $date = new DateTime();
    switch ($order_data['length']) {
        case '1month':
            $expire = $date->getTimestamp() + 2592000;
            break;
        case '3months':
            $expire = $date->getTimestamp() + 7776000;
            break;
        case '6months':
            $expire = $date->getTimestamp() + 15552000;
            break;
        case 'lifetime':
            $expire = 2000000000;
            break;
    }

    $post_body = file_get_contents('php://input');
    $data = json_decode($post_body, true);

    global $db;
    $update_order = $db->prepare("UPDATE orders SET status=:status WHERE secret=:secret");
    $update_order->bindValue(':status', $data['status']);
    $update_order->bindValue(':secret', $_GET['secret']);
    $update_order->execute();

    switch ($data['status']) {
        case 100:
            $updateSub = $db->prepare("UPDATE users SET expire=:expire WHERE username=:username");
            $updateSub->bindValue(':expire', $expire);
            $updateSub->bindValue(':username', $order_data['username']);
            $updateSub->execute();  
        break;
        case 51:
            $hwid = NULL;
            if ($user_data['hwid'] != 0) $hwid = $user_data['hwid'];
            add_ban($order_data['username'], $hwid, get_client_ip(), "Chargeback");
        break;
    }

?>