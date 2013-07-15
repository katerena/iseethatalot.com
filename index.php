<?php
$word  = $_POST['word'];
$image = $_POST['image'];
$id    = $_GET['id'];

if ($word !== null) { // create alot
  if ($image == null) {
  	$image = "";
  }
  $conn = new PDO('mysql:host=localhost;dbname=kukmbr_alot', 'kukmbr_alot', 'kukmbr_alot1');
  $conn->prepare("INSERT INTO alot (word,image) VALUES (:word,:image)")->execute(array(':word'  => $word, ':image' => $image));
  $id = $conn->lastInsertId(); 
  header('Location: http://iseethatalot.com?id=' . $id);
  die();
} else {
	if ($id == null) { 
	 $id = 48;
	}
  $conn = new PDO('mysql:host=localhost;dbname=kukmbr_alot', 'kukmbr_alot', 'kukmbr_alot1');
  $query = $conn->prepare("SELECT word, image FROM alot WHERE id=:id");
  $query->execute(array(':id' => $id));
  $row = $query->fetch();
  if(!$row){
	header('Location: http://iseethatalot.com?id=65');
  die();
  }
  $word = $row['word'];
  $image = $row['image'];
  if ($word==null || $image==null) {
	header('Location: http://iseethatalot.com?id=65');
  die();
  }
}

function get_best($n){
	return array(); //TODO return top-curated alots!
}

function show_alot($id, $word, $image) {
?>
<!-- Share alot -->
<a href="https://twitter.com/share" class="twitter-share-button" data-url="http://iseethatalot.com?id=<?php echo $id; ?>" data-via="seethatalot" data-hashtags="alot">Tweet</a>
<script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?'http':'https';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+'://platform.twitter.com/widgets.js';fjs.parentNode.insertBefore(js,fjs);}}(document, 'script', 'twitter-wjs');</script>

<!-- Show alot -->
<EMBED class="alot" src='svg.php?src=<?php echo $image; ?>&word=<?php echo $word; ?>' />
Share: <a href="http://iseethatalot.com?id=<?php echo $id; ?>">http://iseethatalot.com?id=<?php echo $id; ?></a>
<?php
}

?>
<!DOCTYPE html>
<HTML>
<HEAD>
<TITLE>I SEE THAT ALOT!</TITLE>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<link href="//netdna.bootstrapcdn.com/twitter-bootstrap/2.3.2/css/bootstrap-combined.min.css" rel="stylesheet">
<LINK type='text/css' rel='stylesheet' href='styles.css' />
</HEAD>
<BODY>

<DIV class="container">

<DIV id="brand"><A href=http://iseethatalot.com>ISEETHATALOT.COM</A></DIV>

<DIV id="thisAlot">
<?php show_alot($id, $word, $image); ?>
</DIV>

<DIV id="howIsAlotFormed">
	<FORM METHOD="POST">
		<DIV class="prefix">Do you observe <del>a lot</del> <a href=http://hyperboleandahalf.blogspot.com/2010/04/alot-is-better-than-you-at-everything.html target=_blank>alot</a> of something?</DIV>
		<DIV class="input-boxes">
			<INPUT type="text" name="word" placeholder="what is there alot of?" />	
			<INPUT type="text"  name="image" placeholder="image URL to tile alot" />
		</DIV>
		<INPUT type="submit" value="make this alot" class="btn make-alot-button btn-primary btn-large" />
	</FORM>
</DIV>

<DIV id="thoseAlots">
<?php
foreach (get_best(5) as $alot){
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
        based on <a href=http://hyperboleandahalf.blogspot.com/2010/04/alot-is-better-than-you-at-everything.html target=_blank>hyperbole and a half / allie brosh</A>
    </p>
  </div>
</div>

<!-- Alot of tracking -->
<script>
  (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
  (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
  m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
  })(window,document,'script','//www.google-analytics.com/analytics.js','ga');

  ga('create', 'UA-42448581-1', 'iseethatalot.com');
  ga('send', 'pageview');

</script>

</BODY>
</HTML>