<?php
    $page = 'dashboard';
    require_once('layout/header.php');

    $hwidStatus = HardwareID::checkUserRequest($user->username);

	$date = new DateTime();
	$timestamp = $date->getTimestamp();

?>
    <div class="header">
        <h2>Welcome back, <?= $user->username; ?></h2>
    </div>
    <div class="info-container">
        <div class="main-info">
            <div class="column">
                <h2>Your account</h2>
                <div class="body">
                    <ul>
                        <li><b>Username </b><span><?= $user->username; ?></span></li>
                        <li><b>E-Mail </b><span><?= $user->email; ?></span></li>
                        <li><b>ID </b><span><?= $user->id; ?></span></li>
                    </ul>
                    <div class="btn-row">
                        <button class="btn-main" id="changePassword">Change Password</button>
                    </div>
                </div>
            </div>
            <div class="column <?= !$user->subActive() ? 'no-sub-box' : null ?>">
                <h2>Subscription</h2>
                <div class="body">
                    <ul>
                        <li><b>Status </b><span class="green">Undetected</span></li>
                        <li><b>Expires </b><span class="<?= $user->subTime() === 'Inactive' ? 'inactive' : ''; ?>"><?= $user->subTime(); ?></span></li>
                        <li><b>Version </b><span>6.55</span></li>
                    </ul>
                    <? if ($hwidStatus === 0) { ?> <button class="btn-main declined" disabled><i class="fas fa-exclamation-triangle"></i> Request denied</button> <? } ?>
                    <? if ($hwidStatus === -1) { ?> <button class="btn-main pending" disabled><i class="fas fa-spinner"></i> Pending Request</button> <? } ?>
                    <? if ($hwidStatus === 1 && $user->subActive()) { ?> <button class="btn-main" id="hwid_reset_open">Request HWID Reset</button> <? } ?>
                </div>
            </div>
        </div>
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