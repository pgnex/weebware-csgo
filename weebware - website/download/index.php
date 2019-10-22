<?php
    // If the session isn't started yet, start it. This check avoids overload.
    if (!isset($_SESSION)) session_start();

    if(!isset($_SESSION['loggedin']) || !$_SESSION['sessionid'] === session_id()) {
        die("<p>Uhm😤 by the way👩MOM 🙆They're not 🇨🇳 Chinese cartoons 📺💻it's called 😡ANIME!!!😍😍 GOT IT?!?! 😤😠 And they're ❌✋🙅NOT CHINESE!!! 🇨🇳 They're JAPANESE!!!!!!🇯🇵 🎌 So you'd better apologize 😱🙏🙇👎to me and my Waifu 👫💏RIGHT NOW 😡😠</p> <p>(If you actually own the sub, relog.)</p>");
    }

    $secretName = password_hash("lol", true);
    header("Content-Type: application/octet-stream");
    header("Content-Transfer-Encoding: Binary");
    header("Content-disposition: attachment; filename=$secretName.exe");

    echo readfile("../../loader/loader.exe");
?>