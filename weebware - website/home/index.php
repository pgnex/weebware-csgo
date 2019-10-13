<?php
    $page = 'dashboard';
    require_once('layout/header.php');

	// make sure user is signed in
	require_login();

	// initialize database connection
	db_connect();

	$data = get_user_data($_SESSION['username']);
	$date = new DateTime();
	
	$timestamp = $date->getTimestamp();
	
	
	if (get_referrer_info($data['username'])) 
	    $referrer_info = get_referrer_info($data['username']);
	    
	    
	$reset_flag = get_hwid_reset_status($data['username']);
	    
	    
    if (isset($_POST['type'])) {
        switch ($_POST['type']) {
            case 'hwid_reset':
                if (!request_hwid_reset($data['username'], $_POST['reason'])) {
                    $error = "Pending :)";
                }
                break;
            case 'change_password':
                if ($_POST['new_password1'] === $_POST['new_password2'])
                    update_password($data['username'], $_POST['new_password1']);
                break;
                
        }
    }
	
?>


    <div class="header">
        <h2 style="margin-bottom: 5rem;">Welcome back, <?=$data['username']?></h2>
    </div>
    <div class="info-container">
        <div class="main-info">
            <div class="column">
                <h2>Your account</h2>
                <div class="body">
                    <ul>
                        <li><b>Username </b><span><?=$data['username']?></span></li>
                        <li><b>E-Mail </b><span><?=$data['email']?></span></li>
                        <li><b>ID </b><span><?=$data['id']?></span></li>
                    </ul>
                    <div class="btn-row">
                        <button class="btn-main" id="changePassword">Change Password</button>
                    </div>
                </div>
            </div>
            <div class="column">
                <h2>Subscription</h2>
                <div class="body">
                    <ul>
                        <li><b>Status </b><span class="green">Undetected</span></li>
                        <li><b>Expires </b><span><?=sub_expire_date($data['expire'])?></span></li>
                        <li><b>Version </b><span>6.55</span></li>
                    </ul>
                    <? if ($reset_flag === 0) { ?> <button class="btn-main" disabled><i class="fas fa-exclamation-triangle"></i> Request denied</button> <? } ?>
                    <? if ($reset_flag === -1) { ?> <button class="btn-main" disabled><i class="fas fa-spinner"></i> Pending Request</button> <? } ?>
                    <? if ($reset_flag === 1) { ?> <button class="btn-main" id="hwid_reset_open">Request HWID Reset</button> <? } ?>
                    
                </div>
            </div>
        </div>
        <!-- <div class="main-info">
            <div class="column">
                <h2>Referal code</h2>
                <div class="body">
                    <ul>
                        <li><b>Code </b><span>Elian</span></li>
                        <li><b>Used </b><span>3 Times</span></li>
                        <li><b>Money </b><span>$ 10,50</span></li>
                    </ul>
                </div>
            </div>
        </div> -->
        <div class="info-row">
            <div class="column">
                <h2>Latest <span>version 6.55</span></h2>
                <div class="body">
                    <ul>
                        <li>Skin changer redesigned</li>
                        <li>Stattrak added</li>
                        <li>Nametags added</li>
                        <li>Bounding box & Corner box made more compact</li>
                        <li>Glow chams improved</li>
                        <li>Changed font size</li>
                        <li>Night mode & Night sky applies after map change</li>
                    </ul>
                </div>
            </div>
            <div class="column">
                <h2>Discord <span>server</span></h2>
                <div class="body">
                    <p>Join our Discord to get help, chat with the <br>
                        community, suggest new features and more!</p>
                    <button class="btn-main" onclick="window.open('https://discordapp.com/invite/EqmU4hn', '_blank');">Join the server</button>
                </div>
            </div>
        </div>
    </div>
    
<?php 
    require_once('assets/partials/modals/index.php'); 
    require_once('layout/footer.php'); 
?>