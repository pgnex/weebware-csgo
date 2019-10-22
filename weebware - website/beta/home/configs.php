<?php require_once('layout/header.php'); ?>
    <div class="config-list">
        <div class="header">
            <h2>Config browser</h2>
        </div>
        <?php for($i = 0; $i < 10; $i++) { ?> 
            <div class="config">
                <div class="title">
                    <h3>Some config</h3>
                    <p>Elian</p>
                </div>
                <div class="tags">
                    <ul>
                        <li class="rage">Rage</li>
                        <li class="skins">Skins</li>
                    </ul>
                </div>
                <div class="ratings">
                    <ul>
                        <li><i class="fas fa-thumbs-up"></i> 102</li>
                        <li><i class="fas fa-thumbs-down"></i> 5</li>
                    </ul>
                </div>
                <div class="action">
                    <i class="fas fa-download"></i>
                </div>
                <div class="action">
                    <i class="fas fa-exclamation-circle"></i>
                </div>
            </div>
        <?php } ?>
        <ul class="pagination">
            <li><i class="fas fa-chevron-left"></i></li>
            <li>1</li>
            <li>2</li>
            <li>3</li>
            <li><i class="fas fa-chevron-right"></i></li>
        </ul>
    </div>
<?php require_once('layout/footer.php'); ?>