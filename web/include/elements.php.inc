<?php
/**
 * Expects to receive a db row as an associative array
 * @param Alot $alot
 */
function show_alot($alot)
{
    $id = $alot->id;
    $alot_img = $alot->asset_url();
    $word = $alot->safe_word();

    if (!$alot_img) {
        if ($alot->processing_error()) {
            ?>
            <div class="alot error" data-alot-id="<?php echo $id ?>">
                <div class="message muted">
                    Alas, there was problem with your alot :( :( :(
                    <br/>
                    Try pointing us to a different image URL.
                </div>
            </div>
        <?php
        } else {
            ?>
            <div class="alot generating" data-alot-id="<?php echo $id ?>">
                <div class="message muted">
                    Patience! Your alot is awakening...
                </div>
            </div>
        <?php
        }
    } else {
        ?>
        <IMG class="alot" src="<?php echo $alot_img ?>" alt="alot of <?php echo $word ?>"/>
    <?php
    }
}

/**
 * Display a list of alots with rating controls.
 * @param Alot[] $alots
 */
function show_alot_list($alots) {
    foreach ($alots as $alot){
        $link_url = $alot->url();
        ?>
        <div class='alot-list-item' data-alot-id="<?php echo $alot->id ?>">
            <a href='<?php echo $link_url?>'>
                <?php show_alot($alot); ?>
            </a>
            <div class="rating-stats hide">
                <?php rating_stats($alot); ?>
            </div>
            <div class="rating-buttons">
                <button type="button" class="btn up-vote" value="1">
                    <i class="icon-thumbs-up"></i> <?php echo $alot->up_votes ?>
                </button>
                <button type="button" class="btn down-vote" value="-1">
                    <i class="icon-thumbs-down"></i> <?php echo $alot->down_votes ?>
                </button>
            </div>
            <span class="added-date">
                <?php echo $alot->nice_creation_time() ?>
            </span>
        </div>
    <?php
    }
}

/**
 * Display the sharing controls.
 * @param Alot $alot
 */
function sharing_controls($alot) {
    $alot_url = $alot->url();
    ?>
    <div class="sharing clearfix">
        <!-- Share alot on Twitter -->
        <a href="https://twitter.com/share"
           class="twitter-share-button"
           data-url="<?php echo $alot_url ?>"
           data-via="seethatalot"
           data-text="I see #alot of <?php echo $alot->safe_word() ?>!"
           title="Tweet this alot!"
            >Tweet</a>
        <!-- Twitter JS -->
        <script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?'http':'https';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+'://platform.twitter.com/widgets.js';fjs.parentNode.insertBefore(js,fjs);}}(document, 'script', 'twitter-wjs');</script>

        <!-- Share alot url -->
        <span class="share-link">
            Share:
            <input type="text" class="share-link-input" value="<?php echo $alot_url; ?>" readonly="true"/>
        </span>
    </div>
    <?php
}

function rating_stats($alot) {
    ?>
    <span class="up-votes">
            <i class="icon-thumbs-up"></i> <?php echo $alot->up_votes ?>
        </span>
    <span class="down-votes">
        <i class="icon-thumbs-down"></i> <?php echo $alot->down_votes ?>
    </span>
    <?php
}