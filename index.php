<?php

include 'include/this_alot.php.inc';
include 'include/util.php.inc';

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
    
    $id = $db->insert_alot($word, $image);
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
$default = FALSE;
if (isset($_GET['id'])) {
    $id = $_GET['id'];
    
    //must find alot    
    $row = $db->get_alot($id);
    if ($row === FALSE) {
        $config->error(404, 'alot not found');
    } else {
        //And alot data
        $word = $row['word'];
        $image = $row['image'];    
        if (!$word || !$image) {
            $config->error(404, 'alot not found');
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

        <DIV id="thisAlot">
            <?php 
            if ($id) {
                show_alot($config->alot_url($id), $word, $image);
            } else { ?>
                <IMG class="alot" src='img/alots/default.png' />
            <?php } ?>
        </DIV>

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

</BODY>
</HTML>