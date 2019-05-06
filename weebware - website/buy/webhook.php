<?php 

    $secret = $_GET['secret'];

    // connect to the database
	include secret_directory('config.php'); // SQL Server stuff
	$server_server = $config['server'];
	$server_username = $config['username'];
	$server_password = $config['password'];
	$server_dbname = $config['dbname'];
	$server_status = $config['status'];
	$conn = new PDO('mysql:host=' . $server_server . ';dbname=' . $server_dbname, $server_username, $server_password,[PDO::ATTR_DEFAULT_FETCH_MODE=>PDO::FETCH_ASSOC]);

    $checkSecret = $conn->prepare("SELECT * FROM orders WHERE secret=:secret");
    $checkSecret->bindValue(':secret', $secret);
    $checkSecret->execute();
    $checkSecretResult = $checkSecret->fetch();
    $user = $checkSecretResult['user'];
    $length = $checkSecretResult['length'];
    
    
    date_default_timezone_set('UTC');
    $date = new DateTime();
    switch ($length) {
        case '1':
            $expireValue = $date->getTimestamp() + 2592000;
            break;
        case '3':
            $expireValue = $date->getTimestamp() + 7776000;
            break;
        case '69':
            $expireValue = 2000000000;
    }
    
    $post_body = file_get_contents('php://input');
    $data = json_decode($post_body, true);
    
    $createOrder = $conn->prepare("UPDATE orders SET status=:status WHERE secret=:secret");
    $createOrder->bindValue(':status', $data['status']);
    $createOrder->bindValue(':secret', $secret);
    $createOrder->execute();
    
    
    if ($checkSecret->rowCount() > 0) {
        switch ($data['status']) {
            case 100:
                $updateSub = $conn->prepare("UPDATE users SET expire=:expire WHERE username=:user");
                $updateSub->bindValue(':expire', $expireValue);
                $updateSub->bindValue(':user', $user);
                $updateSub->execute();  
            break;
            case 51:
                $updateSub = $conn->prepare("UPDATE users SET rank=:rank WHERE username=:user");
                $updateSub->bindValue(':rank', -1);
                $updateSub->bindValue(':user', $user);
                $updateSub->execute();  
            break;
        }
    }

	function secret_directory($fileName) {
		return 'RmNy5KApCY/' . $fileName;
	}

?>