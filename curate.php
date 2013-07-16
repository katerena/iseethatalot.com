<?php
include 'include/util.php.inc';

$config = read_config();
$db = $config->mkdb();

$config->require_curator();

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $id = NULL;
    $rating = NULL;
    if (isset($_POST['id'])) {
        $id = $_POST['id'];
    }
    if (isset($_POST['rating'])) {
        $rating = $_POST['rating'];
    }

	if (!$id || !$rating) {
		header("HTTP/1.0 400 Bad Request");
		echo 'Missing parameters';
		die();
	}
	
    if ($db->set_alot_rating($id, $rating)) {
        echo "Rating $rating saved for alot $id";
    } else {
		header("HTTP/1.0 404 Not Found");
		echo 'Rating not saved';
    }
	
	die();
}

$alots = $db->get_alots();
if ($alots === FALSE) {
    $config->error(404, 'No more alots');
}

?>
<!DOCTYPE html>
<html>
<head>
	<title>Alot of content</title>
	<link href="//netdna.bootstrapcdn.com/twitter-bootstrap/2.3.2/css/bootstrap-combined.min.css" rel="stylesheet">
</head>
<body>

	<h1>Curate content</h1>
	
	<table class="table">
	
	<tr>
		<th>ID</th>
		<th>Date</th>
		<th>Alot of...</th>
		<th>Awesomeness</th>
	</tr>
	
	<?php
	foreach ($alots as $alot) {
	?>
	<tr data-id="<?php echo $alot['ID']?>">
		<td><?php echo $alot['ID'] ?></td>
		<td><?php echo $alot['added'] ?></td>
		<td>
			<EMBED src='svg.php?src=<?php echo htmlentities($alot['image'])?>&word=<?php echo htmlentities($alot['word'])?>'  type="image/svg+xml" />
		</td>
		<td>
			<div class="btn-group">
				<button type='button' class="btn rating-button" value="1">1</button>
				<button type='button' class="btn rating-button" value="2">2</button>
				<button type='button' class="btn rating-button" value="3">3</button>
				<button type='button' class="btn rating-button" value="4">4</button>
				<button type='button' class="btn rating-button" value="5">5</button>
			</div>
		</td>
	</tr>
	<?php } ?>
	</table>

<script src="//ajax.googleapis.com/ajax/libs/jquery/1.10.2/jquery.min.js"></script>
<script type="text/javascript">
	$(document).ready(function() {
		$('.btn-group .rating-button').on('click', function() {
			var btn = $(this);
			var rating = btn.val();
			var id = btn.parents('tr').data('id');
			
			var allbuttons = $(this).parent().find('.btn');
			allbuttons.prop('disabled', true);
			
			$.post('curate.php', {
				id: id,
				rating: rating
			})
			.done(function() {
				allbuttons.removeClass('active');
				btn.addClass('active');
			})
			.error(function() {
				alert('Error submitting rating ' + rating + ' for alot #' + id);
			})
			.always(function() {
				allbuttons.prop('disabled', false);
			});
		});
	});
</script>
</body>
</html>