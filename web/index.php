<?php

include_once 'include/util.php.inc';
include_once 'include/this_alot.php.inc'; 

$config = read_config();
$db = $config->mkdb();

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $word = NULL;
    $image = NULL;
    if (isset($_POST['word'])) {
        $word = $_POST['word'];
    }
    if (isset($_POST['image'])) {
        $image = $_POST['image'];
    }
    
    if (!$image || !$word) {
        $config->error(400, 'sad alot');
    }
    
    $id = $db->insert_alot($word, $image, NULL);
    if (!$id) {
        $config->error(500, 'alot more broken');
    }
  
    //Redirect to the alot's page
    header('Location: ' . $config->alot_url($id));
    die();
}

//The default case -- a GET request
//id is optional
$id = NULL;
if (isset($_GET['id'])) {
    $id = $_GET['id'];

    if ($id) {
        //Generate the url to this alot
        $alot_url = $config->alot_url($id);
        
        //must find alot    
        $row = $db->get_alot($id);
        if ($row === FALSE) {
            $config->error(404, 'alot not found');
        } else {
            //And alot data
            $composed_url = $row['composed_url'];
            $word = htmlentities($row['word']);
            if (!$word) {
                $config->error(404, 'alot not found!');
            }
        }
    }
}
?>
<!DOCTYPE html>
<HTML>
<HEAD>
    <TITLE>I SEE THAT ALOT!</TITLE>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link href="//netdna.bootstrapcdn.com/twitter-bootstrap/2.3.2/css/bootstrap-combined.min.css" rel="stylesheet">
    <LINK type='text/css' rel='stylesheet' href='css/styles.css' />

    <meta name="description" content="Do you see alot of something?">
    <meta property="og:description" content="Do you see alot of something?">

    <meta name="twitter:card" content="summary_large_image">
    <meta name="twitter:site" content="@seethatalot">
    <meta name="twitter:domain" content="iseethatalot.com">
    <?php if ($id) { ?>
        <meta property="og:title" content="ALOT OF <?php echo strtoupper($word) ?>">
        <meta property="og:image" content="<?php echo $composed_url ?>">
    <?php } else { ?>
        <meta property="og:title" content="I SEE THAT ALOT">
        <meta property="og:image" content="<?php echo $config->root_url() ?>img/alots/default.png">
    <?php } ?>
</HEAD>
<BODY>

    <div id="header" class="navbar navbar-fixed-top navbar-inverse">
        <div class="navbar-inner">
            <div class="container">
                <A class="brand" href="<?php echo $config->root_url() ?>">ISEETHATALOT.COM</A>
            </div>
        </div>
    </div>

    <DIV id="main-content" class="container">
        <div class="row">
            <div id="thisAlot" class="span6">
            <?php if ($id) {
                //Show a custom alot
                show_alot($row);
                ?>
                <div class="sharing">
                    <!-- Share alot on Twitter -->
                    <a href="https://twitter.com/share"
                       class="twitter-share-button"
                       data-url="<?php echo $alot_url; ?>"
                       data-via="seethatalot"
                       data-text="I see #alot of <?php echo $word ?>!"
                       title="Tweet this alot!"
                        >Tweet</a>
                    <!-- Twitter JS -->
                    <script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?'http':'https';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+'://platform.twitter.com/widgets.js';fjs.parentNode.insertBefore(js,fjs);}}(document, 'script', 'twitter-wjs');</script>

                    <!-- Share alot url -->
                    <span class="share-link">
                    Share:
                    <a href="<?php echo $alot_url; ?>">
                        <?php echo $alot_url; ?>
                    </a>
                    </span>
                </div>
                <?php
            } else { 
                //Show the alot splash image
            ?>
                <IMG class="alot" src='img/alots/default.png' />
            <?php } ?>
            </div>

            <DIV id="howIsAlotFormed" class="span6">
                <div class="padding-box">
                    <DIV class="prefix">
                        Do you observe <del>a lot</del> alot of something?
                        <a class="muted" href="http://hyperboleandahalf.blogspot.com/2010/04/alot-is-better-than-you-at-everything.html" target="_blank">What is an alot?</a>
                    </DIV>
                    <FORM METHOD="POST">
                        <DIV class="input-boxes">
                            <INPUT class="word-input" type="text" name="word" placeholder="what do you see alot of?"/>
                            <INPUT class="image-input" type="text"  name="image" placeholder="paste an image URL here"/>
                        </DIV>
                        <BUTTON type="submit" class="btn make-alot-button btn-primary btn-large">make this alot</button>
                    </FORM>
                    <p class="muted credit">
                        iseethatalot.com created by
                        <a href="http://twitter.com/mjbrks">@mjbrks</a>/<a href="http://students.washington.edu/mjbrooks">michael</a>
                        and
                        <a href="http://twitter.com/anachrobot">@anachrobot</a>/<a href="http://anachrobot.us">katie</a>
                        <br/>
                        inspired by <a href=http://hyperboleandahalf.blogspot.com/2010/04/alot-is-better-than-you-at-everything.html target=_blank>hyperbole and a half / allie brosh</A>
                    </p>
                </div>
            </DIV>
<!--        <DIV id="top-rated" class="alot-list span3">-->
<!--            <h4>Alot Better</h4>-->
<!--            --><?php
//            foreach ($db->get_best() as $alot){
//                $link_url = $config->alot_url($alot['id']);
//                echo "<a href='$link_url'>";
//                show_alot($alot);
//                echo '</a>';
//            }
//            ?>
<!--        </DIV>-->
        </div>
        <div class="row">
            <DIV id="most-recent" class="alot-list span12">
                <h4>Seen alot recently...</h4>
                <?php
                foreach ($db->get_alots(0, 8) as $alot){
                    $link_url = $config->alot_url($alot['id']);
                    ?>
                    <div class='alot-list-item' data-alot-id="<?php echo $alot['id'] ?>">
                        <a href='<?php echo $link_url?>'>
                            <?php show_alot($alot); ?>
                        </a>
                        <div class="rating-stats hide">
                            <?php rating_stats($alot); ?>
                        </div>
                        <div class="rating-buttons">
                            <button type="button" class="btn up-vote" value="1">
                                <i class="icon-thumbs-up"></i> <?php echo $alot['up_votes'] ?>
                            </button>
                            <button type="button" class="btn down-vote" value="-1">
                                <i class="icon-thumbs-down"></i> <?php echo $alot['down_votes'] ?>
                            </button>
                        </div>
                        <span class="added-date">
                            <?php echo $config->alot_created_time($alot); ?>
                        </span>
                    </div>
                <?php } ?>
            </DIV>
        </div>
    </DIV>

    <div id="footer">
        <div class="container">
            Created with
            <a target="_blank" href="http://php.net/">PHP</a>,
            <a target="_blank" href="http://python.org/">Python</a>,
            and
            <a target="_blank" href="http://getbootstrap.com/">Twitter Bootstrap</a>.
            <a target="_blank" href="http://github.com/katiek/iseethatalot.com">Check us out on GitHub</a>.
        </div>
    </div>

<!-- Alot of tracking -->
<script>
  (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
  (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
  m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
  })(window,document,'script','//www.google-analytics.com/analytics.js','ga');

  ga('create', '<?php echo $config->ga_code() ?>', 'iseethatalot.com');
  ga('send', 'pageview');
</script>
<script src="//ajax.googleapis.com/ajax/libs/jquery/1.10.2/jquery.min.js"></script>
<script src="js/main.js"></script>
<script>
$(document).ready(function() {
    running_alot();
});
</script>

</BODY>
</HTML>