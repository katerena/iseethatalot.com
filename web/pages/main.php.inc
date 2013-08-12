<?php

include_once 'include/this_alot.php.inc';
$db = $config->mkdb();

if ($config->is_post()) {
    $word = $config->post('word');
    $image = $config->post('image');

    if (!$image || !$word) {
        $config->http_status(400, 'sad alot');
        die();
    }

    $alot = $db->insert_alot($word, $image, NULL);
    if (!$alot) {
        $config->http_status(500, 'alot more broken');
        die();
    }

    //Redirect to the alot's page
    header('Location: ' . $alot->url());
    die();
}

//The default case -- a GET request
//id is optional
$id = $config->get_param('id');

$ordinal = $config->get_param('ordinal');
$word = $config->get_param('word');
if (!$id && $ordinal && $word) {
    $id = Alot::parse_ordinal($ordinal);
}

if ($id) {
    //must find alot
    $alot = $db->get_alot($id);
    if (!$alot || ($word && ($alot->slug_word() !== $word))) {
        $config->error(404, 'alot not found');
    }
} else {
    $alot = NULL;
}
?>
<!DOCTYPE html>
<HTML>
<HEAD>
    <TITLE>I SEE THAT ALOT!</TITLE>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link href="//netdna.bootstrapcdn.com/twitter-bootstrap/2.3.2/css/bootstrap-combined.min.css" rel="stylesheet">
    <LINK type='text/css' rel='stylesheet' href='<?php echo $config->asset_url('css/styles.css')?>' />

    <meta name="description" content="Do you see alot of something?">
    <meta property="og:description" content="Do you see alot of something?">

    <meta name="twitter:card" content="summary_large_image">
    <meta name="twitter:site" content="@seethatalot">
    <meta name="twitter:domain" content="iseethatalot.com">
    <?php if ($id) { ?>
        <meta property="og:title" content="ALOT OF <?php echo strtoupper($alot->safe_word()) ?>">
        <meta property="og:image" content="<?php echo $alot->asset_url() ?>">
    <?php } else { ?>
        <meta property="og:title" content="I SEE THAT ALOT">
        <meta property="og:image" content="<?php echo $config->alot_of_default()?>">
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
            <?php if ($alot) {
                //Show a custom alot
                show_alot($alot);
                sharing_controls($alot);
            } else {
                //Show the alot splash image
                ?>
                <IMG class="alot" src='<?php echo $config->alot_of_default()?>' />
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
            show_alot_list($db->get_alots(0, 8));
            ?>
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
<script src="<?php echo $config->asset_url('js/main.js')?>"></script>
<script>
    $(document).ready(function() {
        running_alot({
            root: '<?php echo $config->root_url() ?>'
        });
    });
</script>

</BODY>
</HTML>