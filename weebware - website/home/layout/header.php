<?php

	require '../../inc/utils.php';
	require '../../inc/session.php';

	db_connect();
	
	$data = get_user_data($_SESSION['username']);

    function navMatch($list, $output){
        foreach($list as $item){
            if($item == $_SERVER["SCRIPT_NAME"]){
                return $output;
            }
        }

        return "";
    }
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <meta name="theme-color" content="#D950C4">
    <link rel="icon" type="image/png" href="https://weebware.net/images/favicon.png">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.2.0/css/all.css">
    <link rel="stylesheet" href="assets/css/circular-std.css">
    <link rel="stylesheet" href="assets/css/main.css">
    <link rel="stylesheet" href="assets/css/style.css">
    <link rel="stylesheet" href="assets/css/scrollbar.css">
    <link rel="stylesheet" href="assets/css/responsive.css">
    <?php if($page == 'purchase') echo '<script src="assets/js/checkout.js" defer="defer"></script>'; ?>
    <script defer src="assets/js/index.js"></script>
    <?= navMatch(["/home/index.php", "/home"], '<script defer src="assets/js/mainPage.js"></script>') ?>
    <title>weebware</title>
</head>
<body>
    <div class="mobile-bar">
        <i id="openNav" class="fas fa-bars"></i>
    </div>
    <nav>
        <div class="logo">
            <img src="https://weebware.net/images/weebware_logo.png" alt="Weebware logo">
        </div>
        <ul>
            <li <?= navMatch(["/home/index.php", "/home/"], 'class="active"') ?>>
                <a href="index.php">
                    <img src="assets\images\dashboard<?= navMatch(["/home/index.php", "/home/"], '_active') ?>.svg" alt="">
                </a>
            </li>
            <? if ($data["expire"] < 2000000000) { ?>
            <li>
                <a href="purchase.php" <?= navMatch(["/home/purchase.php", "/home/purchase"], 'class="active"') ?>>
                    <img src="assets\images\shopping_cart<?= navMatch(["/home/purchase.php", "/home/purchase"], '_active') ?>.svg" alt="">
                </a>
            </li>
            <? } ?>
            <!-- <li>
                <a href="/configs.php" <?= navMatch(["/configs.php", "/configs"], 'class="active"') ?>>
                    <img src="assets\images\config<?= navMatch(["/configs.php", "/configs"], '_active') ?>.svg" alt="">
                </a>
            </li> -->
            <li>
                <a href="">
                    <img src="assets\images\download.svg" alt="">
                </a>
            </li>
        </ul>
        <ul>
            <li>
                <a href="?logout=1">
                    <img src="assets\images\logout.svg" alt="">
                </a>
            </li>
        </ul>
    </nav>
    <main>
        <div class="content">