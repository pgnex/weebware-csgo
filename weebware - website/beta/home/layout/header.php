<?php
    require_once ('../../inc/web/index.php');

    function navMatch($list, $output){
        foreach($list as $item){
            if($item == $_SERVER["SCRIPT_NAME"]){
                return $output;
            }
        }

        return "";
    }

    if(isset($_SESSION['loggedin']) && $_SESSION['sessionid'] === session_id()) {
        $user = unserialize($_SESSION['loggedin']);
    } else {
        header('Location: /login');
    }

    $showAlert = false;

    if(isset($_SESSION['popup'])) {
        global $showAlert;

        require_once('assets/partials/popup/index.php');

        $showAlert = true;

      if(!isset($_POST['hwid_request']) && !isset($_POST['changePassword']) && !isset($_POST['hwid_reset'])) {
          unset($_SESSION['popup']);
      }
    }
    
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <meta name="theme-color" content="#EB6123">
    <link rel="icon" type="image/png" href="https://weebware.net/images/favicon.png">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.2.0/css/all.css">
    <link rel="stylesheet" href="assets/css/circular-std.css">
    <link rel="stylesheet" href="assets/css/main.css?v=<?= password_hash('test', PASSWORD_DEFAULT) ?>">
    <link rel="stylesheet" href="assets/css/style.css?v=<?= password_hash('test', PASSWORD_DEFAULT) ?>">
    <link rel="stylesheet" href="assets/css/scrollbar.css">
    <link rel="stylesheet" href="assets/css/responsive.css?v=<?= password_hash('test', PASSWORD_DEFAULT) ?>">
    <script src="assets/js/chart.min.js"></script>
    <?php if(isset($page)) if($page == 'admin') echo '<script src="assets/js/admin.js?v=1" defer="defer"></script>'; ?>
    <?= isset($_SESSION['popup']) ? '<script defer src="assets/js/alert.js"></script>' : ''; ?>
    <?php if(isset($page)) if($page == 'purchase') echo '<script src="assets/js/checkout.js" defer="defer"></script>'; ?>
    <?= isset($_GET['request']) ? '<script defer src="assets/js/handle_hwid.js"></script>' : '' ?>
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
            <img src="assets/images/logo.png" alt="Weebware logo">
        </div>
        <ul>
            <li <?= navMatch(["/home/index.php", "/home/"], 'class="active"') ?>>
                <a href="index.php">
                    <img src="assets\images\dashboard<?= navMatch(["/home/index.php", "/home/"], '_active') ?>.svg" alt="">
                </a>
            </li>
            <? if ($user->sub < 2000000000) { ?>
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
             <? if ($user->admin) { ?>
                <li>
                    <a href="admin.php" class="badge-container <?= navMatch(["/home/admin.php", "/home/admin"], 'active')?>" >
                        <span class="badge"><?= HardwareID::countRequests(); ?></span>
                        <img src="assets\images\admin<?= navMatch(["/home/admin.php", "/home/admin"], '_active') ?>.svg" alt="">
                    </a>
                </li>
            <? } ?>
            <? if ($user->subActive()) { ?>
                <li>
                    <a href="https://weebware.net/download/">
                        <img src="assets\images\download.svg" alt="">
                    </a>
                </li>
            <? } ?>
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