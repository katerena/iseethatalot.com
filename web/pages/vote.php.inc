<?php

include_once 'include/elements.php.inc';

if ($config->is_post()) {
    $db = $config->mkdb();

    $id = $config->post('id');
    $vote = $config->post('vote');

    if (!($vote == -1 || $vote == 1)) {
        $config->http_status(400, 'Invalid vote');
        die();
    }

    if (!$id) {
        $config->http_status(400, 'Invalid alot id');
        die();
    }

    if ($db->vote_for_alot($id, $vote)) {
        $alot = $db->get_alot($id);
        rating_stats($alot);
    } else {
        $config->http_status(404, 'Not Found');
        die();
    }
}
