<?php

	require '../inc/utils.php';
	require '../inc/session.php';

	// make sure user is signed in
	require_login();

	// initialize database connection
    db_connect();


    if(isset($_POST['getDiscount'])) {
        $discountCode = $_POST['getDiscount'];
        $return = "0";
        
        $code_data = get_referral_code_info($discountCode);
        
        if (isset($code_data['discount']))
            $return = $code_data['discount'];

        echo json_encode(["return" => $return]);
    }