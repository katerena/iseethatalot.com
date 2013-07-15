<?php
// Check username and password:
$users = array(
	'admin' => 'alotbetterthanyou'
);

if (isset($_SERVER['PHP_AUTH_USER']) && isset($_SERVER['PHP_AUTH_PW'])){
 
    $username = $_SERVER['PHP_AUTH_USER'];
    $password = $_SERVER['PHP_AUTH_PW'];
 
    if (isset($users[$username]) && $users[$username] === $password) {
	$authenticated = TRUE;
    }
} 
if (!$authenticated) {
    header('WWW-Authenticate: Basic realm="My Realm"');
    header('HTTP/1.0 401 Unauthorized');
    echo 'Authentication Required';
    exit;
}

$conn = new PDO('mysql:host=localhost;dbname=kukmbr_alot', 'kukmbr_alot', 'kukmbr_alot1');

if (isset($_POST['id']) && isset($_POST['rating'])) {
	$rating = $_POST['rating'];
	$id = $_POST['id'];
	
	if (!$id || !$rating) {
		header("HTTP/1.0 400 Bad Request");
		echo 'Missing parameters';
		die();
	}
	
	$stmt = $conn->prepare('UPDATE alot SET curator_rating=:rating WHERE id=:id');
	$stmt->bindParam(':rating', $rating);
	$stmt->bindParam(':id', $id);
	
	if ($stmt->execute()) {
		echo "Rating $rating saved for alot $id";
	} else {
		header("HTTP/1.0 404 Not Found");
		echo 'Rating not saved';
	}
	
	die();
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
	$stmt = $conn->prepare('SELECT * FROM alot ORDER BY added DESC');
	$stmt->execute();
	$alots = $stmt->fetchAll();
	
	foreach ($alots as $alot) {
	?>
	<tr data-id="<?php echo $alot['ID']?>">
		<td><?php echo $alot['ID'] ?></td>
		<td><?php echo $alot['added'] ?></td>
		<td>
			<EMBED src='svg.php?src=<?php echo $alot['image']?>&word=<?php echo $alot['word']?>'  type="image/svg+xml" />
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