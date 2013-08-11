<?php

include_once 'include/util.php.inc';
include_once 'include/this_alot.php.inc';

$config = read_config();
$db = $config->mkdb();

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $id = NULL;
    $vote = NULL;
    if (isset($_POST['id'])) {
        $id = $_POST['id'];
    }
    if (isset($_POST['vote'])) {
        $vote = $_POST['vote'];
    }

    if (!($vote == -1 || $vote == 1)) {
        header("HTTP/1.0 400 Invalid vote");
        die();
    }

    if (!$id) {
        header("HTTP/1.0 400 Invalid alot id");
        die();
    }

    if ($db->vote_for_alot($id, $vote)) {
        $alot = $db->get_alot($id);
        rating_stats($alot);
    } else {
        header("HTTP/1.0 404 Not Found");
        die();
    }
}
