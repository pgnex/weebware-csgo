<?php 
    $page = 'purchase';
    require_once('layout/header.php');
    
?>
    <div class="main-purchase">
        <form action="" method="post">
            <div class="content">
                <div class="header">
                    <h2>Order a subscription</h2>
                </div>
                <div class="row-shop">
                    <div class="group-label">
                        <label for="">Subscription length</label>
                        <select name="length" id="duration">
                                <option value="1month">1 Month</option>
                                <option value="3months">3 Months</option>
                                <option value="lifetime">Lifetime</option>
                        </select>
                    </div>
                    <div class="group-label">
                        <label for="">Coupon code</label>
                        <input type="text" name="discount" placeholder="Enter code here">
                    </div>
                </div>
                <div class="group-label">
                    <label for="">Payment method</label>
                    <select name="length" id="duration" disabled>
                            <option value="" disabled selected hidden>Paypal</option>
                            <option disabled>Booooh 👻</option>
                    </select>
                </div>
                    <div class="bottom-row-purchase">
                        <div class="price">
                            <p>Total price in USD</p>
                            <p class="act-price">$<span id="price">11.99</span></p>
                        </div>

                        <button type="submit" name="purchase" class="btn-main">Purchase</button>
                    </div>
                </div>
        </form>
    </div>
<?php require_once('layout/footer.php'); ?>