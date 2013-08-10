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
            $alot_img = $row['alot_img'];
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
</HEAD>
<BODY>

    <DIV class="container">

        <H1 id="brand"><A href="<?php echo $config->root_url() ?>">ISEETHATALOT.COM</A></H1>

        <div id="thisAlot">
            <?php if ($id) { ?>
                <h2>alot of <?php echo $word ?></h2>
            
                <!-- Share alot url -->
                Share: 
                    <a href="<?php echo $alot_url; ?>">
                        <?php echo $alot_url; ?>
                    </a>
                
                <!-- Share alot on Twitter -->
                <a href="https://twitter.com/share" 
                    class="twitter-share-button" 
                    data-url="<?php echo $alot_url; ?>" 
                    data-via="seethatalot" 
                    data-hashtags="alot">Tweet</a>
                <!-- Twitter JS -->
                <script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?'http':'https';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+'://platform.twitter.com/widgets.js';fjs.parentNode.insertBefore(js,fjs);}}(document, 'script', 'twitter-wjs');</script>
            
            <?php
                //Show a custom alot
                show_alot($id, $alot_img, $word);
            } else { 
                //Show the alot splash image
            ?>
                <IMG class="alot" src='img/alots/default.png' />
            <?php } ?>
        </div>

        <DIV id="howIsAlotFormed">
            <FORM METHOD="POST">
                <DIV class="prefix">
                    Do you observe <del>a lot</del> 
                    <a href=http://hyperboleandahalf.blogspot.com/2010/04/alot-is-better-than-you-at-everything.html target=_blank>alot</a>
                    of something?
                </DIV>
                <DIV class="input-boxes">
                    <INPUT type="text" name="word" placeholder="what is there alot of?" />	
                    <INPUT type="text"  name="image" placeholder="image URL to tile alot" />
                </DIV>
                <INPUT type="submit" value="make this alot" class="btn make-alot-button btn-primary btn-large" />
            </FORM>
        </DIV>

        <DIV id="thoseAlots">
            <?php
            foreach ($db->get_best(5) as $alot){
            }
            ?>
        </DIV>

    </DIV>

    <div id="footer">
        <div class="container">
            <p class="muted credit">by
                <a href="http://twitter.com/anachrobot">@anachrobot</a>/<a href="http://anachrobot.us">katie</a>
                +
                <a href="http://twitter.com/mjbrks">@mjbrks</a>/<a href="http://students.washington.edu/mjbrooks">michael</a>.

                inspired by <a href=http://hyperboleandahalf.blogspot.com/2010/04/alot-is-better-than-you-at-everything.html target=_blank>hyperbole and a half / allie brosh</A>
            </p>
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
<script>
(function() {
    var ALOT_REFRESH_INTERVAL = 2500;
    var PLACEHOLDER_SELECTOR = '.generating';
    var ALOT_SELECTOR = '.alot';
    var ALOT_URL = 'this_alot.php';
    var ALOT_ID = null;
    
    function scheduleRefresh() {
        setTimeout(checkOnAlot, ALOT_REFRESH_INTERVAL);
    }

    function checkOnAlot() {
        console.log("Checking on alot...");
        
        $.get(ALOT_URL, {
            id: ALOT_ID
        })
        .done(function(content) {
            $(ALOT_SELECTOR).replaceWith(content);
            var generatingMsg = $(PLACEHOLDER_SELECTOR);
            if (generatingMsg.length) {
                scheduleRefresh();
            }
        })
        .fail(function() {
            alert("Sorry, we're having alot of problems :(");
            scheduleRefresh();
        });
    }

    $(document).ready(function() {
        var generatingMsg = $(PLACEHOLDER_SELECTOR);
        
        if (generatingMsg.length) {
            ALOT_ID = generatingMsg.data('alot-id');
            scheduleRefresh();
        }
    });
})();
</script>

</BODY>
</HTML>