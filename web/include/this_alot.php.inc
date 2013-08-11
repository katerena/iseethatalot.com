<?php
//Expects to receive a db row as an associative array
function show_alot($alot)
{
    $id = $alot['id'];
    $alot_img = $alot['composed_url'];
    $word = htmlentities($alot['word']);
    $error = ($alot['processed'] == 1) && ($alot['status']);

    if (!$alot_img) {
        if ($error) {
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

function rating_stats($alot) {
    ?>
    <span class="up-votes">
            <i class="icon-thumbs-up"></i> <?php echo $alot['up_votes'] ?>
        </span>
    <span class="down-votes">
        <i class="icon-thumbs-down"></i> <?php echo $alot['down_votes'] ?>
    </span>
    <?php
}