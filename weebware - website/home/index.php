<?php
	require '../inc/utils.php';
	require '../inc/session.php';

	// make sure user is signed in
	require_login();

	// initialize database connection
	db_connect();

	$data = get_user_data($_SESSION['username']);
	$date = new DateTime();

	if (isset($_GET['reset']) && $data['rank'] == 2 && check_user_exists($_GET['reset'])) {
		update_hwid($_GET['reset'], 0);
	}

	$filename = generate_random_string(16);
	
?>


<!-- Justin Garofolo © 2018 -->
<html>
	<head>
    <meta charset="UTF-8">
		<title>weebware - dashboard</title>
		<!--CSS-->
		<link rel="stylesheet" type="text/css" href="styles/style.css">
		<link rel="stylesheet" type="text/css" href="styles/particle.css">
		<link rel="icon" type="image/png" href="../images/favicon.png"/>
		<!--JQuery Awesome-->
		<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
		<!--Font Awesome-->
		<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.1.1/css/all.css" integrity="sha384-O8whS3fhG2OnA5Kas0Y9l3cfpmYjapjI0E4theH4iuMD+pLhbf6JI0jIMfYcK3yZ" crossorigin="anonymous">
		<!--Particle stuff (background)-->
		<script src="js/particle.js" defer="defer"></script>
		<div id="particles-js"></div>
		<script src="https://cdn.jsdelivr.net/particles.js/2.0.0/particles.min.js"></script>
		<script>
			// tab control stuff
			function openTab(evt, tabID) {
    			var i, tabcontent, tablinks;
    			// Get all elements with class="tabcontent" and hide them
    			tabcontent = document.getElementsByClassName("tabcontent");
    			for (i = 0; i < tabcontent.length; i++) {
        			tabcontent[i].style.display = "none";
    			}
    			// Get all elements with class="tablinks" and remove the class "active"
    			tablinks = document.getElementsByClassName("tablinks");
    			for (i = 0; i < tablinks.length; i++) {
        			tablinks[i].className = tablinks[i].className.replace(" active", "");
    			}
    			// Show the current tab, and add an "active" class to the link that opened the tab
    			document.getElementById(tabID).style.display = "block";
    			evt.currentTarget.className += " active";
			}
		</script>
	</head>
	<body>
        <div class="container">
            <script>
                // other javascript functions
                function redirectTo(s) { document.location = s; }
            </script>
		<div class="tab">
                <button type="submit" class="quickicon" onclick="redirectTo('?logout=1')"><i class="fas fa-power-off"></i></button>
				<button type="submit" class="quickicon" onclick="redirectTo('../discord')"><i class="fas fa-question"></i></button>
                <img src="../images/weebware_logo.png" class="header_img_dashboard">
  			    <a class="tablinks" id="tabAccountBtn" onclick="openTab(event, 'tabAccount')"><i class="fas fa-user-tie" style="padding: 3px;"></i> Account</a>
  			 <? if ($data['expire'] < $date->getTimestamp()) { ?>   <a class="tablinks" onclick="openTab(event, 'tabPurchase')"><i class="fas fa-shopping-cart"></i> Purchase</a> <? } ?>
        </div>

		<div id="tabAccount" class="tabcontent">
  			<h3>Account Information</h3>
 	 		<p class="preinformation">Username:</p> 
 	 		<p class="information"><?=$data['username']?> [<?=$data['id']?>]</p>
 	 		<br>
 	 		<p class="preinformation">HWID:</p> 
 	 		<p class="information"><a href="../discord" style="color: #6f7072; font-size: 12px">[request reset]</a></p>
 	 		<br><br>
 	 		<h3>Subscription</h3>
 	 		<p class="preinformation">Active: </p> 
 	 		<p class="information"><?=(is_sub_active($data['username'], $data['expire'])) ? 'True' : 'False';?></p>
 	 		<br>
 	 		<? if ($data['expire'] > $date->getTimestamp()) { ?><p class="preinformation">Expires:</p> <? } ?>
 	 		<? if ($data['expire'] > $date->getTimestamp()) { ?> <p class="information"><?=$data['expire'] >= 2000000000 ? 'Never' : date("F d, Y h:i A", $data['expire'])?></p> <? } ?>
 	 		<br>
			<br>
			<br>
			<?php if ($data['expire'] > time()) {  ?>
			
            <a href="https://weebware.net/download/loader.exe" download="<?=$filename?>.exe">
                <button type="submit" class="normal" style="display: block; max-width: 285px">Download</button>
            </a>
			
			
			 <? } ?>

 	 	<br>
        </div>

		<div id="tabPurchase" class="tabcontent">
			<h3>Extend Your Subscription</h3>
			<form action="purchase.php" method="post">
				<p class="preinformation">Subscription Length:</p><br>
				<select name="length" id="length" style="border-radius: 5px;" class="normal" style="display: block;" required autofocus="">
            		<option value="" disabled selected hidden>Duration</option>
                	<option value="1month">1 Month</option>
                	<option value="3months">3 Months</option>
                	<option value="6months">6 Months</option>
                	<option value="lifetime">Lifetime</option>
                </select>
                <br>
            	<p class="preinformation">Payment Method:</p><br>
				<select name="paymentmethod" id="paymentmethod" style="border-radius: 5px;" class="normal" style="display: block;" required autofocus="">
                	<option value="paypal">PayPal</option>
            	</select>
 				</br><p class="preinformation">Referrer:</p><br>
			    <input type="text" id="referrer" name="referrer" class="normal">
			    <p class="preinformation priceInfo">Price:</p> 
 				<p class="information priceInfo" id="priceLabel">$0.00</p>
				<button type="submit" class="normal" id="purchaseBtn" style="display: block;">Extend Subscription</button>
			</form>
  			<p>Note: Our payments are made handled via a third party API, which does not directly support cards.<br>
  			If PayPal is not available in your country, please contact an administrator to pay with a card.<br><br>
  			For BTC send payment based at the current price to 3LPFRNCxyzRR2Qn7PaMJHQwe5y7KAaVoaR <br>
  			And send an email to nex@weebware.net with the TXID, and intended subscription length.</p>
        </div>

		

        <div id="tabExtendSubscription" class="tabcontent">

        </div>
        
        	<script>
                    // initialization
                    $('#tabExtendBtn').hide();
                        document.getElementById("tabAccountBtn").click();
                        $('#length').change(function() {
                        var selected = this.value;
                        var price = 0;
                        switch (selected) {
                            case "1month": price = "$14.00"; break;
                            case "3months": price = "$26.00"; break;
                            case "6months": price = "$38.00"; break;
                            case "lifetime": price = "$46.00"; break;
                        }
                        $('#priceLabel').text(price);
                        $('.priceInfo').show();
                        $('#purchaseBtn').show();
                    });
                    $('.priceInfo').hide();
                    $('#purchaseBtn').hide();
                    // some other stuff
                </script>
            </div>
	</body>
</html>

