<?php
function show_alot($alot_url, $word, $image) {
?>
    <h2>alot of <?php echo htmlentities($word); ?></h2>
    
    Share: 
        <a href="<?php echo $alot_url; ?>">
            <?php echo $alot_url; ?>
        </a>
    
    <!-- Share alot -->
    <a href="https://twitter.com/share" class="twitter-share-button" data-url="<?php echo $alot_url; ?>" data-via="seethatalot" data-hashtags="alot">Tweet</a>
    <script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?'http':'https';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+'://platform.twitter.com/widgets.js';fjs.parentNode.insertBefore(js,fjs);}}(document, 'script', 'twitter-wjs');</script>
    
    <!-- Show alot -->
    <EMBED class="alot" src='svg.php?src=<?php echo $image; ?>&word=<?php echo $word; ?>' type="image/svg+xml" />
<?php
}