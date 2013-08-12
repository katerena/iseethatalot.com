<?php

include_once 'include/util.php.inc';

$config = read_config();

$routes = array(
    '/vote'                         => 'pages/vote.php.inc',
    '/alot/(:id)'                   => 'pages/get_alot.php.inc',
    '/curate'                       => 'pages/curate.php.inc',
    '/curate/(:page)'               => 'pages/curate.php.inc',
    '/(:ordinal)/alot/of/(:word)'   => 'pages/main.php.inc',
    '/(:id)'                        => 'pages/main.php.inc',
    ''                              => 'pages/main.php.inc',
);

$page = $config->route($routes);

if ($page == NULL) {
    $config->error(404, 'Not Found');
} else {
    include_once $page;
}

