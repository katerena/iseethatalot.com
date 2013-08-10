<?php
function show_alot($id, $alot_img, $word) {
    
?>
    <!-- Show alot -->
    <?php if (!$alot_img) { ?>
        <div class="alot generating muted" data-alot-id="<?php echo $id ?>">
            Patience! Your alot is awakening...
        </div>
    <?php } else { ?>
        <IMG class="alot" src="<?php echo $alot_img?>" alt="alot of <?php echo $word ?>"/>
    <?php } 
}
