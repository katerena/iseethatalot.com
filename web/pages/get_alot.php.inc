<?php
include_once 'include/elements.php.inc';

//Id is required
$id = $config->get_param('id');
if ($id) {
    //Get a copy of the db
    $db = $config->mkdb();

    //must find alot
    $alot = $db->get_alot($id);
    if (!$alot) {
        $config->error(404, 'alot not found');
    } else {
        show_alot($alot);
    }
} else {
    echo 'Which alot!?!? THERE ARE SO MANY!';
}
