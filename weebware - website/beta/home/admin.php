<?php
    $page = 'admin';
    require_once('layout/header.php');

    // If the user doesn't have an admin status, he must not be here.
    if (!$user->admin) {
        header("location: index.php");
        die();
    }

    // Fetch the data that is going to be shown...
    $playerList = isset($_GET['search']) ? Admin::searchPlayer($_GET['search']) : Admin::fetchPlayerList();
    $pendingRequests = HardwareID::fetchPendingRequests();

    if (isset($_POST['action'])) {
        if ($_POST['action'] == "hwid_reset") {
            if ($_POST['value'] == "accept") {
                // Set status to accepted.
                update_hwid_reset($_POST['id'], $_POST['username'], 'ACCEPTED');

                $_SESSION['popup'] = [
                        "message" => "HWID of {$_POST['username']} has been reset!",
                        "class" => "succes-alert"
                    ];

                header('location: admin.php');
            }  else if ($_POST['value'] == "decline") {
                // Set status to denied.
               update_hwid_reset($_POST['id'], $_POST['username'], 'DENIED');

               // Show confirm pop-up.
                $_SESSION['popup'] = [
                    "message" => "HWID Request of {$_POST['username']} has been denied!",
                    "class" => "danger-alert"
                ];

                header('location: admin.php');
            } else {
                header('location: admin.php');
            }
        }
    }
    
?>
    <h2 class="admin-header">Admin panel</h2>
    <div class="admin-main">
        <div class="overview-chart">
            <h3>Overview</h3>
            <div class="filters">
                <div class="group-label">
                    <div class="date-input">
                        <label for="">From:</label>
                        <input type="date" value="<?= date('Y-m-d', strtotime('-1 week')); ?>">
                    </div>
                </div>
                <div class="group-label">
                    <div class="date-input">
                        <label for="">To:</label>
                        <input type="date" value="<?=  date('Y-m-d'); ?>">
                    </div>
                </div>
            </div>
            <canvas id="main-chart"></canvas>
        </div>
        <div class="list">
            <h3>Requested HWID resets</h3>
            <ul>
                <?php foreach ($pendingRequests as $request) {  ?>
                    <li onclick="location.href='?request=<?= $request["id"] ?>'" class="hwid-item">
                            <div class="hwid-time">
                                <span><?= $request["username"] ?></span>
                                <span><?= General::time("@".$request['timestamp']) ?></span>
                            </div>
                            <span>
                                <i class="fas fa-paste"></i>
                            </span>
                    </li>
                <?php } if (empty($pendingRequests)) {
                    // If there are no pending HWID Requests, show this message
                    echo '<li class="empty-list">
                            <span>
                                 <i class="fas fa-tasks" style="margin-right: 10px;"></i> 
                                 <h3>No requests...</h3> 
                                 There are no pending requests...
                             </span>
                         </li>';
                } ?>
            </ul>
        </div>
    </div>
    <div class="list-row">
        <div class="player-list">
            <div class="header-list">
                <h3>Player list</h3>
                <input id="search" class="searchbar" autocomplete="off" type="search" placeholder="Search username">
            </div>
            <div class="data-list">
                <div>
                    <p>
                        <span>#</span>
                        <span>Username</span>
                    </p>
                </div>
                <?php foreach ($playerList as $player) { ?>
                    <div>
                        <p>
                            <span><?= $player['id'] ?></span>
                            <span><?= $player['username'] ?></span>
                            <span><a href="?player=<?= $player['id'] ?>" class="btn-main"><i class="fas fa-cogs"></i></a></span>
                        </p>
                    </div>
                <?php } ?>
            </div>
        </div>
        <div class="player-list">
            <div class="header-list">
                <h3>Ban list</h3>
                <input class="searchbar" autocomplete="off" type="text" placeholder="Search username">
            </div>
            <div class="data-list">
                <div>
                    <p>
                        <span>#</span>
                        <span>Username</span>
                    </p>
                </div>
                <div>
                    <p>
                        <span>1</span>
                        <span>nex</span>
                        <span><a href="?player=<?= $player['id'] ?>" class="btn-main"><i class="fas fa-unlock-alt"></i></a></span>
                    </p>
                </div>
                <div>
                    <p>
                        <span>2</span>
                        <span>shiina</span>
                        <span><a href="?player=<?= $player['id'] ?>" class="btn-main"><i class="fas fa-unlock-alt"></i></a></span>
                    </p>
                </div>
                <div>
                    <p>
                        <span>3</span>
                        <span>Elian</span>
                        <span><a href="?player=<?= $player['id'] ?>" class="btn-main"><i class="fas fa-unlock-alt"></i></a></span>
                    </p>
                </div>
                <div>
                    <p>
                        <span>4</span>
                        <span>laser</span>
                        <span><a href="?player=<?= $player['id'] ?>" class="btn-main"><i class="fas fa-unlock-alt"></i></a></span>
                    </p>
                </div>
                <div>
                    <p>
                        <span>5</span>
                        <span>giituu44</span>
                        <span><a href="?player=<?= $player['id'] ?>" class="btn-main"><i class="fas fa-unlock-alt"></i></a></span>
                    </p>
                </div>
                <div>
                    <p>
                        <span>6</span>
                        <span>Lyon</span>
                        <span><a href="?player=<?= $player['id'] ?>" class="btn-main"><i class="fas fa-unlock-alt"></i></a></span>
                    </p>
                </div>
                <div>
                    <p>
                        <span>7</span>
                        <span>nadav1234</span>
                        <span><a href="?player=<?= $player['id'] ?>" class="btn-main"><i class="fas fa-unlock-alt"></i></a></span>
                    </p>
                </div>
                <div>
                    <p>
                        <span>8</span>
                        <span>veno</span>
                        <span><a href="?player=<?= $player['id'] ?>" class="btn-main"><i class="fas fa-unlock-alt"></i></a></span>
                    </p>
                </div>
                <div>
                    <p>
                        <span>9</span>
                        <span>deezyslays</span>
                        <span><a href="?player=<?= $player['id'] ?>" class="btn-main"><i class="fas fa-unlock-alt"></i></a></span>
                    </p>
                </div>
                <div>
                    <p>
                        <span>10</span>
                        <span>melo</span>
                        <span><a href="?player=<?= $player['id'] ?>" class="btn-main"><i class="fas fa-unlock-alt"></i></a></span>
                    </p>
                </div>
            </div>
            </div>
        </div>
    </div>

    <script>
        Array.min = function( array ){
            return Math.min.apply( Math, array );
        };

        const data = [10, 8, 10, 7, 10, 7, 10];

        new Chart(document.getElementById('main-chart').getContext('2d'), {
            type: 'line',
            data: {
                datasets: [{
                    label: 'Test',
                    backgroundColor: "#1a1a1c",
                    borderColor: "#EB6123",
                    data: data,
                    borderWidth: 3
                }],
                labels: [
                    "31st",
                    "5th",
                    "10th",
                    "15th",
                    "20th",
                    "25th",
                    "31th",
                ]
            },
            options: {
                responsive: true,
                maintainAspectRatio: false,
                tooltips: {
                    callbacks: {
                        label: (item, data) => data.labels[item.index] + ': ' + data.datasets[0].data[item.index] + '%'
                    }
                },
                layout: {
                    padding: {
                        right: 48,
                        top: 12,
                        bottom: 12,
                    }
                },
                legend: {
                    display: false,
                },
                scales: {
                    xAxes: [{
                        display: true,
                        gridLines: {
                            drawBorder: false,
                        },
                    }],
                    yAxes: [{
                        display: false,
                        ticks: {
                            min: Array.min(data) - 1,
                        }
                    }]
                }
            }
        });
    </script>

<?php
    if (isset($_GET['request'])) {
        require_once('assets/partials/modals/HWID_reset/handle.php');
    }

    require_once('layout/footer.php'); 
?>