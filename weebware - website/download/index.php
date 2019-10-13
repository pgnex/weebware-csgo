<?php
    $secretName = password_hash("lol", true);
    header("Content-Type: application/octet-stream");
    header("Content-Transfer-Encoding: Binary");
    header("Content-disposition: attachment; filename=$secretName.exe"); 
    echo readfile("https://weebware.net/download/loader.exe");