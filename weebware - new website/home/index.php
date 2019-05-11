<?php
	require '../inc/utils.php';
	require '../inc/session.php';

	// make sure user is signed in
	require_login();

	// initialize database connection
	db_connect();

	$data = get_user_data($_SESSION['username']);

	if (isset($_GET['reset']) && $data['rank'] == 2 && check_user_exists($_GET['reset'])) {
		update_hwid($_GET['reset'], 0);
	}
	
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
		<script src="http://cdn.jsdelivr.net/particles.js/2.0.0/particles.min.js"></script>
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
                <img src="../images/weebware_logo.png" class="header_img_dashboard">
  			    <a class="tablinks" id="tabAccountBtn" onclick="openTab(event, 'tabAccount')"><i class="fas fa-user-tie" style="padding: 3px;"></i> Account</a>
  			    <a class="tablinks" onclick="openTab(event, 'tabShoutbox')"><i class="fas fa-comment" style="padding: 3px;"></i> Shoutbox</a>
		<? if ($data['rank'] == 2) { ?> <a class="tablinks" onclick="openTab(event, 'tabAdmin')"><i class="fas fa-user-shield" style="padding: 3px;"></i> Admin</a> <? } ?>
        </div>

		<div id="tabAccount" class="tabcontent">
  			<h3>Account Information</h3>
 	 		<p class="preinformation">Username:</p> 
 	 		<p class="information"><?=$data['username']?> [<?=$data['id']?>]</p>
 	 		<br>
 	 		<p class="preinformation">HWID:</p> 
 	 		<p class="information"><?=$data['hwid']?> [reset]</p>
 	 		<br><br>
 	 		<h3>Subscription</h3>
 	 		<p class="preinformation">Active: </p> 
 	 		<p class="information"><?=(is_sub_active($data['username'], $data['expire'])) ? 'True' : 'False';?></p>
 	 		<br>
 	 		<p class="preinformation">Expires:</p> 
 	 		<p class="information"><?=$data['expire'] >= 2000000000 ? 'Never' : date("F d, Y h:i A", $data['expire'])?></p>
 	 		<br>


		<? if (!($data['expire'] >= 2000000000)) { ?> 
			<button type="submit" class="extend" onclick="openTab(event, 'tabExtendSubscription')">Extend Subscription</button> 
		<? } ?>

 	 	<br>
        </div>

		<div id="tabShoutbox" class="tabcontent">
  			<h3>Shoutbox</h3>
              <p class="preinformation">You have 0 unused invite codes.</p><br>
        </div>


		<div id="tabAdmin" class="tabcontent">
  			<h3>Admin</h3>
              <form action="" method="get"> 
			  <input type="text" id="username" name="username" style="max-width: 50%">
			  <label for="username">Username</label>
			  <button type="submit" class="normal" id="tabAdminBtn" style="display: block; max-width: 285px">Submit</button>
			  <br>
			  <br>

		<? if (isset($_GET['username']) && check_user_exists($_GET['username'])) { 		
			$admin_data = get_user_data($_GET['username']);
		?>

			<p class="preinformation">Username:</p> 
 	 		<p class="information"><?=$admin_data['username']?> [<?=$admin_data['id']?>]</p>
 	 		<br>
 	 		<p class="preinformation">HWID:</p> 
 	 		<p class="information"><?=$admin_data['hwid']?> <a href="?reset=<?=$admin_data['username']?>" style="color: #6f7072; font-size: 12px">[reset]</a></p>
 	 		<br>
 	 		<p class="preinformation">Email:</p> 
 	 		<p class="information"><?=$admin_data['email']?></p>

		<? } ?>
			  </form>
        </div>
		

        <div id="tabExtendSubscription" class="tabcontent">
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
 				<p class="preinformation priceInfo">Price:</p> 
 				<p class="information priceInfo" id="priceLabel">$0.00</p>
				<button type="submit" class="normal" id="purchaseBtn" style="display: block;">Extend Subscription</button>
			</form>
  			<p>Note: Our payments are made handled via a third party API, which does not support cards.<br>
  			If PayPal is not available in your country, please contact an administrator to pay with a card.</p>
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
                            case "3months": price = "$24.00"; break;
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

